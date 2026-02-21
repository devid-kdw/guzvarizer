#include "src/ui/components/metering/LevelVisualizer.h"

namespace neon::ui {

LevelVisualizer::LevelVisualizer() {
  startTimerHz(45);
}

void LevelVisualizer::setInputLevelDb(float db) noexcept {
  targetInputDb_.store(juce::jlimit(-90.0f, 6.0f, db), std::memory_order_relaxed);
}

void LevelVisualizer::setOutputLevelDb(float db) noexcept {
  targetOutputDb_.store(juce::jlimit(-90.0f, 6.0f, db), std::memory_order_relaxed);
}

void LevelVisualizer::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat().reduced(4.0f);
  g.setColour(juce::Colour::fromRGB(10, 16, 26));
  g.fillRoundedRectangle(area, 5.0f);

  g.setColour(juce::Colours::white.withAlpha(0.18f));
  g.drawRoundedRectangle(area, 5.0f, 1.0f);

  const auto inNorm = juce::jmap(smoothedInput_, -90.0f, 6.0f, 0.0f, 1.0f);
  const auto outNorm = juce::jmap(smoothedOutput_, -90.0f, 6.0f, 0.0f, 1.0f);

  auto barArea = area.reduced(10.0f, 8.0f);
  auto inBar = barArea.removeFromTop((barArea.getHeight() - 6.0f) * 0.5f);
  barArea.removeFromTop(6.0f);
  auto outBar = barArea;

  g.setColour(juce::Colours::white.withAlpha(0.1f));
  g.fillRoundedRectangle(inBar, 2.0f);
  g.fillRoundedRectangle(outBar, 2.0f);

  g.setColour(juce::Colour::fromRGB(0, 220, 255).withAlpha(0.85f));
  g.fillRoundedRectangle(inBar.withWidth(inBar.getWidth() * inNorm), 2.0f);

  g.setColour(juce::Colour::fromRGB(255, 88, 160).withAlpha(0.85f));
  g.fillRoundedRectangle(outBar.withWidth(outBar.getWidth() * outNorm), 2.0f);

  g.setFont(11.0f);
  g.setColour(juce::Colours::white.withAlpha(0.65f));
  g.drawText("IN", inBar.toNearestInt().translated(-2, -14), juce::Justification::topLeft, false);
  g.drawText("OUT", outBar.toNearestInt().translated(-2, -14), juce::Justification::topLeft, false);
}

void LevelVisualizer::timerCallback() {
  const auto inTarget = targetInputDb_.load(std::memory_order_relaxed);
  const auto outTarget = targetOutputDb_.load(std::memory_order_relaxed);

  smoothedInput_ += (inTarget - smoothedInput_) * 0.28f;
  smoothedOutput_ += (outTarget - smoothedOutput_) * 0.28f;

  repaint();
}

}  // namespace neon::ui
