#pragma once

#include <JuceHeader.h>

#include <functional>

namespace neon::ui {

class NeonKnob : public juce::Component, private juce::Slider::Listener {
 public:
  enum class Mapping {
    kLinear,
    kLogarithmic,
  };

  struct Config {
    juce::String label;
    juce::String parameterId;
    double minValue = 0.0;
    double maxValue = 1.0;
    double defaultValue = 0.5;
    Mapping mapping = Mapping::kLinear;
    juce::Colour ringColour = juce::Colour::fromRGB(0, 255, 210);
    bool showValueReadout = false;
    juce::String assetId;
    bool showLabel = true;
  };

  explicit NeonKnob(Config config);
  ~NeonKnob() override;

  void setOnValueChanged(std::function<void(double)> callback);
  void setValue(double value,
                juce::NotificationType notification = juce::sendNotificationSync);
  double value() const noexcept;
  double minValue() const noexcept;
  double maxValue() const noexcept;
  const juce::String& parameterId() const noexcept;

  juce::Slider& slider() noexcept { return knob_; }
  const juce::Slider& slider() const noexcept { return knob_; }

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  void sliderValueChanged(juce::Slider* slider) override;
  void updateValueLabel();
  void loadAssets();

  Config config_;
  juce::Slider knob_;
  juce::Label label_;
  juce::Label valueLabel_;
  std::function<void(double)> onValueChanged_;

  juce::Image ringBase_;
  juce::Image ringActive_;
  juce::Image ticks_;
  juce::Image cap_;
  juce::Image pointer_;
  juce::Image glow_;
};

}  // namespace neon::ui
