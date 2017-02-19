#pragma once

// Copyright 2016-2017 oneup

#include <array>

#include "core/audio/Channel1.hpp"
#include "core/audio/Channel2.hpp"
#include "core/audio/Channel3.hpp"
#include "core/audio/Channel4.hpp"
#include "core/Base.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

struct Machine;

static inline Serializer& operator<<(Serializer &s, const Audio &audio);
static inline Deserializer& operator>>(Deserializer &s, Audio &audio);

class Audio {
    Machine *m_;

    Channel1 ch1_;
    friend class Channel1;

    Channel2 ch2_;
    friend class Channel2;

	Channel3 ch3_;
	friend class Channel3;

	Channel4 ch4_;
	friend class Channel4;

    u8 nr50_, nr51_, nr52_;
    unsigned timer_div_, seq_step_;

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_, 'a','u','d','i','o');
    friend Serializer& operator<<(Serializer &s, const Audio &audio);
    friend Deserializer& operator>>(Deserializer &d, Audio &audio);

    void TickLength();
    void TickSweep();
    void TickVolume();
	void TickOutput();

    void GenerateSample();
public:
    Audio(Machine *m);
    Audio(const Audio&) = delete;
    Audio(Audio&&) = delete;
    Audio& operator=(const Audio&) = delete;
    Audio& operator=(Audio&&) = delete;

    u8 ReadNR10(bool force = false) const { return ch1_.ReadNR10(force); }
    void WriteNR10(u8 val, bool force = false) { ch1_.WriteNR10(val, force); }
    u8 ReadNR11(bool force = false) const { return ch1_.ReadNR11(force); }
    void WriteNR11(u8 val, bool force = false) { ch1_.WriteNR11(val, force); }
    u8 ReadNR12(bool force = false) const { return ch1_.ReadNR12(force); }
    void WriteNR12(u8 val, bool force = false) { ch1_.WriteNR12(val, force); }
    u8 ReadNR13(bool force = false) const { return ch1_.ReadNR13(force); }
    void WriteNR13(u8 val, bool force = false) { ch1_.WriteNR13(val, force); }
    u8 ReadNR14(bool force = false) const { return ch1_.ReadNR14(force); }
    void WriteNR14(u8 val, bool force = false) { ch1_.WriteNR14(val, force); }

    u8 ReadNR20(bool force = false) const { return ch2_.ReadNR20(force); }
    void WriteNR20(u8 val, bool force = false) { ch2_.WriteNR20(val, force); }
    u8 ReadNR21(bool force = false) const { return ch2_.ReadNR21(force); }
    void WriteNR21(u8 val, bool force = false) { ch2_.WriteNR21(val, force); }
    u8 ReadNR22(bool force = false) const { return ch2_.ReadNR22(force); }
    void WriteNR22(u8 val, bool force = false) { ch2_.WriteNR22(val, force); }
    u8 ReadNR23(bool force = false) const { return ch2_.ReadNR23(force); }
    void WriteNR23(u8 val, bool force = false) { ch2_.WriteNR23(val, force); }
    u8 ReadNR24(bool force = false) const { return ch2_.ReadNR24(force); }
    void WriteNR24(u8 val, bool force = false) { ch2_.WriteNR24(val, force); }

    u8 ReadNR30(bool force = false) const { return ch3_.ReadNR30(force); }
    void WriteNR30(u8 val, bool force = false) { ch3_.WriteNR30(val, force); }
    u8 ReadNR31(bool force = false) const { return ch3_.ReadNR31(force); }
    void WriteNR31(u8 val, bool force = false) { ch3_.WriteNR31(val, force); }
    u8 ReadNR32(bool force = false) const { return ch3_.ReadNR32(force); }
    void WriteNR32(u8 val, bool force = false) { ch3_.WriteNR32(val, force); }
    u8 ReadNR33(bool force = false) const { return ch3_.ReadNR33(force); }
    void WriteNR33(u8 val, bool force = false) { ch3_.WriteNR33(val, force); }
    u8 ReadNR34(bool force = false) const { return ch3_.ReadNR34(force); }
    void WriteNR34(u8 val, bool force = false) { ch3_.WriteNR34(val, force); }

    u8 ReadNR40(bool force = false) const { return ch4_.ReadNR40(force); }
    void WriteNR40(u8 val, bool force = false) { ch4_.WriteNR40(val, force); }
    u8 ReadNR41(bool force = false) const { return ch4_.ReadNR41(force); }
    void WriteNR41(u8 val, bool force = false) { ch4_.WriteNR41(val, force); }
    u8 ReadNR42(bool force = false) const { return ch4_.ReadNR42(force); }
    void WriteNR42(u8 val, bool force = false) { ch4_.WriteNR42(val, force); }
    u8 ReadNR43(bool force = false) const { return ch4_.ReadNR43(force); }
    void WriteNR43(u8 val, bool force = false) { ch4_.WriteNR43(val, force); }
    u8 ReadNR44(bool force = false) const { return ch4_.ReadNR44(force); }
    void WriteNR44(u8 val, bool force = false) { ch4_.WriteNR44(val, force); }

    u8 ReadNR50(bool /* force */ = false) const { return nr50_; }
    void WriteNR50(u8 val, bool /* force */ = false) { nr50_ = val; }
    u8 ReadNR51(bool /* force */ = false) const { return nr51_; }
    void WriteNR51(u8 val, bool /* force */ = false) { nr51_ = val; }
    u8 ReadNR52(bool /* force */ = false) const { return nr52_; }
    void WriteNR52(u8 val, bool force = false) {
        u8 mask = force ? 0xFF : 0x80;
        nr52_ &= ~mask;
        nr52_ |= (val & mask);
    }

    u8 ReadWAV(u8 ndx, bool force = false) const { return ch3_.ReadWAV(ndx, force); }
    void WriteWAV(u8 ndx, u8 val, bool force = false) { ch3_.WriteWAV(ndx, val, force); }

    bool Tick();
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

}   // namespace gb1
