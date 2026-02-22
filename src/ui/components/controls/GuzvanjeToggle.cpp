#include "src/ui/components/controls/GuzvanjeToggle.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

GuzvanjeToggle::GuzvanjeToggle() {
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

void GuzvanjeToggle::setActive(bool active,
                               juce::NotificationType notification) {
  button_.setToggleState(active, notification);
  repaint();
}

bool GuzvanjeToggle::isActive() const noexcept {
  return button_.getToggleState();
}

void GuzvanjeToggle::setOnToggle(std::function<void(bool)> callback) {
  onToggle_ = std::move(callback);
}

void GuzvanjeToggle::paint(juce::Graphics &g) {
  const auto off = assets::image("gz_control_guzvanje_toggle_off_260x94_png");
  const auto on = assets::image("gz_control_guzvanje_toggle_on_260x94_png");
  const auto hover =
      assets::image("gz_control_guzvanje_toggle_hover_260x94_png");
  const auto pressed =
      assets::image("gz_control_guzvanje_toggle_pressed_260x94_png");

  const juce::Image *frame = &off;
  if (button_.isDown() && pressed.isValid()) {
    frame = &pressed;
  } else if (isActive() && on.isValid()) {
    frame = &on;
  } else if (button_.isOver() && hover.isValid()) {
    frame = &hover;
  }

  if (!assets::drawIfValid(g, *frame, getLocalBounds().toFloat())) {
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
  }
}

void GuzvanjeToggle::resized() { button_.setBounds(getLocalBounds()); }

} // namespace neon::ui
