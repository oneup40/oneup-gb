// Copyright 2017 oneup

#include "core/audio/Channel2.hpp"

#include "core/audio/Audio.hpp"

namespace gb1 {

Channel2::Channel2(Audio &audio)
    : audio(audio),
      r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
      vol_(0), vol_div_(0),
      ctr_(0), step_(0x80),
      vout_(0x00)
{}

void Channel2::TickVolume() {
    if (!(r2_ & 0x7)) { return; }

    --vol_div_;
    if (vol_div_ == 0) {
        vol_div_ = r2_ & 0x7;

        if (vol_ < 0xF && (r2_ & 0x08)) { ++vol_; }
        else if (vol_ > 0 && !(r2_ & 0x08)) { --vol_; }
    }
}

void Channel2::TickLength() {
    if (r4_ & 0x40) {
        r1_ = (r1_ & ~0x3F) | ((r1_ + 1) & 0x3F);

        if (!(r1_ & 0x3F)) {
            // disable channel
            //std::cerr << "expired, disabling ch2" << std::endl;
            audio.nr52_ &= ~0x02;
        }
    }
}

void Channel2::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }

    ctr_ = ((r4_ & 0x07) << 8) | r3_;

    step_ >>= 1;
    if (!step_) { step_ = 0x8000; }

    unsigned mask = 0;
    switch (r1_ & 0xC0) {
        case 0x00: mask = 0x0003; break;
        case 0x40: mask = 0xC003; break;
        case 0x80: mask = 0xC03F; break;
        case 0xC0: mask = 0x3FFC; break;
    }

    vout_ = (step_ & mask) ? vol_ : 0;
}

void Channel2::WriteNR24(u8 val, bool /* force */) {
    if (val & 0x80) { audio.nr52_ |= 0x02; }
    r4_ = val;
    ctr_ = ((r4_ & 0x07) << 8) | r3_;
}

}   // namespace gb1
