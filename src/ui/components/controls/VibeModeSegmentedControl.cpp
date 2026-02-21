#include "src/ui/components/controls/VibeModeSegmentedControl.h"

namespace neon::ui {

VibeModeSegmentedControl::VibeModeSegmentedControl() {
  smoothButton_.setButtonText("SMOOTH");
  punchButton_.setButtonText("PUNCH");
  dirtyButton_.setButtonText("DIRTY");

  const int radioGroup = 9001;
  smoothButton_.setRadioGroupId(radioGroup);
  punchButton_.setRadioGroupId(radioGroup);
  dirtyButton_.setRadioGroupId(radioGroup);

  smoothButton_.setClickingTogglesState(true);
  punchButton_.setClickingTogglesState(true);
  dirtyButton_.setClickingTogglesState(true);

  smoothButton_.onClick = [this] {
    setMode(neon::VibeMode::kSmooth);
  };
  punchButton_.onClick = [this] {
    setMode(neon::VibeMode::kPunch);
  };
  dirtyButton_.onClick = [this] {
    setMode(neon::VibeMode::kDirty);
  };

  for (auto* button : {&smoothButton_, &punchButton_, &dirtyButton_}) {
    button->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(0, 255, 196).withAlpha(0.48f));
    button->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(16, 24, 40));
    button->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    button->setColour(juce::TextButton::textColourOffId, juce::Colours::white.withAlpha(0.8f));
    addAndMakeVisible(*button);
  }

  selectButtonForMode(mode_);
}

void VibeModeSegmentedControl::setMode(neon::VibeMode mode, juce::NotificationType notification) {
  mode_ = mode;
  selectButtonForMode(mode_);

  if (notification != juce::dontSendNotification && onModeChanged_) {
    onModeChanged_(mode_);
  }
}

neon::VibeMode VibeModeSegmentedControl::mode() const noexcept {
  return mode_;
}

void VibeModeSegmentedControl::setOnModeChanged(std::function<void(neon::VibeMode)> callback) {
  onModeChanged_ = std::move(callback);
}

void VibeModeSegmentedControl::resized() {
  auto area = getLocalBounds();
  const auto width = area.getWidth() / 3;
  smoothButton_.setBounds(area.removeFromLeft(width).reduced(2));
  punchButton_.setBounds(area.removeFromLeft(width).reduced(2));
  dirtyButton_.setBounds(area.reduced(2));
}

void VibeModeSegmentedControl::selectButtonForMode(neon::VibeMode mode) {
  smoothButton_.setToggleState(mode == neon::VibeMode::kSmooth, juce::dontSendNotification);
  punchButton_.setToggleState(mode == neon::VibeMode::kPunch, juce::dontSendNotification);
  dirtyButton_.setToggleState(mode == neon::VibeMode::kDirty, juce::dontSendNotification);
}

}  // namespace neon::ui
