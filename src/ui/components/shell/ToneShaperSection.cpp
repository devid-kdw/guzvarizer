#include "src/ui/components/shell/ToneShaperSection.h"

#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

ToneShaperSection::ToneShaperSection(ThemeManager &themeManager)
    : themeManager_(themeManager), title_(themeManager, "TONE SHAPER"),
      saturationKnob_({"SATURATION", params::kSaturationDrive, 0.0, 100.0,
                       25.0, NeonKnob::Mapping::kLinear,
                       juce::Colour::fromRGB(255, 92, 145), false,
                       "saturation"}),
      harmonicsKnob_({"HARMONICS", params::kHarmonics, 0.0, 100.0, 40.0,
                      NeonKnob::Mapping::kLinear,
                      juce::Colour::fromRGB(0, 239, 220), false,
                      "harmonics"}),
      gritKnob_({"GRIT", params::kGrit, 0.0, 100.0, 20.0,
                 NeonKnob::Mapping::kLinear,
                 juce::Colour::fromRGB(255, 181, 44), false, "grit"}),
      airKnob_({"AIR", params::kAir, 0.0, 100.0, 30.0,
                NeonKnob::Mapping::kLinear,
                juce::Colour::fromRGB(111, 199, 255), false, "air"}),
      colorKnob_({"COLOR", params::kAirFreq, 1000.0, 20000.0, 8000.0,
                  NeonKnob::Mapping::kLogarithmic,
                  juce::Colour::fromRGB(182, 119, 255), false, "color"}) {
  themeManager_.addListener(this);

  addAndMakeVisible(title_);
  title_.setVisible(false);
  addAndMakeVisible(saturationKnob_);
  addAndMakeVisible(harmonicsKnob_);
  addAndMakeVisible(gritKnob_);
  addAndMakeVisible(airKnob_);
  addAndMakeVisible(colorKnob_);
}

ToneShaperSection::~ToneShaperSection() { themeManager_.removeListener(this); }

std::array<NeonKnob *, 5> ToneShaperSection::toneKnobs() {
  return {&saturationKnob_, &harmonicsKnob_, &gritKnob_, &airKnob_,
          &colorKnob_};
}

void ToneShaperSection::paint(juce::Graphics &g) { juce::ignoreUnused(g); }

void ToneShaperSection::resized() {
  constexpr float baseX = 16.0f;
  constexpr float baseY = 418.0f;
  constexpr float baseW = 1148.0f;
  constexpr float baseH = 220.0f;

  const auto sx = static_cast<float>(getWidth()) / baseW;
  const auto sy = static_cast<float>(getHeight()) / baseH;

  const auto setKnobFromGlobal =
      [this, sx, sy, baseX, baseY](NeonKnob &knob, float globalCx,
                                   float globalCy, float diameter) {
        constexpr float knobExtraHeight = 24.0f;
        const auto localCx = (globalCx - baseX) * sx;
        const auto localCy = (globalCy - baseY) * sy;
        const auto localD = diameter * juce::jmin(sx, sy);
        const auto localExtra = knobExtraHeight * sy;

        knob.setBounds(juce::Rectangle<int>(
            juce::roundToInt(localCx - (localD * 0.5f)),
            juce::roundToInt(localCy - (localD * 0.5f) - (localExtra * 0.5f)),
            juce::roundToInt(localD), juce::roundToInt(localD + localExtra)));
      };

  setKnobFromGlobal(saturationKnob_, 140.0f, 546.0f, 86.0f);
  setKnobFromGlobal(harmonicsKnob_, 365.0f, 546.0f, 86.0f);
  setKnobFromGlobal(gritKnob_, 590.0f, 546.0f, 86.0f);
  setKnobFromGlobal(airKnob_, 815.0f, 546.0f, 86.0f);
  setKnobFromGlobal(colorKnob_, 1040.0f, 546.0f, 86.0f);
}

void ToneShaperSection::themeChanged() { repaint(); }

} // namespace neon::ui
