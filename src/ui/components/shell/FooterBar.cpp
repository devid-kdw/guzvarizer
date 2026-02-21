#include "src/ui/components/shell/FooterBar.h"

namespace neon::ui {

FooterBar::FooterBar() {
  addAndMakeVisible(vibeLabel_);
  addAndMakeVisible(vibeMode_);
}

void FooterBar::setVibeMode(neon::VibeMode mode, juce::NotificationType notification) {
  vibeMode_.setMode(mode, notification);
}

void FooterBar::setVibeModeChanged(std::function<void(neon::VibeMode)> callback) {
  vibeMode_.setOnModeChanged(std::move(callback));
}

void FooterBar::setStatusLed(bool on) {
  statusLedOn_ = on;
  repaint();
}

void FooterBar::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat();
  g.setColour(juce::Colour::fromRGB(8, 14, 24));
  g.fillRoundedRectangle(area, 8.0f);

  g.setColour(juce::Colour::fromRGB(0, 255, 207).withAlpha(0.18f));
  g.drawRoundedRectangle(area, 8.0f, 1.0f);

  auto ledArea = getLocalBounds().removeFromRight(42).reduced(8);
  g.setColour(statusLedOn_ ? juce::Colour::fromRGB(0, 255, 166) : juce::Colour::fromRGB(80, 80, 80));
  g.fillEllipse(ledArea.toFloat());
  g.setColour(juce::Colours::white.withAlpha(0.35f));
  g.drawEllipse(ledArea.toFloat(), 1.0f);
}

void FooterBar::resized() {
  auto area = getLocalBounds().reduced(12);
  vibeLabel_.setBounds(area.removeFromLeft(170));
  vibeMode_.setBounds(area.removeFromLeft(410).reduced(4, 12));
}

}  // namespace neon::ui
