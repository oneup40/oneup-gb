// Copyright 2016-2017 oneup

#include "core/audio/Audio.hpp"

#include "core/Frontend.hpp"
#include "core/Machine.hpp"

namespace gb1 {

void Audio::TickLength() {
    if (nr52_ & 0x01) { ch1_.TickLength(); }
    if (nr52_ & 0x02) { ch2_.TickLength(); }
	if (nr52_ & 0x04) { ch3_.TickLength(); }
	if (nr52_ & 0x08) { ch4_.TickLength(); }
}

void Audio::TickSweep() {
    if (nr52_ & 0x01) { ch1_.TickSweep(); }
}

void Audio::TickVolume() {
    if (nr52_ & 0x01) { ch1_.TickVolume(); }
    if (nr52_ & 0x02) { ch2_.TickVolume(); }
	if (nr52_ & 0x08) { ch4_.TickVolume(); }
}

void Audio::TickOutput() {
	if (nr52_ & 0x01) { ch1_.TickOutput(); }
	if (nr52_ & 0x02) { ch2_.TickOutput(); }
	if (nr52_ & 0x04) { ch3_.TickOutput(); }
	if (nr52_ & 0x08) { ch4_.TickOutput(); }
}

void Audio::GenerateSample() {
	double so1 = 0.0, so2 = 0.0;

	if (nr52_ & 0x80) {
		unsigned so1_n = 0, so2_n = 0;
		// (0, 0)

		if (nr52_ & 0x01) {
			if (nr51_ & 0x10) { so1 += ch1_.Vout() / 15.0; ++so1_n; }
			if (nr51_ & 0x01) { so2 += ch1_.Vout() / 15.0; ++so2_n; }
		}

		if (nr52_ & 0x02) {
			if (nr51_ & 0x20) { so1 += ch2_.Vout() / 15.0; ++so1_n; }
			if (nr51_ & 0x02) { so2 += ch2_.Vout() / 15.0; ++so2_n; }
		}

		if (nr52_ & 0x04) {
			if (nr51_ & 0x40) { so1 += ch3_.Vout() / 15.0; ++so1_n; }
			if (nr51_ & 0x04) { so2 += ch3_.Vout() / 15.0; ++so2_n; }
		}

		if (nr52_ & 0x08) {
			if (nr51_ & 0x80) { so1 += ch4_.Vout() / 15.0; ++so1_n; }
			if (nr51_ & 0x08) { so2 += ch4_.Vout() / 15.0; ++so2_n; }
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

	m_->frontend.OutputAudioFrame(left, right);
}

Audio::Audio(Machine *m)
    : m_(m),
      ch1_(*this), ch2_(*this), ch3_(*this), ch4_(*this),
      nr50_(0x00), nr51_(0x00), nr52_(0x7F),
      timer_div_(0), seq_step_(7)
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

} // namespace gb1
