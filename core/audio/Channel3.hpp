#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

class Channel3;

static inline Serializer& operator<<(Serializer &s, const Channel3 &ch);
static inline Deserializer& operator>>(Deserializer &d, Channel3 &ch);

class Audio;

class Channel3 {
    Audio &audio;

    u8 r0_, r1_, r2_, r3_, r4_;
    unsigned ctr_, ndx_;

    u8 vout_;

    std::array<u8, 16> wave_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'c', 'h', 'a', 'n', '3');
    friend Serializer& operator<<(Serializer &s, const Channel3 &ch);
    friend Deserializer& operator>>(Deserializer &d, Channel3 &ch);
public:
    Channel3(Audio &audio);
    Channel3(const Channel3&) = delete;
    Channel3(Channel3&&) = delete;
    Channel3& operator=(const Channel3&) = delete;
    Channel3& operator=(Channel3&&) = delete;

    void TickLength();
    void TickOutput();

    u8 ReadNR30(bool /* force */ = false) const { return r0_; }
    void WriteNR30(u8 val, bool /* force */ = false) { r0_ = val; }
    u8 ReadNR31(bool /* force */ = false) const { return r1_; }
    void WriteNR31(u8 val, bool /* force */ = false) { r1_ = val; }
    u8 ReadNR32(bool /* force */ = false) const { return r2_; }
    void WriteNR32(u8 val, bool /* force */ = false) { r2_ = val; }
    u8 ReadNR33(bool /* force */ = false) const { return r3_; }
    void WriteNR33(u8 val, bool /* force */ = false) {
        r3_ = val;
        ctr_ = ((r4_ & 0x07) << 8) | r3_;
    }
    u8 ReadNR34(bool /* force */ = false) const { return r4_; }
    void WriteNR34(u8 val, bool /* force */ = false);

    u8 ReadWAV(unsigned n, bool /* force */ = false) const { return wave_[n & (wave_.size() - 1)]; }
    void WriteWAV(unsigned n, u8 val, bool /* force */ = false) { wave_[n & (wave_.size() - 1)] = val; }

    u8 Vout() const { return vout_; }
};

static inline Serializer& operator<<(Serializer &s, const Channel3 &ch) {
    s.Start(Channel3::code_);

    return s << ch.r0_ << ch.r1_ << ch.r2_ << ch.r3_ << ch.r4_
             << ch.ctr_ << ch.ndx_
             << ch.vout_
             << ch.wave_;
}

static inline Deserializer& operator>>(Deserializer &d, Channel3 &ch) {
    d.Start(Channel3::code_);

    return d >> ch.r0_ >> ch.r1_ >> ch.r2_ >> ch.r3_ >> ch.r4_
             >> ch.ctr_ >> ch.ndx_
             >> ch.vout_
             >> ch.wave_;
}

}   // namespace gb1
