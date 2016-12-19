#pragma once

// Copyright 2016 oneup

#include "Base.hpp"
#include "Serializer.hpp"

#include <vector>

namespace gblr {

enum MapperNumber : u8 {
	kMapperNone = 0,
	kMapperMBC1 = 1,
	kMapperMBC2 = 2,
	//kMapperMBC3 = 3,
	//kMapperMBC5 = 5,
};

static inline Serializer& operator<<(Serializer &s, MapperNumber n) {
	return s << static_cast<u64>(n);
}

static inline Deserializer& operator>>(Deserializer &d, MapperNumber &n) {
	u64 x;
	d >> x;
	if (!d) { return d; }

	n = static_cast<MapperNumber>(x);
	return d;
}

static constexpr const size_t kBankShift = 14;
static constexpr const size_t kBankSize = 1ul << kBankShift;
static_assert(kBankSize == 16 * 1024, "kBankSize != 16KiB");

struct Machine;

class Mapper;

static inline Serializer& operator<<(Serializer &s, const Mapper &m);
static inline Deserializer& operator>>(Deserializer &d, Mapper &m);

class Mapper {
	Machine *m_;

	bool ram_enable_;
	MapperNumber number_;
	size_t bank_;

    size_t RomIndex(u16 addr);
    size_t RamIndex(u16 addr);

    static constexpr const u8 version_ = 0x01;
    static constexpr const u64 code_ = eight_cc(version_, 'm','a','p','p','e','r');
    friend Serializer& operator<<(Serializer &s, const Mapper &m);
    friend Deserializer& operator>>(Deserializer &d, Mapper &m);
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

static inline Serializer& operator<<(Serializer &s, const Mapper &m) {
	std::basic_string<u8> ram(m.ram.begin(), m.ram.end());

	s.Start(Mapper::code_);
	return s << m.ram_enable_ << m.number_ << m.bank_ << ram;
}

static inline Deserializer& operator>>(Deserializer &d, Mapper &m) {
	std::basic_string<u8> ram;

	d.Start(Mapper::code_);
	d >> m.ram_enable_ >> m.number_ >> m.bank_ >> ram;
	if (!d) { return d; }

	std::copy(ram.begin(), ram.end(), m.ram.begin());

	return d;
}

}	// namespace gblr
