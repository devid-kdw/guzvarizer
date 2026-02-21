#pragma once

#include <JuceHeader.h>

#include <atomic>

namespace neon::ui {

class GainReductionMeter : public juce::Component, private juce::Timer {
 public:
  GainReductionMeter();

  void setGainReductionDb(float db) noexcept;

  void paint(juce::Graphics& g) override;

 private:
  void timerCallback() override;

  static constexpr int kUiTimerHz = 45;
  static constexpr float kSmoothingTauSeconds = 0.08f;

  std::atomic<float> targetDb_{0.0f};
  float smoothedDb_ = 0.0f;
};

}  // namespace neon::ui
