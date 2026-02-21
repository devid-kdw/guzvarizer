#pragma once

#include <cmath>

namespace neon::dsp {

struct CompressorParams {
  float thresholdDb = -18.0f;
  float attackMs = 10.0f;
  float releaseMs = 120.0f;
  float ratio = 4.0f;
  float outputDb = 0.0f;
  float kneeWidthDb = 6.0f;
};

/// Feed-forward compressor with peak envelope detector.
/// Realtime-safe: no allocation, no locks in processBlock.
class CompressorCore {
public:
  void prepare(double sampleRate, int maxBlockSize);
  void reset();

  void setParams(const CompressorParams &params) noexcept;

  /// Process audio in-place. Returns peak gain reduction (dB, negative) for
  /// metering. Optional bias arrays allow per-sample parameter modulation.
  float processBlock(float *left, float *right, int numSamples,
                     const float *thresholdBiases = nullptr,
                     const float *ratioBiases = nullptr) noexcept;

private:
  CompressorParams params_{};
  double sampleRate_ = 44100.0;

  // Envelope follower state (in dB domain)
  float envelopeDb_ = -90.0f;

  // Cached coefficients (derived from params + sampleRate)
  float attackCoeff_ = 0.0f;
  float releaseCoeff_ = 0.0f;

  void updateCoefficients() noexcept;

  static float gainToDb(float gain) noexcept {
    return gain > 1e-10f ? 20.0f * std::log10(gain) : -90.0f;
  }

  static float dbToGain(float db) noexcept {
    return std::pow(10.0f, db * 0.05f);
  }
};

} // namespace neon::dsp
