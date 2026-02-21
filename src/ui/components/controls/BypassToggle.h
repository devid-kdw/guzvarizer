#pragma once

#include <JuceHeader.h>

#include <functional>

namespace neon::ui {

class BypassToggle : public juce::Component {
 public:
  BypassToggle();

  void setActive(bool active, juce::NotificationType notification = juce::sendNotificationSync);
  bool isActive() const noexcept;

  void setOnToggle(std::function<void(bool)> callback);

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  juce::ToggleButton button_;
  std::function<void(bool)> onToggle_;
};

}  // namespace neon::ui
