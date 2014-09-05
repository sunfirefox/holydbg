cmake_minimum_required(VERSION 2.8)

project(distorm C)

option(DISTORM_STATIC "Build distorm as a static library" OFF)
option(DISTORM_SUPPORT_64BIT_OFFSET "Include support for 64 bit offsets in distorm" ON)
option(DISTORM_LIGHT "Perform a lightweight build of distorm" OFF)
option(DISTORM_DISABLE_BACKWARD_COMPATIBILITY "Remove backward compatibility in distorm" OFF)
option(DISTORM_DISABLE_FORMAT_FUNCTIONALITY "Remove format functionality in distorm" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(BuildConfigH)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/distorm_conf.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/include/distorm3/distorm_conf.h)

add_subdirectory(src)
