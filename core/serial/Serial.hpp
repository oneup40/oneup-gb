#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

class Serial;

static inline Serializer& operator<<(Serializer &s, const Serial &serial);
static inline Deserializer& operator>>(Deserializer &d, Serial &serial);

struct Machine;

class Serial {
    Machine *m_;

    u8 sb_, sc_, bits_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 's','e','r','i','a','l');
    friend Serializer& operator<<(Serializer &s, const Serial &serial);
    friend Deserializer& operator>>(Deserializer &d, Serial &serial);
public:
    explicit Serial(Machine *m) : m_(m), sb_(0), sc_(0), bits_(0) {}
    Serial(const Serial&) = delete;
    Serial(Serial&&) = delete;
    Serial& operator=(const Serial&) = delete;
    Serial& operator=(Serial&&) = delete;

    u8 ReadSB(bool /* force */ = false) { return sb_; }
    void WriteSB(u8 val, bool /* force */ = false) { sb_ = val; }
    u8 ReadSC(bool /* force */ = false) { return sc_; }
    void WriteSC(u8 val, bool /* force */ = false) {
        if (val & 0x80) { bits_ = 8; }
        else            { bits_ = 0; }
        sc_ = val;
    }

    bool Tick();
};

static inline Serializer& operator<<(Serializer &s, const Serial &serial) {
    s.Start(Serial::code_);
    return s << serial.sb_ << serial.sc_ << serial.bits_;
}

static inline Deserializer& operator>>(Deserializer &d, Serial &serial) {
    d.Start(Serial::code_);
    d >> serial.sb_ >> serial.sc_ >> serial.bits_;

    return d;
}

}   // namespace gb1
