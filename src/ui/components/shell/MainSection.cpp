#include "src/ui/components/shell/MainSection.h"

#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

MainSection::MainSection(ThemeManager &themeManager)
    : themeManager_(themeManager), title_(themeManager, "COMPRESSION"),
      thresholdKnob_({"THRESHOLD", params::kThresholdDb, -60.0, 0.0, -18.0,
                      NeonKnob::Mapping::kLinear,
                      juce::Colour::fromRGB(0, 224, 255), false,
                      "threshold"}),
      attackKnob_({"ATTACK", params::kAttackMs, 0.1, 200.0, 10.0,
                   NeonKnob::Mapping::kLogarithmic,
                   juce::Colour::fromRGB(0, 255, 181), false, "attack"}),
      releaseKnob_({"RELEASE", params::kReleaseMs, 10.0, 2000.0, 120.0,
                    NeonKnob::Mapping::kLogarithmic,
                    juce::Colour::fromRGB(255, 92, 198), false, "release"}),
      ratioKnob_({"RATIO", params::kRatio, 1.0, 20.0, 4.0,
                  NeonKnob::Mapping::kLogarithmic,
                  juce::Colour::fromRGB(120, 196, 255), false, "ratio"}),
      outputKnob_({"OUTPUT", params::kOutputDb, -24.0, 24.0, 0.0,
                   NeonKnob::Mapping::kLinear,
                   juce::Colour::fromRGB(255, 170, 50), false, "output"}) {
  themeManager_.addListener(this);

  addAndMakeVisible(title_);
  title_.setVisible(false);
  addAndMakeVisible(thresholdKnob_);
  addAndMakeVisible(attackKnob_);
  addAndMakeVisible(releaseKnob_);
  addAndMakeVisible(ratioKnob_);
  addAndMakeVisible(outputKnob_);

  addAndMakeVisible(levelVisualizer_);
  addAndMakeVisible(gainReductionMeter_);
}

MainSection::~MainSection() { themeManager_.removeListener(this); }

std::array<NeonKnob *, 5> MainSection::compressorKnobs() {
  return {&thresholdKnob_, &attackKnob_, &releaseKnob_, &ratioKnob_,
          &outputKnob_};
}

void MainSection::paint(juce::Graphics &g) { juce::ignoreUnused(g); }

void MainSection::resized() {
  constexpr float baseX = 16.0f;
  constexpr float baseY = 106.0f;
  constexpr float baseW = 1148.0f;
  constexpr float baseH = 264.0f;

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

  setKnobFromGlobal(thresholdKnob_, 154.0f, 254.0f, 96.0f);
  setKnobFromGlobal(attackKnob_, 314.0f, 254.0f, 96.0f);
  setKnobFromGlobal(releaseKnob_, 505.0f, 254.0f, 96.0f);
  setKnobFromGlobal(ratioKnob_, 696.0f, 254.0f, 96.0f);
  setKnobFromGlobal(outputKnob_, 887.0f, 254.0f, 96.0f);

  const auto local = [sx, sy, baseX, baseY](float gx, float gy, float gw,
                                             float gh) {
    return juce::Rectangle<int>(juce::roundToInt((gx - baseX) * sx),
                                juce::roundToInt((gy - baseY) * sy),
                                juce::roundToInt(gw * sx),
                                juce::roundToInt(gh * sy));
  };

  levelVisualizer_.setBounds(local(938.0f, 126.0f, 210.0f, 86.0f));
  gainReductionMeter_.setBounds(local(938.0f, 220.0f, 210.0f, 146.0f));
}

void MainSection::themeChanged() { repaint(); }

} // namespace neon::ui
