#include "src/dsp/tone_shaper/ToneShaper.h"

#include <algorithm>
#include <cmath>

namespace neon::dsp {

namespace {
constexpr float kPi = 3.14159265358979323846f;
}

void ToneShaper::prepare(double sampleRate, int /*maxBlockSize*/) {
  sampleRate_ = sampleRate > 0.0 ? sampleRate : 44100.0;
  updateFilterCoefficients();
}

void ToneShaper::reset() {
  hsfStateL_ = 0.0f;
  hsfStateR_ = 0.0f;
}

void ToneShaper::setParams(const ToneShaperParams &params) noexcept {
  const bool freqChanged = (params.airFreq != params_.airFreq);
  params_ = params;
  if (freqChanged) {
    updateFilterCoefficients();
  }
}

void ToneShaper::updateFilterCoefficients() noexcept {
  // Simple 1st-order high-shelf coefficient
  // alpha = exp(-2*pi*freq/sr) — controls the crossover frequency
  if (sampleRate_ > 0.0 && params_.airFreq > 0.0f) {
    const float wc =
        2.0f * kPi * params_.airFreq / static_cast<float>(sampleRate_);
    hsfAlpha_ = std::exp(-wc);
  } else {
    hsfAlpha_ = 0.0f;
  }
}

float ToneShaper::waveshape(float sample, float drive01, float harmonics01,
                            float grit01) noexcept {
  if (drive01 < 0.001f)
    return sample;

  // Scale drive: 0-100% → 1.0-10.0 multiplier
  const float driveGain = 1.0f + drive01 * 0.09f;
  float x = sample * driveGain;

  // Soft-clip via tanh
  float shaped = std::tanh(x);

  // Blend harmonics: mix between clean and shaped
  float out = sample + (shaped - sample) * harmonics01;

  // Grit: asymmetric distortion — positive half gets extra push
  if (grit01 > 0.001f) {
    const float gritAmount = grit01 * 0.3f;
    if (out > 0.0f) {
      out += gritAmount * out * out; // soft asymmetry
      out = std::tanh(out);          // re-clip
    }
  }

  return out;
}

void ToneShaper::processBlock(float *left, float *right,
                              int numSamples) noexcept {
  const float drive01 = params_.saturationDrive * 0.01f;
  const float harm01 = params_.harmonics * 0.01f;
  const float grit01 = params_.grit * 0.01f;
  const float air01 = params_.air * 0.01f;

  // Skip processing entirely if all params are near zero
  const bool doWaveshape =
      (drive01 > 0.001f || harm01 > 0.001f || grit01 > 0.001f);
  const bool doAir = (air01 > 0.001f);

  if (!doWaveshape && !doAir)
    return;

  for (int i = 0; i < numSamples; ++i) {
    float L = left[i];
    float R = (right != nullptr) ? right[i] : 0.0f;

    // --- Waveshaper saturation ---
    if (doWaveshape) {
      L = waveshape(L, drive01, harm01, grit01);
      R = waveshape(R, drive01, harm01, grit01);
    }

    // --- Air / high-shelf exciter ---
    if (doAir) {
      // 1st-order high-shelf: extract high frequencies, boost by air amount
      const float highL = L - hsfAlpha_ * hsfStateL_ - (1.0f - hsfAlpha_) * L;
      hsfStateL_ = L;
      L += highL * air01 * 2.0f; // boost highs

      const float highR = R - hsfAlpha_ * hsfStateR_ - (1.0f - hsfAlpha_) * R;
      hsfStateR_ = R;
      R += highR * air01 * 2.0f;
    }

    left[i] = L;
    if (right != nullptr)
      right[i] = R;
  }
}

} // namespace neon::dsp
