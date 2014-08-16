include(BuildUnit)

define_build_unit(lib_hdbg "${CMAKE_SOURCE_DIR}/src/holydbg")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(ERROR "The ${CMAKE_CXX_COMPILER} compiler has no C++11 support."
                "Please use a different C++ compiler" )
endif()

if(HOLYDBG_CXX_GNU OR HOLYDBG_CXX_CLANG)
  add_definitions("-fvisibility=hidden")
endif(HOLYDBG_CXX_GNU OR HOLYDBG_CXX_CLANG)

find_package(Boost COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  unit_add_include_dirs(lib_hdbg ${Boost_INCLUDE_DIRS})
  unit_add_link_deps(lib_hdbg ${Boost_LIBRARIES})
endif()

add_subdirectory(arch)
add_subdirectory(binfmt)
add_subdirectory(dbg)
add_subdirectory(os)

get_property(lib_hdbg_include_dirs GLOBAL PROPERTY lib_hdbg_INC_DIRS)
include_directories(${holydbg_include_dirs})

if(LIBHDBG_STATIC)
  set(lib_hdbg_linkage STATIC)
else()
  set(lib_hdbg_linkage SHARED)
endif()

get_property(lib_hdbg_src_files GLOBAL PROPERTY lib_hdbg_SRCS)
get_property(lib_hdbg_link_deps GLOBAL PROPERTY lib_hdbg_DEPS)
add_library(hdbg ${lib_hdbg_linkage} ${lib_hdbg_src_files})
target_link_libraries(hdbg ${lib_hdbg_link_deps})

install(TARGETS hdbg
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)

if(HOLYDBG_BUILD_TESTS)
  add_subdirectory(tests)
endif(HOLYDBG_BUILD_TESTS)
