// Copyright 2017 oneup

#include "core/io/IO.hpp"

#include "core/Machine.hpp"

namespace gb1 {

u8 IO::Read(u16 addr, bool force) {
    u8 result = 0;

    if (addr < 0x8000)      { result = m_->mapper.ReadROM(addr, force); }
    else if (addr < 0xA000) { result = m_->lcd.ReadVRAM(addr, force); }
    else if (addr < 0xC000) { result = m_->mapper.ReadRAM(addr, force); }
    else if (addr < 0xFE00) { result = m_->wram[addr & (m_->wram.size() - 1)]; }
    else if (addr < 0xFEA0) { result = m_->lcd.ReadOAM(addr, force); }
    else if (addr < 0xFF00) { result = 0; }
    else if (addr < 0xFF80) {
        switch (addr & 0xFF) {
            case 0x00:  result = m_->joypad.ReadJoyp(force); break;
            case 0x04:  result = m_->timer.ReadDiv(force); break;
            case 0x05:  result = m_->timer.tima_; break;
            case 0x06:  result = m_->timer.tma_; break;
            case 0x07:  result = m_->timer.tac_; break;
            case 0x0F:  result = m_->cpu.if_ & 0xFF; break;
            case 0x10:  result = m_->audio.ch1_.r0_; break;
            case 0x11:  result = m_->audio.ch1_.r1_; break;
            case 0x12:  result = m_->audio.ch1_.r2_; break;
            case 0x13:  result = m_->audio.ch1_.r3_; break;
            case 0x14:  result = m_->audio.ch1_.r4_; break;
            case 0x15:  result = m_->audio.ch2_.r0_; break;
            case 0x16:  result = m_->audio.ch2_.r1_; break;
            case 0x17:  result = m_->audio.ch2_.r2_; break;
            case 0x18:  result = m_->audio.ch2_.r3_; break;
            case 0x19:  result = m_->audio.ch2_.r4_; break;
            case 0x1A:  result = m_->audio.ch3_.r0_; break;
            case 0x1B:  result = m_->audio.ch3_.r1_; break;
            case 0x1C:  result = m_->audio.ch3_.r2_; break;
            case 0x1D:  result = m_->audio.ch3_.r3_; break;
            case 0x1E:  result = m_->audio.ch3_.r4_; break;
            case 0x1F:  result = m_->audio.ch4_.r0_; break;
            case 0x20:  result = m_->audio.ch4_.r1_; break;
            case 0x21:  result = m_->audio.ch4_.r2_; break;
            case 0x22:  result = m_->audio.ch4_.r3_; break;
            case 0x23:  result = m_->audio.ch4_.r4_; break;
            case 0x24:  result = m_->audio.nr50_; break;
            case 0x25:  result = m_->audio.nr51_; break;
            case 0x26:  result = m_->audio.nr52_; break;
            case 0x30:  result = m_->audio.ch3_.wave_[0x0]; break;
            case 0x31:  result = m_->audio.ch3_.wave_[0x1]; break;
            case 0x32:  result = m_->audio.ch3_.wave_[0x2]; break;
            case 0x33:  result = m_->audio.ch3_.wave_[0x3]; break;
            case 0x34:  result = m_->audio.ch3_.wave_[0x4]; break;
            case 0x35:  result = m_->audio.ch3_.wave_[0x5]; break;
            case 0x36:  result = m_->audio.ch3_.wave_[0x6]; break;
            case 0x37:  result = m_->audio.ch3_.wave_[0x7]; break;
            case 0x38:  result = m_->audio.ch3_.wave_[0x8]; break;
            case 0x39:  result = m_->audio.ch3_.wave_[0x9]; break;
            case 0x3A:  result = m_->audio.ch3_.wave_[0xA]; break;
            case 0x3B:  result = m_->audio.ch3_.wave_[0xB]; break;
            case 0x3C:  result = m_->audio.ch3_.wave_[0xC]; break;
            case 0x3D:  result = m_->audio.ch3_.wave_[0xD]; break;
            case 0x3E:  result = m_->audio.ch3_.wave_[0xE]; break;
            case 0x3F:  result = m_->audio.ch3_.wave_[0xF]; break;
            case 0x40:  result = m_->lcd.lcdc_; break;
            case 0x41:  result = m_->lcd.stat_; break;
            case 0x42:  result = m_->lcd.scy_; break;
            case 0x43:  result = m_->lcd.scx_; break;
            case 0x44:  result = m_->lcd.ly_; break;
            case 0x45:  result = m_->lcd.lyc_; break;
            case 0x46:  result = m_->lcd.ReadDMA(force); break;
            case 0x47:  result = m_->lcd.bgp_; break;
            case 0x48:  result = m_->lcd.obp0_; break;
            case 0x49:  result = m_->lcd.obp1_; break;
            case 0x4A:  result = m_->lcd.wy_; break;
            case 0x4B:  result = m_->lcd.wx_; break;
            default:    result = 0; break;
        }
    }
    else if (addr < 0xFFFF) { result = m_->hram[addr & (m_->hram.size() - 1)]; }
    else                    { result = m_->cpu.ie_ & 0xFF; }

    if (obs_) { obs_->Read(*this, addr, result, force); }
    return result;
}

void IO::Write(u16 addr, u8 val, bool force) {
    if (addr < 0x8000)      { m_->mapper.WriteROM(addr, val, force); }
    else if (addr < 0xA000) { m_->lcd.WriteVRAM(addr, val, force); }
    else if (addr < 0xC000) { m_->mapper.WriteRAM(addr, val, force); }
    else if (addr < 0xFE00) { m_->wram[addr & (m_->wram.size() - 1)] = val; }
    else if (addr < 0xFEA0) { m_->lcd.WriteOAM(addr, val, force); }
    else if (addr < 0xFF00) { }
    else if (addr < 0xFF80) {
        if (0xFF10 <= addr && addr < 0xFF40) { m_->audio.Write(addr, val, force); }
        else {
            switch (addr & 0xFF) {
                case 0x00:  m_->joypad.WriteJoyp(val, force); break;
                case 0x04:  m_->timer.WriteDiv(force); break;
                case 0x05:  m_->timer.tima_ = val; break;
                case 0x06:  m_->timer.tma_ = val; break;
                case 0x07:  m_->timer.tac_ = val; break;
                case 0x0F:  m_->cpu.if_ = val & 0x1F; break;
                case 0x40:  m_->lcd.lcdc_ = val; break;
                case 0x41:  m_->lcd.stat_ = val; break;
                case 0x42:  m_->lcd.scy_ = val; break;
                case 0x43:  m_->lcd.scx_ = val; break;
                case 0x44:  m_->lcd.ly_ = 0; break;
                case 0x45:  m_->lcd.lyc_ = val; break;
                case 0x46:  m_->lcd.WriteDMA(val, force); break;
                case 0x47:  m_->lcd.bgp_ = val; break;
                case 0x48:  m_->lcd.obp0_ = val; break;
                case 0x49:  m_->lcd.obp1_ = val; break;
                case 0x4A:  m_->lcd.wy_ = val; break;
                case 0x4B:  m_->lcd.wx_ = val; break;
                default:    break;
            }
        }
    }
    else if (addr < 0xFFFF) { m_->hram[addr & (m_->hram.size() - 1)] = val; }
    else                    { m_->cpu.ie_ = val & 0x1F; }

    if (obs_) { obs_->Write(*this, addr, val, force); }
}

}
