#pragma once

// Copyright 2016 oneup

#include "core/Frontend.hpp"
#include "core/Machine.hpp"

#include <SDL.h>

#include <array>

namespace gb1 {
namespace sdl {

class SDLFrontend : public Frontend {
    SDL_Window *wnd_;
    SDL_Surface *wndsurf_, *surf_;
    Machine m_;
    bool frame_ready_, quit_;
    u8 btn_;
    u32 frames_;

    void RunMachine();
    void Redraw();
    void PollInput();
    void SyncTime();
public:
    SDLFrontend();
    SDLFrontend(const SDLFrontend&) = delete;
    SDLFrontend(SDLFrontend&&) = delete;
    SDLFrontend& operator=(const SDLFrontend&) = delete;
    SDLFrontend& operator=(SDLFrontend&&) = delete;

    Button InputJoypad() override;
    void OutputAudioFrame(i16 left, i16 right) override;
    void OutputVideoFrame(std::array<std::array<u8, 40>, 144>&& frame) override;

    bool LoadGame(const std::string &path);
    int Run();
};

}   // namespace sdl
}   // namespace gb1
