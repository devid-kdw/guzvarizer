#include "src/ui/components/theme/ColorStripIndicator.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

ColorStripIndicator::ColorStripIndicator(ThemeManager &themeManager)
    : themeManager_(themeManager) {
  themeManager_.addListener(this);
}

ColorStripIndicator::~ColorStripIndicator() { themeManager_.removeListener(this); }

void ColorStripIndicator::setReactiveLevel(float normalized) noexcept {
  const float target = juce::jlimit(0.0f, 1.0f, normalized);
  if (std::abs(target - reactiveLevel_) > 0.005f) {
    reactiveLevel_ = target;
    repaint();
  }
}

void ColorStripIndicator::paint(juce::Graphics &g) {
  auto area = getLocalBounds().toFloat();

  const auto base = assets::image("gz_strip_color_strip_base_base_980x22_png");
  const auto mask =
      assets::image("gz_strip_color_strip_segment_mask_base_980x22_png");
  const auto hotspot =
      assets::image("gz_strip_color_strip_hotspot_glow_72x32_png");

  assets::drawIfValid(g, base, area);
  assets::drawIfValid(g, mask, area, 0.92f);

  const auto x = juce::jmap(reactiveLevel_, 0.0f, 1.0f, area.getX(), area.getRight());
  const auto hotspotBounds = juce::Rectangle<float>(x - 22.0f, area.getCentreY() - 10.0f,
                                                    44.0f, 20.0f);
  assets::drawIfValid(g, hotspot, hotspotBounds, 0.95f);
}

void ColorStripIndicator::themeChanged() { repaint(); }

} // namespace neon::ui
