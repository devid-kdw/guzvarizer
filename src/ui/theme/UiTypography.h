#pragma once

#include <JuceHeader.h>

namespace neon::ui::fonts {

inline juce::String pickDisplayTypeface() {
  static const juce::String selected = [] {
    const auto available = juce::Font::findAllTypefaceNames();
    const juce::StringArray preferred{
        "Avenir Next Condensed", "Avenir Next", "Bahnschrift",
        "DIN Alternate", "Verdana"};

    for (const auto& candidate : preferred) {
      if (available.contains(candidate, true)) {
        return candidate;
      }
    }

    return juce::String{};
  }();

  return selected;
}

inline juce::Font makeDisplayFont(float height,
                                  const juce::String& style = "Bold",
                                  float kerning = 0.06f) {
  auto options = juce::FontOptions(height).withStyle(style);
  const auto face = pickDisplayTypeface();
  if (face.isNotEmpty()) {
    options = options.withName(face);
  }
  return juce::Font(options).withExtraKerningFactor(kerning);
}

}  // namespace neon::ui::fonts
