// Copyright 2017 oneup

#include "core/audio/Channel3.hpp"

#include "core/audio/Audio.hpp"

namespace gb1 {

Channel3::Channel3(Audio &audio)
    : audio(audio),
      r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
      ctr_(0), ndx_(0),
      vout_(0x00)
{
    wave_.fill(0);
}


void Channel3::TickLength() {
    if (!(r0_ & 0x80)) { return; }

    if (r4_ & 0x40) {
        ++r1_;

        if (!r1_) {
            // disable channel
            audio.nr52_ &= ~0x04;
        }
    }
}

void Channel3::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }

    ctr_ = ((r4_ & 0x07) << 8) | r3_;

    if (!(r0_ & 0x80)) { vout_ = 0; return; }

    u8 sample = wave_[ndx_ >> 1];
    if (ndx_ & 0x01)    { sample &= 0x0F; }
    else                { sample >>= 4; }

    ++ndx_;
    ndx_ &= 0x1F;

    if (!(r2_ & 0x60))  { vout_ = 0; }
    else                { vout_ = sample >> ((r2_ & 0x60) >> 5); }
}

void Channel3::WriteNR34(u8 val, bool /* force */) {
    if (val & 0x80) { audio.nr52_ |= 0x04; }
    r4_ = val;
    ctr_ = ((r4_ & 0x07) << 8) | r3_;
}

}   // namespace gb1
