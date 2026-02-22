#include "src/ui/components/text/SectionTitle.h"

namespace neon::ui {

SectionTitle::SectionTitle(ThemeManager &themeManager, juce::String text)
    : themeManager_(themeManager), text_(std::move(text)) {
  themeManager_.addListener(this);
}

SectionTitle::~SectionTitle() { themeManager_.removeListener(this); }

void SectionTitle::setText(const juce::String &text) {
  text_ = text;
  repaint();
}

void SectionTitle::setJustification(juce::Justification justification) {
  justification_ = justification;
  repaint();
}

void SectionTitle::paint(juce::Graphics &g) {
  const auto area = getLocalBounds().toFloat();
  const auto text = text_.toUpperCase();

  // Use a standard semi-bold sans-serif font
  g.setFont(juce::FontOptions(17.0f).withStyle("Semi-Bold"));

  // Saturated glow layer derived from theme
  g.setColour(themeManager_.accent().withAlpha(0.35f));
  g.drawText(text, area.translated(0.0f, 1.0f), justification_, true);

  // Sharp core text
  g.setColour(themeManager_.textPrimary());
  g.drawText(text, area, justification_, true);
}

void SectionTitle::themeChanged() { repaint(); }

} // namespace neon::ui
