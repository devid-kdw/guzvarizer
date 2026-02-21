#include "src/dsp/util/LfoOscillator.h"

#include <cmath>

namespace neon::dsp {

namespace {
constexpr float kTwoPi = 6.283185307f;
}

void LfoOscillator::prepare(double sampleRate) noexcept {
  sampleRate_ = sampleRate > 0.0 ? sampleRate : 44100.0;
}

void LfoOscillator::reset(float phase) noexcept {
  phase_ = phase;
}

void LfoOscillator::setFrequency(float frequencyHz) noexcept {
  frequencyHz_ = frequencyHz;
}

float LfoOscillator::process() noexcept {
  const auto output = std::sin(phase_ * kTwoPi);
  const auto increment = static_cast<float>(frequencyHz_ / sampleRate_);
  phase_ += increment;

  if (phase_ >= 1.0f) {
    phase_ -= 1.0f;
  }

  return output;
}

}  // namespace neon::dsp
