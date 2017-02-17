#pragma once

// Copyright 2017 oneup

#include <string>

#include <cassert>

#include "core/Base.hpp"

namespace gb1 {

enum Optype : u8 {
    OP_NOP,
    OP_LD, OP_PUSH, OP_POP,
    OP_INC8, OP_INC16, OP_DEC8, OP_DEC16, OP_ADD8, OP_ADD16, OP_ADDSP, OP_ADC, OP_SUB, OP_SBC, OP_CP, OP_DAA,
    OP_RLA, OP_RLCA, OP_RRA, OP_RRCA,
    OP_RL, OP_RLC, OP_RR, OP_RRC, OP_SLA, OP_SRA, OP_SWAP, OP_SRL, OP_BIT, OP_RES, OP_SET,
    OP_CPL, OP_AND, OP_XOR, OP_OR,
    OP_SCF, OP_CCF, OP_DI, OP_EI,
    OP_JR, OP_JP, OP_CALL, OP_RET, OP_RETI, OP_STOP, OP_HALT, OP_RST,
    OP_CB,
    OP_UD
};

static inline std::string to_string(Optype op) {
    switch (op) {
        case OP_NOP:        return "nop";
        case OP_LD:         return "ld";
        case OP_PUSH:       return "push";
        case OP_POP:        return "pop";
        case OP_INC8:       return "inc";
        case OP_INC16:      return "inc";
        case OP_DEC8:       return "dec";
        case OP_DEC16:      return "dec";
        case OP_ADD8:       return "add";
        case OP_ADD16:      return "add";
        case OP_ADDSP:      return "ld";
        case OP_ADC:        return "adc";
        case OP_SUB:        return "sub";
        case OP_SBC:        return "sbc";
        case OP_CP:         return "cp";
        case OP_DAA:        return "daa";
        case OP_RRA:        return "rra";
        case OP_RRCA:       return "rrca";
        case OP_RL:         return "rl";
        case OP_RLC:        return "rlc";
        case OP_RR:         return "rr";
        case OP_RRC:        return "rrc";
        case OP_SLA:        return "sla";
        case OP_SRA:        return "sra";
        case OP_SWAP:       return "swap";
        case OP_SRL:        return "srl";
        case OP_BIT:        return "bit";
        case OP_RES:        return "res";
        case OP_SET:        return "set";
        case OP_CPL:        return "cpl";
        case OP_AND:        return "and";
        case OP_XOR:        return "xor";
        case OP_OR:         return "or";
        case OP_SCF:        return "scf";
        case OP_CCF:        return "ccf";
        case OP_DI:         return "di";
        case OP_EI:         return "ei";
        case OP_JR:         return "jr";
        case OP_JP:         return "jp";
        case OP_CALL:       return "call";
        case OP_RET:        return "ret";
        case OP_RETI:       return "reti";
        case OP_STOP:       return "stop";
        case OP_HALT:       return "halt";
        case OP_RST:        return "rst";
        case OP_UD:         return "ud";
        default:
            assert(0);
            return "???";
    }
}

}   // namespace gb1
