// Copyright 2017 oneup

#include "core/audio/Channel3.hpp"

#include "core/audio/Audio.hpp"

namespace gb1 {

Channel4::Channel4(Audio &audio)
    : audio(audio),
    r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
    vol_(0), vol_div_(0),
    lsfr_(0x7FFF), lsfr_div_(1),
    vout_(0x00)
{}


void Channel4::TickVolume() {
    if (!(r2_ & 0x7)) { return; }

    --vol_div_;
    if (vol_div_ == 0) {
        vol_div_ = r2_ & 0x7;

        if (vol_ < 0xF && (r2_ & 0x08)) { ++vol_; }
        else if (vol_ > 0 && !(r2_ & 0x08)) { --vol_; }
    }
}

void Channel4::TickLength() {
    if (r4_ & 0x40) {
        r1_ = (r1_ & ~0x3F) | ((r1_ + 1) & 0x3F);

        if (!(r1_ & 0x3F)) {
            // disable channel
            audio.nr52_ &= ~0x08;
        }
    }
}

void Channel4::TickOutput() {
    if ((r3_ >> 4) >= 14) { return; }

    --lsfr_div_;
    if (lsfr_div_) { return; }

    lsfr_div_ = (r3_ & 0x07) << 3;
    if (!lsfr_div_) { lsfr_div_ = 2; }
    // (0x02, 0x38)

    lsfr_div_ <<= (r3_ >> 4);
    // (0x02, 0x70000)

    auto prev_lsfr = lsfr_;
    u8 bit = ((lsfr_ >> 1) ^ lsfr_) & 0x1;
    lsfr_ >>= 1;

    if (r3_ & 0x08) {
        lsfr_ &= ~0x40;
        lsfr_ |= (bit << 6);
    }

    lsfr_ &= ~0x4000;
    lsfr_ |= (bit << 14);

    static bool warned = false;
    if (!lsfr_ && !warned) {
        std::cerr << "!! LSFR is 0" << std::endl;
        std::cerr << "prev_lsfr: $" << to_hex(prev_lsfr, 4) << std::endl;
        std::cerr << ((r3_ & 0x08) ? "7-bit mode" : "15-bit mode") << std::endl;
        warned = true;
    }

    // output is inverted
    vout_ = (lsfr_ & 0x01) ? 0 : vol_;
}

void Channel4::WriteNR44(u8 val, bool /* force */) {
    if (val & 0x80) {
        audio.nr52_ |= 0x08;
        lsfr_ = 0x7FFF;
    }
    r4_ = val;
}

}   // namespace gb1
