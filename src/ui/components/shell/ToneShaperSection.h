#pragma once

#include <JuceHeader.h>

#include <array>

#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/text/SectionTitle.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class ToneShaperSection : public juce::Component,
                          private ThemeManager::Listener {
public:
  explicit ToneShaperSection(ThemeManager &themeManager);
  ~ToneShaperSection() override;

  std::array<NeonKnob *, 5> toneKnobs();

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  void themeChanged() override;

  ThemeManager &themeManager_;
  SectionTitle title_;
  NeonKnob saturationKnob_;
  NeonKnob harmonicsKnob_;
  NeonKnob gritKnob_;
  NeonKnob airKnob_;
  NeonKnob colorKnob_;
};

} // namespace neon::ui
