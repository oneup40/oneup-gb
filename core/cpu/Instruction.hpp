#pragma once

// Copyright 2017 oneup

#include "core/cpu/AddrMode.hpp"
#include "core/cpu/ConditionCode.hpp"
#include "core/cpu/Optype.hpp"

namespace gb1 {

struct Instruction {
    u32 src, dst;
    u16 pc, src_ea, dst_ea;

    u8 bytes[3];
    u8 nbytes;
    Optype op;

    AddrMode dst_am;
    AddrMode src_am;

    ConditionCode cc;
    u8 cycles, extra;
    bool taken;

    Instruction()
            : src(0), dst(0), pc(0), src_ea(0), dst_ea(0),
              bytes{0,0,0}, nbytes(0), op(OP_NOP),
              dst_am(AM_NONE), src_am(AM_NONE),
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
