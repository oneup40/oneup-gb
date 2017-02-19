#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"
#include "core/io/IOObserver.hpp"

namespace gb1 {

struct Machine;

class IO {
    Machine *m_;
    IOObserver *obs_;
public:
    explicit IO(Machine *m, IOObserver *observer = nullptr) : m_(m), obs_(observer) {}
    u8 Read(u16 addr, bool force = false);
    void Write(u16 addr, u8 val, bool force = false);
};

}   // namespace gb1
