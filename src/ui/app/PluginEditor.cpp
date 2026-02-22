#include "src/ui/app/PluginEditor.h"

#include "src/host/GuzvarizerProcessor.h"
#include "src/shared/constants/UiMetrics.h"
#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

PluginEditor::PluginEditor(neon::host::GuzvarizerProcessor &processor)
    : juce::AudioProcessorEditor(processor), processor_(processor),
      bindingAdapter_(processor.parameters()),
      meterClient_(processor.meterSource()) {
  setResizable(true, true);
  setResizeLimits(metrics::kMinEditorWidth, metrics::kMinEditorHeight, 1920,
                  1200);
  setSize(metrics::kDefaultEditorWidth, metrics::kDefaultEditorHeight);

  addAndMakeVisible(mainPanel_);
  bindParameters();

  processor_.parameters().addParameterListener(params::kAccentHue, this);
  processor_.parameters().addParameterListener(params::kNeonSaturation, this);
  processor_.parameters().addParameterListener(params::kGlowIntensity, this);
  processor_.parameters().addParameterListener(params::kPanelBrightness, this);
  processor_.parameters().addParameterListener(params::kGuzvanjeEnabled, this);
  processor_.parameters().addParameterListener(params::kLfoSyncEnabled, this);

  updateThemeState();

  startTimerHz(30);
}

PluginEditor::~PluginEditor() {
  processor_.parameters().removeParameterListener(params::kAccentHue, this);
  processor_.parameters().removeParameterListener(params::kNeonSaturation,
                                                  this);
  processor_.parameters().removeParameterListener(params::kGlowIntensity, this);
  processor_.parameters().removeParameterListener(params::kPanelBrightness,
                                                  this);
  processor_.parameters().removeParameterListener(params::kGuzvanjeEnabled,
                                                  this);
  processor_.parameters().removeParameterListener(params::kLfoSyncEnabled,
                                                  this);
}

void PluginEditor::resized() { mainPanel_.setBounds(getLocalBounds()); }

void PluginEditor::bindParameters() {
  for (auto *knob : mainPanel_.mainSection().compressorKnobs()) {
    bindingAdapter_.bindKnob(*knob);
  }

  for (auto *knob : mainPanel_.toneShaperSection().toneKnobs()) {
    bindingAdapter_.bindKnob(*knob);
  }

  bindingAdapter_.bindBypass(mainPanel_.headerBar().bypassToggle(),
                             params::kBypass);
  bindingAdapter_.bindVibeMode(mainPanel_.footerBar().vibeModeControl(),
                               params::kVibeMode);
  bindingAdapter_.bindGuzvanje(mainPanel_.footerBar().guzvanjeToggle(),
                               params::kGuzvanjeEnabled);
  bindingAdapter_.bindKnob(mainPanel_.footerBar().lfoRateControl().rateKnob());
  bindingAdapter_.bindLfoSyncEnabled(
      mainPanel_.footerBar().lfoRateControl().syncButton(),
      params::kLfoSyncEnabled);
  bindingAdapter_.bindLfoSyncDivision(
      mainPanel_.footerBar().lfoRateControl().divisionCombo(),
      params::kLfoSyncDivision);

  mainPanel_.footerBar().guzvanjeToggle().setOnToggle([this](bool active) {
    mainPanel_.footerBar().lfoRateControl().setGuzvanjeActive(active);
  });
}

void PluginEditor::timerCallback() {
  const auto snap = meterClient_.getLatestSnapshot();
  auto &mainSec = mainPanel_.mainSection();

  mainSec.gainReductionMeter().setGainReductionDb(snap.gainReductionDb);
  mainSec.levelVisualizer().setInputLevelDb(snap.inputLevelDb);
  mainSec.levelVisualizer().setOutputLevelDb(snap.outputLevelDb);
  mainPanel_.colorStrip().setReactiveLevel(snap.rmsNormalized);
}

void PluginEditor::parameterChanged(const juce::String &parameterID,
                                    float /*newValue*/) {
  if (parameterID == params::kAccentHue ||
      parameterID == params::kNeonSaturation ||
      parameterID == params::kGlowIntensity ||
      parameterID == params::kPanelBrightness) {
    juce::MessageManager::callAsync([this]() { updateThemeState(); });
  } else if (parameterID == params::kGuzvanjeEnabled) {
    const bool active = processor_.parameters()
                            .getRawParameterValue(params::kGuzvanjeEnabled)
                            ->load() > 0.5f;
    juce::MessageManager::callAsync([this, active]() {
      mainPanel_.footerBar().lfoRateControl().setGuzvanjeActive(active);
    });
  } else if (parameterID == params::kLfoSyncEnabled) {
    const bool sync = processor_.parameters()
                          .getRawParameterValue(params::kLfoSyncEnabled)
                          ->load() > 0.5f;
    juce::MessageManager::callAsync([this, sync]() {
      mainPanel_.footerBar().lfoRateControl().setSyncMode(sync);
    });
  }
}

void PluginEditor::updateThemeState() {
  const auto getParamVal = [this](const juce::String &id) {
    return processor_.parameters().getRawParameterValue(id)->load();
  };

  ThemeState state;
  state.accentHue = getParamVal(params::kAccentHue);
  state.neonSaturation = getParamVal(params::kNeonSaturation);
  state.glowIntensity = getParamVal(params::kGlowIntensity);
  state.panelBrightness = getParamVal(params::kPanelBrightness);

  mainPanel_.themeManager().setState(state);
}

} // namespace neon::ui
