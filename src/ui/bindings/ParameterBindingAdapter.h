#pragma once

#include <JuceHeader.h>

#include <memory>
#include <vector>

#include "src/ui/components/controls/BypassToggle.h"
#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/controls/VibeModeSegmentedControl.h"

namespace neon::ui {

class ParameterBindingAdapter {
 public:
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
  using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

  explicit ParameterBindingAdapter(juce::AudioProcessorValueTreeState& parameters);

  void bindKnob(NeonKnob& knob);
  void bindBypass(BypassToggle& bypass, const juce::String& parameterId);
  void bindVibeMode(VibeModeSegmentedControl& control, const juce::String& parameterId);

 private:
  juce::AudioProcessorValueTreeState& parameters_;
  std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments_;
  std::vector<std::unique_ptr<ButtonAttachment>> buttonAttachments_;
  std::vector<std::unique_ptr<ComboBoxAttachment>> comboAttachments_;
};

}  // namespace neon::ui
