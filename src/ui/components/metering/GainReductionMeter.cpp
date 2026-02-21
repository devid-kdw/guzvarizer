#include "src/ui/components/metering/GainReductionMeter.h"

namespace neon::ui {

GainReductionMeter::GainReductionMeter() {
  startTimerHz(45);
}

void GainReductionMeter::setGainReductionDb(float db) noexcept {
  targetDb_.store(juce::jlimit(-30.0f, 0.0f, db), std::memory_order_relaxed);
}

void GainReductionMeter::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat().reduced(4.0f);
  g.setColour(juce::Colour::fromRGB(8, 12, 18));
  g.fillRoundedRectangle(area, 6.0f);

  g.setColour(juce::Colours::white.withAlpha(0.2f));
  g.drawRoundedRectangle(area, 6.0f, 1.0f);

  const auto normalized = juce::jmap(smoothedDb_, -30.0f, 0.0f, 1.0f, 0.0f);
  const auto fillHeight = area.getHeight() * normalized;
  auto fillArea = area.withY(area.getBottom() - fillHeight).withHeight(fillHeight);

  juce::ColourGradient gradient(juce::Colour::fromRGB(255, 64, 140), fillArea.getTopLeft(),
                                juce::Colour::fromRGB(0, 255, 180), fillArea.getBottomLeft(), false);
  g.setGradientFill(gradient);
  g.fillRoundedRectangle(fillArea, 4.0f);

  g.setColour(juce::Colours::white.withAlpha(0.5f));
  g.setFont(11.0f);
  g.drawText("0", getLocalBounds().removeFromTop(14), juce::Justification::centredRight, false);
  g.drawText("-10", getLocalBounds().withTrimmedTop(getHeight() / 3), juce::Justification::centredRight, false);
  g.drawText("-20", getLocalBounds().withTrimmedTop((getHeight() * 2) / 3), juce::Justification::centredRight, false);
}

void GainReductionMeter::timerCallback() {
  const auto target = targetDb_.load(std::memory_order_relaxed);
  smoothedDb_ += (target - smoothedDb_) * 0.22f;
  repaint();
}

}  // namespace neon::ui
