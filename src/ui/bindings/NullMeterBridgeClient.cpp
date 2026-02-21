#include "src/ui/bindings/MeterBridgeClient.h"

#include <memory>

namespace neon::ui {

namespace {

class NullMeterBridgeClient final : public MeterBridgeClient {
 public:
  MeterSnapshot getLatestSnapshot() const noexcept override {
    return {};
  }
};

}  // namespace

std::unique_ptr<MeterBridgeClient> CreateNullMeterBridgeClient() {
  return std::make_unique<NullMeterBridgeClient>();
}

}  // namespace neon::ui
