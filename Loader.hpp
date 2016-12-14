#pragma once

// Copyright 2016 oneup

#include <iostream>

#include "libretro.h"

namespace gblr {

struct Machine;

class Loader {
public:
    static bool Load(const struct retro_game_info *game, Machine *m);
};

}    // namespace gblr
