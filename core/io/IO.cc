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
            case 0x00:  result = m_->joypad.ReadJOYP(force); break;
            case 0x01:  result = m_->serial.ReadSB(force); break;
            case 0x02:  result = m_->serial.ReadSC(force); break;
            case 0x04:  result = m_->timer.ReadDIV(force); break;
            case 0x05:  result = m_->timer.ReadTIMA(force); break;
            case 0x06:  result = m_->timer.ReadTMA(force); break;
            case 0x07:  result = m_->timer.ReadTAC(force); break;
            case 0x0F:  result = m_->cpu.ReadIF(force); break;
            case 0x10:  result = m_->audio.ReadNR10(force); break;
            case 0x11:  result = m_->audio.ReadNR11(force); break;
            case 0x12:  result = m_->audio.ReadNR12(force); break;
            case 0x13:  result = m_->audio.ReadNR13(force); break;
            case 0x14:  result = m_->audio.ReadNR14(force); break;
            case 0x15:  result = m_->audio.ReadNR20(force); break;
            case 0x16:  result = m_->audio.ReadNR21(force); break;
            case 0x17:  result = m_->audio.ReadNR22(force); break;
            case 0x18:  result = m_->audio.ReadNR23(force); break;
            case 0x19:  result = m_->audio.ReadNR24(force); break;
            case 0x1A:  result = m_->audio.ReadNR30(force); break;
            case 0x1B:  result = m_->audio.ReadNR31(force); break;
            case 0x1C:  result = m_->audio.ReadNR32(force); break;
            case 0x1D:  result = m_->audio.ReadNR33(force); break;
            case 0x1E:  result = m_->audio.ReadNR34(force); break;
            case 0x1F:  result = m_->audio.ReadNR40(force); break;
            case 0x20:  result = m_->audio.ReadNR41(force); break;
            case 0x21:  result = m_->audio.ReadNR42(force); break;
            case 0x22:  result = m_->audio.ReadNR43(force); break;
            case 0x23:  result = m_->audio.ReadNR44(force); break;
            case 0x24:  result = m_->audio.ReadNR50(force); break;
            case 0x25:  result = m_->audio.ReadNR51(force); break;
            case 0x26:  result = m_->audio.ReadNR52(force); break;
            case 0x30:  result = m_->audio.ReadWAV(0x0, force); break;
            case 0x31:  result = m_->audio.ReadWAV(0x1, force); break;
            case 0x32:  result = m_->audio.ReadWAV(0x2, force); break;
            case 0x33:  result = m_->audio.ReadWAV(0x3, force); break;
            case 0x34:  result = m_->audio.ReadWAV(0x4, force); break;
            case 0x35:  result = m_->audio.ReadWAV(0x5, force); break;
            case 0x36:  result = m_->audio.ReadWAV(0x6, force); break;
            case 0x37:  result = m_->audio.ReadWAV(0x7, force); break;
            case 0x38:  result = m_->audio.ReadWAV(0x8, force); break;
            case 0x39:  result = m_->audio.ReadWAV(0x9, force); break;
            case 0x3A:  result = m_->audio.ReadWAV(0xA, force); break;
            case 0x3B:  result = m_->audio.ReadWAV(0xB, force); break;
            case 0x3C:  result = m_->audio.ReadWAV(0xC, force); break;
            case 0x3D:  result = m_->audio.ReadWAV(0xD, force); break;
            case 0x3E:  result = m_->audio.ReadWAV(0xE, force); break;
            case 0x3F:  result = m_->audio.ReadWAV(0xF, force); break;
            case 0x40:  result = m_->lcd.ReadLCDC(force); break;
            case 0x41:  result = m_->lcd.ReadSTAT(force); break;
            case 0x42:  result = m_->lcd.ReadSCY(force); break;
            case 0x43:  result = m_->lcd.ReadSCX(force); break;
            case 0x44:  result = m_->lcd.ReadLY(force); break;
            case 0x45:  result = m_->lcd.ReadLYC(force); break;
            case 0x46:  result = m_->lcd.ReadDMA(force); break;
            case 0x47:  result = m_->lcd.ReadBGP(force); break;
            case 0x48:  result = m_->lcd.ReadOBP0(force); break;
            case 0x49:  result = m_->lcd.ReadOBP1(force); break;
            case 0x4A:  result = m_->lcd.ReadWY(force); break;
            case 0x4B:  result = m_->lcd.ReadWX(force); break;
            default:    result = 0; break;
        }
    }
    else if (addr < 0xFFFF) { result = m_->hram[addr & (m_->hram.size() - 1)]; }
    else                    { result = m_->cpu.ReadIE(force); }

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
        switch (addr & 0xFF) {
            case 0x00:  m_->joypad.WriteJOYP(val, force); break;
            case 0x01:  m_->serial.WriteSB(val, force); break;
            case 0x02:  m_->serial.WriteSC(val, force); break;
            case 0x04:  m_->timer.WriteDIV(val, force); break;
            case 0x05:  m_->timer.WriteTIMA(val, force); break;
            case 0x06:  m_->timer.WriteTMA(val, force); break;
            case 0x07:  m_->timer.WriteTAC(val, force); break;
            case 0x0F:  m_->cpu.WriteIF(val, force); break;
            case 0x10:  m_->audio.WriteNR10(val, force); break;
            case 0x11:  m_->audio.WriteNR11(val, force); break;
            case 0x12:  m_->audio.WriteNR12(val, force); break;
            case 0x13:  m_->audio.WriteNR13(val, force); break;
            case 0x14:  m_->audio.WriteNR14(val, force); break;
            case 0x15:  m_->audio.WriteNR20(val, force); break;
            case 0x16:  m_->audio.WriteNR21(val, force); break;
            case 0x17:  m_->audio.WriteNR22(val, force); break;
            case 0x18:  m_->audio.WriteNR23(val, force); break;
            case 0x19:  m_->audio.WriteNR24(val, force); break;
            case 0x1A:  m_->audio.WriteNR30(val, force); break;
            case 0x1B:  m_->audio.WriteNR31(val, force); break;
            case 0x1C:  m_->audio.WriteNR32(val, force); break;
            case 0x1D:  m_->audio.WriteNR33(val, force); break;
            case 0x1E:  m_->audio.WriteNR34(val, force); break;
            case 0x1F:  m_->audio.WriteNR40(val, force); break;
            case 0x20:  m_->audio.WriteNR41(val, force); break;
            case 0x21:  m_->audio.WriteNR42(val, force); break;
            case 0x22:  m_->audio.WriteNR43(val, force); break;
            case 0x23:  m_->audio.WriteNR44(val, force); break;
            case 0x24:  m_->audio.WriteNR50(val, force); break;
            case 0x25:  m_->audio.WriteNR51(val, force); break;
            case 0x26:  m_->audio.WriteNR52(val, force); break;
            case 0x30:  m_->audio.WriteWAV(0x0, val, force); break;
            case 0x31:  m_->audio.WriteWAV(0x1, val, force); break;
            case 0x32:  m_->audio.WriteWAV(0x2, val, force); break;
            case 0x33:  m_->audio.WriteWAV(0x3, val, force); break;
            case 0x34:  m_->audio.WriteWAV(0x4, val, force); break;
            case 0x35:  m_->audio.WriteWAV(0x5, val, force); break;
            case 0x36:  m_->audio.WriteWAV(0x6, val, force); break;
            case 0x37:  m_->audio.WriteWAV(0x7, val, force); break;
            case 0x38:  m_->audio.WriteWAV(0x8, val, force); break;
            case 0x39:  m_->audio.WriteWAV(0x9, val, force); break;
            case 0x3A:  m_->audio.WriteWAV(0xA, val, force); break;
            case 0x3B:  m_->audio.WriteWAV(0xB, val, force); break;
            case 0x3C:  m_->audio.WriteWAV(0xC, val, force); break;
            case 0x3D:  m_->audio.WriteWAV(0xD, val, force); break;
            case 0x3E:  m_->audio.WriteWAV(0xE, val, force); break;
            case 0x3F:  m_->audio.WriteWAV(0xF, val, force); break;
            case 0x40:  m_->lcd.WriteLCDC(val, force); break;
            case 0x41:  m_->lcd.WriteSTAT(val, force); break;
            case 0x42:  m_->lcd.WriteSCY(val, force); break;
            case 0x43:  m_->lcd.WriteSCX(val, force); break;
            case 0x44:  m_->lcd.WriteLY(val, force); break;
            case 0x45:  m_->lcd.WriteLYC(val, force); break;
            case 0x46:  m_->lcd.WriteDMA(val, force); break;
            case 0x47:  m_->lcd.WriteBGP(val, force); break;
            case 0x48:  m_->lcd.WriteOBP0(val, force); break;
            case 0x49:  m_->lcd.WriteOBP1(val, force); break;
            case 0x4A:  m_->lcd.WriteWY(val, force); break;
            case 0x4B:  m_->lcd.WriteWX(val, force); break;
            default:    break;
        }
    }
    else if (addr < 0xFFFF) { m_->hram[addr & (m_->hram.size() - 1)] = val; }
    else                    { m_->cpu.WriteIE(val, force); }

    if (obs_) { obs_->Write(*this, addr, val, force); }
}

std::string IO::AddrName(u16 addr) {
    if (addr < 0x8000)      { return "ROM"; }
    else if (addr < 0xA000) { return "VRAM"; }
    else if (addr < 0xC000) { return "SRAM"; }
    else if (addr < 0xFE00) { return "WRAM"; }
    else if (addr < 0xFEA0) { return "OAM"; }
    else if (addr < 0xFF00) { return "HOLE"; }
    else if (addr < 0xFF80) {
        switch (addr & 0xFF) {
            case 0x00:  return "JOYP";
            case 0x01:  return "SB";
            case 0x02:  return "SC";
            case 0x04:  return "DIV";
            case 0x05:  return "TIMA";
            case 0x06:  return "TMA";
            case 0x07:  return "TAC";
            case 0x0F:  return "IF";
            case 0x10:  return "NR10";
            case 0x11:  return "NR11";
            case 0x12:  return "NR12";
            case 0x13:  return "NR13";
            case 0x14:  return "NR14";
            case 0x15:  return "NR20";
            case 0x16:  return "NR21";
            case 0x17:  return "NR22";
            case 0x18:  return "NR23";
            case 0x19:  return "NR24";
            case 0x1A:  return "NR30";
            case 0x1B:  return "NR31";
            case 0x1C:  return "NR32";
            case 0x1D:  return "NR33";
            case 0x1E:  return "NR34";
            case 0x1F:  return "NR40";
            case 0x20:  return "NR41";
            case 0x21:  return "NR42";
            case 0x22:  return "NR43";
            case 0x23:  return "NR44";
            case 0x24:  return "NR50";
            case 0x25:  return "NR51";
            case 0x26:  return "NR52";
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3A:
            case 0x3B:
            case 0x3C:
            case 0x3D:
            case 0x3E:
            case 0x3F:
                return "WAV";
            case 0x40:  return "LCDC";
            case 0x41:  return "STAT";
            case 0x42:  return "SCY";
            case 0x43:  return "SCX";
            case 0x44:  return "LY";
            case 0x45:  return "LYC";
            case 0x46:  return "DMA";
            case 0x47:  return "BGP";
            case 0x48:  return "OBP0";
            case 0x49:  return "OBP1";
            case 0x4A:  return "WY";
            case 0x4B:  return "WX";
            default:    return "HOLE";
        }
    }
    else if (addr < 0xFFFF) { return "HRAM"; }
    else                    { return "IE"; }
}

}
