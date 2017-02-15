#pragma once

// Copyright 2016-2017 oneup

#include <iostream>
#include <vector>

#include "core/Base.hpp"

namespace gb1 {

struct Machine;

class Loader {
public:
    static bool Load(std::vector<u8>&& data, Machine *m);
};

}    // namespace gb1
