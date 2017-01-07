#pragma once

// Copyright 2016 oneup

#include <array>

#include "Base.hpp"
#include "Serializer.hpp"

namespace gblr {

class Channel1;
class Audio;

static inline Serializer& operator<<(Serializer &s, const Channel1 &ch);
static inline Deserializer& operator>>(Deserializer &d, Channel1 &ch);

static inline Serializer& operator<<(Serializer &s, const Audio &audio);
static inline Deserializer& operator>>(Deserializer &d, Audio &audio);

struct Machine;

class Channel1 {
    friend struct Machine;

    Audio &audio;
    friend class Audio;

    u8 r0_, r1_, r2_, r3_, r4_;
    u8 r2_shadow_;
    unsigned ctr_, step_;

    u8 vout_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'c','h','a','n','1');
    friend Serializer& operator<<(Serializer &s, const Channel1 &ch);
    friend Deserializer& operator>>(Deserializer &d, Channel1 &ch);

    void TickSweep();
    void TickVolume();
    void TickLength();
    void TickOutput();
public:
    Channel1(Audio &audio);
    Channel1(const Channel1&) = delete;
    Channel1(Channel1&&) = delete;
    Channel1& operator=(const Channel1&) = delete;
    Channel1& operator=(Channel1&&) = delete;

    void Write(unsigned n, u8 val, bool force);
};

static inline Serializer& operator<<(Serializer &s, const Channel1 &ch) {
    s.Start(Channel1::code_);

    return s << ch.r0_ << ch.r1_ << ch.r2_ << ch.r3_ << ch.r4_
             << ch.r2_shadow_
             << ch.ctr_ << ch.step_
             << ch.vout_;
}

static inline Deserializer& operator>>(Deserializer &d, Channel1 &ch) {
    d.Start(Channel1::code_);

    return d >> ch.r0_ >> ch.r1_ >> ch.r2_ >> ch.r3_ >> ch.r4_
             >> ch.r2_shadow_
             >> ch.ctr_ >> ch.step_
             >> ch.vout_;
}


class Audio {
    Machine *m_;
    friend struct Machine;

    Channel1 ch1_;
    friend class Channel1;

    u8 nr50_, nr51_, nr52_;
    unsigned timer_div_, seq_step_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'a','u','d','i','o');
    friend Serializer& operator<<(Serializer &s, const Audio &audio);
    friend Deserializer& operator>>(Deserializer &d, Audio &audio);

    void TickLength();
    void TickSweep();
    void TickVolume();

    void GenerateSample();
public:
    Audio(Machine *m);
    Audio(const Audio&) = delete;
    Audio(Audio&&) = delete;
    Audio& operator=(const Audio&) = delete;
    Audio& operator=(Audio&&) = delete;

    bool Tick();
    void Write(u16 addr, u8 val, bool force);
};

static inline Serializer& operator<<(Serializer &s, const Audio &audio) {
    s.Start(Audio::code_);

    return s << audio.ch1_
             << audio.nr50_ << audio.nr51_ << audio.nr52_
             << audio.timer_div_ << audio.seq_step_;
}

static inline Deserializer& operator>>(Deserializer &d, Audio &audio) {
    d.Start(Audio::code_);

    return d >> audio.ch1_
             >> audio.nr50_ >> audio.nr51_ >> audio.nr52_
             >> audio.timer_div_ >> audio.seq_step_;
}

}   // namespace gblr
