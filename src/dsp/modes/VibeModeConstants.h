#pragma once

#include "src/shared/types/VibeMode.h"

namespace neon::dsp {

/// Per-vibe-mode DSP bias constants.
/// These modify the compressor and tone shaper behavior to create
/// distinct sonic characters without changing user-facing parameter values.
struct VibeModePreset {
  // Compressor biases (additive to user params)
  float attackBias = 0.0f;  // ms added/subtracted
  float releaseBias = 0.0f; // ms added/subtracted
  float kneeBias = 0.0f;    // dB added to soft knee width

  // Tone shaper biases (multiplicative — scale factors)
  float saturationScale = 1.0f;
  float harmonicsScale = 1.0f;
  float gritScale = 1.0f;
};

/// Returns the morph preset for the given VibeMode.
inline constexpr VibeModePreset
getVibeModePreset(neon::VibeMode mode) noexcept {
  switch (mode) {
  case VibeMode::kSmooth:
    return {
        .attackBias = 5.0f,      // slower attack
        .releaseBias = 30.0f,    // slower release
        .kneeBias = 4.0f,        // wider, softer knee
        .saturationScale = 0.6f, // less saturation
        .harmonicsScale = 0.5f,
        .gritScale = 0.3f,
    };
  case VibeMode::kPunch:
    return {
        .attackBias = -3.0f,     // faster attack
        .releaseBias = -20.0f,   // faster release
        .kneeBias = -2.0f,       // tighter knee
        .saturationScale = 1.0f, // neutral saturation
        .harmonicsScale = 1.0f,
        .gritScale = 1.0f,
    };
  case VibeMode::kDirty:
    return {
        .attackBias = -1.0f,
        .releaseBias = 0.0f,
        .kneeBias = -3.0f,       // hard knee
        .saturationScale = 1.8f, // more saturation
        .harmonicsScale = 2.0f,  // more harmonics
        .gritScale = 2.5f,       // much more grit
    };
  default:
    return {};
  }
}

} // namespace neon::dsp
