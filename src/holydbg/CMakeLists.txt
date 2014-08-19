include(BuildUnit)
define_build_unit(holydbg)

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

unit_add_sources(holydbg main.cc)
unit_add_link_deps(holydbg hdbg)

get_property(holydbg_src_files    GLOBAL PROPERTY holydbg_SRCS)
get_property(holydbg_include_dirs GLOBAL PROPERTY holydbg_INC_DIRS)
get_property(holydbg_link_deps    GLOBAL PROPERTY holydbg_DEPS)

message("" ${holydbg_src_files})

add_executable(holydbg ${holydbg_src_files})
include_directories(${holydbg_include_dirs})
target_link_libraries(holydbg ${holydbg_link_deps})

install(TARGETS holydbg
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)
