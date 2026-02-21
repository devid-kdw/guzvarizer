#include "src/host/GuzvarizerProcessor.h"

#include "src/host/parameters/ParameterLayout.h"
#include "src/shared/types/ParameterIds.h"
#include "src/ui/app/PluginEditor.h"

namespace neon::host {

GuzvarizerProcessor::GuzvarizerProcessor()
    : juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                                 .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters_(*this, nullptr, "PARAMETERS", CreateParameterLayout()) {}

void GuzvarizerProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/) {}

void GuzvarizerProcessor::releaseResources() {}

bool GuzvarizerProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet()) {
    return false;
  }

  return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono() ||
         layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void GuzvarizerProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  const auto bypassParam = parameters_.getRawParameterValue(params::kBypass);
  if (bypassParam != nullptr && bypassParam->load(std::memory_order_relaxed) >= 0.5f) {
    return;
  }

  juce::ignoreUnused(buffer);
  // DSP chain will be introduced in subsequent milestones.
}

juce::AudioProcessorEditor* GuzvarizerProcessor::createEditor() {
  return new neon::ui::PluginEditor(*this);
}

bool GuzvarizerProcessor::hasEditor() const {
  return true;
}

const juce::String GuzvarizerProcessor::getName() const {
  return JucePlugin_Name;
}

bool GuzvarizerProcessor::acceptsMidi() const {
  return false;
}

bool GuzvarizerProcessor::producesMidi() const {
  return false;
}

bool GuzvarizerProcessor::isMidiEffect() const {
  return false;
}

double GuzvarizerProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int GuzvarizerProcessor::getNumPrograms() {
  return 1;
}

int GuzvarizerProcessor::getCurrentProgram() {
  return 0;
}

void GuzvarizerProcessor::setCurrentProgram(int /*index*/) {}

const juce::String GuzvarizerProcessor::getProgramName(int /*index*/) {
  return {};
}

void GuzvarizerProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/) {}

void GuzvarizerProcessor::getStateInformation(juce::MemoryBlock& destData) {
  const auto state = parameters_.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void GuzvarizerProcessor::setStateInformation(const void* data, int sizeInBytes) {
  const std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
  if (xmlState == nullptr) {
    return;
  }

  if (!xmlState->hasTagName(parameters_.state.getType())) {
    return;
  }

  parameters_.replaceState(juce::ValueTree::fromXml(*xmlState));
}

}  // namespace neon::host

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new neon::host::GuzvarizerProcessor();
}
