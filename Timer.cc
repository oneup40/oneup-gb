// Copyright 2016 oneup

#include "Timer.hpp"

#include "Machine.hpp"

namespace gblr {

Timer::Timer(Machine *m)
	: m_(m),
	  tima_(0), tma_(0), tac_(0),
	  fulldiv_(0)
{}

u8 Timer::ReadDiv(bool) {
	return fulldiv_ >> 8;
}

void Timer::WriteDiv(u8 val, bool force) {
	if (force) 	{ fulldiv_ = val << 8; }
	else 		{ fulldiv_ = 0; }
}

bool Timer::Tick() {
	u32 prev = fulldiv_,
		next = fulldiv_ + 1;

	++fulldiv_;

	if (tac_ & 0x04) {
		u32 bit = 0;

		switch (tac_ & 0x03) {
			case 0:	bit = 0x200; break;
			case 1: bit = 0x008; break;
			case 2: bit = 0x020; break;
			case 3: bit = 0x080; break;
		}

		if ((next & bit) && !(prev & bit)) {	// falling edge
			++tima_;

			if (!tima_) {
				tima_ = tma_;
				m_->Interrupt(0x04);
			}
		}
	}

	return true;
}

}	// namespace gblr
