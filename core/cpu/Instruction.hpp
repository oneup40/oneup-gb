#pragma once

// Copyright 2017 oneup

#include "core/cpu/AddrMode.hpp"
#include "core/cpu/ConditionCode.hpp"
#include "core/cpu/Operand.hpp"
#include "core/cpu/Optype.hpp"

namespace gb1 {

struct Instruction {
    u16 pc;

    u8 bytes[3];
    u8 nbytes;

    Optype op;
    Operand src, dst;

    ConditionCode cc;
    u8 cycles, extra;
    bool taken;

    Instruction()
            : pc(0),
			  bytes{0,0,0}, nbytes(0),
			  op(OP_NOP), src(), dst(),
              cc(CC_ALWAYS), cycles(0), extra(0), taken(false)
    {}

    u8 *byte_ptr() { return &bytes[nbytes]; }
};

static inline std::string to_string(const Instruction &ins) {
    std::string s;
    s += to_hex(ins.pc, 4);
    s += "  ";

    for (auto i=0ul; i < ins.nbytes; ++i) {
        s += to_hex(ins.bytes[i], 2);
        s += " ";
    }
    while (s.length() < 16) { s += " "; }

    s += to_string(ins.op);
    while (s.length() < 22) { s += " "; }

    if (ins.cc != CC_ALWAYS) {
        s += to_string(ins.cc);
        s += ", ";
    }

    s += to_string(ins.dst);
    if (ins.dst.am != AM_NONE && ins.src.am != AM_NONE) {
        s += ", ";
    }

    s += to_string(ins.src);
    return s;
}

}
