unit_add_sources(lib_hdbg
                   this_process.cc
                   process_kill_guard.cc
                   process_stop_guard.cc )
add_subdirectory(arch)
add_subdirectory(dbg)
add_subdirectory(enum)
