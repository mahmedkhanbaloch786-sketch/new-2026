cmake_minimum_required(VERSION 3.20)
project(ColorSuiteOFX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENFX_INCLUDE_DIR "" CACHE PATH "Path to OpenFX SDK include directory")
if(NOT EXISTS "${OPENFX_INCLUDE_DIR}/ofxCore.h")
  message(FATAL_ERROR "Set OPENFX_INCLUDE_DIR to the official OpenFX SDK include directory (must contain ofxCore.h).")
endif()

add_library(ColorSuite MODULE src/ColorSuite.cpp)
target_include_directories(ColorSuite PRIVATE "${OPENFX_INCLUDE_DIR}")
target_compile_definitions(ColorSuite PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
if(MSVC)
  target_compile_options(ColorSuite PRIVATE /W4 /EHsc)
  set_target_properties(ColorSuite PROPERTIES PREFIX "")
endif()

set(OFX_BUNDLE_DIR "${CMAKE_SOURCE_DIR}/dist/ColorSuite.ofx.bundle")
set(OFX_PLUGIN_DIR "${OFX_BUNDLE_DIR}/Contents/Win64")
set_target_properties(ColorSuite PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${OFX_PLUGIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${OFX_PLUGIN_DIR}"
  OUTPUT_NAME "ColorSuite"
  SUFFIX ".ofx"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OFX_PLUGIN_DIR}"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OFX_PLUGIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OFX_PLUGIN_DIR}"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OFX_PLUGIN_DIR}"
)

add_custom_command(TARGET ColorSuite POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OFX_BUNDLE_DIR}/Contents/Resources"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_SOURCE_DIR}/resources/Info.plist"
          "${OFX_BUNDLE_DIR}/Contents/Info.plist"
  COMMENT "Assembling basic OFX bundle structure"
)
