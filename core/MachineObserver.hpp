#pragma once

// Copyright 2017 oneup

namespace gb1 {

class CPUObserver;
class IOObserver;

struct MachineObserver {
    CPUObserver *cpu;
    IOObserver *io;

    MachineObserver() : cpu(nullptr), io(nullptr) {}
    MachineObserver(const MachineObserver&) = delete;
    MachineObserver(MachineObserver&&) = delete;
    MachineObserver& operator=(const MachineObserver&) = delete;
    MachineObserver& operator=(MachineObserver&&) = delete;
};

}   // namespace gb1
