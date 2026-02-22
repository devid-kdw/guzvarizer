#include "src/ui/components/controls/NeonKnob.h"

#include <cmath>

#include "src/ui/assets/UiAssetRepository.h"
#include "src/ui/theme/UiTypography.h"

namespace neon::ui {

namespace {

constexpr float kRotaryStartRadians = juce::degreesToRadians(130.0f);
constexpr float kRotaryEndRadians = juce::degreesToRadians(409.0f);

bool isMainKnob(const juce::String& id) {
  return id == "threshold" || id == "attack" || id == "release" ||
         id == "ratio" || id == "output";
}

bool isToneKnob(const juce::String& id) {
  return id == "saturation" || id == "harmonics" || id == "grit" ||
         id == "air" || id == "color";
}

bool isLfoKnob(const juce::String& id) { return id == "lfo_rate"; }

int labelHeightFor(const NeonKnob::Config& config) {
  return isLfoKnob(config.assetId.trim().toLowerCase()) ? 14 : 18;
}

int assetSizeFor(const juce::String& id) {
  if (id == "lfo_rate") {
    return 192;
  }
  if (isToneKnob(id)) {
    return 224;
  }
  return 256;
}

juce::String makeResourceName(const juce::String& assetId,
                              const juce::String& layer,
                              const juce::String& state,
                              int size) {
  return "gz_knob_" + assetId + "_" + layer + "_" + state + "_" +
         juce::String(size) + "x" + juce::String(size) + "_png";
}

float normalizedValue(const NeonKnob::Config& config, double value) {
  return static_cast<float>(juce::jmap(value, config.minValue, config.maxValue,
                                       0.0, 1.0));
}

juce::Rectangle<float> centeredSquare(juce::Rectangle<float> area) {
  const auto side = juce::jmin(area.getWidth(), area.getHeight());
  return juce::Rectangle<float>(area.getCentreX() - (side * 0.5f),
                                area.getCentreY() - (side * 0.5f), side, side);
}

}  // namespace

NeonKnob::NeonKnob(Config config) : config_(std::move(config)) {
  knob_.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  knob_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  knob_.setRange(config_.minValue, config_.maxValue);
  knob_.setDoubleClickReturnValue(true, config_.defaultValue);
  knob_.setValue(config_.defaultValue, juce::dontSendNotification);
  knob_.addListener(this);
  knob_.setAlpha(0.0f);

  if (config_.mapping == Mapping::kLogarithmic && config_.minValue > 0.0 &&
      config_.maxValue > config_.minValue) {
    knob_.setSkewFactorFromMidPoint(
        std::sqrt(config_.minValue * config_.maxValue));
  }

  const auto labelColor = juce::Colour::fromRGB(255, 176, 56);
  const auto labelHeight = static_cast<float>(labelHeightFor(config_));

  label_.setText(config_.label.toUpperCase(), juce::dontSendNotification);
  label_.setJustificationType(juce::Justification::centred);
  label_.setFont(fonts::makeDisplayFont(labelHeight * 0.66f, "Heavy", 0.09f));
  label_.setColour(juce::Label::textColourId, labelColor.withAlpha(0.92f));
  label_.setVisible(config_.showLabel);

  valueLabel_.setJustificationType(juce::Justification::centred);
  valueLabel_.setFont(fonts::makeDisplayFont(12.0f, "Bold", 0.08f));
  valueLabel_.setColour(juce::Label::textColourId, labelColor.withAlpha(0.88f));
  valueLabel_.setVisible(config_.showValueReadout);

  addAndMakeVisible(knob_);
  addAndMakeVisible(label_);
  addAndMakeVisible(valueLabel_);

  loadAssets();
  updateValueLabel();
}

NeonKnob::~NeonKnob() { knob_.removeListener(this); }

void NeonKnob::setOnValueChanged(std::function<void(double)> callback) {
  onValueChanged_ = std::move(callback);
}

void NeonKnob::setValue(double value, juce::NotificationType notification) {
  knob_.setValue(value, notification);
  updateValueLabel();
}

double NeonKnob::value() const noexcept { return knob_.getValue(); }

double NeonKnob::minValue() const noexcept { return config_.minValue; }

double NeonKnob::maxValue() const noexcept { return config_.maxValue; }

const juce::String& NeonKnob::parameterId() const noexcept {
  return config_.parameterId;
}

void NeonKnob::paint(juce::Graphics& g) {
  auto dialArea = getLocalBounds().toFloat();
  const auto labelHeight = static_cast<float>(labelHeightFor(config_));
  constexpr float valueHeight = 18.0f;

  if (config_.showLabel) {
    dialArea.removeFromBottom(labelHeight);
  }
  if (config_.showValueReadout) {
    dialArea.removeFromBottom(valueHeight);
  }

  dialArea = centeredSquare(dialArea);

  if (dialArea.isEmpty()) {
    return;
  }

  const auto normalized = normalizedValue(config_, knob_.getValue());
  const auto angle = juce::jmap(normalized, 0.0f, 1.0f, kRotaryStartRadians,
                                kRotaryEndRadians);

  assets::drawIfValid(g, glow_, dialArea, 0.35f + (normalized * 0.55f));
  assets::drawIfValid(g, ringBase_, dialArea);

  if (normalized > 0.0001f) {
    juce::Path arc;
    arc.addCentredArc(dialArea.getCentreX(), dialArea.getCentreY(),
                      dialArea.getWidth() * 0.43f, dialArea.getWidth() * 0.43f,
                      0.0f, kRotaryStartRadians, angle, true);

    juce::Path arcMask;
    const auto arcThickness = juce::jmax(2.0f, dialArea.getWidth() * 0.085f);
    juce::PathStrokeType(arcThickness, juce::PathStrokeType::curved,
                         juce::PathStrokeType::rounded)
        .createStrokedPath(arcMask, arc);

    if (ringActive_.isValid()) {
      g.saveState();
      g.reduceClipRegion(arcMask);
      g.drawImage(ringActive_, dialArea, juce::RectanglePlacement::stretchToFit);
      g.restoreState();
    } else {
      g.setColour(config_.ringColour.withAlpha(0.9f));
      g.strokePath(arc,
                   juce::PathStrokeType(juce::jmax(1.2f, dialArea.getWidth() * 0.03f),
                                        juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));
    }
  }

  assets::drawIfValid(g, ticks_, dialArea);
  assets::drawIfValid(g, cap_, dialArea);

  if (pointer_.isValid()) {
    const auto scaleX = dialArea.getWidth() / static_cast<float>(pointer_.getWidth());
    const auto scaleY =
        dialArea.getHeight() / static_cast<float>(pointer_.getHeight());

    auto transform = juce::AffineTransform::scale(scaleX, scaleY)
                         .translated(dialArea.getX(), dialArea.getY());
    transform = transform.rotated(angle + juce::MathConstants<float>::halfPi,
                                  dialArea.getCentreX(), dialArea.getCentreY());

    juce::Path clipCircle;
    clipCircle.addEllipse(dialArea.reduced(1.5f));
    g.saveState();
    g.reduceClipRegion(clipCircle);
    g.drawImageTransformed(pointer_, transform, false);
    g.restoreState();
  } else {
    juce::Path pointerPath;
    pointerPath.startNewSubPath(dialArea.getCentre());
    pointerPath.lineTo(dialArea.getCentre().getPointOnCircumference(
        dialArea.getWidth() * 0.34f, angle));
    g.setColour(config_.ringColour);
    g.strokePath(pointerPath,
                 juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered,
                                      juce::PathStrokeType::rounded));
  }
}

void NeonKnob::resized() {
  auto area = getLocalBounds();
  const auto labelHeight = labelHeightFor(config_);
  constexpr int valueHeight = 18;

  juce::Rectangle<int> labelArea;
  juce::Rectangle<int> valueArea;

  if (config_.showLabel) {
    labelArea = area.removeFromBottom(labelHeight);
  }
  if (config_.showValueReadout) {
    valueArea = area.removeFromBottom(valueHeight);
  }

  knob_.setBounds(area);
  label_.setBounds(labelArea);
  valueLabel_.setBounds(valueArea);
}

void NeonKnob::sliderValueChanged(juce::Slider* slider) {
  if (slider != &knob_) {
    return;
  }

  updateValueLabel();
  repaint();

  if (onValueChanged_) {
    onValueChanged_(knob_.getValue());
  }
}

void NeonKnob::updateValueLabel() {
  valueLabel_.setText(juce::String(knob_.getValue(), 2),
                      juce::dontSendNotification);
}

void NeonKnob::loadAssets() {
  juce::String assetId = config_.assetId.trim().toLowerCase();
  if (assetId.isEmpty()) {
    assetId = config_.label.trim().toLowerCase();
  }

  if (!isMainKnob(assetId) && !isToneKnob(assetId) && assetId != "lfo_rate") {
    return;
  }

  const auto size = assetSizeFor(assetId);
  ringBase_ = assets::image(makeResourceName(assetId, "ring", "base", size));
  ringActive_ =
      assets::image(makeResourceName(assetId, "ring", "active", size));
  ticks_ = assets::image(makeResourceName(assetId, "ticks", "base", size));
  cap_ = assets::image(makeResourceName(assetId, "cap", "base", size));
  pointer_ =
      assets::image(makeResourceName(assetId, "pointer", "base", size));
  glow_ = assets::image(makeResourceName(assetId, "glow", "base", size));
}

}  // namespace neon::ui
