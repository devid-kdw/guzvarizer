#include "src/ui/assets/UiAssetRepository.h"

#include <map>
#include <string>

#include "BinaryData.h"

namespace neon::ui::assets {

namespace {

std::map<std::string, juce::Image>& cache() {
  static std::map<std::string, juce::Image> storage;
  return storage;
}

const juce::Image& loadAndCache(const juce::String& resourceName) {
  const auto key = resourceName.toStdString();
  auto& storage = cache();
  if (const auto it = storage.find(key); it != storage.end()) {
    return it->second;
  }

  int sizeBytes = 0;
  juce::Image decoded;
  if (const auto* data = BinaryData::getNamedResource(resourceName.toRawUTF8(),
                                                      sizeBytes);
      data != nullptr && sizeBytes > 0) {
    decoded = juce::ImageFileFormat::loadFrom(data,
                                              static_cast<size_t>(sizeBytes));
  }

  const auto [it, inserted] = storage.emplace(key, decoded);
  juce::ignoreUnused(inserted);
  return it->second;
}

}  // namespace

const juce::Image& image(const char* resourceName) {
  return loadAndCache(juce::String(resourceName));
}

const juce::Image& image(const juce::String& resourceName) {
  return loadAndCache(resourceName);
}

bool drawIfValid(juce::Graphics& g, const juce::Image& img,
                 juce::Rectangle<float> destination, float alpha) {
  if (!img.isValid()) {
    return false;
  }

  g.setOpacity(alpha);
  g.drawImage(img, destination, juce::RectanglePlacement::stretchToFit);
  g.setOpacity(1.0f);
  return true;
}

}  // namespace neon::ui::assets
