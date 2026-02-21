#include "src/ui/app/PluginEditor.h"

#include "src/shared/constants/UiMetrics.h"

namespace neon::ui {

PluginEditor::PluginEditor(juce::AudioProcessor& processor) : juce::AudioProcessorEditor(processor) {
  setResizable(true, true);
  setResizeLimits(metrics::kMinEditorWidth, metrics::kMinEditorHeight, 1920, 1200);
  setSize(metrics::kDefaultEditorWidth, metrics::kDefaultEditorHeight);

  addAndMakeVisible(mainPanel_);
}

void PluginEditor::resized() {
  mainPanel_.setBounds(getLocalBounds());
}

}  // namespace neon::ui
