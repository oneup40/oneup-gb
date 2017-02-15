// Copyright 2017 oneup

#include "frontend/sdl/SDLFrontend.hpp"

#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: gb1-sdl <rom>" << std::endl;
        return 1;
    }

    gb1::sdl::SDLFrontend frontend;
    if (!frontend.LoadGame(argv[1])) {
        std::cerr << "error loading ROM" << std::endl;
        return 1;
    }

    return frontend.Run();
}
