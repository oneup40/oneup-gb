#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

#include <array>

namespace gb1 {

struct Machine;

class LCD;

static inline Serializer& operator<<(Serializer &s, const LCD &lcd);
static inline Deserializer& operator>>(Deserializer &d, LCD &lcd);

class LCD {
    Machine *m_;
    friend struct Machine;

    u8 lcdc_, stat_, scy_, scx_, ly_, lyc_, dma_, bgp_, obp0_, obp1_, wy_, wx_;
    unsigned dot_;

    std::array<std::array<u8, 40>, 144> frame_;

    u8 dma_ticks_;

    std::array<u8, 10> line_sprites_;
    u8 n_line_sprites_;

    static constexpr const u8 version_ = 0x02;
    static constexpr const u64 code_ = eight_cc(version_, 'l','c','d');
    friend Serializer& operator<<(Serializer &s, const LCD &lcd);
    friend Deserializer& operator>>(Deserializer &d, LCD &lcd);

    u8 FindTilenum(u8 y, u8 x, bool alt_base);
    std::pair<u8, u8> FindPattern(u8 tilenum, u8 y, bool alt_base);
    u8 ExtractPatternDot(std::pair<u8, u8> pattern, u8 x);
    u8 PalettizeDot(u8 dot, u8 palette);
    void DetermineLineSprites();

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

    return s << lcd.lcdc_ << lcd.stat_ << lcd.scy_ << lcd.scx_
             << lcd.ly_ << lcd.lyc_ << lcd.dma_
             << lcd.bgp_ << lcd.obp0_ << lcd.obp1_
             << lcd.wy_ << lcd.wx_
             << lcd.dot_
             << lcd.frame_
             << lcd.dma_ticks_
             << lcd.line_sprites_ << lcd.n_line_sprites_
             << lcd.vram
             << lcd.oam;
}

static inline Deserializer& operator>>(Deserializer &d, LCD &lcd) {
    d.Start(LCD::code_);

    d >> lcd.lcdc_ >> lcd.stat_ >> lcd.scy_ >> lcd.scx_
      >> lcd.ly_ >> lcd.lyc_ >> lcd.dma_
      >> lcd.bgp_ >> lcd.obp0_ >> lcd.obp1_
      >> lcd.wy_ >> lcd.wx_
      >> lcd.dot_
      >> lcd.frame_
      >> lcd.dma_ticks_
      >> lcd.line_sprites_ >> lcd.n_line_sprites_
      >> lcd.vram
      >> lcd.oam;
    if (!d) { return d; }

    return d;
}

}    // namespace gb1
