#include "src/dsp/compressor/CompressorCore.h"

#include <juce_core/juce_core.h>

namespace neon::dsp {

void CompressorCore::prepare(double /*sampleRate*/, int /*maxBlockSize*/) {}

void CompressorCore::reset() {}

void CompressorCore::setParams(const CompressorParams& params) noexcept {
  params_ = params;
}

void CompressorCore::processBlock(float* left, float* right, int numSamples) noexcept {
  juce::ignoreUnused(left, right, numSamples, params_);
  // Placeholder: compression algorithm is introduced in M2.
}

}  // namespace neon::dsp
