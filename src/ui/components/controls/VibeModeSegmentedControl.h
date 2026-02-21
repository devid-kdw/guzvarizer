#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/shared/types/VibeMode.h"

namespace neon::ui {

class VibeModeSegmentedControl : public juce::Component {
 public:
  VibeModeSegmentedControl();

  void setMode(neon::VibeMode mode, juce::NotificationType notification = juce::sendNotificationSync);
  neon::VibeMode mode() const noexcept;

  void setOnModeChanged(std::function<void(neon::VibeMode)> callback);

  void resized() override;

 private:
  void selectButtonForMode(neon::VibeMode mode);

  juce::TextButton smoothButton_;
  juce::TextButton punchButton_;
  juce::TextButton dirtyButton_;

  neon::VibeMode mode_ = neon::VibeMode::kSmooth;
  std::function<void(neon::VibeMode)> onModeChanged_;
};

}  // namespace neon::ui
