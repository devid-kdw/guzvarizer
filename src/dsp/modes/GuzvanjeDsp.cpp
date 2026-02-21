#include "src/dsp/modes/GuzvanjeDsp.h"

#include <cmath>

namespace neon::dsp {

void GuzvanjeDsp::prepare(double sampleRate) noexcept {
  sampleRate_ = sampleRate > 0.0 ? sampleRate : 44100.0;
  lfo_.prepare(sampleRate);

  // Calculate crossfade increment: how much to move per sample for 30ms ramp
  const float rampSamples =
      (kCrossfadeTimeMs * 0.001f) * static_cast<float>(sampleRate_);
  crossfadeIncrement_ = rampSamples > 0.0f ? (1.0f / rampSamples) : 1.0f;
}

void GuzvanjeDsp::reset() noexcept {
  lfo_.reset();
  crossfade_ = 0.0f;
  crossfadeTarget_ = 0.0f;
  enabled_ = false;
}

void GuzvanjeDsp::setEnabled(bool enabled) noexcept {
  enabled_ = enabled;
  crossfadeTarget_ = enabled ? 1.0f : 0.0f;
}

void GuzvanjeDsp::setLfoRate(float rateHz) noexcept {
  currentLfoRate_ = rateHz;
  if (!lfoSyncEnabled_) {
    lfo_.setFrequency(rateHz);
  }
}

void GuzvanjeDsp::setLfoSync(bool syncEnabled, int division,
                             float hostBpm) noexcept {
  lfoSyncEnabled_ = syncEnabled;

  if (syncEnabled && hostBpm > 0.0f) {
    // Compute sync frequency from BPM and division
    const auto div = static_cast<neon::LfoSyncDivision>(
        division < 8 ? division : 4); // clamp to valid range, default 1/16
    const float bpc = neon::beatsPerCycle(div);
    const float freqHz = (hostBpm / 60.0f) / bpc;
    lfo_.setFrequency(freqHz);
  } else {
    // Hz mode or no valid BPM — use manual rate
    lfo_.setFrequency(currentLfoRate_);
  }
}

GuzvanjeDsp::GuzvanjeBias GuzvanjeDsp::processSample() noexcept {
  // --- Smooth crossfade ramp ---
  if (crossfade_ < crossfadeTarget_) {
    crossfade_ += crossfadeIncrement_;
    if (crossfade_ > crossfadeTarget_)
      crossfade_ = crossfadeTarget_;
  } else if (crossfade_ > crossfadeTarget_) {
    crossfade_ -= crossfadeIncrement_;
    if (crossfade_ < crossfadeTarget_)
      crossfade_ = crossfadeTarget_;
  }

  if (crossfade_ <= 0.0f) {
    return {}; // fully OFF — no bias, no LFO
  }

  // --- LFO modulation ---
  const float lfoValue = lfo_.process(); // -1..+1 (sine)

  // Scale everything by crossfade for smooth transition
  const float depth = crossfade_;

  return {
      .thresholdOffset =
          kThresholdBias * depth + lfoValue * kLfoDepthDb * depth,
      .ratioOffset = kRatioBias * depth,
      .lfoMod = (lfoValue * 0.5f + 0.5f) * depth, // normalize to 0..1
  };
}

} // namespace neon::dsp
