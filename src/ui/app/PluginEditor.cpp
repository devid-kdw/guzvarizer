#include "src/ui/app/PluginEditor.h"

#include "src/host/GuzvarizerProcessor.h"
#include "src/shared/constants/UiMetrics.h"
#include "src/shared/types/ParameterIds.h"

namespace neon::ui {

PluginEditor::PluginEditor(neon::host::GuzvarizerProcessor& processor)
    : juce::AudioProcessorEditor(processor), bindingAdapter_(processor.parameters()) {
  setResizable(true, true);
  setResizeLimits(metrics::kMinEditorWidth, metrics::kMinEditorHeight, 1920, 1200);
  setSize(metrics::kDefaultEditorWidth, metrics::kDefaultEditorHeight);

  addAndMakeVisible(mainPanel_);
  bindParameters();
}

void PluginEditor::resized() {
  mainPanel_.setBounds(getLocalBounds());
}

void PluginEditor::bindParameters() {
  for (auto* knob : mainPanel_.mainSection().compressorKnobs()) {
    bindingAdapter_.bindKnob(*knob);
  }

  for (auto* knob : mainPanel_.toneShaperSection().toneKnobs()) {
    bindingAdapter_.bindKnob(*knob);
  }

  bindingAdapter_.bindBypass(mainPanel_.headerBar().bypassToggle(), params::kBypass);
  bindingAdapter_.bindVibeMode(mainPanel_.footerBar().vibeModeControl(), params::kVibeMode);
}

}  // namespace neon::ui
