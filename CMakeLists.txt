cmake_minimum_required(VERSION 2.8)

project(holydbg)
set(holydbg_VERSION 0x000100)

option(LIBHDBG_STATIC "Build libhdbg as a static library" OFF)
option(HOLYDBG_BUILD_TESTS "Build holydbg tests" ON)

if(HOLYDBG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(gtest-1.7.0)
  include_directories(gtest-1.7.0/include)
endif(HOLYDBG_BUILD_TESTS)

add_subdirectory(distorm)
include_directories(distorm/include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(BuildConfigH)
add_subdirectory(src)

