unit_add_sources(lib_hdbg debug_control_reg.cc )
unit_add_link_deps(lib_hdbg distorm3)
add_subdirectory(x86_64)
