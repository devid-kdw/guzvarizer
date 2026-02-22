#pragma once

#include <JuceHeader.h>
#include <functional>

#include "src/shared/types/LfoSyncDivision.h"
#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class LfoRateControl : public juce::Component, private ThemeManager::Listener {
public:
  LfoRateControl(ThemeManager &themeManager);
  ~LfoRateControl() override;

  void
  setSyncMode(bool isSync,
              juce::NotificationType notification = juce::sendNotificationSync);
  bool isSyncMode() const noexcept;

  void setGuzvanjeActive(bool active);

  NeonKnob &rateKnob() noexcept { return rateKnob_; }
  juce::ToggleButton &syncButton() noexcept { return syncButton_; }
  juce::ComboBox &divisionCombo() noexcept { return divisionCombo_; }

  void resized() override;
  void paint(juce::Graphics &g) override;

private:
  void updateVisibility();
  void themeChanged() override;

  NeonKnob rateKnob_;
  juce::ToggleButton syncButton_;
  juce::ComboBox divisionCombo_;
  ThemeManager &themeManager_;

  bool isGuzvanjeActive_ = false;
};

} // namespace neon::ui
