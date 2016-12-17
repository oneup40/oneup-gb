#pragma once

// Copyright 2016 oneup

#include "Base.hpp"

namespace gblr {

struct Machine;

class Joypad {
	Machine *m_;

	u8 joyp_;
public:
	Joypad(Machine *m);
	Joypad(const Joypad&) = delete;
	Joypad(Joypad&&) = delete;
	Joypad& operator=(const Joypad&) = delete;
	Joypad& operator=(Joypad&&) = delete;

	u8 ReadJoyp(bool force);
	void WriteJoyp(u8 val, bool force);
};

}	// namespace gblr
