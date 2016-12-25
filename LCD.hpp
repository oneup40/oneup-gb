#pragma once

// Copyright 2016 oneup

#include "Base.hpp"
#include "Serializer.hpp"

#include <array>

namespace gblr {

struct Machine;

class LCD;

static inline Serializer& operator<<(Serializer &s, const LCD &lcd);
static inline Deserializer& operator>>(Deserializer &d, LCD &lcd);

class LCD {
    Machine *m_;
    friend struct Machine;

    u8 lcdc_, stat_, scy_, scx_, ly_, lyc_, dma_, bgp_, obp0_, obp1_, wy_, wx_;
    unsigned dot_;

    std::array<std::array<u32, 160>, 144> frame_;

    u8 dma_ticks_;

    static constexpr const u8 version_ = 0x01;
    static constexpr const u64 code_ = eight_cc(version_, 'l','c','d');
    friend Serializer& operator<<(Serializer &s, const LCD &lcd);
    friend Deserializer& operator>>(Deserializer &d, LCD &lcd);

    u8 FindTilenum(u8 y, u8 x, bool alt_base);
    std::pair<u8, u8> FindPattern(u8 tilenum, u8 y, bool alt_base);
    u8 ExtractPatternDot(std::pair<u8, u8> pattern, u8 x);
    u8 PalettizeDot(u8 dot, u8 palette);

    u8 RenderWindowDot();
    u8 RenderBackgroundDot();
    u8 RenderSpriteDot(bool bgwnd);
    void RenderDot();
public:
    LCD(Machine *m);
    LCD(const LCD&) = delete;
    LCD(LCD&&) = delete;
    LCD& operator=(const LCD&) = delete;
    LCD& operator=(LCD&&) = delete;

    std::array<u8, 0x2000> vram;
    std::array<u8, 0xA0> oam;

    u8 ReadVRAM(u16 addr, bool force = false);
    void WriteVRAM(u16 addr, u8 val, bool force = false);

    u8 ReadOAM(u16 addr, bool force = false);
    void WriteOAM(u16 addr, u8 val, bool force = false);

    u8 ReadDMA(bool force = false);
    void WriteDMA(u8 val, bool force = false);

    bool Tick();
};

static inline Serializer& operator<<(Serializer &s, const LCD &lcd) {
    s.Start(LCD::code_);

    std::basic_string<u8> frame;
    for (size_t y = 0; y < lcd.frame_.size(); ++y) {
        for (size_t x = 0; x < lcd.frame_[y].size(); x += 4) {
            u8 dots = 0;

            for (size_t i=0; i < 4; ++i) {
                dots <<= 2;

                switch (lcd.frame_[y][x + i]) {
                    case 0x00d0f8e0:    dots |= 0; break;
                    case 0x0070c088:    dots |= 1; break;
                    case 0x00566834:    dots |= 2; break;
                    case 0x00201808:    dots |= 3; break;
                    default:            dots |= 3; break;
                }
            }

            frame.push_back(dots);
        }
    }

    return s << lcd.lcdc_ << lcd.stat_ << lcd.scy_ << lcd.scx_
             << lcd.ly_ << lcd.lyc_ << lcd.dma_
             << lcd.bgp_ << lcd.obp0_ << lcd.obp1_
             << lcd.wy_ << lcd.wx_
             << lcd.dot_
             << frame
             << lcd.dma_ticks_
             << lcd.vram
             << lcd.oam;
}

static inline Deserializer& operator>>(Deserializer &d, LCD &lcd) {
    d.Start(LCD::code_);

    std::basic_string<u8> frame;

    d >> lcd.lcdc_ >> lcd.stat_ >> lcd.scy_ >> lcd.scx_
      >> lcd.ly_ >> lcd.lyc_ >> lcd.dma_
      >> lcd.bgp_ >> lcd.obp0_ >> lcd.obp1_
      >> lcd.wy_ >> lcd.wx_
      >> lcd.dot_
      >> frame
      >> lcd.dma_ticks_
      >> lcd.vram
      >> lcd.oam;
    if (!d) { return d; }

    if (frame.size() != lcd.frame_.size() * lcd.frame_[0].size() / 4) {
        std::cerr << "frame.size() == " << frame.size() << std::endl;
        std::cerr << "should be " << lcd.frame_.size() * lcd.frame_[0].size() / 4 << std::endl;
        d.Fail();
        return d;
    }

    size_t y = 0, x = 0;
    for (auto dots : frame) {
        for (unsigned i=0; i < 4; ++i) {
            u32 px = 0;

            switch (dots & 3) {
                case 0:    px = 0x00d0f8e0; break;
                case 1:    px = 0x0070c088; break;
                case 2:    px = 0x00566834; break;
                case 3:    px = 0x00201808; break;
                default: px = 0x00201808; break;
            }
            dots >>= 2;

            lcd.frame_[y][x + (4 - i)] = px;
        }
    }

    return d;
}

}    // namespace gblr
