// Copyright 2016 oneup

#include "Machine.hpp"

#include <chrono>
#include <iostream>

#include <cassert>

#include "LRConnector.hpp"

namespace gblr {

Machine::Machine(LRConnector *frontend)
	: frontend(frontend),
	cpu(this), lcd(this), mapper(this), joypad(this), timer(this), audio(this),
	t(0),
	frame_ready(false)
{
	wram.fill(0);
	hram.fill(0);
}

//static std::chrono::nanoseconds s_cpu_time, s_lcd_time, s_timer_time, s_audio_time, s_total_time;

Machine::~Machine() {
	std::chrono::nanoseconds elapsed((long long)(t / .004194304));
	std::cerr << t << " ticks, " << elapsed.count() / 1000000000. << " s" << std::endl;

	//std::cerr << "Destroying machine" << std::endl
	//	<< t << " ticks" << std::endl
	//	<< elapsed.count() / 1000000000. << " s" << std::endl
	//	<< "CPU emulation usage: " << double(s_cpu_time.count()) / elapsed.count() * 100 << " (" << s_cpu_time.count() / 1000000000. << " s)" << std::endl
	//	<< "LCD emulation usage: " << double(s_lcd_time.count()) / elapsed.count() * 100 << " (" << s_lcd_time.count() / 1000000000. << " s)" << std::endl
	//	<< "Timer emulation usage: " << double(s_timer_time.count()) / elapsed.count() * 100 << " (" << s_timer_time.count() / 1000000000. << " s)" << std::endl
	//	<< "Audio emulation usage: " << double(s_audio_time.count()) / elapsed.count() * 100 << " (" << s_audio_time.count() / 1000000000. << " s)" << std::endl
	//	<< "Total usage:" << double(s_total_time.count()) / elapsed.count() * 100 << " (" << s_total_time.count() / 1000000000. << " s)" << std::endl;
}

bool Machine::Tick() {
	using namespace std::chrono;

    bool good = true;

	//auto T0 = system_clock::now();

    //auto t0 = system_clock::now();
    good = lcd.Tick() && good;
    //s_lcd_time += system_clock::now() - t0;

    if ((t & 0x03) == 0x00) {
        //t0 = system_clock::now();
        good = cpu.Tick() && good;
        //s_cpu_time += system_clock::now() - t0;

        //t0 = system_clock::now();
        good = timer.Tick() && good;
        //s_timer_time += system_clock::now() - t0;
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
    if (addr < 0x8000)      { return mapper.ReadROM(addr, force); }
    else if (addr < 0xA000) { return lcd.ReadVRAM(addr, force); }
    else if (addr < 0xC000) { return mapper.ReadRAM(addr, force); }
    else if (addr < 0xFE00) { return wram[addr & (wram.size() - 1)]; }
    else if (addr < 0xFEA0) { return lcd.ReadOAM(addr, force); }
    else if (addr < 0xFF00) { return 0; }
    else if (addr < 0xFF80) {
        switch (addr & 0xFF) {
            case 0x00:  return joypad.ReadJoyp(force);
            case 0x04:  return timer.ReadDiv(force);
            case 0x05:  return timer.tima_;
            case 0x06:  return timer.tma_;
            case 0x07:  return timer.tac_;
            case 0x0F:  return cpu.if_ & 0xFF;
            case 0x10:  return audio.ch1_.r0_;
            case 0x11:  return audio.ch1_.r1_;
            case 0x12:  return audio.ch1_.r2_;
            case 0x13:  return audio.ch1_.r3_;
            case 0x14:  return audio.ch1_.r4_;
			case 0x15:  return audio.ch2_.r0_;
			case 0x16:  return audio.ch2_.r1_;
			case 0x17:  return audio.ch2_.r2_;
			case 0x18:  return audio.ch2_.r3_;
			case 0x19:  return audio.ch2_.r4_;
			case 0x1A:  return audio.ch3_.r0_;
			case 0x1B:  return audio.ch3_.r1_;
			case 0x1C:  return audio.ch3_.r2_;
			case 0x1D:  return audio.ch3_.r3_;
			case 0x1E:  return audio.ch3_.r4_;
			case 0x1F:  return audio.ch4_.r0_;
			case 0x20:  return audio.ch4_.r1_;
			case 0x21:  return audio.ch4_.r2_;
			case 0x22:  return audio.ch4_.r3_;
			case 0x23:  return audio.ch4_.r4_;
            case 0x24:  return audio.nr50_;
            case 0x25:  return audio.nr51_;
            case 0x26:  return audio.nr52_;
			case 0x30:	return audio.ch3_.wave_[0x0];
			case 0x31:	return audio.ch3_.wave_[0x1];
			case 0x32:	return audio.ch3_.wave_[0x2];
			case 0x33:	return audio.ch3_.wave_[0x3];
			case 0x34:	return audio.ch3_.wave_[0x4];
			case 0x35:	return audio.ch3_.wave_[0x5];
			case 0x36:	return audio.ch3_.wave_[0x6];
			case 0x37:	return audio.ch3_.wave_[0x7];
			case 0x38:	return audio.ch3_.wave_[0x8];
			case 0x39:	return audio.ch3_.wave_[0x9];
			case 0x3A:	return audio.ch3_.wave_[0xA];
			case 0x3B:	return audio.ch3_.wave_[0xB];
			case 0x3C:	return audio.ch3_.wave_[0xC];
			case 0x3D:	return audio.ch3_.wave_[0xD];
			case 0x3E:	return audio.ch3_.wave_[0xE];
			case 0x3F:	return audio.ch3_.wave_[0xF];
            case 0x40:  return lcd.lcdc_;
            case 0x41:  return lcd.stat_;
            case 0x42:  return lcd.scy_;
            case 0x43:  return lcd.scx_;
            case 0x44:  return lcd.ly_;
            case 0x45:  return lcd.lyc_;
            case 0x46:  return lcd.ReadDMA(force);
            case 0x47:  return lcd.bgp_;
            case 0x48:  return lcd.obp0_;
            case 0x49:  return lcd.obp1_;
            case 0x4A:  return lcd.wy_;
            case 0x4B:  return lcd.wx_;
            default:    return 0;
        }
    }
    else if (addr < 0xFFFF) { return hram[addr & (hram.size() - 1)]; }
    else                    { return cpu.ie_ & 0xFF; }
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
}

void Machine::Interrupt(u8 num) { cpu.Interrupt(num); }

void Machine::Log(const std::string &msg) { std::cerr << msg << std::endl; }

bool Machine::LoadGame(const retro_game_info *game) { return Loader::Load(game, this); }

void Machine::UnloadGame() { mapper.Unload(); }

bool Machine::FrameReady() const { return frame_ready; }

const u32* Machine::GetFrame() const { return &lcd.frame_[0][0]; }

void Machine::ResetFrame() { frame_ready = false; }

Serializer& operator<<(Serializer &s, const Machine &m) {
    std::basic_string<u8> wram(m.wram.begin(), m.wram.end()),
                          hram(m.hram.begin(), m.hram.end());
    s.Start(m.code_);
    return s << m.cpu << m.lcd << m.mapper << m.joypad << m.timer << m.audio << wram << hram << m.frame_ready;
}

Deserializer& operator>>(Deserializer &d, Machine &m) {
    std::basic_string<u8> wram, hram;

    d.Start(m.code_);

    d >> m.cpu;
    if (!d) { m.frontend->ShowMessage("Error deserializing CPU", 600); return d; }

    d >> m.lcd;
    if (!d) { m.frontend->ShowMessage("Error deserializing LCD", 600); return d; }

    d >> m.mapper;
    if (!d) { m.frontend->ShowMessage("Error deserializing mapper", 600); return d; }

    d >> m.joypad;
    if (!d) { m.frontend->ShowMessage("Error deserializing joypad", 600); return d; }

    d >> m.timer;
    if (!d) { m.frontend->ShowMessage("Error deserializing timer", 600); return d; }

    d >> m.audio;
    if (!d) { m.frontend->ShowMessage("Error deserializing audio", 600); return d; }

    d >> wram;
    if (!d) { m.frontend->ShowMessage("Error deserializing WRAM", 600); return d; }

    d >> hram;
    if (!d) { m.frontend->ShowMessage("Error deserializing HRAM", 600); return d; }

    d >> m.frame_ready;
    if (!d) { m.frontend->ShowMessage("Error deserializing machine", 600); return d; }

    std::copy(wram.begin(), wram.end(), m.wram.begin());
    std::copy(hram.begin(), hram.end(), m.hram.begin());

    return d;
}


}    // namespace gblr


