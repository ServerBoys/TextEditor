
cmake_minimum_required (VERSION 3.8)
project(Editor)

file(GLOB SOURCES "src/*.cpp")
add_library(Editor SHARED ${SOURCES})
target_include_directories(Editor PUBLIC include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Editor PROPERTY CXX_STANDARD 20)
endif()