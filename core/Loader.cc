// Copyright 2016 oneup

#include "Loader.hpp"

#include <string>

#include <cassert>

#include "Base.hpp"
#include "Machine.hpp"
#include "Mapper.hpp"

namespace gb1 {

bool Loader::Load(std::vector<u8>&& data, Machine *machine) {
    if (data.size() < 2 * kRomBankSize) {
        machine->Log("Expected at least two 16KiB banks");
        return false;
    }

    assert(0x143 < game->size);
    if ((data[0x143] & 0xC0) == 0xC0) {
        machine->Log("CGB-only cartridges not supported");
        return false;
    }

    size_t expected = 0;
    assert(0x148 < game->size);
    switch (data[0x148]) {
        case 0x00: expected = 2 * kRomBankSize; break;
        case 0x01: expected = 4 * kRomBankSize; break;
        case 0x02: expected = 8 * kRomBankSize; break;
        case 0x03: expected = 16 * kRomBankSize; break;
        case 0x04: expected = 32 * kRomBankSize; break;
        case 0x05: expected = 64 * kRomBankSize; break;
        case 0x06: expected = 128 * kRomBankSize; break;
        case 0x07: expected = 256 * kRomBankSize; break;
        case 0x52: expected = 72 * kRomBankSize; break;
        case 0x53: expected = 80 * kRomBankSize; break;
        case 0x54: expected = 96 * kRomBankSize; break;
        default:
            machine->Log("Unexpected ROM size indicator");
            return false;
    }

    if (data.size() != expected) {
        machine->Log("ROM size didn't match ROM size indicator");
        return false;
    }

    assert(0x147 < game->size);
    u8 cart = data[0x147];

    MapperNumber mapper = kMapperNone;

    switch (cart) {
        case 0x00:
            mapper = kMapperNone;
            break;

        case 0x01: case 0x02: case 0x03:
            mapper = kMapperMBC1;
            break;

        case 0x05: case 0x06:
            mapper = kMapperMBC2;
            break;

        case 0x0F: case 0x10: case 0x11: case 0x12: case 0x13:
            mapper = kMapperMBC3;
            break;

        case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E:
            mapper = kMapperMBC5;
            break;

        default:
            std::cerr << "unsupported cartridge type: $" << to_hex(cart, 2) << std::endl;
            return false;
    }

    size_t ram_size = 0;
    switch (data[0x149]) {
        case 0x00:  ram_size = 0; break;
        case 0x01:  ram_size = 2 * 1024; break;
        case 0x02:  ram_size = 1 * kRamBankSize; break;
        case 0x03:  ram_size = 4 * kRamBankSize; break;
        case 0x04:  ram_size = 16 * kRamBankSize; break;
        case 0x05:  ram_size = 8 * kRamBankSize; break;

        default:
            std::cerr << "Unexpected RAM size indicator: $" << to_hex(data[0x149], 2) << std::endl;
            return false;
    }

    machine->mapper.Init(mapper, std::move(data), ram_size);
    return true;
}

}    // namespace gb1
