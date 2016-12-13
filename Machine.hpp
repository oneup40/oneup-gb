#pragma once

// Copyright 2016 oneup

#include "CPU.hpp"

namespace gblr {

class Machine {
	CPU cpu_;
public:
	Machine();
	Machine(const Machine&) = delete;
	Machine(Machine&&) = delete;
	Machine& operator=(const Machine&) = delete;
	Machine& operator=(Machine&&) = delete;

	bool Tick();
	void Read(u16 addr, size_t len, u8 *dst, bool force = false);
	void Write(u16 addr, size_t len, const u8 *src, bool force = false);
	void Interrupt(u8 num);
	void Log(const std::string &msg);

	const CPU& cpu() { return cpu_; }
};

} // namespace gblr
