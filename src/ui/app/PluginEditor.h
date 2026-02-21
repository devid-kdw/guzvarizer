#pragma once

#include <JuceHeader.h>

#include "src/host/bridge/AtomicMeterBridgeClient.h"
#include "src/ui/bindings/ParameterBindingAdapter.h"
#include "src/ui/components/shell/MainPanel.h"

namespace neon::host {
class GuzvarizerProcessor;
}

namespace neon::ui {

class PluginEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
  explicit PluginEditor(neon::host::GuzvarizerProcessor &processor);

  void resized() override;

private:
  void bindParameters();
  void timerCallback() override;

  ParameterBindingAdapter bindingAdapter_;
  neon::host::AtomicMeterBridgeClient meterClient_;
  MainPanel mainPanel_;
};

} // namespace neon::ui
