#include "src/ui/components/shell/FooterBar.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

FooterBar::FooterBar(ThemeManager &themeManager)
    : themeManager_(themeManager), vibeLabel_(themeManager, "VIBE MODE"),
      vibeMode_(themeManager), lfoRateControl_(themeManager) {
  themeManager_.addListener(this);
  vibeLabel_.setVisible(false);

  addAndMakeVisible(vibeLabel_);
  addAndMakeVisible(vibeMode_);
  addAndMakeVisible(guzvanjeToggle_);
  addAndMakeVisible(lfoRateControl_);
}

FooterBar::~FooterBar() { themeManager_.removeListener(this); }

void FooterBar::setVibeMode(neon::VibeMode mode,
                            juce::NotificationType notification) {
  vibeMode_.setMode(mode, notification);
}

void FooterBar::setVibeModeChanged(
    std::function<void(neon::VibeMode)> callback) {
  vibeMode_.setOnModeChanged(std::move(callback));
}

void FooterBar::setStatusLed(bool on) {
  statusLedOn_ = on;
  repaint();
}

void FooterBar::paint(juce::Graphics &g) {
  const auto ledOn = assets::image("gz_control_led_status_on_48x48_png");
  const auto ledOff = assets::image("gz_control_led_status_off_48x48_png");
  const auto ledBounds =
      juce::Rectangle<float>(static_cast<float>(getWidth() - 66.0f),
                             static_cast<float>((getHeight() - 28.0f) * 0.5f),
                             28.0f, 28.0f);

  assets::drawIfValid(g, statusLedOn_ ? ledOn : ledOff, ledBounds);
}

void FooterBar::resized() {
  vibeLabel_.setBounds(20, 12, 120, 28);
  vibeMode_.setBounds(165, 10, 284, 66);
  guzvanjeToggle_.setBounds(465, 11, 180, 65);
  lfoRateControl_.setBounds(656, 2, 210, 84);
}

void FooterBar::themeChanged() { repaint(); }

} // namespace neon::ui
