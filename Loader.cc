// Copyright 2016 oneup

#include "Loader.hpp"

#include <string>

#include <cassert>

#include "Base.hpp"
#include "Machine.hpp"
#include "Mapper.hpp"

namespace gblr {

bool Loader::Load(const struct retro_game_info *game, Machine *machine) {
    const u8 *data = static_cast<const u8*>(game->data);

    if (game->size < 2 * kBankSize) {
        machine->Log("Expected at least two 16KiB banks");
        return false;
    }

    assert(0x143 < game->size);
    if ((data[0x143] & 0xc0) == 0xc0) {
        machine->Log("CGB-only cartridges not supported");
        return false;
    }

    size_t expected = 0;
    assert(0x148 < game->size);
    switch (data[0x148]) {
        case 0x00: expected = 2 * kBankSize; break;
        case 0x01: expected = 4 * kBankSize; break;
        case 0x02: expected = 8 * kBankSize; break;
        case 0x03: expected = 16 * kBankSize; break;
        case 0x04: expected = 32 * kBankSize; break;
        case 0x05: expected = 64 * kBankSize; break;
        case 0x06: expected = 128 * kBankSize; break;
        case 0x07: expected = 256 * kBankSize; break;
        case 0x52: expected = 72 * kBankSize; break;
        case 0x53: expected = 80 * kBankSize; break;
        case 0x54: expected = 96 * kBankSize; break;
        default:
            machine->Log("Unexpected ROM size indicator");
            return false;
    }

    if (game->size != expected) {
        machine->Log("ROM size didn't match ROM size indicator");
        return false;
    }

    assert(0x147 < game->size);
    u8 cart = data[0x147];

    MapperNumber mapper = kMapperNone;

    switch (cart) {
        case 0: mapper = kMapperNone; break;
        case 1: mapper = kMapperMBC1; break;
        case 5: mapper = kMapperMBC2; break;
        default:
            std::cerr << "unsupported cartridge type: $" << to_hex(cart, 2) << std::endl;
            return false;
    }

    machine->mapper.Init(mapper, data, game->size);
    return true;
}

}    // namespace gblr
