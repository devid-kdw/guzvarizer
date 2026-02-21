#pragma once

#include <JuceHeader.h>

#include "src/ui/bindings/ParameterBindingAdapter.h"
#include "src/ui/components/shell/MainPanel.h"

namespace neon::host {
class GuzvarizerProcessor;
}

namespace neon::ui {

class PluginEditor : public juce::AudioProcessorEditor {
 public:
  explicit PluginEditor(neon::host::GuzvarizerProcessor& processor);

  void resized() override;

 private:
  void bindParameters();

  ParameterBindingAdapter bindingAdapter_;
  MainPanel mainPanel_;
};

}  // namespace neon::ui
