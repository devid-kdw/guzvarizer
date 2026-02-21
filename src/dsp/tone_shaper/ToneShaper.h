#pragma once

namespace neon::dsp {

/// Tone shaper parameters from APVTS.
struct ToneShaperParams {
  float saturationDrive = 25.0f; // 0..100 %
  float harmonics = 40.0f;       // 0..100 %
  float grit = 20.0f;            // 0..100 %
  float air = 30.0f;             // 0..100 %
  float airFreq = 8000.0f;       // 1000..20000 Hz
};

/// Simple tone shaping stage: waveshaper saturation + high-shelf exciter.
/// Runs after the compressor in the processing chain.
/// Realtime-safe: no allocation, no locks.
class ToneShaper {
public:
  void prepare(double sampleRate, int maxBlockSize);
  void reset();

  void setParams(const ToneShaperParams &params) noexcept;

  void processBlock(float *left, float *right, int numSamples) noexcept;

private:
  ToneShaperParams params_{};
  double sampleRate_ = 44100.0;

  // High-shelf filter state (1st-order) for air/exciter — per channel
  float hsfStateL_ = 0.0f;
  float hsfStateR_ = 0.0f;

  // Cached filter coefficients
  float hsfAlpha_ = 0.0f;

  void updateFilterCoefficients() noexcept;

  /// Waveshaper: soft-clip with drive and harmonic blend
  static float waveshape(float sample, float drive01, float harmonics01,
                         float grit01) noexcept;
};

} // namespace neon::dsp
