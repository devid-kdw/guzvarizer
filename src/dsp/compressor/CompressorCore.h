#pragma once

namespace neon::dsp {

struct CompressorParams {
  float thresholdDb = -18.0f;
  float attackMs = 10.0f;
  float releaseMs = 120.0f;
  float ratio = 4.0f;
  float outputDb = 0.0f;
};

class CompressorCore {
 public:
  void prepare(double sampleRate, int maxBlockSize);
  void reset();

  void setParams(const CompressorParams& params) noexcept;

  void processBlock(float* left, float* right, int numSamples) noexcept;

 private:
  CompressorParams params_{};
};

}  // namespace neon::dsp
