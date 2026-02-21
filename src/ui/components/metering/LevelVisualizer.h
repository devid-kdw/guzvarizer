#pragma once

#include <JuceHeader.h>

#include <atomic>

namespace neon::ui {

class LevelVisualizer : public juce::Component, private juce::Timer {
 public:
  LevelVisualizer();

  void setInputLevelDb(float db) noexcept;
  void setOutputLevelDb(float db) noexcept;

  void paint(juce::Graphics& g) override;

 private:
  void timerCallback() override;

  std::atomic<float> targetInputDb_{-90.0f};
  std::atomic<float> targetOutputDb_{-90.0f};
  float smoothedInput_ = -90.0f;
  float smoothedOutput_ = -90.0f;
};

}  // namespace neon::ui
