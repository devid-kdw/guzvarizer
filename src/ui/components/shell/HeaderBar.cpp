#include "src/ui/components/shell/HeaderBar.h"

#include "src/ui/theme/UiTypography.h"

namespace neon::ui {

HeaderBar::HeaderBar(ThemeManager &themeManager)
    : themeManager_(themeManager),
      title_(themeManager, juce::String::fromUTF8("GUŽVARIZER")) {
  title_.setJustification(juce::Justification::centred);
  themeManager_.addListener(this);
  addAndMakeVisible(title_);
  title_.setVisible(false);
  addAndMakeVisible(bypassToggle_);
}

HeaderBar::~HeaderBar() { themeManager_.removeListener(this); }

void HeaderBar::setPluginName(const juce::String &name) {
  pluginName_ = name.toUpperCase();
  title_.setText(name.toUpperCase());
}

void HeaderBar::setBypass(bool bypassed, juce::NotificationType notification) {
  bypassToggle_.setActive(bypassed, notification);
}

void HeaderBar::setBypassChanged(std::function<void(bool)> callback) {
  bypassToggle_.setOnToggle(std::move(callback));
}

void HeaderBar::paint(juce::Graphics &g) {
  auto titleArea = getLocalBounds().reduced(220, 8).toFloat();
  titleArea.translate(0.0f, -1.0f);

  g.setFont(fonts::makeDisplayFont(24.5f, "Heavy", 0.08f));
  g.setColour(juce::Colour::fromRGB(255, 176, 56).withAlpha(0.30f));
  g.drawText(pluginName_, titleArea.translated(0.0f, 1.2f),
             juce::Justification::centred, true);

  g.setColour(juce::Colour::fromRGB(255, 182, 64).withAlpha(0.98f));
  g.drawText(pluginName_, titleArea, juce::Justification::centred, true);
}

void HeaderBar::resized() {
  auto area = getLocalBounds();
  title_.setBounds(area.reduced(220, 8));

  constexpr int buttonW = 160;
  constexpr int buttonH = 48;
  bypassToggle_.setBounds(area.getWidth() - buttonW - 24,
                          (area.getHeight() - buttonH) / 2, buttonW, buttonH);
}

void HeaderBar::themeChanged() { repaint(); }

} // namespace neon::ui
