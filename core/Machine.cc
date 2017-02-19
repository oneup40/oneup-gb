// Copyright 2016-2017 oneup

#include "core/Machine.hpp"

#include <chrono>
#include <iostream>

#include <cassert>

#include "core/Frontend.hpp"

namespace gb1 {

Machine::Machine(Frontend& frontend, const MachineObserver &observer)
    : frontend(frontend),
      cpu(this, observer.cpu), lcd(this), mapper(), joypad(this), timer(this), audio(this),
      t(0),
      obs(observer.io)
{
    wram.fill(0);
    hram.fill(0);
}

bool Machine::Tick() {
	using namespace std::chrono;

    bool good = true;

	//auto T0 = system_clock::now();

    //auto t0 = system_clock::now();
    good = lcd.Tick() && good;
    //s_lcd_time += system_clock::now() - t0;

    if ((t & 0x07) == 0x00) {
        //t0 = system_clock::now();
        good = timer.Tick() && good;
        //s_timer_time += system_clock::now() - t0;
    }

    if ((t & 0x03) == 0x00) {
        //t0 = system_clock::now();
        good = cpu.Tick() && good;
        //s_cpu_time += system_clock::now() - t0;
    }

	if ((t & 0x01) == 0x00) {
		//t0 = system_clock::now();
		good = audio.Tick() && good;
		//s_audio_time += system_clock::now() - t0;
	}

    ++t;

	//s_total_time += system_clock::now() - T0;
    return good;
}

u8 Machine::Read(u16 addr, bool force) {
    u8 result = 0;

    if (addr < 0x8000)      { result = mapper.ReadROM(addr, force); }
    else if (addr < 0xA000) { result = lcd.ReadVRAM(addr, force); }
    else if (addr < 0xC000) { result = mapper.ReadRAM(addr, force); }
    else if (addr < 0xFE00) { result = wram[addr & (wram.size() - 1)]; }
    else if (addr < 0xFEA0) { result = lcd.ReadOAM(addr, force); }
    else if (addr < 0xFF00) { result = 0; }
    else if (addr < 0xFF80) {
        switch (addr & 0xFF) {
            case 0x00:  result = joypad.ReadJoyp(force); break;
            case 0x04:  result = timer.ReadDiv(force); break;
            case 0x05:  result = timer.tima_; break;
            case 0x06:  result = timer.tma_; break;
            case 0x07:  result = timer.tac_; break;
            case 0x0F:  result = cpu.if_ & 0xFF; break;
            case 0x10:  result = audio.ch1_.r0_; break;
            case 0x11:  result = audio.ch1_.r1_; break;
            case 0x12:  result = audio.ch1_.r2_; break;
            case 0x13:  result = audio.ch1_.r3_; break;
            case 0x14:  result = audio.ch1_.r4_; break;
			case 0x15:  result = audio.ch2_.r0_; break;
			case 0x16:  result = audio.ch2_.r1_; break;
			case 0x17:  result = audio.ch2_.r2_; break;
			case 0x18:  result = audio.ch2_.r3_; break;
			case 0x19:  result = audio.ch2_.r4_; break;
			case 0x1A:  result = audio.ch3_.r0_; break;
			case 0x1B:  result = audio.ch3_.r1_; break;
			case 0x1C:  result = audio.ch3_.r2_; break;
			case 0x1D:  result = audio.ch3_.r3_; break;
			case 0x1E:  result = audio.ch3_.r4_; break;
			case 0x1F:  result = audio.ch4_.r0_; break;
			case 0x20:  result = audio.ch4_.r1_; break;
			case 0x21:  result = audio.ch4_.r2_; break;
			case 0x22:  result = audio.ch4_.r3_; break;
			case 0x23:  result = audio.ch4_.r4_; break;
            case 0x24:  result = audio.nr50_; break;
            case 0x25:  result = audio.nr51_; break;
            case 0x26:  result = audio.nr52_; break;
			case 0x30:	result = audio.ch3_.wave_[0x0]; break;
			case 0x31:	result = audio.ch3_.wave_[0x1]; break;
			case 0x32:	result = audio.ch3_.wave_[0x2]; break;
			case 0x33:	result = audio.ch3_.wave_[0x3]; break;
			case 0x34:	result = audio.ch3_.wave_[0x4]; break;
			case 0x35:	result = audio.ch3_.wave_[0x5]; break;
			case 0x36:	result = audio.ch3_.wave_[0x6]; break;
			case 0x37:	result = audio.ch3_.wave_[0x7]; break;
			case 0x38:	result = audio.ch3_.wave_[0x8]; break;
			case 0x39:	result = audio.ch3_.wave_[0x9]; break;
			case 0x3A:	result = audio.ch3_.wave_[0xA]; break;
			case 0x3B:	result = audio.ch3_.wave_[0xB]; break;
			case 0x3C:	result = audio.ch3_.wave_[0xC]; break;
			case 0x3D:	result = audio.ch3_.wave_[0xD]; break;
			case 0x3E:	result = audio.ch3_.wave_[0xE]; break;
			case 0x3F:	result = audio.ch3_.wave_[0xF]; break;
            case 0x40:  result = lcd.lcdc_; break;
            case 0x41:  result = lcd.stat_; break;
            case 0x42:  result = lcd.scy_; break;
            case 0x43:  result = lcd.scx_; break;
            case 0x44:  result = lcd.ly_; break;
            case 0x45:  result = lcd.lyc_; break;
            case 0x46:  result = lcd.ReadDMA(force); break;
            case 0x47:  result = lcd.bgp_; break;
            case 0x48:  result = lcd.obp0_; break;
            case 0x49:  result = lcd.obp1_; break;
            case 0x4A:  result = lcd.wy_; break;
            case 0x4B:  result = lcd.wx_; break;
            default:    result = 0; break;
        }
    }
    else if (addr < 0xFFFF) { result = hram[addr & (hram.size() - 1)]; }
    else                    { result = cpu.ie_ & 0xFF; }

    if (obs) { obs->Read(*this, addr, result, force); }
    return result;
}

void Machine::Write(u16 addr, u8 val, bool force) {
    if (addr < 0x8000)      { mapper.WriteROM(addr, val, force); }
    else if (addr < 0xA000) { lcd.WriteVRAM(addr, val, force); }
    else if (addr < 0xC000) { mapper.WriteRAM(addr, val, force); }
    else if (addr < 0xFE00) { wram[addr & (wram.size() - 1)] = val; }
    else if (addr < 0xFEA0) { lcd.WriteOAM(addr, val, force); }
    else if (addr < 0xFF00) { }
    else if (addr < 0xFF80) {
        if (0xFF10 <= addr && addr < 0xFF40) { audio.Write(addr, val, force); }
        else {
            switch (addr & 0xFF) {
                case 0x00:  joypad.WriteJoyp(val, force); break;
                case 0x04:  timer.WriteDiv(force); break;
                case 0x05:  timer.tima_ = val; break;
                case 0x06:  timer.tma_ = val; break;
                case 0x07:  timer.tac_ = val; break;
                case 0x0F:  cpu.if_ = val & 0x1F; break;
                case 0x40:  lcd.lcdc_ = val; break;
                case 0x41:  lcd.stat_ = val; break;
                case 0x42:  lcd.scy_ = val; break;
                case 0x43:  lcd.scx_ = val; break;
                case 0x44:  lcd.ly_ = 0; break;
                case 0x45:  lcd.lyc_ = val; break;
                case 0x46:  lcd.WriteDMA(val, force); break;
                case 0x47:  lcd.bgp_ = val; break;
                case 0x48:  lcd.obp0_ = val; break;
                case 0x49:  lcd.obp1_ = val; break;
                case 0x4A:  lcd.wy_ = val; break;
                case 0x4B:  lcd.wx_ = val; break;
                default:    break;
            }
        }
    }
    else if (addr < 0xFFFF) { hram[addr & (hram.size() - 1)] = val; }
    else                    { cpu.ie_ = val & 0x1F; }

    if (obs) { obs->Write(*this, addr, val, force); }
}

void Machine::Interrupt(u8 num) { cpu.Interrupt(num); }

void Machine::Log(const std::string &msg) { std::cerr << msg << std::endl; }

bool Machine::LoadGame(std::vector<u8>&& data) { return Loader::Load(std::move(data), this); }

void Machine::UnloadGame() { mapper.Unload(); }

Serializer& operator<<(Serializer &s, const Machine &m) {
    std::basic_string<u8> wram(m.wram.begin(), m.wram.end()),
                          hram(m.hram.begin(), m.hram.end());
    s.Start(m.code_);
    return s << m.cpu << m.lcd << m.mapper << m.joypad << m.timer << m.audio << wram << hram;
}

Deserializer& operator>>(Deserializer &d, Machine &m) {
    std::basic_string<u8> wram, hram;

    d.Start(m.code_);

    d >> m.cpu;
    if (!d) { m.Log("Error deserializing CPU"); return d; }

    d >> m.lcd;
    if (!d) { m.Log("Error deserializing LCD"); return d; }

    d >> m.mapper;
    if (!d) { m.Log("Error deserializing mapper"); return d; }

    d >> m.joypad;
    if (!d) { m.Log("Error deserializing joypad"); return d; }

    d >> m.timer;
    if (!d) { m.Log("Error deserializing timer"); return d; }

    d >> m.audio;
    if (!d) { m.Log("Error deserializing audio"); return d; }

    d >> wram;
    if (!d) { m.Log("Error deserializing WRAM"); return d; }

    d >> hram;
    if (!d) { m.Log("Error deserializing HRAM"); return d; }

    std::copy(wram.begin(), wram.end(), m.wram.begin());
    std::copy(hram.begin(), hram.end(), m.hram.begin());

    return d;
}

}    // namespace gb1

