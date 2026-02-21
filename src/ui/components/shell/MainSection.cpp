#include "src/ui/components/shell/MainSection.h"

#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

MainSection::MainSection()
    : thresholdKnob_({"THRESHOLD", params::kThresholdDb, -60.0, 0.0, -18.0, NeonKnob::Mapping::kLinear,
                      juce::Colour::fromRGB(0, 224, 255), true}),
      attackKnob_({"ATTACK", params::kAttackMs, 0.1, 200.0, 10.0, NeonKnob::Mapping::kLogarithmic,
                   juce::Colour::fromRGB(0, 255, 181), true}),
      releaseKnob_({"RELEASE", params::kReleaseMs, 10.0, 2000.0, 120.0, NeonKnob::Mapping::kLogarithmic,
                    juce::Colour::fromRGB(255, 92, 198), true}),
      ratioKnob_({"RATIO", params::kRatio, 1.0, 20.0, 4.0, NeonKnob::Mapping::kLogarithmic,
                  juce::Colour::fromRGB(120, 196, 255), true}),
      outputKnob_({"OUTPUT", params::kOutputDb, -24.0, 24.0, 0.0, NeonKnob::Mapping::kLinear,
                   juce::Colour::fromRGB(255, 170, 50), true}) {
  addAndMakeVisible(thresholdKnob_);
  addAndMakeVisible(attackKnob_);
  addAndMakeVisible(releaseKnob_);
  addAndMakeVisible(ratioKnob_);
  addAndMakeVisible(outputKnob_);

  addAndMakeVisible(levelVisualizer_);
  addAndMakeVisible(gainReductionMeter_);
}

std::array<NeonKnob*, 5> MainSection::compressorKnobs() {
  return {&thresholdKnob_, &attackKnob_, &releaseKnob_, &ratioKnob_, &outputKnob_};
}

void MainSection::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat();
  g.setColour(juce::Colour::fromRGB(11, 18, 30));
  g.fillRoundedRectangle(area, 10.0f);

  g.setColour(juce::Colour::fromRGB(0, 255, 200).withAlpha(0.18f));
  g.drawRoundedRectangle(area, 10.0f, 1.0f);
}

void MainSection::resized() {
  auto area = getLocalBounds().reduced(12);

  auto rightMeterArea = area.removeFromRight(170);
  levelVisualizer_.setBounds(rightMeterArea.removeFromTop(90));
  rightMeterArea.removeFromTop(8);
  gainReductionMeter_.setBounds(rightMeterArea.reduced(12, 4));

  juce::FlexBox knobRow;
  knobRow.flexWrap = juce::FlexBox::Wrap::noWrap;
  knobRow.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
  knobRow.alignItems = juce::FlexBox::AlignItems::center;

  for (auto* knob : compressorKnobs()) {
    knobRow.items.add(juce::FlexItem(*knob).withMinWidth(120.0f).withMinHeight(140.0f).withFlex(1.0f));
  }

  knobRow.performLayout(area.toFloat());
}

}  // namespace neon::ui
