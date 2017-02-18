add_executable(gb1-cpu-trace gb1_cpu_trace.cc)
target_link_libraries(gb1-cpu-trace gb1)

add_executable(gb1-cpu-call-tree gb1_cpu_call_tree.cc)
target_link_libraries(gb1-cpu-call-tree gb1)