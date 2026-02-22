#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/ui/components/controls/BypassToggle.h"
#include "src/ui/components/text/SectionTitle.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class HeaderBar : public juce::Component, private ThemeManager::Listener {
public:
  explicit HeaderBar(ThemeManager &themeManager);
  ~HeaderBar() override;

  void setPluginName(const juce::String &name);
  void
  setBypass(bool bypassed,
            juce::NotificationType notification = juce::sendNotificationSync);
  void setBypassChanged(std::function<void(bool)> callback);

  BypassToggle &bypassToggle() noexcept { return bypassToggle_; }
  const BypassToggle &bypassToggle() const noexcept { return bypassToggle_; }

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  void themeChanged() override;

  ThemeManager &themeManager_;
  juce::String pluginName_ = juce::String::fromUTF8("GUŽVARIZER");
  SectionTitle title_;
  BypassToggle bypassToggle_;
};

} // namespace neon::ui
