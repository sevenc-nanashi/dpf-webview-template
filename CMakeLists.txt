# CMake file for DISTRHO Plugins #
# ------------------------------ #

cmake_minimum_required(VERSION 3.24)

set(DPF_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/DPF")
include(./deps/DPF/cmake/DPF-plugin.cmake)

project(my_plugin)
# MSVC only: Use UTF-8 code page
if(MSVC)
  add_compile_options("/utf-8")
endif()

# Main sources
file(GLOB_RECURSE dsp_sources "src/plugin_*.cpp")
file(GLOB_RECURSE ui_sources "src/ui_*.cpp")
file(GLOB_RECURSE common_sources "src/*.cpp")

# Dependencies
file(GLOB_RECURSE deps_mimetypes_sources "deps/MimeTypes/*.cpp")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  execute_process(
    COMMAND xxd -i -n UI_ZIP ${CMAKE_SOURCE_DIR}/src/ui.zip ${CMAKE_SOURCE_DIR}/src/ui_zip.h
  )
  message(STATUS "UI_ZIP generated")
elseif (NOT CMAKE_BUILD_TYPE)
  message(FATAL_ERROR "Build type not set")
else()
  message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

list(REMOVE_ITEM common_sources ${dsp_sources} ${ui_sources})

dpf_add_plugin(
  my_plugin
  TARGETS vst3
  UI_TYPE external
  FILES_COMMON
    ${common_sources}
  FILES_DSP
    src/plugin.cpp
    ${dsp_sources}
  FILES_UI
    src/ui.cpp
    ${ui_sources}
    ${deps_mimetypes_sources}
)

target_include_directories(
  my_plugin PUBLIC "src" "deps"
)
target_compile_features(
  my_plugin PUBLIC cxx_std_17
)
