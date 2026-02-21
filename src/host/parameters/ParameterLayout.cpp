#include "src/host/parameters/ParameterLayout.h"

#include "src/shared/types/LfoSyncDivision.h"
#include "src/shared/types/ParameterIds.h"

namespace neon::host {

namespace {

juce::NormalisableRange<float> linearRange(float minValue, float maxValue) {
  return juce::NormalisableRange<float>(minValue, maxValue);
}

juce::NormalisableRange<float> logRange(float minValue, float maxValue, float midpoint) {
  juce::NormalisableRange<float> range(minValue, maxValue);
  range.setSkewForCentre(midpoint);
  return range;
}

juce::StringArray lfoDivisionChoices() {
  juce::StringArray choices;
  for (const auto* label : neon::kLfoSyncDivisionLabels) {
    choices.add(label);
  }
  return choices;
}

}  // namespace

juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout() {
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
  parameters.reserve(22);

  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kThresholdDb, "THRESHOLD", linearRange(-60.0f, 0.0f), -18.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kAttackMs, "ATTACK", logRange(0.1f, 200.0f, 10.0f), 10.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kReleaseMs, "RELEASE", logRange(10.0f, 2000.0f, 120.0f), 120.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kRatio, "RATIO", logRange(1.0f, 20.0f, 4.0f), 4.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kOutputDb, "OUTPUT", linearRange(-24.0f, 24.0f), 0.0f));

  parameters.push_back(std::make_unique<juce::AudioParameterBool>(params::kBypass, "BYPASS", false));
  parameters.push_back(
      std::make_unique<juce::AudioParameterChoice>(params::kVibeMode, "VIBE", juce::StringArray{"SMOOTH", "PUNCH", "DIRTY"}, 0));

  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kSaturationDrive, "SATURATION", linearRange(0.0f, 100.0f), 25.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kHarmonics, "HARMONICS", linearRange(0.0f, 100.0f), 40.0f));
  parameters.push_back(
      std::make_unique<juce::AudioParameterFloat>(params::kGrit, "GRIT", linearRange(0.0f, 100.0f), 20.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kAir, "AIR", linearRange(0.0f, 100.0f), 30.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kAirFreq, "AIR FREQ", logRange(1000.0f, 20000.0f, 8000.0f), 8000.0f));

  parameters.push_back(
      std::make_unique<juce::AudioParameterBool>(params::kGuzvanjeEnabled, "GU\u017dVANJE", false));
  parameters.push_back(
      std::make_unique<juce::AudioParameterFloat>(params::kLfoRateHz, "LFO RATE", logRange(0.05f, 20.0f, 2.0f), 2.0f));
  parameters.push_back(
      std::make_unique<juce::AudioParameterBool>(params::kLfoSyncEnabled, "LFO SYNC", false));
  parameters.push_back(std::make_unique<juce::AudioParameterChoice>(params::kLfoSyncDivision, "LFO DIV", lfoDivisionChoices(),
                                                                     static_cast<int>(neon::LfoSyncDivision::k1_16)));

  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kAccentHue, "Accent Hue", linearRange(0.0f, 360.0f), 190.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kGlowIntensity, "Glow", linearRange(0.0f, 100.0f), 65.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kPanelBrightness, "Panel", linearRange(0.0f, 100.0f), 35.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kNeonSaturation, "Neon Sat", linearRange(0.0f, 100.0f), 80.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kScanlinesAmount, "Scanlines", linearRange(0.0f, 100.0f), 20.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      params::kGrainAmount, "Grain", linearRange(0.0f, 100.0f), 15.0f));

  return {parameters.begin(), parameters.end()};
}

}  // namespace neon::host
