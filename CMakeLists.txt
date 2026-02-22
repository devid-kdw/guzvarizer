cmake_minimum_required(VERSION 3.25)

project(Guzvarizer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

if(DEFINED GUZVARIZER_JUCE_DIR)
  add_subdirectory(${GUZVARIZER_JUCE_DIR} JUCE)
elseif(DEFINED ENV{JUCE_DIR})
  add_subdirectory($ENV{JUCE_DIR} JUCE)
else()
  FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.2
  )
  FetchContent_MakeAvailable(juce)
endif()

juce_add_plugin(Guzvarizer
  COMPANY_NAME "guzvarizer"
  COMPANY_WEBSITE "https://github.com/devid-kdw/guzvarizer"
  BUNDLE_ID "com.guzvarizer.plugin"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE
  PLUGIN_MANUFACTURER_CODE GzVr
  PLUGIN_CODE GzvR
  FORMATS VST3
  PRODUCT_NAME "Guzvarizer"
)

juce_generate_juce_header(Guzvarizer)

file(GLOB_RECURSE GUZVARIZER_UI_ASSETS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/assets/ui/*.png"
  "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.png"
)

juce_add_binary_data(GuzvarizerAssets
  SOURCES
    ${GUZVARIZER_UI_ASSETS}
)

target_sources(Guzvarizer PRIVATE
  src/dsp/compressor/CompressorCore.cpp
  src/dsp/modes/GuzvanjeDsp.cpp
  src/dsp/tone_shaper/ToneShaper.cpp
  src/dsp/util/LfoOscillator.cpp
  src/host/GuzvarizerProcessor.cpp
  src/host/parameters/ParameterLayout.cpp
  src/ui/app/PluginEditor.cpp
  src/ui/assets/UiAssetRepository.cpp
  src/ui/bindings/NullMeterBridgeClient.cpp
  src/ui/bindings/ParameterBindingAdapter.cpp
  src/ui/components/controls/BypassToggle.cpp
  src/ui/components/controls/GuzvanjeToggle.cpp
  src/ui/components/controls/LfoRateControl.cpp
  src/ui/components/controls/NeonKnob.cpp
  src/ui/components/controls/VibeModeSegmentedControl.cpp
  src/ui/components/metering/GainReductionMeter.cpp
  src/ui/components/metering/LevelVisualizer.cpp
  src/ui/components/shell/FooterBar.cpp
  src/ui/components/shell/HeaderBar.cpp
  src/ui/components/shell/MainPanel.cpp
  src/ui/components/shell/MainSection.cpp
  src/ui/components/shell/ToneShaperSection.cpp
  src/ui/components/text/SectionTitle.cpp
  src/ui/components/theme/ColorStripIndicator.cpp
  src/ui/theme/ThemeManager.cpp
)

target_include_directories(Guzvarizer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Guzvarizer PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
)

target_link_libraries(Guzvarizer PRIVATE
  juce::juce_audio_utils
  juce::juce_dsp
  GuzvarizerAssets
)
