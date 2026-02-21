#pragma once

#include <JuceHeader.h>

#include <functional>

#include "src/ui/components/controls/BypassToggle.h"
#include "src/ui/components/text/SectionTitle.h"

namespace neon::ui {

class HeaderBar : public juce::Component {
 public:
  HeaderBar();

  void setPluginName(const juce::String& name);
  void setBypass(bool bypassed, juce::NotificationType notification = juce::sendNotificationSync);
  void setBypassChanged(std::function<void(bool)> callback);

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  SectionTitle title_{"NEON COMPRESSOR"};
  BypassToggle bypassToggle_;
};

}  // namespace neon::ui
