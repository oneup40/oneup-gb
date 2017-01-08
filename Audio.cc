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
      ctr_(0), step_(0x8000),
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
    if (!step_) { step_ = 0x8000; }

    unsigned mask = 0;
    switch (r1_ & 0xC0) {
        /*case 0x00:  mask = 0x01; break;
        case 0x40:  mask = 0x81; break;
        case 0x80:  mask = 0x87; break;
        case 0xC0:  mask = 0x7E; break;*/
		case 0x00: mask = 0x0003; break;
		case 0x40: mask = 0xC003; break;
		case 0x80: mask = 0xC03F; break;
		case 0xC0: mask = 0x3FFC; break;
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
		/*case 0x00:  mask = 0x01; break;
		case 0x40:  mask = 0x81; break;
		case 0x80:  mask = 0x87; break;
		case 0xC0:  mask = 0x7E; break;*/
		case 0x00: mask = 0x0003; break;
		case 0x40: mask = 0xC003; break;
		case 0x80: mask = 0xC03F; break;
		case 0xC0: mask = 0x3FFC; break;
	}

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

Channel3::Channel3(Audio &audio)
	: audio(audio),
	  r0_(0x00), r1_(0x00), r2_(0x00), r3_(0x00), r4_(0x00),
	  ctr_(0), ndx_(0),
	  vout_(0x00)
{
	wave_.fill(0);
}


void Channel3::TickLength() {
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

	u8 sample = wave_[ndx_ >> 1];
	if (ndx_ & 0x01)	{ sample &= 0x0F; }
	else				{ sample >>= 4; }

	++ndx_;
	ndx_ &= 0x1F;

	if (!(r2_ & 0x60))	{ vout_ = 0; }
	else				{ vout_ = sample >> ((r2_ & 0x60) >> 5); }
}

void Channel3::Write(unsigned n, u8 val, bool) {
	switch (n) {
	case 0:
		//if ((r0_ & 0x80) != (val & 0x80)) {
		//	if (val & 0x80) { std::cerr << "ch3 start playback" << std::endl; }
		//	else { std::cerr << "ch3 stop playback" << std::endl; }
		//}
		r0_ = val;
		break;
	case 1:
		//std::cerr << "ch3 length counter = " << unsigned(val) << std::endl;
		r1_ = val;
		break;
	case 2:
		r2_ = val;
		break;
	case 3:
		r3_ = val;
		ctr_ = ((r4_ & 0x07) << 8) | r3_;
		//std::cerr << "t = " << audio.m_->t / 4194304.;
		//std::cerr << "    ch3 freq " << 65536 / (2048. - ctr_) << " Hz" << std::endl;
		break;
	case 4:
		//if ((val & 0x60) != (r4_ & 0x60)) {
			//if (val & 0x60) { std::cerr << "ch3 length counter enable" << std::endl; }
			//else { std::cerr << "ch3 length counter disable" << std::endl; }
		//}
		if (val & 0x80) { audio.nr52_ |= 0x04; }
		r4_ = val;
		ctr_ = ((r4_ & 0x07) << 8) | r3_;
		//std::cerr << "t = " << audio.m_->t / 4194304.;
		//std::cerr << "    ch3 freq " << 65536 / (2048. - ctr_) << " Hz" << std::endl;
		break;
	default:
		assert(0);
		break;
	}
}

void Channel3::WriteWave(unsigned n, u8 val) { 
	wave_[n] = val;
	//std::cerr << "ch3 WriteWave: ";
	//for (auto x : wave_) { std::cerr << to_hex(x, 2); }
	//std::cerr << std::endl;
}

void Audio::TickLength() {
    if (nr52_ & 0x01) { ch1_.TickLength(); }
    if (nr52_ & 0x02) { ch2_.TickLength(); }
	if ((nr52_ & 0x04) && (ch3_.r0_ & 0x80)) { ch3_.TickLength(); }
}

void Audio::TickSweep() {
    if (nr52_ & 0x01) { ch1_.TickSweep(); }
}

void Audio::TickVolume() {
    if (nr52_ & 0x01) { ch1_.TickVolume(); }
    if (nr52_ & 0x02) { ch2_.TickVolume(); }
}

void Audio::TickOutput() {
	if (nr52_ & 0x01) { ch1_.TickOutput(); }
	if (nr52_ & 0x02) { ch2_.TickOutput(); }
	if (nr52_ & 0x04) { ch3_.TickOutput(); }
}

void Audio::GenerateSample() {
	double so1 = 0.0, so2 = 0.0;

	if (nr52_ & 0x80) {
		unsigned so1_n = 0, so2_n = 0;
		// (0, 0)

		if (nr52_ & 0x01) {
			if (nr51_ & 0x10) { so1 += ch1_.vout_ / 15.0; ++so1_n; }
			if (nr51_ & 0x01) { so2 += ch1_.vout_ / 15.0; ++so2_n; }
		}

		if (nr52_ & 0x02) {
			if (nr51_ & 0x20) { so1 += ch2_.vout_ / 15.0; ++so1_n; }
			if (nr51_ & 0x02) { so2 += ch2_.vout_ / 15.0; ++so2_n; }
		}

		if ((nr52_ & 0x04) && (ch3_.r0_ & 0x80)) {
			if (nr51_ & 0x40) { so1 += ch3_.vout_ / 15.0; ++so1_n; }
			if (nr51_ & 0x04) { so2 += ch3_.vout_ / 15.0; ++so2_n; }
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
	}

	i16 left = i16(so1 * std::numeric_limits<i16>::max()),
		right = i16(so2 * std::numeric_limits<i16>::max());
	// (-0x7FFF, 0x7FFF)

	//if (rand() < 10) {
	//	std::cerr << to_hex(left, 4) << " | " << to_hex(right, 4) << std::endl;
	//}

	m_->frontend->QueueSample(left, right);
}

Audio::Audio(Machine *m)
    : m_(m),
      ch1_(*this), ch2_(*this), ch3_(*this),
      nr50_(0x00), nr51_(0x00), nr52_(0x7F),
      sample_div_(0), timer_div_(0), seq_step_(7)
{}

bool Audio::Tick() {
	if (nr52_ & 0x80) {
		TickOutput();

		++timer_div_;

		// but the internal timer is only 512 Hz
		// 2097152 Hz / 4096 = 512 Hz
		if (timer_div_ == 4096) {
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
		}
	}

	GenerateSample();

    return true;
}

void Audio::Write(u16 addr, u8 val, bool force) {
    if (0xFF10 <= addr && addr <= 0xFF14)       { ch1_.Write(addr - 0xFF10, val, force); }
    else if (0xFF15 <= addr && addr <= 0xFF19)  { ch2_.Write(addr - 0xFF15, val, force); }
	else if (0xFF1A <= addr && addr <= 0xFF1E)	{ ch3_.Write(addr - 0xFF1A, val, force); }
	else if (0xFF30 <= addr && addr <= 0xFF3F)	{ ch3_.WriteWave(addr & 0xF, val); }
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
