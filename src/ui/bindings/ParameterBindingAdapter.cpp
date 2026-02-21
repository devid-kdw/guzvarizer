#include "src/ui/bindings/ParameterBindingAdapter.h"

namespace neon::ui {

ParameterBindingAdapter::ParameterBindingAdapter(juce::AudioProcessorValueTreeState& parameters)
    : parameters_(parameters) {}

void ParameterBindingAdapter::bindKnob(NeonKnob& knob) {
  sliderAttachments_.push_back(
      std::make_unique<SliderAttachment>(parameters_, knob.parameterId(), knob.slider()));
}

void ParameterBindingAdapter::bindBypass(BypassToggle& bypass, const juce::String& parameterId) {
  buttonAttachments_.push_back(
      std::make_unique<ButtonAttachment>(parameters_, parameterId, bypass.button()));
}

void ParameterBindingAdapter::bindVibeMode(VibeModeSegmentedControl& control, const juce::String& parameterId) {
  comboAttachments_.push_back(
      std::make_unique<ComboBoxAttachment>(parameters_, parameterId, control.modeSelector()));
}

}  // namespace neon::ui
