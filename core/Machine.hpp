#pragma once

// Copyright 2016-2017 oneup

#include <functional>
#include <memory>

#include "core/audio/Audio.hpp"
#include "core/cpu/CPU.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/io/IO.hpp"
#include "core/io/IOObserver.hpp"
#include "core/joypad/Joypad.hpp"
#include "core/lcd/LCD.hpp"
#include "core/Loader.hpp"
#include "core/MachineObserver.hpp"
#include "core/Mapper.hpp"
#include "core/serial/Serial.hpp"
#include "core/timer/Timer.hpp"

namespace gb1 {

enum Button : u8 {
    BTN_RIGHT   = 0x01,
    BTN_LEFT    = 0x02,
    BTN_UP      = 0x04,
    BTN_DOWN    = 0x08,
    BTN_A       = 0x10,
    BTN_B       = 0x20,
    BTN_SELECT  = 0x40,
    BTN_START   = 0x80
};

struct Frontend;

struct Machine {
    Frontend& frontend;
    IO io;

    Audio audio;
    CPU cpu;
    Joypad joypad;
    LCD lcd;
    Mapper mapper;
    Serial serial;
    Timer timer;

    std::array<u8, 0x2000> wram;
    std::array<u8, 0x80> hram;

    u64 t;

    static constexpr const u8 version_ = 0x02;
    static constexpr const u64 code_ = eight_cc(version_,'m','a','c','h','i','n','e');

    explicit Machine(Frontend& frontend, const MachineObserver &observer = MachineObserver());
    Machine(const Machine&) = delete;
    Machine(Machine&&) = delete;
    Machine& operator=(const Machine&) = delete;
    Machine& operator=(Machine&&) = delete;

    void Tick();
    u8 Read(u16 addr, bool force = false);
    void Write(u16 addr, u8 val, bool force = false);
    void Interrupt(u8 num);
    void Log(const std::string &msg);

    bool LoadGame(std::vector<u8>&& data);
    void UnloadGame();
};

Serializer& operator<<(Serializer &s, const Machine &m);
Deserializer& operator>>(Deserializer &d, Machine &m);

} // namespace gb1
