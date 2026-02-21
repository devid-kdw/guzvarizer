#pragma once

#include <JuceHeader.h>

#include "src/ui/components/shell/MainPanel.h"

namespace neon::ui {

class PluginEditor : public juce::AudioProcessorEditor {
 public:
  explicit PluginEditor(juce::AudioProcessor& processor);

  void resized() override;

 private:
  MainPanel mainPanel_;
};

}  // namespace neon::ui
