#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/shared/types/VibeMode.h"
#include "src/ui/components/controls/BypassToggle.h"
#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/controls/VibeModeSegmentedControl.h"

namespace neon::ui {

class ParameterBindingAdapter {
 public:
  using SetValueFn = std::function<void(const juce::String&, float)>;
  using GetValueFn = std::function<float(const juce::String&)>;

  ParameterBindingAdapter(SetValueFn setValue, GetValueFn getValue);

  void bindKnob(NeonKnob& knob);
  void bindBypass(BypassToggle& bypass, const juce::String& parameterId);
  void bindVibeMode(VibeModeSegmentedControl& control, const juce::String& parameterId);

 private:
  SetValueFn setValue_;
  GetValueFn getValue_;
};

}  // namespace neon::ui
