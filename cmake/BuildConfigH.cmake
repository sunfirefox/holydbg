# identify os
if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
  set(HOLYDBG_OS_WINDOWS TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
  set(HOLYDBG_OS_LINUX TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
  set(HOLYDBG_OS_OSX TRUE)
else()
  set(HOLYDBG_OS_UNKNOWN TRUE)
endif()

# identify arch
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64)
  set(HOLYDBG_ARCH_X86_64 TRUE)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86)
  set(HOLYDBG_ARCH_X86 TRUE)
else()
  set(HOLYDBG_ARCH_UNKNOWN TRUE)
endif()

# identify endianness
include(TestBigEndian)
test_big_endian(HOLYDBG_ENDIAN_BE)

# identify compiler (cxx)
if(${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
  set(HOLYDBG_CXX_MSVC TRUE)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set(HOLYDBG_CXX_GNU TRUE)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(HOLYDBG_CXX_CLANG TRUE)
else()
  set(HOLYDBG_CXX_UNKNOWN)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/include/hdbg/config.h)

