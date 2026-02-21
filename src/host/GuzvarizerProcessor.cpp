#include "src/host/GuzvarizerProcessor.h"

#include "src/dsp/modes/VibeModeConstants.h"
#include "src/host/bridge/AtomicMeterBridgeClient.h"
#include "src/host/parameters/ParameterLayout.h"
#include "src/shared/types/ParameterIds.h"
#include "src/shared/types/VibeMode.h"
#include "src/ui/app/PluginEditor.h"

#include <algorithm>
#include <cmath>

namespace neon::host {

namespace {
constexpr float kSmoothingRampSeconds = 0.02f; // 20 ms anti-zipper ramp
constexpr float kDefaultKneeWidthDb = 6.0f;
} // namespace

GuzvarizerProcessor::GuzvarizerProcessor()
    : juce::AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters_(*this, nullptr, "PARAMETERS", CreateParameterLayout()) {
  cacheParameterPointers();
}

void GuzvarizerProcessor::cacheParameterPointers() {
  pThreshold_ = parameters_.getRawParameterValue(params::kThresholdDb);
  pAttack_ = parameters_.getRawParameterValue(params::kAttackMs);
  pRelease_ = parameters_.getRawParameterValue(params::kReleaseMs);
  pRatio_ = parameters_.getRawParameterValue(params::kRatio);
  pOutput_ = parameters_.getRawParameterValue(params::kOutputDb);
  pBypass_ = parameters_.getRawParameterValue(params::kBypass);
  pVibeMode_ = parameters_.getRawParameterValue(params::kVibeMode);
  pSaturation_ = parameters_.getRawParameterValue(params::kSaturationDrive);
  pHarmonics_ = parameters_.getRawParameterValue(params::kHarmonics);
  pGrit_ = parameters_.getRawParameterValue(params::kGrit);
  pAir_ = parameters_.getRawParameterValue(params::kAir);
  pAirFreq_ = parameters_.getRawParameterValue(params::kAirFreq);
  pGuzvanjeEnabled_ =
      parameters_.getRawParameterValue(params::kGuzvanjeEnabled);
  pLfoRateHz_ = parameters_.getRawParameterValue(params::kLfoRateHz);
  pLfoSyncEnabled_ = parameters_.getRawParameterValue(params::kLfoSyncEnabled);
  pLfoSyncDivision_ =
      parameters_.getRawParameterValue(params::kLfoSyncDivision);
}

void GuzvarizerProcessor::prepareToPlay(double sampleRate,
                                        int samplesPerBlock) {
  currentSampleRate_ = sampleRate;

  compressor_.prepare(sampleRate, samplesPerBlock);
  toneShaper_.prepare(sampleRate, samplesPerBlock);
  guzvanje_.prepare(sampleRate);

  guzThresholdBiases_.assign(samplesPerBlock, 0.0f);
  guzRatioBiases_.assign(samplesPerBlock, 0.0f);

  // Initialise smoothed parameters
  smoothThreshold_.reset(sampleRate, kSmoothingRampSeconds);
  smoothAttack_.reset(sampleRate, kSmoothingRampSeconds);
  smoothRelease_.reset(sampleRate, kSmoothingRampSeconds);
  smoothRatio_.reset(sampleRate, kSmoothingRampSeconds);
  smoothOutput_.reset(sampleRate, kSmoothingRampSeconds);

  // Set current values immediately (no ramp on first block)
  smoothThreshold_.setCurrentAndTargetValue(
      pThreshold_->load(std::memory_order_relaxed));
  smoothAttack_.setCurrentAndTargetValue(
      pAttack_->load(std::memory_order_relaxed));
  smoothRelease_.setCurrentAndTargetValue(
      pRelease_->load(std::memory_order_relaxed));
  smoothRatio_.setCurrentAndTargetValue(
      pRatio_->load(std::memory_order_relaxed));
  smoothOutput_.setCurrentAndTargetValue(
      pOutput_->load(std::memory_order_relaxed));

  meterSource_.reset();
}

void GuzvarizerProcessor::releaseResources() {
  compressor_.reset();
  toneShaper_.reset();
  guzvanje_.reset();
  meterSource_.reset();
}

bool GuzvarizerProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
  if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet()) {
    return false;
  }

  return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono() ||
         layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void GuzvarizerProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                       juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  // --- Bypass check ---
  const bool bypassed = pBypass_->load(std::memory_order_relaxed) >= 0.5f;
  if (bypassed) {
    meterSource_.publish(0.0f, -90.0f, -90.0f, 0.0f);
    return;
  }

  const auto numSamples = buffer.getNumSamples();
  const auto numChannels = buffer.getNumChannels();
  if (numSamples == 0 || numChannels == 0)
    return;

  // --- Sanitize Input (NaN/Inf protection) ---
  for (int ch = 0; ch < numChannels; ++ch) {
    float *data = buffer.getWritePointer(ch);
    for (int i = 0; i < numSamples; ++i) {
      if (std::isnan(data[i]) || std::isinf(data[i])) {
        data[i] = 0.0f;
      }
    }
  }

  // --- Read APVTS parameters ---
  smoothThreshold_.setTargetValue(pThreshold_->load(std::memory_order_relaxed));
  smoothAttack_.setTargetValue(pAttack_->load(std::memory_order_relaxed));
  smoothRelease_.setTargetValue(pRelease_->load(std::memory_order_relaxed));
  smoothRatio_.setTargetValue(pRatio_->load(std::memory_order_relaxed));
  smoothOutput_.setTargetValue(pOutput_->load(std::memory_order_relaxed));

  const float threshold = smoothThreshold_.skip(numSamples);
  const float attack = smoothAttack_.skip(numSamples);
  const float release = smoothRelease_.skip(numSamples);
  const float ratio = smoothRatio_.skip(numSamples);
  const float outputGain = smoothOutput_.skip(numSamples);

  // --- Vibe mode morph ---
  const auto vibeIndex =
      static_cast<int>(pVibeMode_->load(std::memory_order_relaxed));
  const auto vibeMode =
      static_cast<neon::VibeMode>(std::clamp(vibeIndex, 0, 2));
  const auto vibePreset = neon::dsp::getVibeModePreset(vibeMode);

  // --- Gužvanje state ---
  const bool guzvanjeOn =
      pGuzvanjeEnabled_->load(std::memory_order_relaxed) >= 0.5f;
  guzvanje_.setEnabled(guzvanjeOn);

  // LFO params
  const float lfoRate = pLfoRateHz_->load(std::memory_order_relaxed);
  const bool lfoSync =
      pLfoSyncEnabled_->load(std::memory_order_relaxed) >= 0.5f;
  const int lfoDivision =
      static_cast<int>(pLfoSyncDivision_->load(std::memory_order_relaxed));

  guzvanje_.setLfoRate(lfoRate);

  // Get host BPM for LFO sync
  float hostBpm = 120.0f; // fallback
  if (auto *playHead = getPlayHead()) {
    if (auto posInfo = playHead->getPosition()) {
      if (auto bpm = posInfo->getBpm()) {
        hostBpm = static_cast<float>(*bpm);
      }
    }
  }
  guzvanje_.setLfoSync(lfoSync, lfoDivision, hostBpm);

  // Process Gužvanje per-sample to populate bias arrays
  neon::dsp::GuzvanjeDsp::GuzvanjeBias guzBias;
  for (int i = 0; i < numSamples; ++i) {
    guzBias = guzvanje_.processSample();
    guzThresholdBiases_[i] = guzBias.thresholdOffset;
    guzRatioBiases_[i] = guzBias.ratioOffset;
  }

  // --- Build compressor params with vibe biases ---
  neon::dsp::CompressorParams cparams;
  cparams.thresholdDb = threshold; // vibe doesn't bias threshold
  cparams.attackMs = std::max(0.1f, attack + vibePreset.attackBias);
  cparams.releaseMs = std::max(10.0f, release + vibePreset.releaseBias);
  cparams.ratio = std::max(1.0f, ratio);
  cparams.outputDb = outputGain;
  cparams.kneeWidthDb =
      std::max(0.5f, kDefaultKneeWidthDb + vibePreset.kneeBias);
  compressor_.setParams(cparams);

  // --- Tone shaper params with vibe biases ---
  neon::dsp::ToneShaperParams tparams;
  tparams.saturationDrive = pSaturation_->load(std::memory_order_relaxed) *
                            vibePreset.saturationScale;
  tparams.harmonics =
      pHarmonics_->load(std::memory_order_relaxed) * vibePreset.harmonicsScale;
  tparams.grit = pGrit_->load(std::memory_order_relaxed) * vibePreset.gritScale;
  tparams.air = pAir_->load(std::memory_order_relaxed);
  tparams.airFreq = pAirFreq_->load(std::memory_order_relaxed);
  toneShaper_.setParams(tparams);

  // --- Measure input peak ---
  float peakIn = 0.0f;
  for (int ch = 0; ch < numChannels; ++ch) {
    const float *data = buffer.getReadPointer(ch);
    for (int i = 0; i < numSamples; ++i) {
      const float s = std::abs(data[i]);
      if (s > peakIn)
        peakIn = s;
    }
  }

  // --- Process: compressor → tone shaper ---
  float *channelL = buffer.getWritePointer(0);
  float *channelR = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

  const float peakGrDb = compressor_.processBlock(
      channelL, channelR, numSamples, guzThresholdBiases_.data(),
      guzRatioBiases_.data());
  toneShaper_.processBlock(channelL, channelR, numSamples);

  // --- Measure output peak + RMS ---
  float peakOut = 0.0f;
  float rmsAccum = 0.0f;
  for (int ch = 0; ch < numChannels; ++ch) {
    const float *data = buffer.getReadPointer(ch);
    for (int i = 0; i < numSamples; ++i) {
      const float s = std::abs(data[i]);
      if (s > peakOut)
        peakOut = s;
      rmsAccum += data[i] * data[i];
    }
  }

  const float peakInDb =
      peakIn > 0.0f ? juce::Decibels::gainToDecibels(peakIn, -90.0f) : -90.0f;
  const float peakOutDb =
      peakOut > 0.0f ? juce::Decibels::gainToDecibels(peakOut, -90.0f) : -90.0f;
  const float rmsNorm =
      std::sqrt(rmsAccum / static_cast<float>(numSamples * numChannels));

  meterSource_.publish(peakGrDb, peakInDb, peakOutDb, rmsNorm);
}

juce::AudioProcessorEditor *GuzvarizerProcessor::createEditor() {
  return new neon::ui::PluginEditor(*this);
}

bool GuzvarizerProcessor::hasEditor() const { return true; }

const juce::String GuzvarizerProcessor::getName() const {
  return JucePlugin_Name;
}

bool GuzvarizerProcessor::acceptsMidi() const { return false; }

bool GuzvarizerProcessor::producesMidi() const { return false; }

bool GuzvarizerProcessor::isMidiEffect() const { return false; }

double GuzvarizerProcessor::getTailLengthSeconds() const { return 0.0; }

int GuzvarizerProcessor::getNumPrograms() { return 1; }

int GuzvarizerProcessor::getCurrentProgram() { return 0; }

void GuzvarizerProcessor::setCurrentProgram(int /*index*/) {}

const juce::String GuzvarizerProcessor::getProgramName(int /*index*/) {
  return {};
}

void GuzvarizerProcessor::changeProgramName(int /*index*/,
                                            const juce::String & /*newName*/) {}

void GuzvarizerProcessor::getStateInformation(juce::MemoryBlock &destData) {
  const auto state = parameters_.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void GuzvarizerProcessor::setStateInformation(const void *data,
                                              int sizeInBytes) {
  const std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState == nullptr) {
    return;
  }

  if (!xmlState->hasTagName(parameters_.state.getType())) {
    return;
  }

  parameters_.replaceState(juce::ValueTree::fromXml(*xmlState));
}

} // namespace neon::host

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new neon::host::GuzvarizerProcessor();
}
