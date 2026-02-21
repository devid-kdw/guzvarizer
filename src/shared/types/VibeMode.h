#pragma once

#include <cstdint>

namespace neon {

enum class VibeMode : std::uint8_t {
  kSmooth = 0,
  kPunch = 1,
  kDirty = 2,
};

}  // namespace neon
