#pragma once

// Copyright 2016-2017 oneup

#include "core/Base.hpp"
#include "core/Machine.hpp"

namespace gb1 {

struct Frontend {
    virtual Button InputJoypad() = 0;
    virtual void OutputAudioFrame(i16 left, i16 right) = 0;
    virtual void OutputVideoFrame(std::array<std::array<u8, 40>, 144>&& frame) = 0;

    virtual bool InputSerial() = 0;
    virtual void OutputSerial(bool bit) = 0;

    virtual ~Frontend() {}
};

}    // namespace gb1
