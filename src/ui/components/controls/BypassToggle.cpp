#include "src/ui/components/controls/BypassToggle.h"

namespace neon::ui {

BypassToggle::BypassToggle() {
  button_.setButtonText("BY-PASS");
  button_.setClickingTogglesState(true);
  button_.onClick = [this] {
    if (onToggle_) {
      onToggle_(button_.getToggleState());
    }
    repaint();
  };

  button_.setColour(juce::ToggleButton::textColourId, juce::Colours::white.withAlpha(0.88f));
  addAndMakeVisible(button_);
}

void BypassToggle::setActive(bool active, juce::NotificationType notification) {
  button_.setToggleState(active, notification);
  repaint();
}

bool BypassToggle::isActive() const noexcept {
  return button_.getToggleState();
}

void BypassToggle::setOnToggle(std::function<void(bool)> callback) {
  onToggle_ = std::move(callback);
}

void BypassToggle::paint(juce::Graphics& g) {
  if (!isActive()) {
    return;
  }

  auto glowBounds = getLocalBounds().toFloat().reduced(3.0f);
  g.setColour(juce::Colour::fromRGB(0, 255, 212).withAlpha(0.2f));
  g.fillRoundedRectangle(glowBounds, 6.0f);
  g.setColour(juce::Colour::fromRGB(0, 255, 212).withAlpha(0.58f));
  g.drawRoundedRectangle(glowBounds, 6.0f, 1.2f);
}

void BypassToggle::resized() {
  button_.setBounds(getLocalBounds().reduced(8, 4));
}

}  // namespace neon::ui
