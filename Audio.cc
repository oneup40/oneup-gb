// Copyright 2016 oneup

#include "Audio.hpp"

#include "LRConnector.hpp"
#include "Machine.hpp"

namespace gblr {

Channel1::Channel1(Audio &audio)
    : audio(audio),
      r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
	  sweep_div_(0),
      vol_(0), vol_div_(0),
      ctr_(0), step_(0x80),
      vout_(0x00)
{}

void Channel1::TickSweep() {
	if (!(r0_ & 0x70)) { return; }

	--sweep_div_;
	if (sweep_div_ == 0) {
		sweep_div_ = (r0_ & 0x70) >> 4;

		u16 new_ctr = ((r4_ & 0x07) << 8) | r3_;
		u16 incr = new_ctr >> (r0_ & 0x07);

		if (r0_ & 0x08) { new_ctr -= incr; }
		else            { new_ctr += incr; }

		if (new_ctr < 2048) {
			r3_ = new_ctr & 0xFF;
			r4_ &= ~0x07;
			r4_ |= (new_ctr >> 8) & 0x07;
		} else {
			// disable channel
			audio.nr52_ &= ~0x01;
		}
	}
}

void Channel1::TickVolume() {
	if (!(r2_ & 0x7)) { return;  }

	--vol_div_;
	if (vol_div_ == 0) {
		vol_div_ = r2_ & 0x7;

		if (vol_ < 0xF && (r2_ & 0x08))     { ++vol_; }
		else if (vol_ > 0 && !(r2_ & 0x08)) { --vol_; }
	}
}

void Channel1::TickLength() {
	if (r4_ & 0x40) {
		r1_ = (r1_ & ~0x3F) | ((r1_ + 1) & 0x3F);

		if (!(r1_ & 0x3F)) {
			// disable channel
			audio.nr52_ &= ~0x01;
		}
	}
}

void Channel1::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }

    ctr_ = ((r4_ & 0x07) << 8) | r3_;

    step_ >>= 1;
    if (!step_) { step_ = 0x80; }

    u8 mask = 0;
    switch (r1_ & 0xC0) {
        case 0x00:  mask = 0x01; break;
        case 0x40:  mask = 0x81; break;
        case 0x80:  mask = 0x87; break;
        case 0xC0:  mask = 0x7E; break;
    }

	vout_ = (step_ & mask) ? vol_ : 0;
}

void Channel1::Write(unsigned n, u8 val, bool) {
    switch (n) {
        case 0:
            r0_ = val;
            break;
        case 1:
            r1_ = val;
            break;
        case 2:
            r2_ = val;
			vol_ = r2_ >> 4;
			vol_div_ = r2_ & 0x07;
            break;
        case 3:
            r3_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        case 4:
			if (val & 0x80) { audio.nr52_ |= 0x01; }
            r4_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        default:
            assert(0);
            break;
    }
}

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
			std::cerr << "expired, disabling ch2" << std::endl;
			audio.nr52_ &= ~0x02;
		}
	}
}

void Channel2::TickOutput() {
    ++ctr_;
    if (ctr_ != 2048) { return; }

    ctr_ = ((r4_ & 0x07) << 8) | r3_;
    
    step_ >>= 1;
    if (!step_) { step_ = 0x80; }

    u8 mask = 0;
    switch (r1_ & 0xC0) {
        case 0x00:  mask = 0x01; break;
        case 0x40:  mask = 0x81; break;
        case 0x80:  mask = 0x87; break;
        case 0xC0:  mask = 0x7E; break;
    }

	//mask = 0x87;

	vout_ = (step_ & mask) ? vol_ : 0;
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
			vol_ = r2_ >> 4;
			vol_div_ = r2_ & 0x07;
            break;
        case 3:
            r3_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        case 4:
			if (val & 0x80) { audio.nr52_ |= 0x02; }
            r4_ = val;
            ctr_ = ((r4_ & 0x07) << 8) | r3_;
            break;
        default:
            assert(0);
            break;
    }
}

void Audio::TickLength() {
    if (nr52_ & 0x01) { ch1_.TickLength(); }
    if (nr52_ & 0x02) { ch2_.TickLength(); }
}

void Audio::TickSweep() {
    if (nr52_ & 0x01) { ch1_.TickSweep(); }
}

void Audio::TickVolume() {
    if (nr52_ & 0x01) { ch1_.TickVolume(); }
    if (nr52_ & 0x02) { ch2_.TickVolume(); }
}

void Audio::GenerateSample() {
	double so1 = 0.0, so2 = 0.0;
	unsigned so1_n = 0, so2_n = 0;
	// (0, 0)

    if (nr52_ & 0x01) {
		ch1_.TickOutput();

		if (nr51_ & 0x10) { so1 += ch1_.vout_ / 15.0; ++so1_n; }
		if (nr51_ & 0x01) { so2 += ch1_.vout_ / 15.0; ++so2_n; }
    }

    if (nr52_ & 0x02) {
		ch2_.TickOutput();

		if (nr51_ & 0x20) { so1 += ch2_.vout_ / 15.0; ++so1_n; }
		if (nr51_ & 0x02) { so2 += ch2_.vout_ / 15.0; ++so2_n; }
    }
	// (0, sox_n)

	if (so1_n) { so1 /= so1_n; }
	if (so2_n) { so2 /= so2_n; }
	// (0, 1)

    so1 *= (nr50_ & 0x07) / 7.0;
    so2 *= ((nr50_ & 0x70) >> 4) / 7.0;
	// (0, 1)

	so1 = (so1 - 0.5) * 2;
	so2 = (so2 - 0.5) * 2;
	// (-1, 1)

	i16 left = i16(so1 * std::numeric_limits<i16>::max()),
		right = i16(so2 * std::numeric_limits<i16>::max());
	// (-0x7FFF, 0x7FFF)

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
    // 1048576 Hz / 2048 = 512 Hz
    if (timer_div_ < 2048) { return true; }
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
    if (0xFF10 <= addr && addr <= 0xFF14)       { ch1_.Write(addr - 0xFF10, val, force); }
    else if (0xFF15 <= addr && addr <= 0xFF19)  { ch2_.Write(addr - 0xFF15, val, force); }
    else {
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
