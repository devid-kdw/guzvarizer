#pragma once

#include <JuceHeader.h>

#include <array>

#include "src/ui/components/controls/NeonKnob.h"
#include "src/ui/components/text/SectionTitle.h"

namespace neon::ui {

class ToneShaperSection : public juce::Component {
 public:
  ToneShaperSection();

  std::array<NeonKnob*, 5> toneKnobs();

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  SectionTitle title_{"TONE SHAPER"};
  NeonKnob saturationKnob_;
  NeonKnob harmonicsKnob_;
  NeonKnob gritKnob_;
  NeonKnob airKnob_;
  NeonKnob colorKnob_;
};

}  // namespace neon::ui
