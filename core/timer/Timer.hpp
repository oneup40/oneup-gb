#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

struct Machine;

class Timer;

static inline Serializer& operator<<(Serializer &s, const Timer &timer);
static inline Deserializer& operator>>(Deserializer &d, Timer &timer);

class Timer {
    friend class IO;

    Machine *m_;

    u8 tima_, tma_, tac_;

    u32 fulldiv_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 't','i','m','e','r');
    friend Serializer& operator<<(Serializer &s, const Timer &timer);
    friend Deserializer& operator>>(Deserializer &d, Timer &timer);
public:
    Timer(Machine *m);
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    u8 ReadDiv(bool force = false);
    void WriteDiv(u8 val, bool force = false);

    bool Tick();
};

static inline Serializer& operator<<(Serializer &s, const Timer &timer) {
    s.Start(Timer::code_);
    return s << timer.tima_ << timer.tma_ << timer.tac_ << timer.fulldiv_;
}

static inline Deserializer& operator>>(Deserializer &d, Timer &timer) {
    d.Start(Timer::code_);
    d >> timer.tima_ >> timer.tma_ >> timer.tac_ >> timer.fulldiv_;

    return d;
}

}    // namespace gb1
