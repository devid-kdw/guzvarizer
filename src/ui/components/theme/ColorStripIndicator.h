#pragma once

#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class ColorStripIndicator : public juce::Component,
                            private ThemeManager::Listener {
public:
  explicit ColorStripIndicator(ThemeManager &themeManager);
  ~ColorStripIndicator() override;

  void setReactiveLevel(float normalized) noexcept;

  void paint(juce::Graphics &g) override;

private:
  void themeChanged() override;

  ThemeManager &themeManager_;
  float reactiveLevel_ = 0.35f;
};

} // namespace neon::ui
