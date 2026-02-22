#include "src/ui/components/shell/MainPanel.h"

#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

MainPanel::MainPanel()
    : headerBar_(themeManager_), mainSection_(themeManager_),
      toneShaperSection_(themeManager_), footerBar_(themeManager_),
      colorStrip_(themeManager_) {
  themeManager_.addListener(this);

  addAndMakeVisible(mainSection_);
  addAndMakeVisible(toneShaperSection_);
  addAndMakeVisible(colorStrip_);
  addAndMakeVisible(footerBar_);
  addAndMakeVisible(headerBar_);
}

MainPanel::~MainPanel() { themeManager_.removeListener(this); }

void MainPanel::paint(juce::Graphics& g) {
  const auto bounds = getLocalBounds().toFloat();

  const auto atmosphere =
      assets::image("gz_shell_background_atmosphere_base_1180x760_png");
  const auto outerFill = assets::image("gz_shell_outer_fill_center_base_988x568_png");
  const auto cornerTl = assets::image("gz_shell_outer_corner_tl_base_96x96_png");
  const auto cornerTr = assets::image("gz_shell_outer_corner_tr_base_96x96_png");
  const auto cornerBl = assets::image("gz_shell_outer_corner_bl_base_96x96_png");
  const auto cornerBr = assets::image("gz_shell_outer_corner_br_base_96x96_png");
  const auto edgeTop = assets::image("gz_shell_outer_edge_top_base_988x96_png");
  const auto edgeBottom = assets::image("gz_shell_outer_edge_bottom_base_988x96_png");
  const auto edgeLeft = assets::image("gz_shell_outer_edge_left_base_96x568_png");
  const auto edgeRight = assets::image("gz_shell_outer_edge_right_base_96x568_png");

  const auto headerFrame = assets::image("gz_shell_header_frame_base_1148x78_png");
  const auto mainFrame = assets::image("gz_shell_main_section_frame_base_1148x264_png");
  const auto toneFrame =
      assets::image("gz_shell_tone_shaper_section_frame_base_1148x220_png");
  const auto footerFrame = assets::image("gz_shell_footer_frame_base_1148x88_png");

  const auto sideLeft = assets::image("gz_shell_side_deco_left_glow_64x520_png");
  const auto sideRight = assets::image("gz_shell_side_deco_right_glow_64x520_png");

  assets::drawIfValid(g, atmosphere, bounds);

  const auto inner = bounds.reduced(96.0f);
  assets::drawIfValid(g, outerFill, inner);

  assets::drawIfValid(g, cornerTl,
                      juce::Rectangle<float>(bounds.getX(), bounds.getY(),
                                             96.0f, 96.0f));
  assets::drawIfValid(g, cornerTr,
                      juce::Rectangle<float>(bounds.getRight() - 96.0f,
                                             bounds.getY(), 96.0f, 96.0f));
  assets::drawIfValid(g, cornerBl,
                      juce::Rectangle<float>(bounds.getX(),
                                             bounds.getBottom() - 96.0f, 96.0f,
                                             96.0f));
  assets::drawIfValid(g, cornerBr,
                      juce::Rectangle<float>(bounds.getRight() - 96.0f,
                                             bounds.getBottom() - 96.0f, 96.0f,
                                             96.0f));

  assets::drawIfValid(g, edgeTop,
                      juce::Rectangle<float>(bounds.getX() + 96.0f,
                                             bounds.getY(),
                                             bounds.getWidth() - 192.0f,
                                             96.0f));
  assets::drawIfValid(g, edgeBottom,
                      juce::Rectangle<float>(bounds.getX() + 96.0f,
                                             bounds.getBottom() - 96.0f,
                                             bounds.getWidth() - 192.0f,
                                             96.0f));
  assets::drawIfValid(g, edgeLeft,
                      juce::Rectangle<float>(bounds.getX(), bounds.getY() + 96.0f,
                                             96.0f,
                                             bounds.getHeight() - 192.0f));
  assets::drawIfValid(g, edgeRight,
                      juce::Rectangle<float>(bounds.getRight() - 96.0f,
                                             bounds.getY() + 96.0f, 96.0f,
                                             bounds.getHeight() - 192.0f));

  assets::drawIfValid(g, headerFrame, headerBar_.getBounds().toFloat());
  assets::drawIfValid(g, mainFrame, mainSection_.getBounds().toFloat());
  assets::drawIfValid(g, toneFrame, toneShaperSection_.getBounds().toFloat());
  assets::drawIfValid(g, footerFrame, footerBar_.getBounds().toFloat());

  const auto sideY = 118.0f;
  assets::drawIfValid(g, sideLeft,
                      juce::Rectangle<float>(40.0f, sideY, 64.0f, 520.0f));
  assets::drawIfValid(g, sideRight,
                      juce::Rectangle<float>(bounds.getRight() - 104.0f, sideY,
                                             64.0f, 520.0f));

  assets::drawIfValid(g,
                      assets::image("gz_overlay_scanline_full_base_1180x760_png"),
                      bounds, 0.18f);
  assets::drawIfValid(g,
                      assets::image("gz_overlay_grain_full_base_1180x760_png"),
                      bounds, 0.20f);
  assets::drawIfValid(g,
                      assets::image("gz_overlay_vignette_full_base_1180x760_png"),
                      bounds, 0.60f);
}

void MainPanel::resized() {
  constexpr float baseW = 1180.0f;
  constexpr float baseH = 760.0f;

  const auto sx = static_cast<float>(getWidth()) / baseW;
  const auto sy = static_cast<float>(getHeight()) / baseH;

  const auto scaled = [sx, sy](float x, float y, float w,
                               float h) -> juce::Rectangle<int> {
    return juce::Rectangle<int>(
        juce::roundToInt(x * sx), juce::roundToInt(y * sy),
        juce::roundToInt(w * sx), juce::roundToInt(h * sy));
  };

  headerBar_.setBounds(scaled(16.0f, 16.0f, 1148.0f, 78.0f));
  mainSection_.setBounds(scaled(16.0f, 106.0f, 1148.0f, 264.0f));
  colorStrip_.setBounds(scaled(100.0f, 382.0f, 980.0f, 22.0f));
  toneShaperSection_.setBounds(scaled(16.0f, 418.0f, 1148.0f, 220.0f));
  footerBar_.setBounds(scaled(16.0f, 656.0f, 1148.0f, 88.0f));
}

void MainPanel::themeChanged() { repaint(); }

}  // namespace neon::ui
