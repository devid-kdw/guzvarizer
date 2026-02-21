#include "src/ui/bindings/ParameterBindingAdapter.h"

#include <cmath>

namespace neon::ui {

ParameterBindingAdapter::ParameterBindingAdapter(SetValueFn setValue, GetValueFn getValue)
    : setValue_(std::move(setValue)), getValue_(std::move(getValue)) {}

void ParameterBindingAdapter::bindKnob(NeonKnob& knob) {
  knob.setOnValueChanged([this, &knob](double value) {
    if (setValue_) {
      setValue_(knob.parameterId(), static_cast<float>(value));
    }
  });

  if (getValue_) {
    knob.setValue(getValue_(knob.parameterId()), juce::dontSendNotification);
  }
}

void ParameterBindingAdapter::bindBypass(BypassToggle& bypass, const juce::String& parameterId) {
  bypass.setOnToggle([this, parameterId](bool active) {
    if (setValue_) {
      setValue_(parameterId, active ? 1.0f : 0.0f);
    }
  });

  if (getValue_) {
    bypass.setActive(getValue_(parameterId) >= 0.5f, juce::dontSendNotification);
  }
}

void ParameterBindingAdapter::bindVibeMode(VibeModeSegmentedControl& control, const juce::String& parameterId) {
  control.setOnModeChanged([this, parameterId](neon::VibeMode mode) {
    if (setValue_) {
      setValue_(parameterId, static_cast<float>(static_cast<int>(mode)));
    }
  });

  if (getValue_) {
    const auto modeIndex = juce::jlimit(0, 2, static_cast<int>(std::round(getValue_(parameterId))));
    control.setMode(static_cast<neon::VibeMode>(modeIndex), juce::dontSendNotification);
  }
}

}  // namespace neon::ui
