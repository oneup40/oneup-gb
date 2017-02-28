// Copyright 2016-2017 oneup

#include "core/Machine.hpp"

#include <chrono>
#include <iostream>

#include <cassert>

#include "core/Frontend.hpp"

namespace gb1 {

Machine::Machine(Frontend& frontend, const MachineObserver &observer)
    : frontend(frontend),
      io(this, observer.io),
      audio(this),
      cpu(this, observer.cpu),
      joypad(this),
      lcd(this),
      mapper(),
      serial(this),
      timer(this, observer.timer),
      t(0)
{
    wram.fill(0);
    hram.fill(0);
}

void Machine::Tick() {
    // This is the main 4 MHz system clock

	using namespace std::chrono;

    lcd.Tick();
    ++t;

    lcd.Tick();
    audio.Tick();
    ++t;

    lcd.Tick();
    ++t;

    lcd.Tick();
    audio.Tick();
    cpu.Tick();
    ++t;

    lcd.Tick();
    ++t;

    lcd.Tick();
    audio.Tick();
    ++t;

    lcd.Tick();
    ++t;

    lcd.Tick();
    audio.Tick();
    cpu.Tick();
    timer.Tick();
    ++t;

    if ((t & 0x200) == 0x00) {
        serial.Tick();
    }
}

u8 Machine::Read(u16 addr, bool force) { return io.Read(addr, force); }
void Machine::Write(u16 addr, u8 val, bool force) { io.Write(addr, val, force); }

void Machine::Interrupt(u8 num) { cpu.Interrupt(num); }

void Machine::Log(const std::string &msg) { std::cerr << msg << std::endl; }

bool Machine::LoadGame(std::vector<u8>&& data) { return Loader::Load(std::move(data), this); }

void Machine::UnloadGame() { mapper.Unload(); }

Serializer& operator<<(Serializer &s, const Machine &m) {
    std::basic_string<u8> wram(m.wram.begin(), m.wram.end()),
                          hram(m.hram.begin(), m.hram.end());
    s.Start(m.code_);
    return s << m.cpu << m.lcd << m.mapper << m.joypad << m.timer << m.audio << m.serial << wram << hram;
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

    d >> m.serial;
    if (!d) { m.Log("Error deserializing serial"); return d; }

    d >> wram;
    if (!d) { m.Log("Error deserializing WRAM"); return d; }

    d >> hram;
    if (!d) { m.Log("Error deserializing HRAM"); return d; }

    std::copy(wram.begin(), wram.end(), m.wram.begin());
    std::copy(hram.begin(), hram.end(), m.hram.begin());

    return d;
}

}    // namespace gb1

