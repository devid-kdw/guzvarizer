#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/shared/types/VibeMode.h"
#include "src/ui/components/controls/GuzvanjeToggle.h"
#include "src/ui/components/controls/LfoRateControl.h"
#include "src/ui/components/controls/VibeModeSegmentedControl.h"
#include "src/ui/components/text/SectionTitle.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class FooterBar : public juce::Component, private ThemeManager::Listener {
public:
  explicit FooterBar(ThemeManager &themeManager);
  ~FooterBar() override;

  void
  setVibeMode(neon::VibeMode mode,
              juce::NotificationType notification = juce::sendNotificationSync);
  void setVibeModeChanged(std::function<void(neon::VibeMode)> callback);
  void setStatusLed(bool on);

  VibeModeSegmentedControl &vibeModeControl() noexcept { return vibeMode_; }
  const VibeModeSegmentedControl &vibeModeControl() const noexcept {
    return vibeMode_;
  }

  GuzvanjeToggle &guzvanjeToggle() noexcept { return guzvanjeToggle_; }
  const GuzvanjeToggle &guzvanjeToggle() const noexcept {
    return guzvanjeToggle_;
  }

  LfoRateControl &lfoRateControl() noexcept { return lfoRateControl_; }
  const LfoRateControl &lfoRateControl() const noexcept {
    return lfoRateControl_;
  }

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  void themeChanged() override;

  ThemeManager &themeManager_;
  SectionTitle vibeLabel_;
  VibeModeSegmentedControl vibeMode_;
  GuzvanjeToggle guzvanjeToggle_;
  LfoRateControl lfoRateControl_;
  bool statusLedOn_ = true;
};

} // namespace neon::ui
