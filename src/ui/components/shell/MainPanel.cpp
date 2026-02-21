#include "src/ui/components/shell/MainPanel.h"

#include "src/shared/constants/UiMetrics.h"

namespace neon::ui {

MainPanel::MainPanel() {
  addAndMakeVisible(headerBar_);
  addAndMakeVisible(mainSection_);
  addAndMakeVisible(colorStrip_);
  addAndMakeVisible(toneShaperSection_);
  addAndMakeVisible(footerBar_);
}

void MainPanel::paint(juce::Graphics& g) {
  auto area = getLocalBounds().toFloat();

  juce::ColourGradient bg(themeManager_.panelBase().darker(0.65f), area.getTopLeft(),
                          themeManager_.panelBase().brighter(0.2f), area.getBottomRight(), false);
  g.setGradientFill(bg);
  g.fillRoundedRectangle(area, metrics::kPanelCornerRadius);

  juce::Path frame;
  frame.addRoundedRectangle(area.reduced(2.0f), metrics::kPanelCornerRadius);
  g.setColour(themeManager_.accentGlow().withAlpha(0.25f));
  g.strokePath(frame, juce::PathStrokeType(1.0f));

  g.setColour(juce::Colours::black.withAlpha(0.3f));
  for (int y = 0; y < getHeight(); y += 4) {
    g.drawHorizontalLine(y, 0.0f, static_cast<float>(getWidth()));
  }
}

void MainPanel::resized() {
  auto area = getLocalBounds().reduced(metrics::kOuterPadding);

  headerBar_.setBounds(area.removeFromTop(metrics::kHeaderHeight));
  area.removeFromTop(metrics::kSectionGap);

  footerBar_.setBounds(area.removeFromBottom(metrics::kFooterHeight));
  area.removeFromBottom(metrics::kSectionGap);

  toneShaperSection_.setBounds(area.removeFromBottom(220));
  area.removeFromBottom(metrics::kSectionGap);

  colorStrip_.setBounds(area.removeFromBottom(metrics::kColorStripHeight));
  area.removeFromBottom(metrics::kSectionGap);

  mainSection_.setBounds(area);
}

}  // namespace neon::ui
