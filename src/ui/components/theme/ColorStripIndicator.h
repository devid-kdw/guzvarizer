#pragma once

#include <JuceHeader.h>

namespace neon::ui {

class ColorStripIndicator : public juce::Component {
 public:
  ColorStripIndicator() = default;

  void setReactiveLevel(float normalized) noexcept;

  void paint(juce::Graphics& g) override;

 private:
  float reactiveLevel_ = 0.35f;
};

}  // namespace neon::ui
