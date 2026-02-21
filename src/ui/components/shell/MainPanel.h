#pragma once

#include <JuceHeader.h>

#include "src/ui/components/shell/FooterBar.h"
#include "src/ui/components/shell/HeaderBar.h"
#include "src/ui/components/shell/MainSection.h"
#include "src/ui/components/shell/ToneShaperSection.h"
#include "src/ui/components/theme/ColorStripIndicator.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class MainPanel : public juce::Component {
 public:
  MainPanel();

  HeaderBar& headerBar() noexcept { return headerBar_; }
  MainSection& mainSection() noexcept { return mainSection_; }
  ToneShaperSection& toneShaperSection() noexcept { return toneShaperSection_; }
  FooterBar& footerBar() noexcept { return footerBar_; }
  ColorStripIndicator& colorStrip() noexcept { return colorStrip_; }
  ThemeManager& themeManager() noexcept { return themeManager_; }

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  ThemeManager themeManager_;

  HeaderBar headerBar_;
  MainSection mainSection_;
  ColorStripIndicator colorStrip_;
  ToneShaperSection toneShaperSection_;
  FooterBar footerBar_;
};

}  // namespace neon::ui
