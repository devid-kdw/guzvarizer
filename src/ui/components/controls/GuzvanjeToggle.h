#pragma once

#include <JuceHeader.h>
#include <functional>

namespace neon::ui {

class GuzvanjeToggle : public juce::Component {
public:
  GuzvanjeToggle();

  void
  setActive(bool active,
            juce::NotificationType notification = juce::sendNotificationSync);
  bool isActive() const noexcept;

  void setOnToggle(std::function<void(bool)> callback);

  juce::ToggleButton &button() noexcept { return button_; }
  const juce::ToggleButton &button() const noexcept { return button_; }

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  juce::ToggleButton button_;
  std::function<void(bool)> onToggle_;
};

} // namespace neon::ui
