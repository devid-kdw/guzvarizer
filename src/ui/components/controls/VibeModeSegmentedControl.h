#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/shared/types/VibeMode.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class VibeModeSegmentedControl : public juce::Component,
                                 private ThemeManager::Listener {
public:
  VibeModeSegmentedControl(ThemeManager &themeManager);
  ~VibeModeSegmentedControl() override;

  void
  setMode(neon::VibeMode mode,
          juce::NotificationType notification = juce::sendNotificationSync);
  neon::VibeMode mode() const noexcept;

  void setOnModeChanged(std::function<void(neon::VibeMode)> callback);

  juce::ComboBox &modeSelector() noexcept { return modeSelector_; }
  const juce::ComboBox &modeSelector() const noexcept { return modeSelector_; }

  void themeChanged() override;
  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  void selectButtonForMode(neon::VibeMode mode);

  juce::TextButton smoothButton_;
  juce::TextButton punchButton_;
  juce::TextButton dirtyButton_;
  juce::ComboBox modeSelector_;
  ThemeManager &themeManager_;

  neon::VibeMode mode_ = neon::VibeMode::kSmooth;
  std::function<void(neon::VibeMode)> onModeChanged_;
};

} // namespace neon::ui
