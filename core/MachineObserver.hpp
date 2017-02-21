#pragma once

// Copyright 2017 oneup

namespace gb1 {

class CPUObserver;
class IOObserver;
class TimerObserver;

struct MachineObserver {
    CPUObserver *cpu = nullptr;
    IOObserver *io = nullptr;
    TimerObserver *timer = nullptr;

    MachineObserver() {}
    MachineObserver(const MachineObserver&) = delete;
    MachineObserver(MachineObserver&&) = delete;
    MachineObserver& operator=(const MachineObserver&) = delete;
    MachineObserver& operator=(MachineObserver&&) = delete;
};

}   // namespace gb1
