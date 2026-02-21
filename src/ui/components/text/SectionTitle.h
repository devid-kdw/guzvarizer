#pragma once

#include <JuceHeader.h>

namespace neon::ui {

class SectionTitle : public juce::Component {
 public:
  explicit SectionTitle(juce::String text);

  void setText(const juce::String& text);
  void setGlowColour(juce::Colour colour);

  void paint(juce::Graphics& g) override;

 private:
  juce::String text_;
  juce::Colour glowColour_ = juce::Colours::aqua.withAlpha(0.55f);
};

}  // namespace neon::ui
