#pragma once

#include "src/host/bridge/MeterBridgeSource.h"
#include "src/ui/bindings/MeterBridgeClient.h"

namespace neon::host {

/// Concrete MeterBridgeClient backed by atomic MeterBridgeSource.
/// Owned by the processor, handed to the editor as a non-owning pointer.
class AtomicMeterBridgeClient final : public neon::ui::MeterBridgeClient {
public:
  explicit AtomicMeterBridgeClient(const MeterBridgeSource &source) noexcept
      : source_(source) {}

  neon::ui::MeterSnapshot getLatestSnapshot() const noexcept override {
    const auto snap = source_.read();
    return {
        snap.gainReductionDb,
        snap.inputLevelDb,
        snap.outputLevelDb,
        snap.rmsNormalized,
    };
  }

private:
  const MeterBridgeSource &source_;
};

} // namespace neon::host
