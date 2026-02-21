#pragma once

#include <JuceHeader.h>

#include "src/ui/theme/ThemeState.h"

namespace neon::ui {

class ThemeManager {
 public:
  class Listener {
   public:
    virtual ~Listener() = default;
    virtual void themeChanged() = 0;
  };

  ThemeManager() = default;

  const ThemeState& state() const noexcept { return state_; }
  void setState(const ThemeState& state);

  void addListener(Listener* listener);
  void removeListener(Listener* listener);

  juce::Colour panelBase() const;
  juce::Colour panelEdge() const;
  juce::Colour accent() const;
  juce::Colour accentGlow() const;
  juce::Colour textPrimary() const;
  juce::Colour textSecondary() const;

 private:
  ThemeState state_{};
  juce::ListenerList<Listener> listeners_;
};

}  // namespace neon::ui
