// Copyright 2016 oneup

#include "CPU.hpp"
#include "Machine.hpp"

namespace gb1 {

bool CPU::FetchInstruction(Instruction *ins) {
    ins->pc = reg_.pc++;
    ins->bytes[ins->nbytes++] = m_->Read(ins->pc);

    if (ins->bytes[0] == 0xcb) {
        ins->bytes[ins->nbytes++] = m_->Read(reg_.pc++);
    }

    return true;
}

bool CPU::Decode(Instruction *ins) {
    assert(ins->nbytes > 0);

    auto op = &op_table[ins->bytes[0]];
    if (op->op == OP_CB) {
        op = &op_cb_table[ins->bytes[1]];
    }

    ins->op = op->op;
    ins->dst_am = op->dst;
    ins->src_am = op->src;
    ins->cc = op->cc;
    ins->cycles = op->cycles;
    ins->extra = op->extra;

    return true;
}

bool CPU::FetchOperand(Instruction *ins, AddrMode am, u16 *ea, u32 *val) {
    switch (am) {
        case AM_NONE:
            return true;
        case AM_IMM8:
        case AM_SPIMM8:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            ++reg_.pc;
            *val = *ins->byte_ptr();
            ++ins->nbytes;
            return true;
        case AM_IMM16:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            *(ins->byte_ptr() + 1) = m_->Read(reg_.pc + 1);
            reg_.pc += 2;
            *val = letoh(*reinterpret_cast<u16*>(ins->byte_ptr()));
            ins->nbytes += 2;
            return true;
        case AM_MIMM16:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            *(ins->byte_ptr() + 1) = m_->Read(reg_.pc + 1);
            reg_.pc += 2;
            *ea = letoh(*reinterpret_cast<u16*>(ins->byte_ptr()));
            ins->nbytes += 2;
            *val = m_->Read(*ea);
            return true;
        case AM_A:
            *val = reg_.a;
            return true;
        case AM_B:
            *val = reg_.b;
            return true;
        case AM_C:
            *val = reg_.c;
            return true;
        case AM_D:
            *val = reg_.d;
            return true;
        case AM_E:
            *val = reg_.e;
            return true;
        case AM_H:
            *val = reg_.h;
            return true;
        case AM_L:
            *val = reg_.l;
            return true;
        case AM_AF:
            *val = reg_.af;
            return true;
        case AM_BC:
            *val = reg_.bc;
            return true;
        case AM_DE:
            *val = reg_.de;
            return true;
        case AM_HL:
            *val = reg_.hl;
            return true;
        case AM_SP:
            *val = reg_.sp;
            return true;
        case AM_MBC:
            *ea = reg_.bc;
            *val = m_->Read(*ea);
            return true;
        case AM_MDE:
            *ea = reg_.de;
            *val = m_->Read(*ea);
            return true;
        case AM_MHL:
            *ea = reg_.hl;
            *val = m_->Read(*ea);
            return true;
        case AM_MHLI:
            *ea = reg_.hl++;
            *val = m_->Read(*ea);
            return true;
        case AM_MHLD:
            *ea = reg_.hl--;
            *val = m_->Read(*ea);
            return true;
        case AM_HMIMM8:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            ++reg_.pc;
            *ea = 0xff00 | *ins->byte_ptr();
            ++ins->nbytes;
            *val = m_->Read(*ea);
            return true;
        case AM_HMC:
            *ea = 0xff00 | reg_.c;
            *val = m_->Read(*ea);
            return true;
        default:
            m_->Log("CPU::FetchOperand: AddrMode " + to_string(am) + " not implemented");
            return false;
    }
}

bool CPU::FetchOperands(Instruction *ins) {
    bool good = FetchOperand(ins, ins->src_am, &ins->src_ea, &ins->src);
    if (!good) { return false; }

    good = FetchOperand(ins, ins->dst_am, &ins->dst_ea, &ins->dst);

    return good;
}

static inline u16 read_u16(Machine *m, u16 addr) {
    // little endian
    u16 val = m->Read(addr);
    val |= m->Read(addr+1) << 8;
    return val;
}

static inline void write_u16(Machine *m, u16 addr, u16 val) {
    // little endian
    m->Write(addr, val & 0xff);
    m->Write(addr+1, (val >> 8) & 0xff);
}

bool CPU::Execute(Instruction *ins) {
    busy_ += ins->cycles;

    u8 tmp8 = 0;
    u32 tmp32 = 0;

    switch (ins->op) {
        case OP_NOP:
            break;
        case OP_LD:
            ins->dst = ins->src;
            break;
        case OP_PUSH:
            reg_.sp -= 2;
            write_u16(m_, reg_.sp, ins->src);
            return true;
        case OP_POP:
            ins->dst = read_u16(m_, reg_.sp);
            reg_.sp += 2;
            return true;
        case OP_INC8:
            tmp32 = ins->dst;
            ++ins->dst;
            reg_.setfb(FLAG_Z, (ins->dst & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_H, (ins->dst & 0xf0) != (tmp32 & 0xf0));
            break;
        case OP_INC16:
            ++ins->dst;
            break;
        case OP_DEC8:
            tmp32 = ins->dst;
            --ins->dst;
            reg_.setfb(FLAG_Z, (ins->dst & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (ins->dst & 0xf0) != (tmp32 & 0xf0));
            break;
        case OP_DEC16:
            --ins->dst;
            break;
        case OP_ADC:
            tmp8 = reg_.getf(FLAG_C);
            /* no break */
        case OP_ADD8:
            tmp32 = ins->dst;
            ins->dst = ins->dst + ins->src + tmp8;
            reg_.setfb(FLAG_Z, (ins->dst & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_H, (tmp32 & 0xf) + (ins->src & 0xf) + tmp8 >= 0x10);
            reg_.setfb(FLAG_C, ins->dst >= 0x100);
            break;
        case OP_ADD16:
            reg_.setfb(FLAG_H, (ins->dst & 0xfff) + (ins->src & 0xfff) > 0xfff);
            ins->dst += ins->src;
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_C, ins->dst >= 0x10000);
            break;
        case OP_ADDSP:
            // sign extend
            ins->src |= (ins->src & 0x80) ? 0xff00 : 0x0000;
            reg_.setfb(FLAG_H, (reg_.sp & 0xf) + (ins->src & 0xf) > 0xf);
            reg_.setfb(FLAG_C, (reg_.sp & 0xff) + (ins->src & 0xff) > 0xff);
            ins->dst = reg_.sp + i8(ins->src);
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            break;
        case OP_CP:
        case OP_SUB:
            tmp32 = reg_.a - ins->src;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (reg_.a & 0xf) - (ins->src & 0xf) >= 0x10);    // remember, unsigned arithmetic
            reg_.setfb(FLAG_C, tmp32 >= 0x100);
            if (ins->op == OP_SUB) { reg_.a = tmp32; }
            break;
        case OP_SBC:
            tmp8 = reg_.getf(FLAG_C);
            tmp32 = ins->dst;
            ins->dst = ins->dst - ins->src - tmp8;
            reg_.setfb(FLAG_Z, (ins->dst & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (tmp32 & 0xf) - (ins->src & 0xf) - tmp8 >= 0x10);    // remember, unsigned arithmetic
            reg_.setfb(FLAG_C, ins->dst >= 0x100);
            break;
        case OP_DAA:
            tmp32 = reg_.a;
            if (reg_.getf(FLAG_N)) {
                if (reg_.getf(FLAG_H)) { tmp32 -= 0x06; }
                if (reg_.getf(FLAG_C)) { tmp32 -= 0x60; }
            } else {
                if ((tmp32 & 0x0F) >= 0x0A || reg_.getf(FLAG_H)) { tmp32 += 0x06; }
                if ((tmp32 & 0xFFF0) >= 0xA0 || reg_.getf(FLAG_C)) {
                    tmp32 += 0x60;
                    reg_.setf(FLAG_C);
                }
            }

            reg_.a = tmp32;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_H);
            break;
        case OP_RLA:
            tmp32 = reg_.a << 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x01 : 0x00;
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (reg_.a & 0x80) != 0);
            reg_.a = tmp32;
            break;
        case OP_RLCA:
            reg_.a = (reg_.a << 1) | (reg_.a >> 7);
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, reg_.a & 0x01);
            break;
        case OP_RRA:
            tmp32 = reg_.a >> 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x80 : 0x00;
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, reg_.a & 0x01);
            reg_.a = tmp32;
            break;
        case OP_RRCA:
            reg_.a = (reg_.a >> 1) | (reg_.a << 7);
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (reg_.a & 0x80) != 0);
            break;
        case OP_RL:
            tmp32 = ins->dst << 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x01 : 0x00;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (ins->dst & 0x80) != 0);
            ins->dst = tmp32;
            break;
        case OP_RLC:
            tmp32 = ins->dst << 1;
            tmp32 |= ins->dst >> 7;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (ins->dst & 0x80) != 0);
            ins->dst = tmp32;
            break;
        case OP_RR:
            tmp32 = ins->dst >> 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x80 : 0x00;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, ins->dst & 0x01);
            ins->dst = tmp32;
            break;
        case OP_RRC:
            tmp32 = ins->dst >> 1;
            tmp32 |= ins->dst << 7;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, ins->dst & 0x01);
            ins->dst = tmp32;
            break;
        case OP_SLA:
            tmp32 = ins->dst << 1;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (ins->dst & 0x80) != 0);
            ins->dst = tmp32;
            break;
        case OP_SRA:
            tmp32 = ins->dst >> 1;
            tmp32 |= ins->dst & 0x80;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, ins->dst & 0x01);
            ins->dst = tmp32;
            break;
        case OP_SWAP:
            tmp32 = ((ins->dst & 0x0f) << 4) | ((ins->dst & 0xf0) >> 4);
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.clrf(FLAG_C);
            ins->dst = tmp32;
            break;
        case OP_SRL:
            tmp32 = ins->dst >> 1;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, ins->dst & 0x01);
            ins->dst = tmp32;
            break;
        case OP_BIT:
            tmp8 = 1 << ((ins->bytes[1] >> 3) & 7);
            reg_.setfb(FLAG_Z, !(ins->dst & tmp8));
            reg_.clrf(FLAG_N);
            reg_.setf(FLAG_H);
            break;
        case OP_RES:
            tmp8 = ~(1 << ((ins->bytes[1] >> 3) & 7));
            ins->dst &= tmp8;
            break;
        case OP_SET:
            tmp8 = 1 << ((ins->bytes[1] >> 3) & 7);
            ins->dst |= tmp8;
            break;
        case OP_CPL:
            reg_.a = ~reg_.a;
            reg_.setf(FLAG_N);
            reg_.setf(FLAG_H);
            break;
        case OP_AND:
            reg_.a &= ins->src;
            reg_.setfb(FLAG_Z, reg_.a == 0);
            reg_.clrf(FLAG_N);
            reg_.setf(FLAG_H);
            reg_.clrf(FLAG_C);
            break;
        case OP_XOR:
            reg_.a ^= ins->src;
            reg_.setfb(FLAG_Z, reg_.a == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.clrf(FLAG_C);
            break;
        case OP_OR:
            reg_.a |= ins->src;
            reg_.setfb(FLAG_Z, reg_.a == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.clrf(FLAG_C);
            break;
        case OP_SCF:
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setf(FLAG_C);
            break;
        case OP_CCF:
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, !reg_.getf(FLAG_C));
            break;
        case OP_DI:
            ime_ = false;
            break;
        case OP_EI:
            ime_ = true;
            break;
        case OP_JR:
        case OP_JP:
        case OP_CALL:
        case OP_RET:
        case OP_RETI:
        case OP_RST:
            ins->taken = false;
            switch (ins->cc) {
                case CC_ALWAYS:
                    ins->taken = true;
                    break;
                case CC_C:
                    ins->taken = reg_.getf(FLAG_C);
                    break;
                case CC_NC:
                    ins->taken = !reg_.getf(FLAG_C);
                    break;
                case CC_Z:
                    ins->taken = reg_.getf(FLAG_Z);
                    break;
                case CC_NZ:
                    ins->taken = !reg_.getf(FLAG_Z);
                    break;
            }

            if (ins->taken) {
                busy_ += ins->extra;
                if (ins->op == OP_JP)         { reg_.pc = ins->src; }
                else if (ins->op == OP_JR)    { reg_.pc += i8(ins->src); }
                else if (ins->op == OP_RST) {
                    reg_.sp -= 2;
                    write_u16(m_, reg_.sp, reg_.pc);
                    reg_.pc = ins->bytes[0] & 0x38;
                }
                else if (ins->op == OP_CALL) {
                    reg_.sp -= 2;
                    write_u16(m_, reg_.sp, reg_.pc);
                    reg_.pc = ins->src;
                }
                else if (ins->op == OP_RETI) {
                    ime_ = true;
                    reg_.pc = read_u16(m_, reg_.sp);
                    reg_.sp += 2;
                }
                else if (ins->op == OP_RET) {
                    reg_.pc = read_u16(m_, reg_.sp);
                    reg_.sp += 2;
                }
                else { assert(0); }
            }

            break;
        case OP_HALT:
            halt_ = true;
            break;
        default:
            m_->Log(std::string("CPU::Execute: Op ") + optype_names[ins->op] + " not implemented");
            return false;
    }

    return true;
}

bool CPU::Store(Instruction *ins) {
    switch (ins->dst_am) {
        case AM_NONE:
            return true;
        case AM_IMM8:
        case AM_IMM16:
        case AM_SPIMM8:
            assert(0);
            return false;
        case AM_MIMM16:
            // special case: LD (imm16), SP seems to be the only instruction that
            // requires a 2-byte write
            if (ins->src_am == AM_SP) {
                write_u16(m_, ins->dst_ea, ins->dst);
                return true;
            }
            /* no break */
        case AM_MBC:
        case AM_MDE:
        case AM_MHL:
        case AM_MHLI:
        case AM_MHLD:
        case AM_HMIMM8:
        case AM_HMC:
            m_->Write(ins->dst_ea, ins->dst);
            return true;
        case AM_A:
            reg_.a = ins->dst;
            break;
        case AM_B:
            reg_.b = ins->dst;
            break;
        case AM_C:
            reg_.c = ins->dst;
            break;
        case AM_D:
            reg_.d = ins->dst;
            break;
        case AM_E:
            reg_.e = ins->dst;
            break;
        case AM_H:
            reg_.h = ins->dst;
            break;
        case AM_L:
            reg_.l = ins->dst;
            break;
        case AM_AF:
            reg_.af = ins->dst & 0xFFF0;    // can't set unused flag bits
            break;
        case AM_BC:
            reg_.bc = ins->dst;
            break;
        case AM_DE:
            reg_.de = ins->dst;
            break;
        case AM_HL:
            reg_.hl = ins->dst;
            break;
        case AM_SP:
            reg_.sp = ins->dst;
            break;
        default:
            m_->Log("CPU::Store: AddrMode " + to_string(ins->dst_am) + " not implemented");
            return false;
    }

    return true;
}

bool CPU::Step() {
    u8 pending = ie_ & if_;
    if (pending) { halt_ = false; }
    if (ime_ && pending) {
        reg_.sp -= 2;
        write_u16(m_, reg_.sp, reg_.pc);
        busy_ += 4;
        halt_ = false;

        ime_ = false;
        if (pending & 0x01)         { if_ &= ~0x01; reg_.pc = 0x0040; }
        else if (pending & 0x02)    { if_ &= ~0x02; reg_.pc = 0x0048; }
        else if (pending & 0x04)    { if_ &= ~0x04; reg_.pc = 0x0050; }
        else if (pending & 0x08)    { if_ &= ~0x08; reg_.pc = 0x0058; }
        else if (pending & 0x10)    { if_ &= ~0x10; reg_.pc = 0x0060; }
        else                        { assert(0); }

        return true;
    }

    if (halt_) { busy_ = 4; return true; }

    Instruction ins;

    bool good = FetchInstruction(&ins);
    if (!good) { return false; }

    good = Decode(&ins);
    if (!good) { return false; }
    if (ins.op == OP_UD) {
        m_->Log("Undefined opcode (" + to_hex(ins.bytes[0], 2) + ")");
        return false;
    }

    good = FetchOperands(&ins);
    if (!good) { return false; }

    good = Execute(&ins);
    if (!good) { return false; }

    good = Store(&ins);
    if (!good) { return false; }

#ifdef DEBUG
    std::string log = to_string(ins);
    while (log.length() < 60) { log += " "; }
    log += to_string(reg_);

    m_->Log(log);
#endif

    return true;
}

CPU::CPU(Machine *machine)
    : busy_(0), if_(0), ie_(0), ime_(false), halt_(false), m_(machine)
{}

bool CPU::Tick() {
    bool good = true;
    if (!busy_) { good = Step(); }

    assert(!good || busy_);
    busy_ -= 4;

    return good;
}

void CPU::Interrupt(u8 num) {
    assert(!(num & ~0x1F));
    if_ |= num;
}

}    // namespace gb1
