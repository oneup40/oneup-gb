#pragma once

// Copyright 2016 oneup

#include "Base.hpp"

#include <vector>

namespace gblr {

enum MapperNumber : u8 {
	kMapperNone = 0,
	kMapperMBC1 = 1,
	kMapperMBC2 = 2,
	//kMapperMBC3 = 3,
	//kMapperMBC5 = 5,
};

static constexpr const size_t kBankShift = 14;
static constexpr const size_t kBankSize = 1ul << kBankShift;
static_assert(kBankSize == 16 * 1024, "kBankSize != 16KiB");

struct Machine;

class Mapper {
	Machine *m_;

	bool ram_enable_;
	MapperNumber number_;
	size_t bank_;

    size_t RomIndex(u16 addr);
    size_t RamIndex(u16 addr);
public:
    std::vector<u8> rom, ram;

	Mapper(Machine *m) : m_(m), ram_enable_(false), number_(kMapperNone), bank_(1), rom(kBankSize * 2, 0), ram()  {}
	Mapper(const Mapper&) = delete;
	Mapper(Mapper&&) = delete;
	Mapper& operator=(const Mapper&) = delete;
	Mapper& operator=(Mapper&&) = delete;

	void Init(MapperNumber number, const u8 *data, size_t size);
	void Unload();

	u8 ReadROM(u16 addr, bool debug);
	void WriteROM(u16 addr, u8 val, bool debug);

	u8 ReadRAM(u16 addr, bool debug);
	void WriteRAM(u16 addr, u8 val, bool debug);
};

}
