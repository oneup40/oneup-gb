#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

struct Machine;

class Joypad;

static inline Serializer& operator<<(Serializer &s, const Joypad &joypad);
static inline Deserializer& operator>>(Deserializer &d, Joypad &joypad);

class Joypad {
    Machine *m_;

    u8 joyp_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'j','o','y','p','a','d');
    friend Serializer& operator<<(Serializer &s, const Joypad &joypad);
    friend Deserializer& operator>>(Deserializer &d, Joypad &joypad);
public:
    Joypad(Machine *m);
    Joypad(const Joypad&) = delete;
    Joypad(Joypad&&) = delete;
    Joypad& operator=(const Joypad&) = delete;
    Joypad& operator=(Joypad&&) = delete;

    u8 ReadJOYP(bool force);
    void WriteJOYP(u8 val, bool force);
};

static inline Serializer& operator<<(Serializer &s, const Joypad &joypad) {
    s.Start(Joypad::code_);
    return s << joypad.joyp_;
}

static inline Deserializer& operator>>(Deserializer &d, Joypad &joypad) {
    d.Start(Joypad::code_);
    return d >> joypad.joyp_;
}

}    // namespace gb1
