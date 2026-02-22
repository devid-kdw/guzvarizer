#include "src/ui/components/metering/LevelVisualizer.h"

#include "src/ui/assets/UiAssetRepository.h"
#include "src/ui/theme/UiTypography.h"

namespace neon::ui {

LevelVisualizer::LevelVisualizer() { startTimerHz(45); }

void LevelVisualizer::setInputLevelDb(float db) noexcept {
  targetInputDb_.store(juce::jlimit(-90.0f, 6.0f, db),
                       std::memory_order_relaxed);
}

void LevelVisualizer::setOutputLevelDb(float db) noexcept {
  targetOutputDb_.store(juce::jlimit(-90.0f, 6.0f, db),
                        std::memory_order_relaxed);
}

void LevelVisualizer::paint(juce::Graphics &g) {
  auto area = getLocalBounds().toFloat();

  const auto frame =
      assets::image("gz_meter_top_visualizer_panel_frame_base_220x96_png");
  assets::drawIfValid(g, frame, area);

  auto inner = area.reduced(area.getWidth() * 0.08f, area.getHeight() * 0.20f);
  auto inRow = inner.removeFromTop(inner.getHeight() * 0.45f);
  inner.removeFromTop(area.getHeight() * 0.06f);
  auto outRow = inner;

  const auto inSprite =
      assets::image("gz_meter_in_bar_segment_sprite_base_18x64_png");
  const auto outSprite =
      assets::image("gz_meter_out_bar_segment_sprite_base_18x64_png");

  const auto inNorm = juce::jlimit(0.0f, 1.0f,
                                   juce::jmap(smoothedInput_, -90.0f, 6.0f,
                                              0.0f, 1.0f));
  const auto outNorm = juce::jlimit(0.0f, 1.0f,
                                    juce::jmap(smoothedOutput_, -90.0f, 6.0f,
                                               0.0f, 1.0f));

  constexpr int segments = 10;
  const auto gap = area.getWidth() * 0.01f;

  const auto drawRow = [&](const juce::Rectangle<float> row, float normalized,
                           const juce::Image &sprite) {
    const auto segW =
        (row.getWidth() - (gap * static_cast<float>(segments - 1))) /
        static_cast<float>(segments);
    const auto activeCount =
        static_cast<int>(std::round(normalized * static_cast<float>(segments)));

    for (int i = 0; i < segments; ++i) {
      auto seg = juce::Rectangle<float>(
          row.getX() + (static_cast<float>(i) * (segW + gap)), row.getY(), segW,
          row.getHeight());

      const float alpha = (i < activeCount) ? 0.98f : 0.10f;
      if (!assets::drawIfValid(g, sprite, seg, alpha)) {
        g.setColour(juce::Colours::white.withAlpha(alpha));
        g.fillRoundedRectangle(seg, 2.0f);
      }
    }
  };

  drawRow(inRow, inNorm, inSprite);
  drawRow(outRow, outNorm, outSprite);

  const auto readoutColor = juce::Colour::fromRGB(255, 176, 56).withAlpha(0.92f);
  g.setColour(readoutColor);
  g.setFont(fonts::makeDisplayFont(9.6f, "Heavy", 0.07f));

  g.drawText(juce::String(smoothedInput_, 1) + " dB",
             inRow.toNearestInt().reduced(2), juce::Justification::centredRight,
             false);
  g.drawText(juce::String(smoothedOutput_, 1) + " dB",
             outRow.toNearestInt().reduced(2), juce::Justification::centredRight,
             false);
}

void LevelVisualizer::timerCallback() {
  const auto inTarget = targetInputDb_.load(std::memory_order_relaxed);
  const auto outTarget = targetOutputDb_.load(std::memory_order_relaxed);

  bool needsRepaint = false;

  if (std::abs(inTarget - smoothedInput_) < 0.1f) {
    if (smoothedInput_ != inTarget) {
      smoothedInput_ = inTarget;
      needsRepaint = true;
    }
  } else {
    smoothedInput_ += (inTarget - smoothedInput_) * 0.28f;
    needsRepaint = true;
  }

  if (std::abs(outTarget - smoothedOutput_) < 0.1f) {
    if (smoothedOutput_ != outTarget) {
      smoothedOutput_ = outTarget;
      needsRepaint = true;
    }
  } else {
    smoothedOutput_ += (outTarget - smoothedOutput_) * 0.28f;
    needsRepaint = true;
  }

  if (needsRepaint) {
    repaint();
  }
}

} // namespace neon::ui
