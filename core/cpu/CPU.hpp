#pragma once

// Copyright 2016-2017 oneup

#include <cassert>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/cpu/Instruction.hpp"
#include "core/cpu/Opcode.hpp"
#include "core/cpu/Regs.hpp"
#include "core/Serializer.hpp"

namespace gb1 {

struct Machine;

// Why does C++ make this so difficult?
// The operator<</>> functions need access to the CPU class members so
//   the definitions must come after the class definition.
// They need access to private CPU class members, so they need friend
//   declarations in the CPU class.
// Friend function declarations can't specify a storage class, so the functions
//   need to be declared again before the CPU class definition so the storage
//   class can be specified.
// They have arguments of type CPU, so CPU must be forward declared.

class CPU;

static inline Serializer& operator<<(Serializer &s, const CPU &cpu);
static inline Deserializer& operator>>(Deserializer &d, CPU &cpu);

class CPU {
    size_t busy_;
    Regs reg_;
    u16 if_, ie_;
    bool ime_, halt_;

    Machine *m_;
    CPUObserver *obs_;

    bool FetchInstruction(Instruction *ins);
    bool Decode(Instruction *ins);
    bool FetchOperand(Instruction *ins, Operand *op);
    bool FetchOperands(Instruction *ins);
    bool Execute(Instruction *ins);
    bool Store(Instruction *ins);

    bool Step();

    static constexpr const u8 version_ = 0x00;
    static constexpr const u64 code_ = eight_cc(version_,'c','p','u');
    friend Serializer& operator<<(Serializer &s, const CPU &cpu);
    friend Deserializer& operator>>(Deserializer &d, CPU &cpu);
public:
    explicit CPU(Machine *machine, CPUObserver *observer = nullptr);
    CPU(const CPU&) = delete;
    CPU(CPU&&) = delete;
    CPU& operator=(const CPU&) = delete;
    CPU& operator=(CPU&&) = delete;

    bool Tick();
    void Interrupt(u8 num);

    u8 ReadIF(bool /* force */ = false) const { return if_ & 0xFF; }
    void WriteIF(u8 val, bool force = false) { if_ = val & (force ? 0xFF : 0x1F); }

    u8 ReadIE(bool /* force */ = false) const { return ie_ & 0xFF; }
    void WriteIE(u8 val, bool force = false) { ie_ = val & (force ? 0xFF : 0x1F); }

    const Regs& GetRegs() const { return reg_; }
};

static inline Serializer& operator<<(Serializer &s, const CPU &cpu) {
    s.Start(CPU::code_);
    return s << cpu.busy_ << cpu.reg_ << cpu.if_ << cpu.ie_ << cpu.ime_ << cpu.halt_;
}

static inline Deserializer& operator>>(Deserializer &d, CPU &cpu) {
    d.Start(CPU::code_);
    return d >> cpu.busy_ >> cpu.reg_ >> cpu.if_ >> cpu.ie_ >> cpu.ime_ >> cpu.halt_;
}

} // namespace gb1
