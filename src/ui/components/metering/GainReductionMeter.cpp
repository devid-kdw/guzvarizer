#include "src/ui/components/metering/GainReductionMeter.h"

#include <cmath>

#include "src/ui/assets/UiAssetRepository.h"
#include "src/ui/theme/UiTypography.h"

namespace neon::ui {

GainReductionMeter::GainReductionMeter() { startTimerHz(kUiTimerHz); }

void GainReductionMeter::setGainReductionDb(float db) noexcept {
  targetDb_.store(juce::jlimit(-30.0f, 0.0f, db), std::memory_order_relaxed);
}

void GainReductionMeter::paint(juce::Graphics &g) {
  auto area = getLocalBounds().toFloat();

  const auto frame = assets::image("gz_meter_gr_panel_frame_base_220x200_png");
  const auto segment = assets::image("gz_meter_gr_segment_sprite_base_16x48_png");

  assets::drawIfValid(g, frame, area);

  auto barsArea = area.reduced(area.getWidth() * 0.08f, area.getHeight() * 0.34f);
  constexpr int numBars = 10;
  const auto gap = area.getWidth() * 0.018f;
  const auto barW =
      (barsArea.getWidth() - (gap * static_cast<float>(numBars - 1))) /
      static_cast<float>(numBars);

  const auto normalized =
      juce::jlimit(0.0f, 1.0f, juce::jmap(smoothedDb_, 0.0f, -30.0f, 0.0f, 1.0f));
  const auto active = static_cast<int>(std::round(normalized * numBars));

  for (int i = 0; i < numBars; ++i) {
    const auto bar = juce::Rectangle<float>(
        barsArea.getX() + static_cast<float>(i) * (barW + gap), barsArea.getY(),
        barW, barsArea.getHeight());

    const float alpha = (i < active) ? 0.98f : 0.12f;
    if (!assets::drawIfValid(g, segment, bar, alpha)) {
      g.setColour(juce::Colours::white.withAlpha(alpha));
      g.fillRoundedRectangle(bar, 2.0f);
    }
  }

  const auto readoutColor = juce::Colour::fromRGB(255, 176, 56).withAlpha(0.92f);
  g.setColour(readoutColor);
  g.setFont(fonts::makeDisplayFont(10.5f, "Heavy", 0.07f));

  auto readout = area.toNearestInt().removeFromBottom(20).reduced(8, 0);
  readout.removeFromLeft(static_cast<int>(area.getWidth() * 0.42f));
  readout.removeFromRight(static_cast<int>(area.getWidth() * 0.04f));
  g.drawText("GR " + juce::String(smoothedDb_, 1) + " dB", readout,
             juce::Justification::centredRight, false);
}

void GainReductionMeter::timerCallback() {
  const auto target = targetDb_.load(std::memory_order_relaxed);
  const auto alpha =
      1.0f -
      std::exp(-1.0f / (kSmoothingTauSeconds * static_cast<float>(kUiTimerHz)));

  if (std::abs(target - smoothedDb_) < 0.05f) {
    if (smoothedDb_ != target) {
      smoothedDb_ = target;
      repaint();
    }
    return;
  }

  smoothedDb_ += (target - smoothedDb_) * alpha;
  repaint();
}

} // namespace neon::ui
