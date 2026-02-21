#pragma once

#include <JuceHeader.h>

#include <array>

#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/metering/GainReductionMeter.h"
#include "src/ui/components/metering/LevelVisualizer.h"

namespace neon::ui {

class MainSection : public juce::Component {
 public:
  MainSection();

  std::array<NeonKnob*, 5> compressorKnobs();
  GainReductionMeter& gainReductionMeter() noexcept { return gainReductionMeter_; }
  LevelVisualizer& levelVisualizer() noexcept { return levelVisualizer_; }

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  NeonKnob thresholdKnob_;
  NeonKnob attackKnob_;
  NeonKnob releaseKnob_;
  NeonKnob ratioKnob_;
  NeonKnob outputKnob_;

  LevelVisualizer levelVisualizer_;
  GainReductionMeter gainReductionMeter_;
};

}  // namespace neon::ui
