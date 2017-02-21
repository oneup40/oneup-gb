#pragma once

// Copyright 2017 oneup

#include "core/Base.hpp"

namespace gb1 {

class Timer;

class TimerObserver {
public:
    virtual void FulldivUpdate(const Timer& /* timer */, u32 /* new_fulldiv */) {}
    virtual void TimaUpdate(const Timer& /* timer */, u8 /* new_tima */) {}

    virtual ~TimerObserver() {}
};

}   // namespace gb1
