#pragma once

#include <JuceHeader.h>

#include "src/host/bridge/AtomicMeterBridgeClient.h"
#include "src/ui/bindings/ParameterBindingAdapter.h"
#include "src/ui/components/shell/MainPanel.h"

namespace neon::host {
class GuzvarizerProcessor;
}

namespace neon::ui {

class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer,
                     private juce::AudioProcessorValueTreeState::Listener {
public:
  explicit PluginEditor(neon::host::GuzvarizerProcessor &processor);
  ~PluginEditor() override;

  void resized() override;

private:
  void bindParameters();
  void timerCallback() override;
  void parameterChanged(const juce::String &parameterID,
                        float newValue) override;
  void updateThemeState();

  neon::host::GuzvarizerProcessor &processor_;
  MainPanel mainPanel_;
  ParameterBindingAdapter bindingAdapter_;
  neon::host::AtomicMeterBridgeClient meterClient_;
};

} // namespace neon::ui
