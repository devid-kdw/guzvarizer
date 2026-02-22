#pragma once

#include <JuceHeader.h>

namespace neon::ui::assets {

const juce::Image& image(const char* resourceName);
const juce::Image& image(const juce::String& resourceName);
bool drawIfValid(juce::Graphics& g, const juce::Image& img,
                 juce::Rectangle<float> destination, float alpha = 1.0f);

}  // namespace neon::ui::assets
