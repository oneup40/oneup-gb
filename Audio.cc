// Copyright 2016 oneup

#include "Audio.hpp"

#include "LRConnector.hpp"
#include "Machine.hpp"

namespace gblr {

Channel1::Channel1(Audio &audio)
    : audio(audio),
      r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
      r2_shadow_(0x00),
      ctr_(0), step_(0),
      vout_(0x00)
{}

void Channel1::TickSweep() {
    // TODO
}

void Channel1::TickVolume() {
    // TODO
}

void Channel1::TickLength() {
    /*std::cerr << "ch1: length tick" << std::endl;
    std::cerr << "\tr1_ before: " << to_hex(r1_, 2) << std::endl;
    r1_ = (r1_ & ~0x3F) | ((r1_ + 1) & 0x3F);
    std::cerr << "\tr1_ after: " << to_hex(r1_, 2) << std::endl;

    if (!(r1_ & 0x3F) && (r4_ & 0x40)) {
        // disable channel
        std::cerr << "\texpired, disabling ch1" << std::endl;
        audio.nr52_ &= ~0x01;
    }*/
}

void Channel1::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }
    //std::cerr << "ch1: output tick" << std::endl;
    //std::cerr << "\tctr_ " << to_hex(ctr_, 3) << std::endl;
    ctr_ = ((r4_ & 0x07) << 8) | r3_;
    //std::cerr << "\treload ctr_ " << to_hex(ctr_, 3) << std::endl;

    /*step_ >>= 1;
    if (!step_) { step_ = 0x80; }
    //std::cerr << "\tstep_ " << to_hex(step_, 2) << std::endl;

    u8 mask = 0;
    switch (r1_ & 0xC0) {
        case 0x00:  mask = 0x01; break;
        case 0x40:  mask = 0x81; break;
        case 0x80:  mask = 0x87; break;
        case 0xC0:  mask = 0x7E; break;
    }
    //std::cerr << "\tduty " << unsigned((mask & 0xC0) >> 6) << ", mask " << to_hex(mask, 2) << std::endl;
	*/

	vout_ = (~vout_) & 0x0F;
    //vout_ = (step_ & mask) ? 0x0F : 0x00;
    //std::cerr << "\tvout_ now " << to_hex(vout_, 2) << " t = " << audio.m_->t / 4194304. << std::endl;
}

void Channel1::Write(unsigned n, u8 val, bool) {
    //std::cerr << "t = " << audio.m_->t / 4194304. << std::endl;
    switch (n) {
        case 0:
            /*std::cerr << "NR10 write " << to_hex(val, 2) << " ( "
                      << "sweep freq " << 128. / ((val >> 4) & 0x07) << " Hz : "
                      << (val & 0x08 ? "-" : "+") << " : "
                      << "shift " << unsigned(val & 0x07) << " )" << std::endl;*/
            r0_ = val;
            break;
        case 1:
            /*std::cerr << "NR11 write " << to_hex(val, 2) << " ( "
                      << "duty " << unsigned(val >> 6) << " : "
                      << "length " << unsigned(val & 0x3F) << " )" << std::endl;*/
            r1_ = val;
            break;
        case 2:
            /*std::cerr << "NR12 write " << to_hex(val, 2) << " ( "
                      << "initial " << unsigned(val >> 4) << " : "
                      << (val & 0x08 ? "+" : "-") << " : "
                      << "vol freq " << 64. / (val & 0x07) << " )" << std::endl;*/
            r2_ = val;
            break;
        case 3:
            r3_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            /*std::cerr << "NR13 write " << to_hex(val, 2) << " ( "
                      << "freq " << 131072 / (2048. - ctr_) << " Hz )" << std::endl;*/
            break;
        case 4:
            r4_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            /*std::cerr << "NR14 write " << to_hex(val, 2) << " ( "
                      << (val & 0x80 ? "T" : ".")
                      << (val & 0x40 ? "L" : ".")
                      << " freq " << 131072 / (2048. - ctr_) << " Hz )" << std::endl;*/
            break;
        default:
            assert(0);
            break;
    }
    //std::cerr << std::endl;
}

Channel2::Channel2(Audio &audio)
    : audio(audio),
      r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
      r2_shadow_(0x00),
      ctr_(0), step_(0),
      vout_(0x00)
{}

void Channel2::TickVolume() {
    // TODO
}

void Channel2::TickLength() {
    /*std::cerr << "ch1: length tick" << std::endl;
    std::cerr << "\tr1_ before: " << to_hex(r1_, 2) << std::endl;
    r1_ = (r1_ & ~0x3F) | ((r1_ + 1) & 0x3F);
    std::cerr << "\tr1_ after: " << to_hex(r1_, 2) << std::endl;

    if (!(r1_ & 0x3F) && (r4_ & 0x40)) {
        // disable channel
        std::cerr << "\texpired, disabling ch1" << std::endl;
        audio.nr52_ &= ~0x01;
    }*/
}

void Channel2::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }
    //std::cerr << "ch1: output tick" << std::endl;
    //std::cerr << "\tctr_ " << to_hex(ctr_, 3) << std::endl;
    ctr_ = ((r4_ & 0x07) << 8) | r3_;
    //std::cerr << "\treload ctr_ " << to_hex(ctr_, 3) << std::endl;

    /*step_ >>= 1;
    if (!step_) { step_ = 0x80; }
    //std::cerr << "\tstep_ " << to_hex(step_, 2) << std::endl;

    u8 mask = 0;
    switch (r1_ & 0xC0) {
        case 0x00:  mask = 0x01; break;
        case 0x40:  mask = 0x81; break;
        case 0x80:  mask = 0x87; break;
        case 0xC0:  mask = 0x7E; break;
    }
    //std::cerr << "\tduty " << unsigned((mask & 0xC0) >> 6) << ", mask " << to_hex(mask, 2) << std::endl;
    */

    vout_ = (~vout_) & 0x0F;
    //vout_ = (step_ & mask) ? 0x0F : 0x00;
    //std::cerr << "\tvout_ now " << to_hex(vout_, 2) << " t = " << audio.m_->t / 4194304. << std::endl;
}

void Channel2::Write(unsigned n, u8 val, bool) {
    switch (n) {
        case 0:
            r0_ = val;
            break;
        case 1:
            r1_ = val;
            break;
        case 2:
            r2_ = val;
            break;
        case 3:
            r3_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        case 4:
            r4_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        default:
            assert(0);
            break;
    }
    //std::cerr << std::endl;
}

void Audio::TickLength() {
    if (nr52_ & 0x01) { ch1_.TickLength(); }
    if (nr52_ & 0x02) { ch1_.TickLength(); }
}

void Audio::TickSweep() {
    if (nr52_ & 0x01) { ch1_.TickSweep(); }
}

void Audio::TickVolume() {
    if (nr52_ & 0x01) { ch1_.TickVolume(); }
    if (nr52_ & 0x02) { ch1_.TickLength(); }
}

void Audio::GenerateSample() {
    std::vector<u8> so1v, so2v;

    if (nr52_ & 0x01) {
        ch1_.TickOutput();

        if (nr51_ & 0x10) { so1v.emplace_back(ch1_.vout_); }
        if (nr51_ & 0x01) { so2v.emplace_back(ch1_.vout_); }
    }

    if (nr52_ & 0x02) {
        ch1_.TickOutput();

        if (nr51_ & 0x10) { so1v.emplace_back(ch2_.vout_); }
        if (nr51_ & 0x01) { so2v.emplace_back(ch2_.vout_); }
    }

    u8 so1 = 0x00, so2 = 0x00;
    for (auto s : so1v) { so1 += s; }
    for (auto s : so2v) { so2 += s; }

    if (so1v.size() == 2) { so1 >>= 1; }
    if (so2v.size() == 2) { so2 >>= 1; }

    so1 |= (nr50_ & 0x07) << 4;
    so2 |= (nr50_ & 0x70);

    //std::cerr << "\t a) " << to_hex(so1, 4) << " : " << to_hex(so2, 4) << std::endl;

    // range: 0x00 - 0x7F
    i16 left  = (so1 << 8) | (so1 << 1),
        right = (so2 << 8) | (so2 << 1);

    //std::cerr << "\t b) " << to_hex(left, 4) << " : " << to_hex(right, 4) << std::endl;

    // range: 0x0000 - 0x7FFE
    left  -= 0x4000;
    right -= 0x4000;

    //std::cerr << "\t c) " << to_hex(left, 4) << " : " << to_hex(right, 4) << std::endl;

    // range: -0x4000 - 0x3FFE
    left  *= 2;
    right *= 2;

    //std::cerr << "\t d) " << to_hex(left, 4) << " : " << to_hex(right, 4) << std::endl;

    // range: -0x8000 - 0x7FFC
    m_->frontend->QueueSample(left, right);
}

Audio::Audio(Machine *m)
    : m_(m),
      ch1_(*this), ch2_(*this),
      nr50_(0x00), nr51_(0x00), nr52_(0x7F),
      timer_div_(0), seq_step_(7)
{}

bool Audio::Tick() {
    if (!(nr52_ & 0x80)) { 
		m_->frontend->QueueSample(0, 0);
		return true;
	}

    // f = 1048576 / (4 * 2 * (2048 - x)) Hz
    // 1048576 is the nominal main clock rate
    // The 4 * 2 is for the eight parts of the waveform
    // 2048 - x is a modulo counter (trigger on mod 2048 = 0 and reset to x)
    GenerateSample();

    ++timer_div_;

    // but the internal timer is only 512 Hz
    // 4194304 Hz / 8192 = 512 Hz
    if (timer_div_ < 8192) { return true; }
    timer_div_ = 0;

    ++seq_step_;
    seq_step_ &= 7;

    switch (seq_step_) {
        case 0x00:  TickLength(); break;
        case 0x02:  TickSweep(); TickLength(); break;
        case 0x04:  TickLength(); break;
        case 0x06:  TickSweep(); TickLength(); break;
        case 0x07:  TickVolume(); break;
        default:    break;
    }

    return true;
}

void Audio::Write(u16 addr, u8 val, bool force) {
    if (0xFF10 <= addr && addr <= 0xFF14)   { ch1_.Write(addr - 0xFF10, val, force); }
    else {
        //std::cerr << "Write $" << to_hex(addr, 4) << " $" << to_hex(val, 2) << std::endl;
        u8 dummy = 0;
        u8 *reg = &dummy;
        u8 mask = 0;

        if (addr == 0xFF24)                { reg = &nr50_; mask = 0xFF; }
        else if (addr == 0xFF25)           { reg = &nr51_; mask = 0xFF; }
        else if (addr == 0xFF26)           { reg = &nr52_; mask = 0x80; }

        if (force) { mask = 0xFF; }
        *reg &= ~mask;
        *reg |= val & mask;
    }
}

} // namespace gblr
