#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

class Channel4;

static inline Serializer& operator<<(Serializer &s, const Channel4 &ch);
static inline Deserializer& operator>>(Deserializer &d, Channel4 &ch);

class Audio;

class Channel4 {
    Audio &audio;

    u8 r0_, r1_, r2_, r3_, r4_;
    u8 vol_, vol_div_;
    u16 lsfr_;
    u32 lsfr_div_;

    u8 vout_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'c', 'h', 'a', 'n', '4');
    friend Serializer& operator<<(Serializer &s, const Channel4 &ch);
    friend Deserializer& operator >> (Deserializer &d, Channel4 &ch);
public:
    Channel4(Audio &audio);
    Channel4(const Channel4&) = delete;
    Channel4(Channel4&&) = delete;
    Channel4& operator=(const Channel4&) = delete;
    Channel4& operator=(Channel4&&) = delete;

    void TickVolume();
    void TickLength();
    void TickOutput();

    u8 ReadNR40(bool /* force */ = false) const { return r0_; }
    void WriteNR40(u8 val, bool /* force */ = false) { r0_ = val; }
    u8 ReadNR41(bool /* force */ = false) const { return r1_; }
    void WriteNR41(u8 val, bool /* force */ = false) { r1_ = val; }
    u8 ReadNR42(bool /* force */ = false) const { return r2_; }
    void WriteNR42(u8 val, bool /* force */ = false) {
        r2_ = val;
        vol_ = r2_ >> 4;
        vol_div_ = r2_ & 0x07;
    }
    u8 ReadNR43(bool /* force */ = false) const { return r3_; }
    void WriteNR43(u8 val, bool /* force */ = false) { r3_ = val; }
    u8 ReadNR44(bool /* force */ = false) const { return r4_; }
    void WriteNR44(u8 val, bool /* force */ = false);

    u8 Vout() const { return vout_; }
};

static inline Serializer& operator<<(Serializer &s, const Channel4 &ch) {
    s.Start(Channel4::code_);

    return s << ch.r0_ << ch.r1_ << ch.r2_ << ch.r3_ << ch.r4_
             << ch.vol_ << ch.vol_div_
             << ch.lsfr_ << ch.lsfr_div_
             << ch.vout_;
}

static inline Deserializer& operator>>(Deserializer &d, Channel4 &ch) {
    d.Start(Channel4::code_);

    return d >> ch.r0_ >> ch.r1_ >> ch.r2_ >> ch.r3_ >> ch.r4_
             >> ch.vol_ >> ch.vol_div_
             >> ch.lsfr_ >> ch.lsfr_div_
             >> ch.vout_;
}

}   // namespace gb1
