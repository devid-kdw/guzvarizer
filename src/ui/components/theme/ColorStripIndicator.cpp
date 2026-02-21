#include "src/ui/components/theme/ColorStripIndicator.h"

namespace neon::ui {

void ColorStripIndicator::setReactiveLevel(float normalized) noexcept {
  reactiveLevel_ = juce::jlimit(0.0f, 1.0f, normalized);
  repaint();
}

void ColorStripIndicator::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat().reduced(2.0f);

  juce::ColourGradient gradient(juce::Colour::fromRGB(255, 24, 138), area.getTopLeft(),
                                juce::Colour::fromRGB(0, 240, 255), area.getTopRight(), false);
  gradient.addColour(0.55, juce::Colour::fromRGB(255, 170, 42));

  g.setGradientFill(gradient);
  g.fillRoundedRectangle(area, 5.0f);

  const auto cursorX = juce::jmap(reactiveLevel_, 0.0f, 1.0f, area.getX(), area.getRight());
  auto cursor = juce::Rectangle<float>(cursorX - 10.0f, area.getY(), 20.0f, area.getHeight());
  g.setColour(juce::Colours::white.withAlpha(0.32f));
  g.fillRoundedRectangle(cursor, 4.0f);

  g.setColour(juce::Colours::white.withAlpha(0.34f));
  g.drawRoundedRectangle(area, 5.0f, 1.0f);
}

}  // namespace neon::ui
