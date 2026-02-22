#pragma once

#include <JuceHeader.h>
#include <vector>

#include "src/dsp/compressor/CompressorCore.h"
#include "src/dsp/modes/GuzvanjeDsp.h"
#include "src/dsp/tone_shaper/ToneShaper.h"
#include "src/dsp/util/LfoOscillator.h"
#include "src/host/bridge/MeterBridgeSource.h"

namespace neon::host {

class GuzvarizerProcessor : public juce::AudioProcessor {
public:
  GuzvarizerProcessor();
  ~GuzvarizerProcessor() override = default;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  juce::AudioProcessorValueTreeState &parameters() noexcept {
    return parameters_;
  }
  const juce::AudioProcessorValueTreeState &parameters() const noexcept {
    return parameters_;
  }

  /// Non-owning access for the editor to read meter data.
  const MeterBridgeSource &meterSource() const noexcept { return meterSource_; }

private:
  juce::AudioProcessorValueTreeState parameters_;

  // DSP components
  neon::dsp::CompressorCore compressor_;
  neon::dsp::ToneShaper toneShaper_;
  neon::dsp::GuzvanjeDsp guzvanje_;

  // Per-sample buffers for parameters
  std::vector<float> sampleThresholds_;
  std::vector<float> sampleRatios_;
  std::vector<float> sampleOutputGains_;

  // Lock-free meter bridge (audio thread writes, UI thread reads)
  MeterBridgeSource meterSource_;

  // Cached raw parameter pointers (valid for plugin lifetime, realtime-safe
  // reads)
  std::atomic<float> *pThreshold_ = nullptr;
  std::atomic<float> *pAttack_ = nullptr;
  std::atomic<float> *pRelease_ = nullptr;
  std::atomic<float> *pRatio_ = nullptr;
  std::atomic<float> *pOutput_ = nullptr;
  std::atomic<float> *pBypass_ = nullptr;
  std::atomic<float> *pVibeMode_ = nullptr;
  std::atomic<float> *pSaturation_ = nullptr;
  std::atomic<float> *pHarmonics_ = nullptr;
  std::atomic<float> *pGrit_ = nullptr;
  std::atomic<float> *pAir_ = nullptr;
  std::atomic<float> *pAirFreq_ = nullptr;
  std::atomic<float> *pGuzvanjeEnabled_ = nullptr;
  std::atomic<float> *pLfoRateHz_ = nullptr;
  std::atomic<float> *pLfoSyncEnabled_ = nullptr;
  std::atomic<float> *pLfoSyncDivision_ = nullptr;

  // Smoothed parameters (anti-zipper)
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>
      smoothThreshold_;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothOutput_;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>
      smoothAttack_;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>
      smoothRelease_;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>
      smoothRatio_;

  double currentSampleRate_ = 44100.0;

  void cacheParameterPointers();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuzvarizerProcessor)
};

} // namespace neon::host
