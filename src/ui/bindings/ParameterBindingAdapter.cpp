#include "src/ui/bindings/ParameterBindingAdapter.h"
#include "src/ui/components/controls/GuzvanjeToggle.h"
#include "src/ui/components/controls/LfoRateControl.h"

namespace neon::ui {

ParameterBindingAdapter::ParameterBindingAdapter(
    juce::AudioProcessorValueTreeState &parameters)
    : parameters_(parameters) {}

void ParameterBindingAdapter::bindKnob(NeonKnob &knob) {
  sliderAttachments_.push_back(std::make_unique<SliderAttachment>(
      parameters_, knob.parameterId(), knob.slider()));
}

void ParameterBindingAdapter::bindBypass(BypassToggle &bypass,
                                         const juce::String &parameterId) {
  buttonAttachments_.push_back(std::make_unique<ButtonAttachment>(
      parameters_, parameterId, bypass.button()));
}

void ParameterBindingAdapter::bindVibeMode(VibeModeSegmentedControl &control,
                                           const juce::String &parameterId) {
  comboAttachments_.push_back(std::make_unique<ComboBoxAttachment>(
      parameters_, parameterId, control.modeSelector()));
}

void ParameterBindingAdapter::bindGuzvanje(neon::ui::GuzvanjeToggle &toggle,
                                           const juce::String &parameterId) {
  buttonAttachments_.push_back(std::make_unique<ButtonAttachment>(
      parameters_, parameterId, toggle.button()));
}

void ParameterBindingAdapter::bindLfoSyncEnabled(
    juce::ToggleButton &toggle, const juce::String &parameterId) {
  buttonAttachments_.push_back(
      std::make_unique<ButtonAttachment>(parameters_, parameterId, toggle));
}

void ParameterBindingAdapter::bindLfoSyncDivision(
    juce::ComboBox &combo, const juce::String &parameterId) {
  comboAttachments_.push_back(
      std::make_unique<ComboBoxAttachment>(parameters_, parameterId, combo));
}

} // namespace neon::ui
