#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/cpu/Instruction.hpp"
#include "core/cpu/Opcode.hpp"
#include "core/cpu/Operand.hpp"
#include "core/cpu/Optype.hpp"

namespace gb1 {

class CPU;

class CPUObserver {
public:
    virtual void StartTick(const CPU& /* cpu */) {}
    virtual void FetchInstruction(const CPU& /* cpu */, u16 /* pc */, u8* /* bytes */, size_t /* nbytes */) {}
    virtual void Decode(const CPU& /* cpu */, const Opcode& /* op */) {}
    virtual void FetchSourceOperand(const CPU& /* cpu */, const Operand& /* op */) {}
    virtual void FetchDestinationOperand(const CPU& /* cpu */, const Operand& /* op */) {}
    virtual void BeforeExecute(const CPU& /* cpu */, const Instruction& /* ins */) {}
    virtual void AfterExecute(const CPU& /* cpu */, const Instruction& /* ins */) {}
    virtual void Store(const CPU& /* cpu */, const Operand& /* dst */) {}

    virtual void RaiseInterrupt(const CPU& /* cpu */, u8 /* num */) {}
    virtual void ServiceInterrupt(const CPU& /* cpu */, u8 /* num */) {}

    virtual ~CPUObserver() {}
};

}   // namespace gb1
