#include "src/ui/components/controls/NeonKnob.h"

#include <cmath>

namespace neon::ui {

namespace {

constexpr float kRotaryStartRadians = juce::degreesToRadians(210.0f);
constexpr float kRotaryEndRadians = juce::degreesToRadians(510.0f);

}  // namespace

NeonKnob::NeonKnob(Config config) : config_(std::move(config)) {
  knob_.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  knob_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  knob_.setRange(config_.minValue, config_.maxValue);
  knob_.setDoubleClickReturnValue(true, config_.defaultValue);
  knob_.setValue(config_.defaultValue, juce::dontSendNotification);
  knob_.addListener(this);

  if (config_.mapping == Mapping::kLogarithmic && config_.minValue > 0.0 && config_.maxValue > config_.minValue) {
    knob_.setSkewFactorFromMidPoint(std::sqrt(config_.minValue * config_.maxValue));
  }

  label_.setText(config_.label.toUpperCase(), juce::dontSendNotification);
  label_.setJustificationType(juce::Justification::centred);
  label_.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.86f));

  valueLabel_.setJustificationType(juce::Justification::centred);
  valueLabel_.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.72f));
  valueLabel_.setVisible(config_.showValueReadout);

  addAndMakeVisible(knob_);
  addAndMakeVisible(label_);
  addAndMakeVisible(valueLabel_);

  updateValueLabel();
}

NeonKnob::~NeonKnob() {
  knob_.removeListener(this);
}

void NeonKnob::setOnValueChanged(std::function<void(double)> callback) {
  onValueChanged_ = std::move(callback);
}

void NeonKnob::setValue(double value, juce::NotificationType notification) {
  knob_.setValue(value, notification);
  updateValueLabel();
}

double NeonKnob::value() const noexcept {
  return knob_.getValue();
}

double NeonKnob::minValue() const noexcept {
  return config_.minValue;
}

double NeonKnob::maxValue() const noexcept {
  return config_.maxValue;
}

const juce::String& NeonKnob::parameterId() const noexcept {
  return config_.parameterId;
}

void NeonKnob::paint(juce::Graphics& g) {
  const auto bounds = getLocalBounds().toFloat();
  auto dialArea = bounds.reduced(12.0f, 18.0f);
  dialArea.removeFromTop(14.0f);
  dialArea.removeFromBottom(config_.showValueReadout ? 24.0f : 8.0f);

  const auto centre = dialArea.getCentre();
  const auto radius = juce::jmin(dialArea.getWidth(), dialArea.getHeight()) * 0.5f - 8.0f;
  const auto normalized = static_cast<float>(juce::jmap(knob_.getValue(), config_.minValue, config_.maxValue, 0.0, 1.0));

  g.setColour(juce::Colour::fromRGB(8, 18, 28));
  g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

  g.setColour(juce::Colours::black.withAlpha(0.62f));
  g.drawEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f, 1.0f);

  juce::Path ticks;
  for (int i = 0; i <= 20; ++i) {
    const auto t = static_cast<float>(i) / 20.0f;
    const auto angle = juce::jmap(t, 0.0f, 1.0f, kRotaryStartRadians, kRotaryEndRadians);
    const auto outer = centre.getPointOnCircumference(radius + 8.0f, angle);
    const auto inner = centre.getPointOnCircumference(radius + (i % 5 == 0 ? 2.5f : 5.5f), angle);
    ticks.startNewSubPath(inner);
    ticks.lineTo(outer);
  }
  g.setColour(juce::Colours::white.withAlpha(0.2f));
  g.strokePath(ticks, juce::PathStrokeType(1.0f));

  juce::Path arc;
  arc.addCentredArc(centre.x, centre.y, radius + 4.0f, radius + 4.0f, 0.0f, kRotaryStartRadians,
                    juce::jmap(normalized, 0.0f, 1.0f, kRotaryStartRadians, kRotaryEndRadians), true);

  g.setColour(config_.ringColour.withAlpha(0.34f));
  g.strokePath(arc, juce::PathStrokeType(6.0f));
  g.setColour(config_.ringColour.withAlpha(0.88f));
  g.strokePath(arc, juce::PathStrokeType(2.2f));
}

void NeonKnob::resized() {
  auto area = getLocalBounds();
  auto labelArea = area.removeFromTop(14);
  auto valueArea = config_.showValueReadout ? area.removeFromBottom(18) : juce::Rectangle<int>();

  label_.setBounds(labelArea);
  valueLabel_.setBounds(valueArea);
  knob_.setBounds(area.reduced(6));
}

void NeonKnob::sliderValueChanged(juce::Slider* slider) {
  if (slider != &knob_) {
    return;
  }

  updateValueLabel();
  if (onValueChanged_) {
    onValueChanged_(knob_.getValue());
  }
}

void NeonKnob::updateValueLabel() {
  valueLabel_.setText(juce::String(knob_.getValue(), 2), juce::dontSendNotification);
}

}  // namespace neon::ui
