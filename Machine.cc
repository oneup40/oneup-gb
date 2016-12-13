// Copyright 2016 oneup

#include "Machine.hpp"

#include <iostream>

#include <cassert>

namespace gblr {

Machine::Machine() : cpu_(this) {}

bool Machine::Tick() {
	bool good = true;
	good = good && cpu_.Tick();
	return good;
}

void Machine::Read(u16 addr, size_t len, u8 *dst, bool force) {
	(void) force;
	while (len) {
		if (addr >= 0xFF00) {
			if (addr == 0xFF0F) 		{ *dst = cpu_.if_; }
			else if (addr == 0xFFFF) 	{ *dst = cpu_.ie_; }
			else 						{ assert(0); }

			++dst;
			--len;
			++addr;
		} else {
			assert(0);
		}
	}
}

void Machine::Write(u16 addr, size_t len, const u8 *src, bool force) {
	(void) force;
	while (len) {
		if (addr >= 0xFF00) {
			if (addr == 0xFF0F) 		{ cpu_.if_ = *src & 0x1F; }
			else if (addr == 0xFFFF) 	{ cpu_.ie_ = *src & 0x1F; }
			else 						{ assert(0); }

			++src;
			--len;
			++addr;
		} else {
			assert(0);
		}
	}
}

void Machine::Interrupt(u8 num) { cpu_.Interrupt(num); }

void Machine::Log(const std::string &msg) { std::cerr << msg << std::endl; }

}	// namespace gblr


