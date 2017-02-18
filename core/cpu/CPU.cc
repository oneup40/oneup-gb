// Copyright 2016-2017 oneup

#include "core/cpu/CPU.hpp"
#include "core/Machine.hpp"

namespace gb1 {

bool CPU::FetchInstruction(Instruction *ins) {
    ins->pc = reg_.pc++;
    ins->bytes[ins->nbytes++] = m_->Read(ins->pc);

    if (ins->bytes[0] == 0xcb) {
        ins->bytes[ins->nbytes++] = m_->Read(reg_.pc++);
    }

    if (obs_) { obs_->FetchInstruction(*this, ins->pc, ins->bytes, ins->nbytes); }
    return true;
}

bool CPU::Decode(Instruction *ins) {
    assert(ins->nbytes > 0);

    auto op = &op_table[ins->bytes[0]];
    if (op->op == OP_CB) {
        op = &op_cb_table[ins->bytes[1]];
    }

    ins->op = op->op;
    ins->dst.am = op->dst;
    ins->src.am = op->src;
    ins->cc = op->cc;
    ins->cycles = op->cycles;
    ins->extra = op->extra;

    if (obs_) { obs_->Decode(*this, *op); }
    return true;
}

bool CPU::FetchOperand(Instruction *ins, Operand *op) {
    switch (op->am) {
        case AM_NONE:
            return true;
        case AM_IMM8:
        case AM_SPIMM8:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            ++reg_.pc;
            op->val = *ins->byte_ptr();
            ++ins->nbytes;
            return true;
        case AM_IMM16:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            *(ins->byte_ptr() + 1) = m_->Read(reg_.pc + 1);
            reg_.pc += 2;
            op->val = letoh(*reinterpret_cast<u16*>(ins->byte_ptr()));
            ins->nbytes += 2;
            return true;
        case AM_MIMM16:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            *(ins->byte_ptr() + 1) = m_->Read(reg_.pc + 1);
            reg_.pc += 2;
            op->ea = letoh(*reinterpret_cast<u16*>(ins->byte_ptr()));
            ins->nbytes += 2;
            op->val = m_->Read(op->ea);
            return true;
        case AM_A:
            op->val = reg_.a;
            return true;
        case AM_B:
            op->val = reg_.b;
            return true;
        case AM_C:
            op->val = reg_.c;
            return true;
        case AM_D:
            op->val = reg_.d;
            return true;
        case AM_E:
            op->val = reg_.e;
            return true;
        case AM_H:
            op->val = reg_.h;
            return true;
        case AM_L:
            op->val = reg_.l;
            return true;
        case AM_AF:
            op->val = reg_.af;
            return true;
        case AM_BC:
            op->val = reg_.bc;
            return true;
        case AM_DE:
            op->val = reg_.de;
            return true;
        case AM_HL:
            op->val = reg_.hl;
            return true;
        case AM_SP:
            op->val = reg_.sp;
            return true;
        case AM_MBC:
            op->ea = reg_.bc;
            op->val = m_->Read(op->ea);
            return true;
        case AM_MDE:
            op->ea = reg_.de;
            op->val = m_->Read(op->ea);
            return true;
        case AM_MHL:
            op->ea = reg_.hl;
            op->val = m_->Read(op->ea);
            return true;
        case AM_MHLI:
            op->ea = reg_.hl++;
            op->val = m_->Read(op->ea);
            return true;
        case AM_MHLD:
            op->ea = reg_.hl--;
            op->val = m_->Read(op->ea);
            return true;
        case AM_HMIMM8:
            *ins->byte_ptr() = m_->Read(reg_.pc);
            ++reg_.pc;
            op->ea = 0xff00 | *ins->byte_ptr();
            ++ins->nbytes;
            op->val = m_->Read(op->ea);
            return true;
        case AM_HMC:
            op->ea = 0xff00 | reg_.c;
            op->val = m_->Read(op->ea);
            return true;
        default:
            m_->Log("CPU::FetchOperand: AddrMode " + to_string(op->am) + " not implemented");
            return false;
    }
}

bool CPU::FetchOperands(Instruction *ins) {
    bool good = FetchOperand(ins, &ins->src);
    if (!good) { return false; }
    if (obs_) { obs_->FetchSourceOperand(*this, ins->src); }

    good = FetchOperand(ins, &ins->dst);
    if (good && obs_) { obs_->FetchDestinationOperand(*this, ins->dst); }

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
    if (obs_) { obs_->BeforeExecute(*this, ins->op, ins->src, ins->dst); }

    busy_ += ins->cycles;

    u8 tmp8 = 0;
    u32 tmp32 = 0;

    u32 &src = ins->src.val,
        &dst = ins->dst.val;

    switch (ins->op) {
        case OP_NOP:
            break;
        case OP_LD:
            dst = src;
            break;
        case OP_PUSH:
            reg_.sp -= 2;
            write_u16(m_, reg_.sp, src);
            break;
        case OP_POP:
            dst = read_u16(m_, reg_.sp);
            reg_.sp += 2;
            break;
        case OP_INC8:
            tmp32 = dst;
            ++dst;
            reg_.setfb(FLAG_Z, (dst & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_H, (dst & 0xf0) != (tmp32 & 0xf0));
            break;
        case OP_INC16:
            ++dst;
            break;
        case OP_DEC8:
            tmp32 = dst;
            --dst;
            reg_.setfb(FLAG_Z, (dst & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (dst & 0xf0) != (tmp32 & 0xf0));
            break;
        case OP_DEC16:
            --dst;
            break;
        case OP_ADC:
            tmp8 = reg_.getf(FLAG_C);
            /* no break */
        case OP_ADD8:
            tmp32 = dst;
            dst = dst + src + tmp8;
            reg_.setfb(FLAG_Z, (dst & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_H, (tmp32 & 0xf) + (src & 0xf) + tmp8 >= 0x10);
            reg_.setfb(FLAG_C, dst >= 0x100);
            break;
        case OP_ADD16:
            reg_.setfb(FLAG_H, (dst & 0xfff) + (src & 0xfff) > 0xfff);
            dst += src;
            reg_.clrf(FLAG_N);
            reg_.setfb(FLAG_C, dst >= 0x10000);
            break;
        case OP_ADDSP:
            // sign extend
            src |= (src & 0x80) ? 0xff00 : 0x0000;
            reg_.setfb(FLAG_H, (reg_.sp & 0xf) + (src & 0xf) > 0xf);
            reg_.setfb(FLAG_C, (reg_.sp & 0xff) + (src & 0xff) > 0xff);
            dst = reg_.sp + i8(src);
            reg_.clrf(FLAG_Z);
            reg_.clrf(FLAG_N);
            break;
        case OP_CP:
        case OP_SUB:
            tmp32 = reg_.a - src;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (reg_.a & 0xf) - (src & 0xf) >= 0x10);    // remember, unsigned arithmetic
            reg_.setfb(FLAG_C, tmp32 >= 0x100);
            if (ins->op == OP_SUB) { reg_.a = tmp32; }
            break;
        case OP_SBC:
            tmp8 = reg_.getf(FLAG_C);
            tmp32 = dst;
            dst = dst - src - tmp8;
            reg_.setfb(FLAG_Z, (dst & 0xff) == 0);
            reg_.setf(FLAG_N);
            reg_.setfb(FLAG_H, (tmp32 & 0xf) - (src & 0xf) - tmp8 >= 0x10);    // remember, unsigned arithmetic
            reg_.setfb(FLAG_C, dst >= 0x100);
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
            tmp32 = dst << 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x01 : 0x00;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (dst & 0x80) != 0);
            dst = tmp32;
            break;
        case OP_RLC:
            tmp32 = dst << 1;
            tmp32 |= dst >> 7;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (dst & 0x80) != 0);
            dst = tmp32;
            break;
        case OP_RR:
            tmp32 = dst >> 1;
            tmp32 |= reg_.getf(FLAG_C) ? 0x80 : 0x00;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, dst & 0x01);
            dst = tmp32;
            break;
        case OP_RRC:
            tmp32 = dst >> 1;
            tmp32 |= dst << 7;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, dst & 0x01);
            dst = tmp32;
            break;
        case OP_SLA:
            tmp32 = dst << 1;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, (dst & 0x80) != 0);
            dst = tmp32;
            break;
        case OP_SRA:
            tmp32 = dst >> 1;
            tmp32 |= dst & 0x80;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, dst & 0x01);
            dst = tmp32;
            break;
        case OP_SWAP:
            tmp32 = ((dst & 0x0f) << 4) | ((dst & 0xf0) >> 4);
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.clrf(FLAG_C);
            dst = tmp32;
            break;
        case OP_SRL:
            tmp32 = dst >> 1;
            reg_.setfb(FLAG_Z, (tmp32 & 0xff) == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.setfb(FLAG_C, dst & 0x01);
            dst = tmp32;
            break;
        case OP_BIT:
            tmp8 = 1 << ((ins->bytes[1] >> 3) & 7);
            reg_.setfb(FLAG_Z, !(dst & tmp8));
            reg_.clrf(FLAG_N);
            reg_.setf(FLAG_H);
            break;
        case OP_RES:
            tmp8 = ~(1 << ((ins->bytes[1] >> 3) & 7));
            dst &= tmp8;
            break;
        case OP_SET:
            tmp8 = 1 << ((ins->bytes[1] >> 3) & 7);
            dst |= tmp8;
            break;
        case OP_CPL:
            reg_.a = ~reg_.a;
            reg_.setf(FLAG_N);
            reg_.setf(FLAG_H);
            break;
        case OP_AND:
            reg_.a &= src;
            reg_.setfb(FLAG_Z, reg_.a == 0);
            reg_.clrf(FLAG_N);
            reg_.setf(FLAG_H);
            reg_.clrf(FLAG_C);
            break;
        case OP_XOR:
            reg_.a ^= src;
            reg_.setfb(FLAG_Z, reg_.a == 0);
            reg_.clrf(FLAG_N);
            reg_.clrf(FLAG_H);
            reg_.clrf(FLAG_C);
            break;
        case OP_OR:
            reg_.a |= src;
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
                if (ins->op == OP_JP)         { reg_.pc = src; }
                else if (ins->op == OP_JR)    { reg_.pc += i8(src); }
                else if (ins->op == OP_RST) {
                    reg_.sp -= 2;
                    write_u16(m_, reg_.sp, reg_.pc);
                    reg_.pc = ins->bytes[0] & 0x38;
                }
                else if (ins->op == OP_CALL) {
                    reg_.sp -= 2;
                    write_u16(m_, reg_.sp, reg_.pc);
                    reg_.pc = src;
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
            m_->Log(std::string("CPU::Execute: Op ") + to_string(ins->op) + " not implemented");
            return false;
    }

    if (obs_) { obs_->AfterExecute(*this, ins->op, ins->src, ins->dst); }
    return true;
}

bool CPU::Store(Instruction *ins) {
    switch (ins->dst.am) {
        case AM_NONE:
            break;
        case AM_IMM8:
        case AM_IMM16:
        case AM_SPIMM8:
            assert(0);
            return false;
        case AM_MIMM16:
            // special case: LD (imm16), SP seems to be the only instruction that
            // requires a 2-byte write
            if (ins->src.am == AM_SP) {
                write_u16(m_, ins->dst.ea, ins->dst.val);
                break;
            }
            /* no break */
        case AM_MBC:
        case AM_MDE:
        case AM_MHL:
        case AM_MHLI:
        case AM_MHLD:
        case AM_HMIMM8:
        case AM_HMC:
            m_->Write(ins->dst.ea, ins->dst.val);
            break;
        case AM_A:
            reg_.a = ins->dst.val;
            break;
        case AM_B:
            reg_.b = ins->dst.val;
            break;
        case AM_C:
            reg_.c = ins->dst.val;
            break;
        case AM_D:
            reg_.d = ins->dst.val;
            break;
        case AM_E:
            reg_.e = ins->dst.val;
            break;
        case AM_H:
            reg_.h = ins->dst.val;
            break;
        case AM_L:
            reg_.l = ins->dst.val;
            break;
        case AM_AF:
            reg_.af = ins->dst.val & 0xFFF0;    // can't set unused flag bits
            break;
        case AM_BC:
            reg_.bc = ins->dst.val;
            break;
        case AM_DE:
            reg_.de = ins->dst.val;
            break;
        case AM_HL:
            reg_.hl = ins->dst.val;
            break;
        case AM_SP:
            reg_.sp = ins->dst.val;
            break;
        default:
            m_->Log("CPU::Store: AddrMode " + to_string(ins->dst.am) + " not implemented");
            return false;
    }

    if (obs_) { obs_->Store(*this, ins->dst); }
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
        u8 num = 0;

        if (pending & 0x01)         { num = 0x01; reg_.pc = 0x0040; }
        else if (pending & 0x02)    { num = 0x02; reg_.pc = 0x0048; }
        else if (pending & 0x04)    { num = 0x04; reg_.pc = 0x0050; }
        else if (pending & 0x08)    { num = 0x08; reg_.pc = 0x0058; }
        else if (pending & 0x10)    { num = 0x10; reg_.pc = 0x0060; }
        else                        { assert(0); }

        if_ &= ~num;

        if (obs_) { obs_->ServiceInterrupt(*this, num); }
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

#ifdef DEBUG_CPU
    std::string log = to_string(ins);
    while (log.length() < 60) { log += " "; }
    log += to_string(reg_);

    m_->Log(log);
#endif

    return true;
}

CPU::CPU(Machine *machine, CPUObserver *observer)
    : busy_(0), if_(0), ie_(0), ime_(false), halt_(false),
      obs_(observer),
      m_(machine)
{}

bool CPU::Tick() {
    if (obs_) { obs_->StartTick(*this); }

    bool good = true;
    if (!busy_) { good = Step(); }

    assert(!good || busy_);
    busy_ -= 4;

    return good;
}

void CPU::Interrupt(u8 num) {
    assert(!(num & ~0x1F));
    if_ |= num;
    if (obs_) { obs_->RaiseInterrupt(*this, num); }
}

}    // namespace gb1
