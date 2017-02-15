// Copyright 2016-2017 oneup

#include "core/Mapper.hpp"

#include <cassert>

#include "core/Machine.hpp"

namespace gb1 {

static constexpr const size_t kInvalidIndex = 0xFFFFFFFF;

size_t Mapper::RomIndex(u16 addr) {
    size_t bank = rom_bank_;

    if (number_ == kMapperMBC1 && extra_rom_bits_) {
        bank |= (ram_bank_ & 3) << 5;
    }

    if (number_ == kMapperMBC1 && !(bank & 0x1F)) {
        bank = 0x01;
    }

    if (number_ == kMapperMBC3 && !bank) {
        bank = 0x01;
    }

    // TODO: determine what happens on out-of-bound accesses (especially non-power-of-2)
    if ((bank << kRomBankShift) >= rom.size()) {
        bank = 0x01;
    }

    size_t index = (bank << kRomBankShift) | (addr & kRomOffsetMask);
    assert(index <= rom.size());
    return index;
}

size_t Mapper::RamIndex(u16 addr) {
    if (ram.empty()) { return kInvalidIndex; }

    // TODO: access RTC
    if (number_ == kMapperMBC3 && ram_bank_ > 3) { return kInvalidIndex; }

    size_t bank = ram_bank_;
    if (number_ == kMapperMBC1 && extra_rom_bits_) {
        bank = 0;
    }

    // TODO: determine what happens on out-of-bound accesses
    if ((bank << kRamBankShift) >= ram.size()) {
        bank = 0x00;
    }

    size_t index = (bank << kRamBankShift) | (addr & kRamOffsetMask);

    if (number_ == kMapperMBC2) {
        // MBC2 is only 512 half-words, not a full bank
        index &= 0x01FF;
    }

    if (ram.size() < kRamBankSize) {
        // It's possible to have less than a full bank of RAM
        index %= ram.size();
    }

    assert(index < ram.size());
    return index;
}

void Mapper::WriteMBC1Register(u16 addr, u8 data) {
    switch (addr & 0x6000) {
        case 0x0000:
            // TODO: determine what non-0x0A values do
            ram_enable_ = ((data & 0x0F) == 0x0A);
            break;
        case 0x2000:
            rom_bank_ = data & 0x1f;
            break;
        case 0x4000:
            ram_bank_ = data & 0x03;
            break;
        case 0x6000:
            extra_rom_bits_ = !(data & 0x01);
            break;
    }
}

void Mapper::WriteMBC2Register(u16 addr, u8 data) {
    switch (addr & 0x6000) {
        case 0x0000:
            //TODO: research this
            if (!(addr & 0x0100)) {
                ram_enable_ = ((data & 0x0F) == 0x0A);
            }
            break;
        case 0x2000:
            rom_bank_ = data & 0x0F;
            break;
        case 0x4000:
        case 0x6000:
            break;
    }
}

void Mapper::WriteMBC3Register(u16 addr, u8 data) {
    switch (addr & 0x6000) {
        case 0x0000:
            ram_enable_ = (data & 0x0F) == 0x0A;
            break;
        case 0x2000:
            rom_bank_ = data & 0x3F;
            break;
        case 0x4000:
            ram_bank_ = data & 0x0F;
            break;
        case 0x6000:
            // TODO: access RTC
            break;
    }
}

void Mapper::WriteMBC5Register(u16 addr, u8 data) {
    switch (addr & 0x7000) {
        case 0x0000:
        case 0x1000:
            ram_enable_ = (data & 0x0F) == 0x0A;
            break;
        case 0x2000:
            rom_bank_ &= ~0x0F;
            rom_bank_ |= data;
            break;
        case 0x3000:
            rom_bank_ &= 0x0F;
            rom_bank_ |= (data & 0x01);
            break;
        case 0x4000:
        case 0x5000:
            ram_bank_ = data & 0x0F;
            break;
        case 0x6000:
        case 0x7000:
            break;
    }
}

Mapper::Mapper()
    : number_(kMapperNone),
      ram_enable_(false), extra_rom_bits_(false),
      rom_bank_(0), ram_bank_(0)
{}

void Mapper::Init(MapperNumber number, std::vector<u8>&& rom, size_t ram_size) {
    if (number == kMapperMBC2) { ram_size = 512; }

    this->number_ = number;
    this->ram_enable_ = false;
    this->extra_rom_bits_ = false;
    this->rom_bank_ = 0x01;
    assert(rom.size() >= kRomBankSize * 2);
    this->ram_bank_ = 0;
    this->rom = std::move(rom);
    this->ram = std::vector<u8>(ram_size, 0);
}

void Mapper::Unload() {
    number_ = kMapperNone;
    ram_enable_ = false;
    extra_rom_bits_ = false;
    rom_bank_ = 0;
    ram_bank_ = 0;
    rom.clear();
    ram.clear();
}

u8 Mapper::ReadROM(u16 addr, bool) {
    size_t index = addr;
    if (addr >= 0x4000) {
        index = RomIndex(addr);
    }

    return rom[index];
}

void Mapper::WriteROM(u16 addr, u8 val, bool debug) {
    size_t index = addr;
    if (addr >= 0x4000) {
        index = RomIndex(addr);
    }

    if (debug) { rom[index] = val; return; }

    switch (number_) {
        case kMapperMBC1: WriteMBC1Register(addr, val); break;
        case kMapperMBC2: WriteMBC2Register(addr, val); break;
        case kMapperMBC3: WriteMBC3Register(addr, val); break;
        case kMapperMBC5: WriteMBC5Register(addr, val); break;

        case kMapperNone:
        default:
            break;
    }
}

u8 Mapper::ReadRAM(u16 addr, bool force) {
    if (!ram_enable_ && !force) { return 0x00; }

    size_t index = RamIndex(addr);
    if (index == kInvalidIndex) { return 0x00; }

    u8 data = ram[index];

    if (number_ == kMapperMBC2 && !force) { data &= 0x0F; }

    return data;
}

void Mapper::WriteRAM(u16 addr, u8 val, bool force) {
    if (!ram_enable_ && !force) { return; }

    if (number_ == kMapperMBC2 && !force) { val &= 0x0F; }

    size_t index = RamIndex(addr);
    if (index == kInvalidIndex) { return; }

    ram[index] = val;
}

}   // namespace gb1
