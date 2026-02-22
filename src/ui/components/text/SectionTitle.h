#pragma once

#include "src/ui/components/metering/LevelVisualizer.h"
#include "src/ui/components/text/SectionTitle.h"
#include "src/ui/theme/ThemeManager.h"

namespace neon::ui {

class SectionTitle : public juce::Component, private ThemeManager::Listener {
public:
  explicit SectionTitle(ThemeManager &themeManager, juce::String text);
  ~SectionTitle() override;

  void setText(const juce::String &text);
  void setJustification(juce::Justification justification);

  void paint(juce::Graphics &g) override;
  void themeChanged() override;

private:
  ThemeManager &themeManager_;
  juce::String text_;
  juce::Justification justification_ = juce::Justification::centredLeft;
};

} // namespace neon::ui
