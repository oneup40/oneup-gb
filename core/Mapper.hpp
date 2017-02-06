#pragma once

// Copyright 2016 oneup

#include "Base.hpp"
#include "Serializer.hpp"

#include <vector>

namespace gb1 {

enum MapperNumber : u8 {
    kMapperNone = 0,
    kMapperMBC1 = 1,
    kMapperMBC2 = 2,
    kMapperMBC3 = 3,
    kMapperMBC5 = 5,
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

static constexpr const size_t kRomBankShift = 14;
static constexpr const size_t kRomBankSize = 1ul << kRomBankShift;
static constexpr const size_t kRomOffsetMask = kRomBankSize - 1;

static_assert(kRomBankSize == 16 * 1024, "kRomBankSize != 16KiB");

static constexpr const size_t kRamBankShift = 13;
static constexpr const size_t kRamBankSize = 1ul << kRamBankShift;
static constexpr const size_t kRamOffsetMask = kRamBankSize - 1;

static_assert(kRamBankSize == 8 * 1024, "kRamBankSize != 8KiB");

struct Machine;

class Mapper;

static inline Serializer& operator<<(Serializer &s, const Mapper &m);
static inline Deserializer& operator>>(Deserializer &d, Mapper &m);

// Mapper registers:
//  None: none
//  MBC1:
//      1-bit RAM enable
//      5-bit ROM bank select
//      2-bit upper ROM/RAM bank select
//      1-bit upper ROM/RAM select
//  MBC2:
//      1-bit RAM enable
//      4-bit ROM bank select
//  MBC3:
//      1-bit RAM / timer enable
//      7-bit ROM bank select
//      4-bit RAM bank / RTC register select
//      1-bit RTC latch
//      5 RTC registers
//  MBC5:
//      1-bit RAM enable
//      9-bit ROM bank select
//      4-bit RAM bank select
//  These are all covered with:
//      1-bit RAM / timer enable
//      9-bit ROM bank select
//      4-bit upper ROM/RAM bank / RTC register select
//      1-bit upper ROM/RAM select
//      1-bit RTC latch
//      5 RTC registers
//  If we break the RTC into its own component, then we only need the first 3.

class Mapper {
    MapperNumber number_;
    bool ram_enable_, extra_rom_bits_;
    size_t rom_bank_, ram_bank_;

    size_t RomIndex(u16 addr);
    size_t RamIndex(u16 addr);

    void WriteMBC1Register(u16 addr, u8 data);
    void WriteMBC2Register(u16 addr, u8 data);
    void WriteMBC3Register(u16 addr, u8 data);
    void WriteMBC5Register(u16 addr, u8 data);

    static constexpr const u8 version_ = 0x02;
    static constexpr const u64 code_ = eight_cc(version_, 'm','a','p','p','e','r');
    friend Serializer& operator<<(Serializer &s, const Mapper &m);
    friend Deserializer& operator>>(Deserializer &d, Mapper &m);
public:
    std::vector<u8> rom, ram;

    Mapper();
    Mapper(const Mapper&) = delete;
    Mapper(Mapper&&) = delete;
    Mapper& operator=(const Mapper&) = delete;
    Mapper& operator=(Mapper&&) = delete;

    void Init(MapperNumber number, std::vector<u8>&& rom, size_t ram_size);
    void Unload();

    u8 ReadROM(u16 addr, bool debug);
    void WriteROM(u16 addr, u8 val, bool debug);

    u8 ReadRAM(u16 addr, bool debug);
    void WriteRAM(u16 addr, u8 val, bool debug);
};

static inline Serializer& operator<<(Serializer &s, const Mapper &m) {
    std::basic_string<u8> ram(m.ram.begin(), m.ram.end());

    s.Start(Mapper::code_);
    return s << m.number_ << m.ram_enable_ << m.extra_rom_bits_
             << m.rom_bank_ << m.ram_bank_
             << ram;
}

static inline Deserializer& operator>>(Deserializer &d, Mapper &m) {
    std::basic_string<u8> ram;

    d.Start(Mapper::code_);
    d >> m.number_ >> m.ram_enable_ >> m.extra_rom_bits_
      >> m.rom_bank_ >> m.ram_bank_
      >> ram;
    if (!d) { return d; }

    std::copy(ram.begin(), ram.end(), m.ram.begin());

    if (ram.size() < kRomBankSize * 2) { ram.resize(kRomBankSize * 2); }

    return d;
}

}    // namespace gb1
