#pragma once

// Copyright 2016 oneup

#include "Base.hpp"

namespace gblr {

struct Machine;

class Timer {
	Machine *m_;
	friend class Machine;

	u8 tima_, tma_, tac_;

	u32 fulldiv_;
public:
	Timer(Machine *m);
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	u8 ReadDiv(bool force = false);
	void WriteDiv(u8 val, bool force = false);

	bool Tick();
};

}	// namespace gblr
