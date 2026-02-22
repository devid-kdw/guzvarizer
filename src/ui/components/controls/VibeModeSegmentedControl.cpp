#include "src/ui/components/controls/VibeModeSegmentedControl.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

namespace {

int toChoiceId(neon::VibeMode mode) { return static_cast<int>(mode) + 1; }

neon::VibeMode toMode(int choiceId) {
  const auto clamped = juce::jlimit(1, 3, choiceId);
  return static_cast<neon::VibeMode>(clamped - 1);
}

} // namespace

VibeModeSegmentedControl::VibeModeSegmentedControl(ThemeManager &themeManager)
    : themeManager_(themeManager) {
  themeManager_.addListener(this);

  smoothButton_.setButtonText({});
  punchButton_.setButtonText({});
  dirtyButton_.setButtonText({});

  const int radioGroup = 9001;
  smoothButton_.setRadioGroupId(radioGroup);
  punchButton_.setRadioGroupId(radioGroup);
  dirtyButton_.setRadioGroupId(radioGroup);

  smoothButton_.setClickingTogglesState(true);
  punchButton_.setClickingTogglesState(true);
  dirtyButton_.setClickingTogglesState(true);

  smoothButton_.setAlpha(0.0f);
  punchButton_.setAlpha(0.0f);
  dirtyButton_.setAlpha(0.0f);

  modeSelector_.addItem("SMOOTH", toChoiceId(neon::VibeMode::kSmooth));
  modeSelector_.addItem("PUNCH", toChoiceId(neon::VibeMode::kPunch));
  modeSelector_.addItem("DIRTY", toChoiceId(neon::VibeMode::kDirty));
  modeSelector_.setVisible(false);
  addAndMakeVisible(modeSelector_);

  smoothButton_.onClick = [this] {
    modeSelector_.setSelectedId(toChoiceId(neon::VibeMode::kSmooth));
  };
  punchButton_.onClick = [this] {
    modeSelector_.setSelectedId(toChoiceId(neon::VibeMode::kPunch));
  };
  dirtyButton_.onClick = [this] {
    modeSelector_.setSelectedId(toChoiceId(neon::VibeMode::kDirty));
  };

  modeSelector_.onChange = [this] {
    mode_ = toMode(modeSelector_.getSelectedId());
    selectButtonForMode(mode_);

    if (onModeChanged_) {
      onModeChanged_(mode_);
    }

    repaint();
  };

  addAndMakeVisible(smoothButton_);
  addAndMakeVisible(punchButton_);
  addAndMakeVisible(dirtyButton_);

  setMode(mode_, juce::dontSendNotification);
}

VibeModeSegmentedControl::~VibeModeSegmentedControl() {
  themeManager_.removeListener(this);
}

void VibeModeSegmentedControl::paint(juce::Graphics &g) {
  const auto shell =
      assets::image("gz_control_vibe_segmented_shell_base_640x120_png");
  assets::drawIfValid(g, shell, getLocalBounds().toFloat());

  const auto smoothOff = assets::image("gz_control_vibe_smooth_off_180x88_png");
  const auto smoothOn = assets::image("gz_control_vibe_smooth_on_180x88_png");
  const auto punchOff = assets::image("gz_control_vibe_punch_off_180x88_png");
  const auto punchOn = assets::image("gz_control_vibe_punch_on_180x88_png");
  const auto dirtyOff = assets::image("gz_control_vibe_dirty_off_180x88_png");
  const auto dirtyOn = assets::image("gz_control_vibe_dirty_on_180x88_png");

  assets::drawIfValid(g,
                      mode_ == neon::VibeMode::kSmooth ? smoothOn : smoothOff,
                      smoothButton_.getBounds().toFloat());
  assets::drawIfValid(g,
                      mode_ == neon::VibeMode::kPunch ? punchOn : punchOff,
                      punchButton_.getBounds().toFloat());
  assets::drawIfValid(g,
                      mode_ == neon::VibeMode::kDirty ? dirtyOn : dirtyOff,
                      dirtyButton_.getBounds().toFloat());
}

void VibeModeSegmentedControl::setMode(neon::VibeMode mode,
                                       juce::NotificationType notification) {
  modeSelector_.setSelectedId(toChoiceId(mode), notification);
  mode_ = mode;
  selectButtonForMode(mode_);
  repaint();
}

neon::VibeMode VibeModeSegmentedControl::mode() const noexcept { return mode_; }

void VibeModeSegmentedControl::setOnModeChanged(
    std::function<void(neon::VibeMode)> callback) {
  onModeChanged_ = std::move(callback);
}

void VibeModeSegmentedControl::resized() {
  auto area = getLocalBounds().reduced(10, 8);
  const auto segmentWidth = area.getWidth() / 3;

  smoothButton_.setBounds(area.removeFromLeft(segmentWidth));
  punchButton_.setBounds(area.removeFromLeft(segmentWidth));
  dirtyButton_.setBounds(area);
  modeSelector_.setBounds(0, 0, 0, 0);
}

void VibeModeSegmentedControl::selectButtonForMode(neon::VibeMode mode) {
  smoothButton_.setToggleState(mode == neon::VibeMode::kSmooth,
                               juce::dontSendNotification);
  punchButton_.setToggleState(mode == neon::VibeMode::kPunch,
                              juce::dontSendNotification);
  dirtyButton_.setToggleState(mode == neon::VibeMode::kDirty,
                              juce::dontSendNotification);
}

void VibeModeSegmentedControl::themeChanged() { repaint(); }

} // namespace neon::ui
