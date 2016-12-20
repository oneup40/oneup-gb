// Copyright 2016 oneup

#include "Mapper.hpp"

#include <cassert>

#include "Machine.hpp"

namespace gblr {

size_t Mapper::RomIndex(u16 addr) {
    size_t index = addr;
    switch (number_) {
        case kMapperNone:
            break;
        case kMapperMBC1:
        case kMapperMBC2:
            if (addr & 0x4000) {
                index &= ~0x4000;
                index |= (bank_ << kBankShift);
            }
            break;
    }

    assert(index < rom.size());
    index &= (rom.size() - 1);
    return index;
}

size_t Mapper::RamIndex(u16 addr) {
    size_t index = addr;

    switch (number_) {
        case kMapperNone:
        case kMapperMBC1:
            assert(0);
            return 0;
        case kMapperMBC2:
            index = addr & 0x01FF;
            break;
    }

    return index;
}

void Mapper::Init(MapperNumber number, const u8 *data, size_t size) {
    number_ = number;
    rom = std::vector<u8>(data, data + size);
    switch (number) {
        case kMapperNone:
        case kMapperMBC1:
            ram = std::vector<u8>();
            break;
        case kMapperMBC2:
            ram = std::vector<u8>(512, 0);
            break;
    }
    bank_ = 1;
}

void Mapper::Unload() {
    number_ = kMapperNone;
    rom.clear();
    ram.clear();
    ram_enable_ = false;
    bank_ = 1;
}

u8 Mapper::ReadROM(u16 addr, bool) {
    size_t index = RomIndex(addr);
    return rom[index];
}

void Mapper::WriteROM(u16 addr, u8 val, bool debug) {
    size_t index = RomIndex(addr);

    if (debug) { rom[index] = val; return; }

    switch (number_) {
        case kMapperNone: break;
        case kMapperMBC1:
            switch (addr & 0x6000) {
                case 0x0000:
                    m_->Log("MBC1 RAM enable not implemented");
                    break;
                case 0x2000:
                    bank_ = val & 0x1f;
                    if (!bank_) { bank_ = 1; }
                    break;
                case 0x4000:
                    m_->Log("MBC1 RAM bank select / ROM upper bank select not implemented");
                    break;
                case 0x6000:
                    m_->Log("MBC1 RAM/ROM mode select not implemented");
                    break;
            }
            break;
        case kMapperMBC2:
            switch (addr & 0x6000) {
                case 0x0000:
                    ram_enable_ = addr & 0x0100;
                    break;
                case 0x2000:
                    bank_ = val & 0x0F;
                    if (!bank_) { bank_ = 1; }
                    break;
                case 0x4000:
                case 0x6000:
                    break;
            }
            break;
    }
}

u8 Mapper::ReadRAM(u16 addr, bool) {
    if (!ram_enable_) { return 0; }

    size_t index = RamIndex(addr);
    return ram[index];
}

void Mapper::WriteRAM(u16 addr, u8 val, bool force) {
    if (!ram_enable_) { return; }

    if (number_ == kMapperMBC2 && !force) {
        // MBC2 ram is 4-bit
        val &= 0x0F;
    }

    size_t index = RamIndex(addr);
    ram[index] = val;
}

}
