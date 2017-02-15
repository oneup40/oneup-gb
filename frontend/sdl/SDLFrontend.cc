// Copyright 2016-2017 oneup

#include "frontend/sdl/SDLFrontend.hpp"

#include <fstream>

namespace gb1 {
namespace sdl {

void SDLFrontend::RunMachine() {
    frame_ready_ = false;

    while (!frame_ready_) {
        for (auto i=0u; i < 10000; ++i) {
            m_.Tick();
        }
    }
}

void SDLFrontend::Redraw() {
    SDL_BlitSurface(surf_, nullptr, wndsurf_, nullptr);
    SDL_UpdateWindowSurface(wnd_);
}

void SDLFrontend::PollInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                quit_ = true;
                break;

            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_RIGHT:    btn_ |= BTN_RIGHT; break;
                    case SDLK_LEFT:     btn_ |= BTN_LEFT; break;
                    case SDLK_UP:       btn_ |= BTN_UP; break;
                    case SDLK_DOWN:     btn_ |= BTN_DOWN; break;
                    case SDLK_z:        btn_ |= BTN_B; break;
                    case SDLK_x:        btn_ |= BTN_A; break;
                    case SDLK_RETURN:   btn_ |= BTN_START; break;
                    case SDLK_RSHIFT:   btn_ |= BTN_SELECT; break;
                    case SDLK_ESCAPE:   quit_ = true; break;
                }
                break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_RIGHT:    btn_ &= ~BTN_RIGHT; break;
                    case SDLK_LEFT:     btn_ &= ~BTN_LEFT; break;
                    case SDLK_UP:       btn_ &= ~BTN_UP; break;
                    case SDLK_DOWN:     btn_ &= ~BTN_DOWN; break;
                    case SDLK_z:        btn_ &= ~BTN_B; break;
                    case SDLK_x:        btn_ &= ~BTN_A; break;
                    case SDLK_RETURN:   btn_ &= ~BTN_START; break;
                    case SDLK_RSHIFT:   btn_ &= ~BTN_SELECT; break;
                }
                break;
        }
    }
}

void SDLFrontend::SyncTime() {
    // We don't need to be so precise as to use floating point.
    // But we should be more precise if we multiply before dividing.
    // However, multiplying first means we're more likely to overflow.
    // frames_ * 1000 will overflow a u32 in a little over 19 hours.
    // 1000 / 60 = 50 / 3
    auto game_ms = frames_ * 50 / 3;
    auto wall_ms = SDL_GetTicks();

    // we're lagging GOGOGOGOGO!
    if (game_ms < wall_ms) { return; }

    if (game_ms - wall_ms > 10) {
        SDL_Delay(game_ms - wall_ms);
    }
}

SDLFrontend::SDLFrontend()
    : m_(*this),
      frame_ready_(false), quit_(false),
      btn_(0),
      frames_(0)
{
    wnd_ = SDL_CreateWindow("oneup-gb",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            160,
                            144,
                            0);
    wndsurf_ = SDL_GetWindowSurface(wnd_);

    surf_ = SDL_CreateRGBSurface(0, 160, 144, 8, 0, 0, 0, 0);

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
    return Button(btn_);
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
    ++frames_;
}

bool SDLFrontend::LoadGame(const std::string &path) {
    std::ifstream is(path, std::ios::binary | std::ios::in);
    if (!is) { return false; }

    is.seekg(0, std::ios::end);
    auto size = is.tellg();
    is.seekg(0);

    std::vector<u8> data(size);
    is.read(reinterpret_cast<char*>(data.data()), data.size());
    if (!is) { return false; }

    return m_.LoadGame(std::move(data));
}

int SDLFrontend::Run() {
    while (!quit_) {
        RunMachine();
        Redraw();
        PollInput();
        SyncTime();
    }

    return 0;
}

}   // namespace sdl
}   // namespace gb1
