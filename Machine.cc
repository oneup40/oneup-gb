// Copyright 2016 oneup

#include "Machine.hpp"

#include <iostream>

#include <cassert>

namespace gblr {

Machine::Machine() : cpu(this), mapper(this) {}

bool Machine::Tick() {
	bool good = true;
	good = good && cpu.Tick();
	return good;
}

u8 Machine::Read(u16 addr, bool force) {
	if (addr < 0x8000) {
		return mapper.Read(addr, force);
	} else if (addr == 0xFF0F) {
		return cpu.if_;
	} else if (addr == 0xFFFF) {
		return cpu.ie_;
	} else {
		assert(0);
		return 0;
	}
}

void Machine::Write(u16 addr, u8 val, bool force) {
	if (addr < 0x8000) {
		mapper.Write(addr, val, force);
	} else if (addr == 0xFF0F) {
		cpu.if_ = val & 0x1F;
	} else if (addr == 0xFFFF) {
		cpu.ie_ = val & 0x1F;
	} else {
		assert(0);
	}
}

void Machine::Interrupt(u8 num) { cpu.Interrupt(num); }

void Machine::Log(const std::string &msg) { std::cerr << msg << std::endl; }

bool Machine::LoadGame(const retro_game_info *game) { return Loader::Load(game, this); }

void Machine::UnloadGame() { mapper.Unload(); }

}	// namespace gblr


