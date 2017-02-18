#pragma once

// Copyright 2017 oneup

#include <string>

#include "core/Base.hpp"
#include "core/cpu/AddrMode.hpp"

namespace gb1 {

struct Operand {
	u32 val;
	u16 ea;
	AddrMode am;

	Operand() : val(0), ea(0), am(AM_NONE) {}
};

static inline std::string to_string(const Operand &op) {
    switch (op.am) {
        case AM_NONE:
            return "";
        case AM_IMM8:
            return "$" + to_hex(op.val, 2);
        case AM_IMM16:
            return "$" + to_hex(op.val, 4);
        case AM_MIMM16:
            return "($" + to_hex(op.ea, 4) + ") = $" + to_hex(op.val, 2);
        case AM_A:
            return "A";
        case AM_B:
            return "B";
        case AM_C:
            return "C";
        case AM_D:
            return "D";
        case AM_E:
            return "E";
        case AM_H:
            return "H";
        case AM_L:
            return "L";
        case AM_AF:
            return "AF";
        case AM_BC:
            return "BC";
        case AM_DE:
            return "DE";
        case AM_HL:
            return "HL";
        case AM_SP:
            return "SP";
        case AM_MBC:
            return "(BC) = $" + to_hex(op.val, 2);
        case AM_MDE:
            return "(DE) = $" + to_hex(op.val, 2);
        case AM_MHL:
            return "(HL) = $" + to_hex(op.val, 2);
        case AM_MHLI:
            return "(HL+) = $" + to_hex(op.val, 2);
        case AM_MHLD:
            return "(HL-) = $" + to_hex(op.val, 2);
        case AM_HMIMM8:
            return "($FF00+$" + to_hex(op.ea & 0xff, 2) + ") = $" + to_hex(op.val, 2);
        case AM_HMC:
            return "($FF00+C) = $" + to_hex(op.val, 2);
        case AM_SPIMM8:
            return "SP+$" + to_hex(op.val & 0xff, 2);
        default:
            assert(0);
            return "XXX";
    }
}

}   // namespace gb1
