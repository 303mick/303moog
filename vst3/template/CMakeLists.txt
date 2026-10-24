cmake_minimum_required (VERSION 3.22)
# Plugin skeleton. Copy this folder to vst3/plugins/<slug>/ and edit the PLUGIN_* values.
set (PLUGIN_NAME          "Template Gain")
set (PLUGIN_CODE          "Tmpl")
set (MANUFACTURER_NAME    "Redline")
set (MANUFACTURER_CODE    "Rdln")
set (PLUGIN_VERSION       "0.1.0")

set (PLUGIN_SOURCES
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/Dsp/Core.h)

# ---- the plugin itself -------------------------------------------------------
juce_add_plugin (TemplateGain
    PRODUCT_NAME "${PLUGIN_NAME}"
    COMPANY_NAME "${MANUFACTURER_NAME}"
    PLUGIN_MANUFACTURER_CODE ${MANUFACTURER_CODE}
    PLUGIN_CODE ${PLUGIN_CODE}
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VERSION "${PLUGIN_VERSION}")

target_sources (TemplateGain PRIVATE ${PLUGIN_SOURCES})
target_compile_definitions (TemplateGain PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0 JUCE_DISPLAY_SPLASH_SCREEN=0)
target_link_libraries (TemplateGain
    PRIVATE juce::juce_audio_utils
    PUBLIC  juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
set_target_properties (TemplateGain PROPERTIES CXX_STANDARD 17)

# ---- JUCE-free DSP tests (fast, run everywhere) ------------------------------
add_executable (dsp_tests tests/dsp_tests.cpp)
set_target_properties (dsp_tests PROPERTIES CXX_STANDARD 17)
add_test (NAME dsp_tests COMMAND dsp_tests)

# ---- headless native UI snapshot --------------------------------------------
juce_add_console_app (snapshot PRODUCT_NAME "snapshot")
target_sources (snapshot PRIVATE tools/snapshot.cpp ${PLUGIN_SOURCES})
target_compile_definitions (snapshot PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JucePlugin_Name="${PLUGIN_NAME}")
target_link_libraries (snapshot PRIVATE juce::juce_audio_utils juce::juce_recommended_config_flags)
set_target_properties (snapshot PROPERTIES CXX_STANDARD 17)
