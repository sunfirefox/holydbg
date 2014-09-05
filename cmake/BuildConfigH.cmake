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

# identify endianness
include(TestBigEndian)
test_big_endian(HOLYDBG_ENDIAN_BE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

