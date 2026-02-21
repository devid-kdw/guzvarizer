#include "src/ui/components/shell/HeaderBar.h"

namespace neon::ui {

HeaderBar::HeaderBar() {
  addAndMakeVisible(title_);
  addAndMakeVisible(bypassToggle_);
}

void HeaderBar::setPluginName(const juce::String& name) {
  title_.setText(name.toUpperCase());
}

void HeaderBar::setBypass(bool bypassed, juce::NotificationType notification) {
  bypassToggle_.setActive(bypassed, notification);
}

void HeaderBar::setBypassChanged(std::function<void(bool)> callback) {
  bypassToggle_.setOnToggle(std::move(callback));
}

void HeaderBar::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat();
  g.setColour(juce::Colour::fromRGB(9, 14, 23));
  g.fillRoundedRectangle(area, 8.0f);

  g.setColour(juce::Colour::fromRGB(0, 255, 220).withAlpha(0.25f));
  g.drawRoundedRectangle(area, 8.0f, 1.0f);
}

void HeaderBar::resized() {
  auto area = getLocalBounds().reduced(10);
  title_.setBounds(area.removeFromLeft(area.getWidth() - 170));
  bypassToggle_.setBounds(area.removeFromRight(160));
}

}  // namespace neon::ui
