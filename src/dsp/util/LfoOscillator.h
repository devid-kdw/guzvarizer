#pragma once

namespace neon::dsp {

class LfoOscillator {
 public:
  void prepare(double sampleRate) noexcept;
  void reset(float phase = 0.0f) noexcept;

  void setFrequency(float frequencyHz) noexcept;
  float process() noexcept;

 private:
  double sampleRate_ = 44100.0;
  float frequencyHz_ = 1.0f;
  float phase_ = 0.0f;
};

}  // namespace neon::dsp
