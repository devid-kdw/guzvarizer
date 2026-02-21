#pragma once

#include <JuceHeader.h>

#include "src/ui/theme/ThemeState.h"

namespace neon::ui {

class ThemeManager {
 public:
  ThemeManager() = default;

  const ThemeState& state() const noexcept { return state_; }
  void setState(const ThemeState& state) noexcept { state_ = state; }

  juce::Colour panelBase() const;
  juce::Colour panelEdge() const;
  juce::Colour accent() const;
  juce::Colour accentGlow() const;
  juce::Colour textPrimary() const;
  juce::Colour textSecondary() const;

 private:
  ThemeState state_{};
};

}  // namespace neon::ui
