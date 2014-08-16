include(CheckIncludeFile)

# check stdint.h
check_include_file(stdint.h DISTORM_HAVE_STDINT_H)

# identify endianness
include(TestBigEndian)
test_big_endian(DISTORM_ENDIAN_BE)

# identify compiler (c)
if(${CMAKE_C_COMPILER_ID} MATCHES MSVC)
  set(DISTORM_CC_MSVC   TRUE)
elseif(${CMAKE_C_COMPILER_ID} MATCHES GNU)
  set(DISTORM_CC_GNU    TRUE)
elseif(${CMAKE_C_COMPILER_ID} MATCHES Clang)
  set(DISTORM_CC_CLANG  TRUE)
elseif(${CMAKE_C_COMPILER_ID} MATCHES Intel)
  set(DISTORM_CC_INTEL  TRUE)
elseif(${CMAKE_C_COMPILER_ID} MATCHES TinyCC)
  set(DISTORM_CC_TINYCC TRUE)
elseif(${CMAKE_C_COMPILER_IF} MATCHES Watcom)
  set(DISTORM_CC_WATCOM TRUE)
else()
  set(DISTORM_CC_UNKNOWN)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/include/distorm3/config.h)
