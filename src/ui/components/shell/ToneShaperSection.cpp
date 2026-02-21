#include "src/ui/components/shell/ToneShaperSection.h"

#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

ToneShaperSection::ToneShaperSection()
    : saturationKnob_({"SATURATION", params::kSaturationDrive, 0.0, 100.0, 25.0, NeonKnob::Mapping::kLinear,
                       juce::Colour::fromRGB(255, 92, 145), true}),
      harmonicsKnob_({"HARMONICS", params::kHarmonics, 0.0, 100.0, 40.0, NeonKnob::Mapping::kLinear,
                      juce::Colour::fromRGB(0, 239, 220), true}),
      gritKnob_({"GRIT", params::kGrit, 0.0, 100.0, 20.0, NeonKnob::Mapping::kLinear,
                 juce::Colour::fromRGB(255, 181, 44), true}),
      airKnob_({"AIR", params::kAir, 0.0, 100.0, 30.0, NeonKnob::Mapping::kLinear,
                juce::Colour::fromRGB(111, 199, 255), true}),
      colorKnob_({"COLOR", params::kAirFreq, 1000.0, 20000.0, 8000.0, NeonKnob::Mapping::kLogarithmic,
                  juce::Colour::fromRGB(182, 119, 255), true}) {
  addAndMakeVisible(title_);
  addAndMakeVisible(saturationKnob_);
  addAndMakeVisible(harmonicsKnob_);
  addAndMakeVisible(gritKnob_);
  addAndMakeVisible(airKnob_);
  addAndMakeVisible(colorKnob_);
}

std::array<NeonKnob*, 5> ToneShaperSection::toneKnobs() {
  return {&saturationKnob_, &harmonicsKnob_, &gritKnob_, &airKnob_, &colorKnob_};
}

void ToneShaperSection::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat();
  g.setColour(juce::Colour::fromRGB(14, 22, 34));
  g.fillRoundedRectangle(area, 10.0f);

  g.setColour(juce::Colour::fromRGB(255, 90, 188).withAlpha(0.18f));
  g.drawRoundedRectangle(area, 10.0f, 1.0f);
}

void ToneShaperSection::resized() {
  auto area = getLocalBounds().reduced(12);
  title_.setBounds(area.removeFromTop(22));
  area.removeFromTop(6);

  juce::FlexBox row;
  row.flexWrap = juce::FlexBox::Wrap::noWrap;
  row.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
  row.alignItems = juce::FlexBox::AlignItems::center;

  for (auto* knob : toneKnobs()) {
    row.items.add(juce::FlexItem(*knob).withMinWidth(120.0f).withMinHeight(132.0f).withFlex(1.0f));
  }

  row.performLayout(area.toFloat());
}

}  // namespace neon::ui
