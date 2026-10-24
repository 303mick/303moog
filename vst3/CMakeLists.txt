cmake_minimum_required (VERSION 3.22)
project (RedlineVST3 VERSION 0.1.0 LANGUAGES C CXX)

# Which plugin to build: a folder name under vst3/ (template, or plugins/<slug>).
set (PLUGIN_DIR "template" CACHE STRING "Plugin folder to build, relative to vst3/")
# Use a local JUCE checkout when given (faster, offline); otherwise fetch the pinned release.
set (JUCE_SOURCE_DIR "" CACHE PATH "Optional local JUCE checkout")

include (CTest)
enable_testing()

if (JUCE_SOURCE_DIR)
    add_subdirectory (${JUCE_SOURCE_DIR} juce EXCLUDE_FROM_ALL)
else()
    include (FetchContent)
    FetchContent_Declare (juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.12
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable (juce)
endif()

add_subdirectory (${PLUGIN_DIR})
