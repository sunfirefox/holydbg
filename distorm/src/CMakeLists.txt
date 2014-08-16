if(DISTORM_CC_GNU OR DISTORM_CC_CLANG)
  add_definitions("-fvisibility=hidden")
endif(DISTORM_CC_GNU OR DISTORM_CC_CLANG)

if(DISTORM_STATIC)
  set(DISTORM_LIB_TYPE STATIC)
else()
  set(DISTORM_LIB_TYPE SHARED)
endif()

FILE(GLOB distorm_src_files *.c)
add_library(distorm3 ${DISTORM_LIB_TYPE} ${distorm_src_files})

