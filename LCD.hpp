#pragma once

// Copyright 2016 oneup

#include "Base.hpp"

namespace gblr {

struct Machine;

class LCD {
	Machine *m_;

	u8 lcdc_, stat_, scy_, scx_, ly_, lyc_, dma_, bgp_, obp0_, obp1_, wy_, wx_;
	unsigned dot_;

	std::array<u8, 0x2000> vram_;
	// note: oam is actually 0xA0 bytes but masks etc. are easier with 0x100
	std::array<u8, 0x100> oam_;

	std::array<std::array<u32, 160>, 144> frame_;

	u8 dma_ticks_;

	friend class Machine;

	u8 RenderWindowDot();
	u8 RenderBackgroundDot();
	u8 RenderSpriteDot(bool bgwnd);
	void RenderDot();
public:
	LCD(Machine *m);
	LCD(const LCD&) = delete;
	LCD(LCD&&) = delete;
	LCD& operator=(const LCD&) = delete;
	LCD& operator=(LCD&&) = delete;

	u8 ReadVRAM(u16 addr, bool force = false);
	void WriteVRAM(u16 addr, u8 val, bool force = false);

	u8 ReadOAM(u16 addr, bool force = false);
	void WriteOAM(u16 addr, u8 val, bool force = false);

	u8 ReadDMA(bool force = false);
	void WriteDMA(u8 val, bool force = false);

	bool Tick();
};

}
