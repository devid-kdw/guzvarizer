#include "src/ui/components/text/SectionTitle.h"

namespace neon::ui {

SectionTitle::SectionTitle(juce::String text) : text_(std::move(text)) {}

void SectionTitle::setText(const juce::String& text) {
  text_ = text;
  repaint();
}

void SectionTitle::setGlowColour(juce::Colour colour) {
  glowColour_ = colour;
  repaint();
}

void SectionTitle::paint(juce::Graphics& g) {
  const auto area = getLocalBounds();
  const auto text = text_.toUpperCase();

  g.setFont(juce::Font(18.0f, juce::Font::bold));
  g.setColour(glowColour_);
  g.drawText(text, area.translated(0, 1), juce::Justification::centredLeft, true);

  g.setColour(juce::Colours::white.withAlpha(0.92f));
  g.drawText(text, area, juce::Justification::centredLeft, true);
}

}  // namespace neon::ui
