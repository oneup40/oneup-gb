#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

class Channel2;

static inline Serializer& operator<<(Serializer &s, const Channel2 &ch);
static inline Deserializer& operator>>(Deserializer &d, Channel2 &ch);

class Audio;

class Channel2 {
    Audio &audio;

    u8 r0_, r1_, r2_, r3_, r4_;
    u8 vol_, vol_div_;
    unsigned ctr_, step_;

    u8 vout_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'c','h','a','n','2');
    friend Serializer& operator<<(Serializer &s, const Channel2 &ch);
    friend Deserializer& operator>>(Deserializer &d, Channel2 &ch);
public:
    Channel2(Audio &audio);
    Channel2(const Channel2&) = delete;
    Channel2(Channel2&&) = delete;
    Channel2& operator=(const Channel2&) = delete;
    Channel2& operator=(Channel2&&) = delete;

    void TickVolume();
    void TickLength();
    void TickOutput();

    u8 ReadNR20(bool /* force */ = false) const { return r0_; }
    void WriteNR20(u8 val, bool /* force */ = false) { r0_ = val; }
    u8 ReadNR21(bool /* force */ = false) const { return r1_; }
    void WriteNR21(u8 val, bool /* force */ = false) { r1_ = val; }
    u8 ReadNR22(bool /* force */ = false) const { return r2_; }
    void WriteNR22(u8 val, bool /* force */ = false) {
        r2_ = val;
        vol_ = r2_ >> 4;
        vol_div_ = r2_ & 0x07;
    }
    u8 ReadNR23(bool /* force */ = false) const { return r3_; }
    void WriteNR23(u8 val, bool /* force */ = false) {
        r3_ = val;
        ctr_ = ((r4_ & 0x07) << 8) | r3_;
    }
    u8 ReadNR24(bool /* force */ = false) const { return r4_; }
    void WriteNR24(u8 val, bool /* force */ = false);

    u8 Vout() const { return vout_; }
};

static inline Serializer& operator<<(Serializer &s, const Channel2 &ch) {
    s.Start(Channel2::code_);

    return s << ch.r0_ << ch.r1_ << ch.r2_ << ch.r3_ << ch.r4_
             << ch.vol_ << ch.vol_div_
             << ch.ctr_ << ch.step_
             << ch.vout_;
}

static inline Deserializer& operator>>(Deserializer &d, Channel2 &ch) {
    d.Start(Channel2::code_);

    return d >> ch.r0_ >> ch.r1_ >> ch.r2_ >> ch.r3_ >> ch.r4_
             >> ch.vol_ >> ch.vol_div_
             >> ch.ctr_ >> ch.step_
             >> ch.vout_;
}

}   // namespace gb1
