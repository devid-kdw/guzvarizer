#include "src/ui/components/controls/BypassToggle.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

BypassToggle::BypassToggle() {
  button_.setButtonText({});
  button_.setClickingTogglesState(true);
  button_.setAlpha(0.0f);

  button_.onClick = [this] {
    if (onToggle_) {
      onToggle_(button_.getToggleState());
    }
    repaint();
  };

  addAndMakeVisible(button_);
}

void BypassToggle::setActive(bool active, juce::NotificationType notification) {
  button_.setToggleState(active, notification);
  repaint();
}

bool BypassToggle::isActive() const noexcept { return button_.getToggleState(); }

void BypassToggle::setOnToggle(std::function<void(bool)> callback) {
  onToggle_ = std::move(callback);
}

void BypassToggle::paint(juce::Graphics& g) {
  const auto off = assets::image("gz_control_bypass_button_off_320x96_png");
  const auto on = assets::image("gz_control_bypass_button_on_320x96_png");
  const auto hover = assets::image("gz_control_bypass_button_hover_320x96_png");
  const auto pressed = assets::image("gz_control_bypass_button_pressed_320x96_png");
  const auto disabled = assets::image("gz_control_bypass_button_disabled_320x96_png");

  const juce::Image* frame = &off;
  if (!button_.isEnabled() && disabled.isValid()) {
    frame = &disabled;
  } else if (button_.isDown() && pressed.isValid()) {
    frame = &pressed;
  } else if (button_.isOver() && hover.isValid()) {
    frame = &hover;
  } else if (isActive() && on.isValid()) {
    frame = &on;
  }

  if (!assets::drawIfValid(g, *frame, getLocalBounds().toFloat())) {
    g.setColour(juce::Colours::black.withAlpha(0.25f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
  }
}

void BypassToggle::resized() { button_.setBounds(getLocalBounds()); }

}  // namespace neon::ui
