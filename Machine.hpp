#pragma once

// Copyright 2016 oneup

#include "libretro.h"

#include "CPU.hpp"
#include "Loader.hpp"
#include "Mapper.hpp"

namespace gblr {

struct Machine {
	CPU cpu;
	Mapper mapper;

	Machine();
	Machine(const Machine&) = delete;
	Machine(Machine&&) = delete;
	Machine& operator=(const Machine&) = delete;
	Machine& operator=(Machine&&) = delete;

	bool Tick();
	u8 Read(u16 addr, bool force = false);
	void Write(u16 addr, u8 val, bool force = false);
	void Interrupt(u8 num);
	void Log(const std::string &msg);

	bool LoadGame(const retro_game_info *game);
	void UnloadGame();
};

} // namespace gblr
