// Copyright 2016 oneup

#include "SDLFrontend.hpp"

namespace gb1 {
namespace sdl {

SDLFrontend::SDLFrontend()
    : m_(*this),
      frame_ready_(false)
{
    surf_ = SDL_CreateRGBSurfaceWithFormat(0, 160, 144, 8, SDL_PIXELFORMAT_INDEX8);

    surf_->format->palette->colors[0].r = 0xC8;
    surf_->format->palette->colors[0].g = 0xEC;
    surf_->format->palette->colors[0].b = 0x2C;
    surf_->format->palette->colors[0].a = 0;

    surf_->format->palette->colors[1].r = 0x7C;
    surf_->format->palette->colors[1].g = 0x98;
    surf_->format->palette->colors[1].b = 0x18;
    surf_->format->palette->colors[1].a = 0;

    surf_->format->palette->colors[2].r = 0x28;
    surf_->format->palette->colors[2].g = 0x52;
    surf_->format->palette->colors[2].b = 0x28;
    surf_->format->palette->colors[2].a = 0;

    surf_->format->palette->colors[3].r = 0x0F;
    surf_->format->palette->colors[3].g = 0x38;
    surf_->format->palette->colors[3].b = 0x0F;
    surf_->format->palette->colors[3].a = 0;
}

Button SDLFrontend::InputJoypad() {
    return Button(0);
}

void SDLFrontend::OutputAudioFrame(i16 left, i16 right) {
    (void) left;
    (void) right;
}

void SDLFrontend::OutputVideoFrame(std::array<std::array<u8, 40>, 144>&& frame) {
    SDL_LockSurface(surf_);

    auto row_ptr = static_cast<u8*>(surf_->pixels);
    for (auto &row : frame) {
        auto ptr = row_ptr;

        for (auto cols : row) {
            for (auto i=0u; i < 4; ++i) {
                *ptr = cols & 3;
                cols >>= 2;
                ++ptr;
            }
        }

        row_ptr += surf_->pitch / sizeof(*row_ptr);
    }

    SDL_UnlockSurface(surf_);

    frame_ready_ = true;
}

int SDLFrontend::Run() {
    while (true) {
        frame_ready_ = false;

        while (!frame_ready_) {
            for (auto i=0u; i < 10000; ++i) {
                m_.Tick();
            }
        }
    }
}

}   // namespace sdl
}   // namespace gb1
