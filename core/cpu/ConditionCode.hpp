#pragma once

// Copyright 2017 oneup

#include <string>

#include <cassert>

#include "core/Base.hpp"

namespace gb1 {

enum ConditionCode : u8 {
    CC_ALWAYS,
    CC_NZ, CC_Z,
    CC_NC, CC_C
};

static inline std::string to_string(ConditionCode cc) {
    switch (cc) {
        case CC_ALWAYS: return "";
        case CC_NZ:     return "NZ";
        case CC_Z:      return "Z";
        case CC_NC:     return "NC";
        case CC_C:      return "C";
        default:
            assert(0);
            return "???";
    }
}

}   // namespace gb1
