#include "src/dsp/compressor/CompressorCore.h"

#include <algorithm>
#include <cmath>

namespace neon::dsp {

namespace {
constexpr float kMinEnvelopeDb = -90.0f;
constexpr float kSoftKneeWidthDb = 6.0f; // soft knee width in dB
} // namespace

void CompressorCore::prepare(double sampleRate, int /*maxBlockSize*/) {
  sampleRate_ = sampleRate > 0.0 ? sampleRate : 44100.0;
  updateCoefficients();
}

void CompressorCore::reset() { envelopeDb_ = kMinEnvelopeDb; }

void CompressorCore::setParams(const CompressorParams &params) noexcept {
  params_ = params;
  updateCoefficients();
}

void CompressorCore::updateCoefficients() noexcept {
  // Time constant: coeff = exp(-1 / (time_sec * sampleRate))
  // Attack: fast response to rising signal
  const float attackSec = params_.attackMs * 0.001f;
  const float releaseSec = params_.releaseMs * 0.001f;

  if (sampleRate_ > 0.0 && attackSec > 0.0f) {
    attackCoeff_ =
        std::exp(-1.0f / (attackSec * static_cast<float>(sampleRate_)));
  } else {
    attackCoeff_ = 0.0f;
  }

  if (sampleRate_ > 0.0 && releaseSec > 0.0f) {
    releaseCoeff_ =
        std::exp(-1.0f / (releaseSec * static_cast<float>(sampleRate_)));
  } else {
    releaseCoeff_ = 0.0f;
  }
}

float CompressorCore::processBlock(float *left, float *right, int numSamples,
                                   const float *thresholds, const float *ratios,
                                   const float *makeupGainsLinear) noexcept {
  const float baseThreshold = params_.thresholdDb;
  const float baseRatio = std::max(params_.ratio, 1.0f);
  const float makeupGain = dbToGain(params_.outputDb);
  const float kneeWidth = std::max(params_.kneeWidthDb, 0.1f);
  const float halfKnee = kneeWidth * 0.5f;

  float peakGrDb = 0.0f; // track peak gain reduction for metering

  for (int i = 0; i < numSamples; ++i) {
    // Determine effective per-sample parameters
    const float currentThreshold = thresholds ? thresholds[i] : baseThreshold;
    const float currentRatio = std::max(1.0f, ratios ? ratios[i] : baseRatio);

    // --- Detect peak level (absolute max of L/R) ---
    float peakSample = std::abs(left[i]);
    if (right != nullptr) {
      peakSample = std::max(peakSample, std::abs(right[i]));
    }

    const float inputDb = gainToDb(peakSample);

    // --- Envelope follower (dB domain, adaptive attack/release) ---
    const float coeff = (inputDb > envelopeDb_) ? attackCoeff_ : releaseCoeff_;
    envelopeDb_ = coeff * envelopeDb_ + (1.0f - coeff) * inputDb;

    // Clamp envelope to sensible floor
    if (envelopeDb_ < kMinEnvelopeDb) {
      envelopeDb_ = kMinEnvelopeDb;
    }

    // --- Gain computer (soft-knee) ---
    float grDb = 0.0f;
    const float overDb = envelopeDb_ - currentThreshold;

    if (overDb <= -halfKnee) {
      // Below knee: no compression
      grDb = 0.0f;
    } else if (overDb >= halfKnee) {
      // Above knee: full compression
      grDb = overDb * (1.0f - 1.0f / currentRatio);
    } else {
      // In knee: quadratic transition
      const float x = overDb + halfKnee;
      grDb = (x * x) / (2.0f * kneeWidth) * (1.0f - 1.0f / currentRatio);
    }

    // grDb is how much to reduce (positive value = reduction)
    const float currentMakeupGain =
        makeupGainsLinear ? makeupGainsLinear[i] : makeupGain;
    const float gainLinear = dbToGain(-grDb) * currentMakeupGain;

    // --- Apply gain ---
    left[i] *= gainLinear;
    if (right != nullptr) {
      right[i] *= gainLinear;
    }

    // Track peak gain reduction (most negative = most compression)
    if (-grDb < peakGrDb) {
      peakGrDb = -grDb;
    }
  }

  return peakGrDb;
}

} // namespace neon::dsp
