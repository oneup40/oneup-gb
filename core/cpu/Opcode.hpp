#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/cpu/AddrMode.hpp"
#include "core/cpu/ConditionCode.hpp"
#include "core/cpu/Optype.hpp"

namespace gb1 {

struct Opcode {
    Optype op;
    AddrMode dst, src;
    ConditionCode cc;
    u8 cycles, extra;
};

const Opcode op_table[] = {
    {OP_NOP, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},            // 00
    {OP_LD, AM_BC, AM_IMM16, CC_ALWAYS, 12, 0},
    {OP_LD, AM_MBC, AM_A, CC_ALWAYS, 8, 0},
    {OP_INC16, AM_BC, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_B, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_B, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RLCA, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_MIMM16, AM_SP, CC_ALWAYS, 20, 0},            // 08
    {OP_ADD16, AM_HL, AM_BC, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_MBC, CC_ALWAYS, 8, 0},
    {OP_DEC16, AM_BC, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_C, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_C, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RRCA, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},

    {OP_STOP, AM_NONE, AM_IMM8, CC_ALWAYS, 4, 0},           // 10
    {OP_LD, AM_DE, AM_IMM16, CC_ALWAYS, 12, 0},
    {OP_LD, AM_MDE, AM_A, CC_ALWAYS, 8, 0},
    {OP_INC16, AM_DE, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_D, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_D, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RLA, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_JR, AM_NONE, AM_IMM8, CC_ALWAYS, 12, 0},            // 18
    {OP_ADD16, AM_HL, AM_DE, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_MDE, CC_ALWAYS, 8, 0},
    {OP_DEC16, AM_DE, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_E, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_E, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RRA, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},

    {OP_JR, AM_NONE, AM_IMM8, CC_NZ, 12, 8},                // 20
    {OP_LD, AM_HL, AM_IMM16, CC_ALWAYS, 12, 0},
    {OP_LD, AM_MHLI, AM_A, CC_ALWAYS, 8, 0},
    {OP_INC16, AM_HL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_H, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_H, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_DAA, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_JR, AM_NONE, AM_IMM8, CC_Z, 12, 8},                 // 28
    {OP_ADD16, AM_HL, AM_HL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_MHLI, CC_ALWAYS, 8, 0},
    {OP_DEC16, AM_HL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_L, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_L, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_CPL, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},

    {OP_JR, AM_NONE, AM_IMM8, CC_NC, 8, 4},                 // 30
    {OP_LD, AM_SP, AM_IMM16, CC_ALWAYS, 12, 0},
    {OP_LD, AM_MHLD, AM_A, CC_ALWAYS, 8, 0},
    {OP_INC16, AM_SP, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_MHL, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_DEC8, AM_MHL, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_LD, AM_MHL, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_SCF, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_JR, AM_NONE, AM_IMM8, CC_C, 12, 8},                 // 38
    {OP_ADD16, AM_HL, AM_SP, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_MHLD, CC_ALWAYS, 8, 0},
    {OP_DEC16, AM_SP, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_INC8, AM_A, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_DEC8, AM_A, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_CCF, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},

    {OP_LD, AM_B, AM_B, CC_ALWAYS, 4, 0},                   // 40
    {OP_LD, AM_B, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_B, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_B, AM_A, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_B, CC_ALWAYS, 4, 0},                   // 48
    {OP_LD, AM_C, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_C, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_C, AM_A, CC_ALWAYS, 4, 0},

    {OP_LD, AM_D, AM_B, CC_ALWAYS, 4, 0},                   // 50
    {OP_LD, AM_D, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_D, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_D, AM_A, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_B, CC_ALWAYS, 4, 0},                   // 58
    {OP_LD, AM_E, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_E, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_E, AM_A, CC_ALWAYS, 4, 0},

    {OP_LD, AM_H, AM_B, CC_ALWAYS, 4, 0},                   // 60
    {OP_LD, AM_H, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_H, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_H, AM_A, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_B, CC_ALWAYS, 4, 0},                   // 68
    {OP_LD, AM_L, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_L, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_L, AM_A, CC_ALWAYS, 4, 0},

    {OP_LD, AM_MHL, AM_B, CC_ALWAYS, 8, 0},                 // 70
    {OP_LD, AM_MHL, AM_C, CC_ALWAYS, 8, 0},
    {OP_LD, AM_MHL, AM_D, CC_ALWAYS, 8, 0},
    {OP_LD, AM_MHL, AM_E, CC_ALWAYS, 8, 0},
    {OP_LD, AM_MHL, AM_H, CC_ALWAYS, 8, 0},
    {OP_LD, AM_MHL, AM_L, CC_ALWAYS, 8, 0},
    {OP_HALT, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_LD, AM_MHL, AM_A, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_B, CC_ALWAYS, 4, 0},                   // 78
    {OP_LD, AM_A, AM_C, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_D, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_E, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_H, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_L, CC_ALWAYS, 4, 0},
    {OP_LD, AM_A, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_A, CC_ALWAYS, 4, 0},

    {OP_ADD8, AM_A, AM_B, CC_ALWAYS, 4, 0},                 // 80
    {OP_ADD8, AM_A, AM_C, CC_ALWAYS, 4, 0},
    {OP_ADD8, AM_A, AM_D, CC_ALWAYS, 4, 0},
    {OP_ADD8, AM_A, AM_E, CC_ALWAYS, 4, 0},
    {OP_ADD8, AM_A, AM_H, CC_ALWAYS, 4, 0},
    {OP_ADD8, AM_A, AM_L, CC_ALWAYS, 4, 0},
    {OP_ADD8, AM_A, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_ADD8, AM_A, AM_A, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_B, CC_ALWAYS, 4, 0},                  // 88
    {OP_ADC, AM_A, AM_C, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_D, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_E, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_H, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_L, CC_ALWAYS, 4, 0},
    {OP_ADC, AM_A, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_ADC, AM_A, AM_A, CC_ALWAYS, 4, 0},

    {OP_SUB, AM_NONE, AM_B, CC_ALWAYS, 4, 0},               // 90
    {OP_SUB, AM_NONE, AM_C, CC_ALWAYS, 4, 0},
    {OP_SUB, AM_NONE, AM_D, CC_ALWAYS, 4, 0},
    {OP_SUB, AM_NONE, AM_E, CC_ALWAYS, 4, 0},
    {OP_SUB, AM_NONE, AM_H, CC_ALWAYS, 4, 0},
    {OP_SUB, AM_NONE, AM_L, CC_ALWAYS, 4, 0},
    {OP_SUB, AM_NONE, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_SUB, AM_NONE, AM_A, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_B, CC_ALWAYS, 4, 0},                  // 98
    {OP_SBC, AM_A, AM_C, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_D, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_E, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_H, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_L, CC_ALWAYS, 4, 0},
    {OP_SBC, AM_A, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_SBC, AM_A, AM_A, CC_ALWAYS, 4, 0},

    {OP_AND, AM_NONE, AM_B, CC_ALWAYS, 4, 0},               // A0
    {OP_AND, AM_NONE, AM_C, CC_ALWAYS, 4, 0},
    {OP_AND, AM_NONE, AM_D, CC_ALWAYS, 4, 0},
    {OP_AND, AM_NONE, AM_E, CC_ALWAYS, 4, 0},
    {OP_AND, AM_NONE, AM_H, CC_ALWAYS, 4, 0},
    {OP_AND, AM_NONE, AM_L, CC_ALWAYS, 4, 0},
    {OP_AND, AM_NONE, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_AND, AM_NONE, AM_A, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_B, CC_ALWAYS, 4, 0},               // A8
    {OP_XOR, AM_NONE, AM_C, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_D, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_E, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_H, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_L, CC_ALWAYS, 4, 0},
    {OP_XOR, AM_NONE, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_XOR, AM_NONE, AM_A, CC_ALWAYS, 4, 0},

    {OP_OR, AM_NONE, AM_B, CC_ALWAYS, 4, 0},                // B0
    {OP_OR, AM_NONE, AM_C, CC_ALWAYS, 4, 0},
    {OP_OR, AM_NONE, AM_D, CC_ALWAYS, 4, 0},
    {OP_OR, AM_NONE, AM_E, CC_ALWAYS, 4, 0},
    {OP_OR, AM_NONE, AM_H, CC_ALWAYS, 4, 0},
    {OP_OR, AM_NONE, AM_L, CC_ALWAYS, 4, 0},
    {OP_OR, AM_NONE, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_OR, AM_NONE, AM_A, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_B, CC_ALWAYS, 4, 0},                // B8
    {OP_CP, AM_NONE, AM_C, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_D, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_E, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_H, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_L, CC_ALWAYS, 4, 0},
    {OP_CP, AM_NONE, AM_MHL, CC_ALWAYS, 8, 0},
    {OP_CP, AM_NONE, AM_A, CC_ALWAYS, 4, 0},

    {OP_RET, AM_NONE, AM_NONE, CC_NZ, 20, 8},               // C0
    {OP_POP, AM_BC, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_JP, AM_NONE, AM_IMM16, CC_NZ, 16, 12},
    {OP_JP, AM_NONE, AM_IMM16, CC_ALWAYS, 16, 0},
    {OP_CALL, AM_NONE, AM_IMM16, CC_NZ, 24, 12},
    {OP_PUSH, AM_NONE, AM_BC, CC_ALWAYS, 16, 0},
    {OP_ADD8, AM_A, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},
    {OP_RET, AM_NONE, AM_NONE, CC_Z, 20, 8},                // C8
    {OP_RET, AM_NONE, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_JP, AM_NONE, AM_IMM16, CC_Z, 16, 12},
    {OP_CB, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_CALL, AM_NONE, AM_IMM16, CC_Z, 24, 12},
    {OP_CALL, AM_NONE, AM_IMM16, CC_ALWAYS, 24, 0},
    {OP_ADC, AM_A, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},

    {OP_RET, AM_NONE, AM_NONE, CC_NC, 20, 8},               // D0
    {OP_POP, AM_DE, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_JP, AM_NONE, AM_IMM16, CC_NC, 16, 12},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_CALL, AM_NONE, AM_IMM16, CC_NC, 24, 12},
    {OP_PUSH, AM_NONE, AM_DE, CC_ALWAYS, 16, 0},
    {OP_SUB, AM_NONE, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},
    {OP_RET, AM_NONE, AM_NONE, CC_C, 20, 8},                // D8
    {OP_RETI, AM_NONE, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_JP, AM_NONE, AM_IMM16, CC_C, 16, 12},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_CALL, AM_NONE, AM_IMM16, CC_C, 24, 12},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_SBC, AM_A, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},

    {OP_LD, AM_HMIMM8, AM_A, CC_ALWAYS, 12, 0},             // E0
    {OP_POP, AM_HL, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_LD, AM_HMC, AM_A, CC_ALWAYS, 8, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_PUSH, AM_NONE, AM_HL, CC_ALWAYS, 16, 0},
    {OP_AND, AM_NONE, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},
    {OP_ADDSP, AM_SP, AM_SPIMM8, CC_ALWAYS, 16, 0},         // E8
    {OP_JP, AM_NONE, AM_HL, CC_ALWAYS, 4, 0},
    {OP_LD, AM_MIMM16, AM_A, CC_ALWAYS, 16, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_XOR, AM_NONE, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},

    {OP_LD, AM_A, AM_HMIMM8, CC_ALWAYS, 12, 0},             // F0
    {OP_POP, AM_AF, AM_NONE, CC_ALWAYS, 12, 0},
    {OP_LD, AM_A, AM_HMC, CC_ALWAYS, 8, 0},
    {OP_DI, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_PUSH, AM_NONE, AM_AF, CC_ALWAYS, 16, 0},
    {OP_OR, AM_NONE, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},
    {OP_ADDSP, AM_HL, AM_SPIMM8, CC_ALWAYS, 12, 0},         // F8
    {OP_LD, AM_SP, AM_HL, CC_ALWAYS, 8, 0},
    {OP_LD, AM_A, AM_MIMM16, CC_ALWAYS, 16, 0},
    {OP_EI, AM_NONE, AM_NONE, CC_ALWAYS, 4, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_UD, AM_NONE, AM_NONE, CC_ALWAYS, 0, 0},
    {OP_CP, AM_NONE, AM_IMM8, CC_ALWAYS, 8, 0},
    {OP_RST, AM_NONE, AM_NONE, CC_ALWAYS, 16, 0},
};
static_assert(sizeof(op_table) == 256 * sizeof(op_table[0]), "op_table does not contain 256 elements");

const Opcode op_cb_table[] = {
    {OP_RLC, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 00
    {OP_RLC, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RLC, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 08
    {OP_RRC, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RRC, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_RL, AM_B, AM_NONE, CC_ALWAYS, 8, 0},                // 10
    {OP_RL, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RL, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_B, AM_NONE, CC_ALWAYS, 8, 0},                // 18
    {OP_RR, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RR, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SLA, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 20
    {OP_SLA, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SLA, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 28
    {OP_SRA, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRA, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SWAP, AM_B, AM_NONE, CC_ALWAYS, 8, 0},              // 30
    {OP_SWAP, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SWAP, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 38
    {OP_SRL, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SRL, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 40
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 48
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 50
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 58
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 60
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 68
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 70
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 78
    {OP_BIT, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_BIT, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 80
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 88
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 90
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // 98
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // A0
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // A8
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // B0
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // B8
    {OP_RES, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_RES, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // C0
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // C8
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // D0
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // D8
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // E0
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // E8
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},

    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // F0
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_B, AM_NONE, CC_ALWAYS, 8, 0},               // F8
    {OP_SET, AM_C, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_D, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_E, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_H, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_L, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_MHL, AM_NONE, CC_ALWAYS, 8, 0},
    {OP_SET, AM_A, AM_NONE, CC_ALWAYS, 8, 0},
};
static_assert(sizeof(op_cb_table) == 256 * sizeof(op_cb_table[0]), "op_cb_table does not contain 256 elements");

}    // namespace gb1
