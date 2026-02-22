#pragma once

#include <JuceHeader.h>

#include <array>

#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/metering/GainReductionMeter.h"
#include "src/ui/components/metering/LevelVisualizer.h"
#include "src/ui/components/text/SectionTitle.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class MainSection : public juce::Component, private ThemeManager::Listener {
public:
  explicit MainSection(ThemeManager &themeManager);
  ~MainSection() override;

  std::array<NeonKnob *, 5> compressorKnobs();
  GainReductionMeter &gainReductionMeter() noexcept {
    return gainReductionMeter_;
  }
  LevelVisualizer &levelVisualizer() noexcept { return levelVisualizer_; }

  void paint(juce::Graphics &g) override;
  void resized() override;

private:
  void themeChanged() override;

  ThemeManager &themeManager_;
  SectionTitle title_;
  NeonKnob thresholdKnob_;
  NeonKnob attackKnob_;
  NeonKnob releaseKnob_;
  NeonKnob ratioKnob_;
  NeonKnob outputKnob_;

  LevelVisualizer levelVisualizer_;
  GainReductionMeter gainReductionMeter_;
};

} // namespace neon::ui
