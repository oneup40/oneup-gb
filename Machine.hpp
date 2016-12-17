#pragma once

// Copyright 2016 oneup

#include "libretro.h"

#include "CPU.hpp"
#include "LCD.hpp"
#include "Loader.hpp"
#include "Mapper.hpp"

namespace gblr {

struct Machine {
	CPU cpu;
	LCD lcd;
	Mapper mapper;
	std::array<u8, 0x2000> wram;
	std::array<u8, 0x80> hram;

	bool frame_ready;

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

	bool FrameReady() const;
	const u32* GetFrame() const;
	void ResetFrame();
};

} // namespace gblr
