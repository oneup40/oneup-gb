#pragma once

// Copyright 2017 oneup

#include <string>

#include <cassert>

#include "core/Base.hpp"

namespace gb1 {

enum AddrMode : u8 {
    AM_NONE,
    AM_IMM8, AM_IMM16,
    AM_MIMM16,
    AM_A, AM_B, AM_C, AM_D, AM_E, AM_H, AM_L,
    AM_AF, AM_BC, AM_DE, AM_HL, AM_SP,
    AM_MBC, AM_MDE, AM_MHL, AM_MHLI, AM_MHLD,
    AM_HMIMM8, AM_HMC,
    AM_SPIMM8
};

static inline std::string to_string(AddrMode am) {
    switch (am) {
        case AM_NONE:   return "none";
        case AM_IMM8:   return "imm8";
        case AM_IMM16:  return "imm16";
        case AM_MIMM16: return "(imm16)";
        case AM_A:      return "A";
        case AM_B:      return "B";
        case AM_C:      return "C";
        case AM_D:      return "D";
        case AM_E:      return "E";
        case AM_H:      return "H";
        case AM_L:      return "L";
        case AM_AF:     return "AF";
        case AM_BC:     return "BC";
        case AM_DE:     return "DE";
        case AM_HL:     return "HL";
        case AM_SP:     return "SP";
        case AM_MBC:    return "(BC)";
        case AM_MDE:    return "(DE)";
        case AM_MHL:    return "(HL)";
        case AM_MHLI:   return "(HLI)";
        case AM_MHLD:   return "(HLD)";
        case AM_HMIMM8: return "($FF00+imm8)";
        case AM_HMC:    return "($FF00+C)";
        case AM_SPIMM8: return "SP+imm8";
        default:
            assert(0);
            return "???";
    }
}


}   // namespace gb1
