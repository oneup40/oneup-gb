#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"
#include "core/timer/TimerObserver.hpp"

namespace gb1 {

struct Machine;

class Timer;

static inline Serializer& operator<<(Serializer &s, const Timer &timer);
static inline Deserializer& operator>>(Deserializer &d, Timer &timer);

class Timer {
    Machine *m_;
    TimerObserver *obs_;

    u8 tima_, tma_, tac_;
    u32 fulldiv_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 't','i','m','e','r');
    friend Serializer& operator<<(Serializer &s, const Timer &timer);
    friend Deserializer& operator>>(Deserializer &d, Timer &timer);
public:
    explicit Timer(Machine *m, TimerObserver *observer = nullptr);
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    u8 ReadTIMA(bool /* force */ = false) const { return tima_; }
    void WriteTIMA(u8 val, bool /* force */ = false) {
        tima_ = val;
        if (obs_) { obs_->TimaUpdate(*this, tima_); }
    }

    u8 ReadTMA(bool /* force */ = false) const { return tma_; }
    void WriteTMA(u8 val, bool /* force */ = false) { tma_ = val; }

    u8 ReadTAC(bool /* force */ = false) const { return tac_; }
    void WriteTAC(u8 val, bool /* force */ = false) { tac_ = val; }

    u8 ReadDIV(bool /* force */ = false) const { return fulldiv_ >> 5; }
    void WriteDIV(u8 val, bool force = false) {
        if (force)  { fulldiv_ = val << 5; }
        else        { fulldiv_ = 0; }
        if (obs_) { obs_->FulldivUpdate(*this, fulldiv_); }
    }

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
