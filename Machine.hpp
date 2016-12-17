#pragma once

// Copyright 2016 oneup

#include <functional>

#include "libretro.h"

#include "CPU.hpp"
#include "Joypad.hpp"
#include "LCD.hpp"
#include "Loader.hpp"
#include "Mapper.hpp"

namespace gblr {

enum Button : u8 {
	BTN_RIGHT	= 0x01,
	BTN_LEFT	= 0x02,
	BTN_UP 		= 0x04,
	BTN_DOWN 	= 0x08,
	BTN_A		= 0x10,
	BTN_B		= 0x20,
	BTN_SELECT	= 0x40,
	BTN_START	= 0x80
};
using ButtonPoller = std::function<Button()>;

struct Machine {
	CPU cpu;
	LCD lcd;
	Mapper mapper;
	Joypad joypad;
	std::array<u8, 0x2000> wram;
	std::array<u8, 0x80> hram;

	bool frame_ready;

	ButtonPoller btn_poller;

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
