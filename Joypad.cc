// Copyright 2016 oneup

#include "Joypad.hpp"
#include "Machine.hpp"

namespace gblr {

Joypad::Joypad(Machine *m) : m_(m), joyp_(0) {}

u8 Joypad::ReadJoyp(bool) {
	// This is all active low logic!
	joyp_ |= 0x0F;

	Button btn = m_->btn_poller();

	if (!(joyp_ & 0x10)) {
		if (btn & BTN_RIGHT) 	{ joyp_ &= ~0x01; }
		if (btn & BTN_LEFT) 	{ joyp_ &= ~0x02; }
		if (btn & BTN_UP)	 	{ joyp_ &= ~0x04; }
		if (btn & BTN_DOWN) 	{ joyp_ &= ~0x08; }
	}

	if (!(joyp_ & 0x20)) {
		if (btn & BTN_A) 		{ joyp_ &= ~0x01; }
		if (btn & BTN_B) 		{ joyp_ &= ~0x02; }
		if (btn & BTN_SELECT)	{ joyp_ &= ~0x04; }
		if (btn & BTN_START) 	{ joyp_ &= ~0x08; }
	}

	return joyp_;
}

void Joypad::WriteJoyp(u8 val, bool force) {
	if (force) 	{ joyp_ = val; }
	else		{ joyp_ = (joyp_ & ~0x30) | (val & 0x30); }
}

}