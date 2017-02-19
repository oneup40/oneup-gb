#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"

namespace gb1 {

class IO;

class IOObserver {
public:
    virtual void Read(const IO& /* io */, u16 /* addr */, u8 /* val */, bool /* force */) {}
    virtual void Write(const IO& /* io */, u16 /* addr */, u8 /* val */, bool /* force */) {}

    virtual ~IOObserver() {}
};

}   // namespace gb1
