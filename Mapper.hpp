#pragma once

// Copyright 2016 oneup

#include "Base.hpp"

#include <vector>

namespace gblr {

enum MapperNumber : u8 {
	kMapperMin = 0,

	kMapperNone = 0,
	kMapperMBC1 = 1,

	kMapperMax = 1,

	kMapperInvalid = 0xff
};

// silence warning about comparing unsigned value to >= 0
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
constexpr static inline bool IsValidMapperNumber(u8 number) {
	return number >= kMapperMin && number <= kMapperMax;
}
#pragma GCC diagnostic pop

static constexpr const size_t kBankShift = 14;
static constexpr const size_t kBankSize = 1ul << kBankShift;
static_assert(kBankSize == 16 * 1024, "kBankSize != 16KiB");

struct Machine;

class Mapper {
	Machine *m_;

	std::vector<u8> data_;
	MapperNumber number_;
	size_t bank_;

    size_t Index(u16 addr);
public:
	Mapper(Machine *m) : m_(m), data_(), number_(kMapperInvalid), bank_(1) {}
	Mapper(const Mapper&) = delete;
	Mapper(Mapper&&) = delete;
	Mapper& operator=(const Mapper&) = delete;
	Mapper& operator=(Mapper&&) = delete;

	void Init(MapperNumber number, const u8 *data, size_t size);
	void Unload();

	u8 Read(u16 addr, bool debug);
	void Write(u16 addr, u8 val, bool debug);
};

}
