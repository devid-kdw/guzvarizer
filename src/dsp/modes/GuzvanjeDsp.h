#pragma once

#include "src/dsp/util/LfoOscillator.h"
#include "src/shared/types/LfoSyncDivision.h"

namespace neon::dsp {

/// Gužvanje mode DSP: applies extra compression bias and LFO modulation
/// when enabled. Provides smooth crossfade on ON/OFF transitions.
/// Realtime-safe: no allocation, no locks.
class GuzvanjeDsp {
public:
  void prepare(double sampleRate) noexcept;
  void reset() noexcept;

  /// Set Gužvanje enable state (0/1) — will smoothly crossfade
  void setEnabled(bool enabled) noexcept;

  /// Set LFO parameters
  void setLfoRate(float rateHz) noexcept;
  void setLfoSync(bool syncEnabled, int division, float hostBpm) noexcept;

  /// Process one block: returns the per-sample Gužvanje modulation depth
  /// [0..1]. Also updates the smooth enable crossfade internally. thresholdBias
  /// and ratioBias are output parameters for the compressor.
  struct GuzvanjeBias {
    float thresholdOffset = 0.0f; // dB (negative = more compression)
    float ratioOffset = 0.0f;     // added to ratio
    float lfoMod = 0.0f; // 0..1 LFO modulation depth (for threshold modulation)
  };

  /// Called per-sample. Returns current bias values.
  GuzvanjeBias processSample() noexcept;

  bool isEnabled() const noexcept { return enabled_; }

private:
  bool enabled_ = false;
  double sampleRate_ = 44100.0;

  // Smooth crossfade for ON/OFF transition
  float crossfade_ = 0.0f; // 0.0 = OFF, 1.0 = fully ON
  float crossfadeTarget_ = 0.0f;
  float crossfadeIncrement_ = 0.0f; // per-sample increment

  // LFO
  LfoOscillator lfo_;
  float currentLfoRate_ = 2.0f;
  float currentEffectiveFreq_ = -1.0f;
  bool lfoSyncEnabled_ = false;

  // Gužvanje compression bias constants
  static constexpr float kThresholdBias = -6.0f; // dB lower threshold when ON
  static constexpr float kRatioBias = 2.0f;      // higher ratio when ON
  static constexpr float kLfoDepthDb = 4.0f; // LFO modulates threshold by ±4 dB

  static constexpr float kCrossfadeTimeMs = 30.0f; // 30ms smooth transition
};

} // namespace neon::dsp
