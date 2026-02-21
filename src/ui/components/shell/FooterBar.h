#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/shared/types/VibeMode.h"
#include "src/ui/components/controls/VibeModeSegmentedControl.h"
#include "src/ui/components/text/SectionTitle.h"

namespace neon::ui {

class FooterBar : public juce::Component {
 public:
  FooterBar();

  void setVibeMode(neon::VibeMode mode, juce::NotificationType notification = juce::sendNotificationSync);
  void setVibeModeChanged(std::function<void(neon::VibeMode)> callback);
  void setStatusLed(bool on);

  VibeModeSegmentedControl& vibeModeControl() noexcept { return vibeMode_; }
  const VibeModeSegmentedControl& vibeModeControl() const noexcept { return vibeMode_; }

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  SectionTitle vibeLabel_{"VIBE MODE"};
  VibeModeSegmentedControl vibeMode_;
  bool statusLedOn_ = true;
};

}  // namespace neon::ui
