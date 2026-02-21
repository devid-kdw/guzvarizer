#pragma once

#include <array>
#include <cstdint>

namespace neon {

enum class LfoSyncDivision : std::uint8_t {
  k1_1 = 0,
  k1_2,
  k1_4,
  k1_8,
  k1_16,
  k1_8T,
  k1_16T,
  k1_4D,
};

inline constexpr std::array<const char*, 8> kLfoSyncDivisionLabels = {
    "1/1", "1/2", "1/4", "1/8", "1/16", "1/8T", "1/16T", "1/4D",
};

inline constexpr std::array<float, 8> kLfoSyncDivisionBeatsPerCycle = {
    4.0f,
    2.0f,
    1.0f,
    0.5f,
    0.25f,
    1.0f / 3.0f,
    1.0f / 6.0f,
    1.5f,
};

inline constexpr float beatsPerCycle(LfoSyncDivision division) {
  return kLfoSyncDivisionBeatsPerCycle[static_cast<std::size_t>(division)];
}

}  // namespace neon
