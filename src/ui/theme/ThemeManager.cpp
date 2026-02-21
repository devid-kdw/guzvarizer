#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

namespace {

float to01(float value) {
  return juce::jlimit(0.0f, 1.0f, value / 100.0f);
}

}  // namespace

void ThemeManager::setState(const ThemeState& state) {
  state_ = state;
  listeners_.call(&ThemeManager::Listener::themeChanged);
}

void ThemeManager::addListener(Listener* listener) {
  listeners_.add(listener);
}

void ThemeManager::removeListener(Listener* listener) {
  listeners_.remove(listener);
}

juce::Colour ThemeManager::panelBase() const {
  const auto brightness = juce::jmap(to01(state_.panelBrightness), 0.08f, 0.28f);
  return juce::Colour::fromHSV(0.58f, 0.25f, brightness, 1.0f);
}

juce::Colour ThemeManager::panelEdge() const {
  return panelBase().brighter(0.28f);
}

juce::Colour ThemeManager::accent() const {
  const auto hue = state_.accentHue / 360.0f;
  const auto saturation = juce::jmap(to01(state_.neonSaturation), 0.4f, 1.0f);
  return juce::Colour::fromHSV(hue, saturation, 0.94f, 1.0f);
}

juce::Colour ThemeManager::accentGlow() const {
  return accent().withAlpha(juce::jmap(to01(state_.glowIntensity), 0.28f, 0.9f));
}

juce::Colour ThemeManager::textPrimary() const {
  return juce::Colours::white.withAlpha(0.93f);
}

juce::Colour ThemeManager::textSecondary() const {
  return juce::Colours::white.withAlpha(0.68f);
}

}  // namespace neon::ui
