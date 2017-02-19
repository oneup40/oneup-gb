#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"

namespace gb1 {

struct Machine;

class IOObserver {
public:
    virtual void Read(const Machine& /* m */, u16 /* addr */, u8 /* val */, bool /* force */) {}
    virtual void Write(const Machine& /* m */, u16 /* addr */, u8 /* val */, bool /* force */) {}

    virtual ~IOObserver() {}
};

}   // namespace gb1
