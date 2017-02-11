find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(GB1SDL_SRC gb1_sdl.cc SDLFrontend.cc)
add_executable(gb1-sdl ${GB1SDL_SRC})
target_link_libraries(gb1-sdl gb1 ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
