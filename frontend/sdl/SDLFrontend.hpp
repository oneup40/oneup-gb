#pragma once

// Copyright 2016 oneup

#include "core/Frontend.hpp"
#include "core/Machine.hpp"
#include "core/MachineObserver.hpp"

#include <SDL.h>

#include <array>

namespace gb1 {
namespace sdl {

class SerialBuffer {
    int bits_;
    char buf_;
public:
    SerialBuffer() : bits_(0), buf_(0) {}
    void Add(bool bit) {
        buf_ <<= 1;
        if (bit) { buf_ |= 0x01; }
        ++bits_;

        if (bits_ == 8) {
            bits_ = 0;
            std::cout << buf_;
        }
    }
};

class SDLFrontend : public Frontend {
    SDL_Window *wnd_;
    SDL_Surface *wndsurf_, *surf_;
    Machine m_;
    bool frame_ready_, quit_;
    u8 btn_;
    u32 frames_;

    SerialBuffer sbuf_;

    void RunMachine();
    void Redraw();
    void PollInput();
    void SyncTime();
public:
    explicit SDLFrontend(const MachineObserver &observer = MachineObserver());
    SDLFrontend(const SDLFrontend&) = delete;
    SDLFrontend(SDLFrontend&&) = delete;
    SDLFrontend& operator=(const SDLFrontend&) = delete;
    SDLFrontend& operator=(SDLFrontend&&) = delete;

    Button InputJoypad() override;
    void OutputAudioFrame(i16 left, i16 right) override;
    void OutputVideoFrame(std::array<std::array<u8, 40>, 144>&& frame) override;

    virtual bool InputSerial() override { return false; }   // TODO
    virtual void OutputSerial(bool bit) override { sbuf_.Add(bit); }

    bool LoadGame(const std::string &path);
    int Run();
};

}   // namespace sdl
}   // namespace gb1
