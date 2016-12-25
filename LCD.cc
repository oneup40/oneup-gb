// Copyright 2016 oneup

#include "LCD.hpp"

#include <algorithm>

#include "Machine.hpp"

namespace gblr {

u8 LCD::FindTilenum(u8 y, u8 x, bool alt_base) {
    size_t addr = alt_base ? 0x1C00 : 0x1800;
    addr |= (y & 0xF8) << 2;
    addr |= x >> 3;

    return vram[addr];
}

std::pair<u8, u8> LCD::FindPattern(u8 tilenum, u8 y, bool alt_base) {
    size_t pattern_ofs = (alt_base && tilenum < 0x80) ? 0x1000 : 0;
    pattern_ofs |= tilenum << 4;
    pattern_ofs |= (y & 7) << 1;

    u8 lower = vram[pattern_ofs],
       upper = vram[pattern_ofs | 1];

    return std::make_pair(lower, upper);
}

u8 LCD::ExtractPatternDot(std::pair<u8, u8> pattern, u8 x) {
    u8 dot = 0;
    u8 mask = 0x80 >> (x & 7);
    if (pattern.first & mask)  { dot |= 1; }
    if (pattern.second & mask) { dot |= 2; }

    return dot;
}

u8 LCD::PalettizeDot(u8 dot, u8 palette) {
    u8 shift = dot * 2;
    return (palette >> shift) & 3;
}

u8 LCD::RenderWindowDot() {
    if ((lcdc_ & 0x21) != 0x21) { return 0x80; }
    if (wy_ > ly_) { return 0x80; }
    if (wx_ < 7) { return 0x80; }
    if (unsigned(wx_) - 7 > dot_ - 80) { return 0x80; }

    u8 y = ly_ - wy_;
    u8 x = (dot_ - 80) - (wx_ - 7);

    u8 tilenum = FindTilenum(y, x, (lcdc_ & 0x40) != 0);
    auto pattern = FindPattern(tilenum, y, (lcdc_ & 0x10) == 0);
    u8 dot = ExtractPatternDot(pattern, x);
    return PalettizeDot(dot, bgp_);
}

u8 LCD::RenderSpriteDot(bool wnd) {
    if (!(lcdc_ & 0x02)) { return 0x80; }

    std::array<u8, 40> sprites;
    size_t n_sprites = 0;

    for (u8 i = 0; i < 40; ++i) {
        u8 addr = i * 4;

        u8 y = oam[addr] - 16;
        u8 y_range = (lcdc_ & 0x04) ? 16 : 8;

        if (y <= ly_ && ly_ < y + y_range) {
            sprites[n_sprites++] = addr;
        }
    }

    std::sort(sprites.begin(),
              sprites.begin() + n_sprites,
              [this](u8 lhs, u8 rhs) -> bool {
                  u8 lhs_x = oam[lhs + 1],
                     rhs_x = oam[rhs + 1];
                  return (lhs_x == rhs_x) ? (lhs < rhs) : (lhs_x < rhs_x);
              });

    if (n_sprites > 10) { n_sprites = 10; }

    u8 i = 0;
    for (; i < n_sprites; ++i) {
        u8 x = oam[sprites[i] + 1] - 8;
        if (dot_ - 80u < x || x + 8u <= dot_ - 80u) { continue; }

        u8 flags = oam[sprites[i] + 3];
        if (wnd && (flags & 0x80)) { continue; }

        break;
    }

    if (i >= n_sprites) { return 0x80; }

    u8 n = sprites[i];

    u8 y = ly_ - oam[n];
    u8 x = (dot_ - 80) - oam[n + 1];
    u8 tilenum = oam[n + 2];

    if (lcdc_ & 0x04) {
        tilenum &= ~0x01;
        tilenum |= (y & 0x08) >> 3;
        y &= ~0x08;
    }

    if (oam[n + 3] & 0x40) { y = 7 - y; }
    if (oam[n + 3] & 0x20) { x = 7 - x; }

    auto pattern = FindPattern(tilenum, y, false);
    u8 dot = ExtractPatternDot(pattern, x);
    if (!dot) { return 0x80; }
    return PalettizeDot(dot, (oam[n + 3] & 0x10) ? obp1_ : obp0_);
}

u8 LCD::RenderBackgroundDot() {
    if (!(lcdc_ & 0x01)) { return 0x80; }

    u8 y = scy_ + ly_;
    u8 x = scx_ + (dot_ - 80);

    u8 tilenum = FindTilenum(y, x, (lcdc_ & 0x08) != 0);
    auto pattern = FindPattern(tilenum, y, (lcdc_ & 0x10) == 0);
    u8 dot = ExtractPatternDot(pattern, x);
    return PalettizeDot(dot, bgp_);
}

void LCD::RenderDot() {
    u8 wnd_dot = RenderWindowDot();
    u8 spr_dot = RenderSpriteDot((wnd_dot & 3) != 0);
    u8 bg_dot  = RenderBackgroundDot();

    u8 dot = spr_dot;
    if (dot == 0x80) { dot = wnd_dot; }
    if (dot == 0x80) { dot = bg_dot; }
    if (dot == 0x80) { dot = bgp_ & 3; }

    u32 color = 0;
    switch (dot) {
        case 0:    color = 0x00d0f8e0; break;
        case 1: color = 0x0070c088; break;
        case 2: color = 0x00566834; break;
        case 3: color = 0x00201808; break;
        default: break;
    }

    frame_[ly_][dot_ - 80] = color;
}

LCD::LCD(Machine *m)
    : m_(m),
      lcdc_(0x91), stat_(0x85),
      scy_(0), scx_(0),
      ly_(0), lyc_(0),
      dma_(0),
      bgp_(0), obp0_(0), obp1_(0),
      wy_(0), wx_(0),
      dot_(0),
      dma_ticks_(0)
{
    vram.fill(0);
    oam.fill(0);
    for (auto &row : frame_) {
        row.fill(0);
    }
}

u8 LCD::ReadVRAM(u16 addr, bool) {
    // TODO: check if vram is accessible
    return vram[addr & (vram.size() - 1)];
}

void LCD::WriteVRAM(u16 addr, u8 val, bool) {
    // TODO: check if vram is accessible
    vram[addr & (vram.size() - 1)] = val;
}

u8 LCD::ReadOAM(u16 addr, bool) {
    // TODO: check if oam is accessible
    assert((addr & 0xFF) < oam.size());
    return oam[addr & 0xFF];
}

void LCD::WriteOAM(u16 addr, u8 val, bool) {
    // TODO: check if oam is accessible
    assert((addr & 0xFF) < oam.size());
    oam[addr & 0xFF] = val;
}

u8 LCD::ReadDMA(bool) {
    return dma_;
}

void LCD::WriteDMA(u8 val, bool) {
    dma_ = val;
    dma_ticks_ = 0xA0;
}

bool LCD::Tick() {
    // LCDC disable
    if (!(lcdc_ & 0x80)) { return true; }

    if (dma_ticks_) {
        u8 ofs = 0xA0 - dma_ticks_;
        oam[ofs] = m_->Read((dma_ << 8) | ofs);
        --dma_ticks_;
    }

    ++dot_;
    if (ly_ < 144) {
        // mode = 3
        if (dot_ == 80) { stat_ |= 0x03; }

        if (dot_ == 252) {
            // mode = 0
            stat_ &= ~0x03;

            // interrupt?
            if (stat_ & 0x08) { m_->Interrupt(0x02); }
        }

        if (dot_ < 80) {
            // TODO: OAM access
        } else if (dot_ < 240) {
            RenderDot();
        }
    }

    if (dot_ == 456) {
        dot_ = 0;

        ++ly_;
        if (ly_ == 154) { ly_ = 0; }

        if (ly_ == lyc_ && (stat_ & 0x04)) { m_->Interrupt(0x02); }

        if (ly_ < 144) {
            // mode = 2
            stat_ &= ~0x01;
            stat_ |= 0x02;

            // interrupt?
            if (stat_ & 0x20) { m_->Interrupt(0x02); }
        } else if (ly_ == 144) {
            m_->frame_ready = true;

            // mode = 1
            stat_ &= ~0x02;
            stat_ |= 0x01;

            // interrupt?
            if (stat_ & 0x10) { m_->Interrupt(0x02); }
            m_->Interrupt(0x01);
        }
    }

    return true;
}

}
