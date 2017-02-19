find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(gb1-cpu-trace gb1_cpu_trace.cc ${PROJECT_SOURCE_DIR}/frontend/sdl/SDLFrontend.cc)
target_link_libraries(gb1-cpu-trace gb1 ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})

add_executable(gb1-cpu-call-tree gb1_cpu_call_tree.cc ${PROJECT_SOURCE_DIR}/frontend/sdl/SDLFrontend.cc)
target_link_libraries(gb1-cpu-call-tree gb1 ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})

add_executable(gb1-analyzer gb1_analyzer.cc ${PROJECT_SOURCE_DIR}/frontend/sdl/SDLFrontend.cc)
target_link_libraries(gb1-analyzer gb1 ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})