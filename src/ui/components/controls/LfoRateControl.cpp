#include "src/ui/components/controls/LfoRateControl.h"

#include "src/shared/types/ParameterIds.h"
#include "src/ui/assets/UiAssetRepository.h"

namespace neon::ui {

LfoRateControl::LfoRateControl(ThemeManager &themeManager)
    : themeManager_(themeManager),
      rateKnob_({"LFO RATE", params::kLfoRateHz, 0.05, 20.0, 2.0,
                 NeonKnob::Mapping::kLogarithmic,
                 juce::Colour::fromRGB(255, 255, 92), false, "lfo_rate", true}) {
  themeManager_.addListener(this);
  addAndMakeVisible(rateKnob_);

  syncButton_.setButtonText({});
  syncButton_.setClickingTogglesState(true);
  syncButton_.setAlpha(0.0f);
  syncButton_.onClick = [this] {
    updateVisibility();
    repaint();
  };
  addAndMakeVisible(syncButton_);

  for (size_t i = 0; i < neon::kLfoSyncDivisionLabels.size(); ++i) {
    divisionCombo_.addItem(neon::kLfoSyncDivisionLabels[i],
                           static_cast<int>(i) + 1);
  }
  addAndMakeVisible(divisionCombo_);

  setGuzvanjeActive(false);
  updateVisibility();
}

LfoRateControl::~LfoRateControl() { themeManager_.removeListener(this); }

void LfoRateControl::setSyncMode(bool isSync,
                                 juce::NotificationType notification) {
  syncButton_.setToggleState(isSync, notification);
  updateVisibility();
  repaint();
}

bool LfoRateControl::isSyncMode() const noexcept {
  return syncButton_.getToggleState();
}

void LfoRateControl::setGuzvanjeActive(bool active) {
  if (isGuzvanjeActive_ != active) {
    isGuzvanjeActive_ = active;

    rateKnob_.setEnabled(active);
    rateKnob_.setAlpha(active ? 1.0f : 0.55f);

    syncButton_.setEnabled(active);
    syncButton_.setAlpha(active ? 1.0f : 0.65f);
    divisionCombo_.setEnabled(active);
    divisionCombo_.setAlpha(active ? 1.0f : 0.55f);

    updateVisibility();
    repaint();
  }
}

void LfoRateControl::updateVisibility() {
  const bool isSync = syncButton_.getToggleState();
  rateKnob_.setVisible(!isSync);
  divisionCombo_.setVisible(isSync);
}

void LfoRateControl::resized() {
  rateKnob_.setBounds(0, -1, 96, 82);
  syncButton_.setBounds(112, 10, 34, 34);

  if (isSyncMode()) {
    divisionCombo_.setBounds(112, 18, 96, 28);
  } else {
    divisionCombo_.setBounds({});
  }
}

void LfoRateControl::paint(juce::Graphics &g) {
  const auto syncOff = assets::image("gz_control_sync_checkbox_off_88x88_png");
  const auto syncOn = assets::image("gz_control_sync_checkbox_on_88x88_png");
  const auto syncDisabled =
      assets::image("gz_control_sync_checkbox_disabled_88x88_png");

  const auto syncBounds = syncButton_.getBounds().toFloat();
  if (!syncButton_.isEnabled() && syncDisabled.isValid()) {
    assets::drawIfValid(g, syncDisabled, syncBounds);
  } else {
    assets::drawIfValid(g, isSyncMode() ? syncOn : syncOff, syncBounds);
  }
}

void LfoRateControl::themeChanged() {
  divisionCombo_.setColour(juce::ComboBox::outlineColourId,
                           themeManager_.accent().withAlpha(0.35f));
  divisionCombo_.setColour(juce::ComboBox::backgroundColourId,
                           themeManager_.panelBase().darker(0.55f));
  repaint();
}

} // namespace neon::ui
