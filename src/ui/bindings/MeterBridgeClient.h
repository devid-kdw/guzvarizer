#pragma once

#include <memory>

namespace neon::ui {

struct MeterSnapshot {
  float gainReductionDb = 0.0f;
  float inputLevelDb = -90.0f;
  float outputLevelDb = -90.0f;
  float rmsNormalized = 0.0f;
};

class MeterBridgeClient {
 public:
  virtual ~MeterBridgeClient() = default;
  virtual MeterSnapshot getLatestSnapshot() const noexcept = 0;
};

std::unique_ptr<MeterBridgeClient> CreateNullMeterBridgeClient();

}  // namespace neon::ui
