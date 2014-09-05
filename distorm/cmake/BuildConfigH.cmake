include(CheckIncludeFile)

# check stdint.h
check_include_file(stdint.h HAVE_STDINT_H)

# identify endianness
include(TestBigEndian)
test_big_endian(DISTORM_BE_SYSTEM)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)
