#pragma once

#include <atomic>

namespace neon::host {

/// Lock-free meter data published by the audio thread and polled by the UI.
/// All members are std::atomic<float> for wait-free single-producer/single-consumer use.
struct MeterBridgeSource {
  std::atomic<float> gainReductionDb{0.0f};
  std::atomic<float> inputLevelDb{-90.0f};
  std::atomic<float> outputLevelDb{-90.0f};
  std::atomic<float> rmsNormalized{0.0f};

  /// Called from audio thread (realtime-safe, no lock, no heap).
  void publish(float gr, float inLevel, float outLevel, float rms) noexcept {
    gainReductionDb.store(gr, std::memory_order_relaxed);
    inputLevelDb.store(inLevel, std::memory_order_relaxed);
    outputLevelDb.store(outLevel, std::memory_order_relaxed);
    rmsNormalized.store(rms, std::memory_order_relaxed);
  }

  /// Called from UI thread.
  struct Snapshot {
    float gainReductionDb = 0.0f;
    float inputLevelDb = -90.0f;
    float outputLevelDb = -90.0f;
    float rmsNormalized = 0.0f;
  };

  Snapshot read() const noexcept {
    return {
        gainReductionDb.load(std::memory_order_relaxed),
        inputLevelDb.load(std::memory_order_relaxed),
        outputLevelDb.load(std::memory_order_relaxed),
        rmsNormalized.load(std::memory_order_relaxed),
    };
  }

  void reset() noexcept {
    gainReductionDb.store(0.0f, std::memory_order_relaxed);
    inputLevelDb.store(-90.0f, std::memory_order_relaxed);
    outputLevelDb.store(-90.0f, std::memory_order_relaxed);
    rmsNormalized.store(0.0f, std::memory_order_relaxed);
  }
};

}  // namespace neon::host
