#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

class Channel1;

static inline Serializer& operator<<(Serializer &s, const Channel1 &ch);
static inline Deserializer& operator>>(Deserializer &d, Channel1 &ch);

class Audio;

class Channel1 {
    Audio &audio;

    u8 r0_, r1_, r2_, r3_, r4_;
    u8 sweep_div_;
    u8 vol_, vol_div_;
    unsigned ctr_, step_;

    u8 vout_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'c','h','a','n','1');
    friend Serializer& operator<<(Serializer &s, const Channel1 &ch);
    friend Deserializer& operator>>(Deserializer &d, Channel1 &ch);
public:
    Channel1(Audio &audio);
    Channel1(const Channel1&) = delete;
    Channel1(Channel1&&) = delete;
    Channel1& operator=(const Channel1&) = delete;
    Channel1& operator=(Channel1&&) = delete;

    void TickSweep();
    void TickVolume();
    void TickLength();
    void TickOutput();

    u8 ReadNR10(bool /* force */ = false) const { return r0_; }
    void WriteNR10(u8 val, bool /* force */ = false) { r0_ = val; }
    u8 ReadNR11(bool /* force */ = false) const { return r1_; }
    void WriteNR11(u8 val, bool /* force */ = false) { r1_ = val; }
    u8 ReadNR12(bool /* force */ = false) const { return r2_; }
    void WriteNR12(u8 val, bool /* force */ = false) {
        r2_ = val;
        vol_ = r2_ >> 4;
        vol_div_ = r2_ & 0x07;
    }
    u8 ReadNR13(bool /* force */ = false) const { return r3_; }
    void WriteNR13(u8 val, bool /* force */ = false) {
        r3_ = val;
        ctr_ = ((r4_ & 0x07) << 8) | r3_;
    }
    u8 ReadNR14(bool /* force */ = false) const { return r4_; }
    void WriteNR14(u8 val, bool /* force */ = false);

    u8 Vout() const { return vout_; }
};

static inline Serializer& operator<<(Serializer &s, const Channel1 &ch) {
    s.Start(Channel1::code_);

    return s << ch.r0_ << ch.r1_ << ch.r2_ << ch.r3_ << ch.r4_
             << ch.sweep_div_
             << ch.vol_ << ch.vol_div_
             << ch.ctr_ << ch.step_
             << ch.vout_;
}

static inline Deserializer& operator>>(Deserializer &d, Channel1 &ch) {
    d.Start(Channel1::code_);

    return d >> ch.r0_ >> ch.r1_ >> ch.r2_ >> ch.r3_ >> ch.r4_
             >> ch.sweep_div_
             >> ch.vol_ >> ch.vol_div_
             >> ch.ctr_ >> ch.step_
             >> ch.vout_;
}

}   // namespace gb1
