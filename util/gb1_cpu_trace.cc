// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/timer/TimerObserver.hpp"
#include "frontend/sdl/SDLFrontend.hpp"

class TraceObserver
        : public gb1::CPUObserver,
          public gb1::TimerObserver
{
public:
    void AfterExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) override {
        std::string log = to_string(ins);
        while (log.length() < 60) { log += " "; }
        log += to_string(cpu.GetRegs());

        std::cout << log << std::endl;
    }

    void RaiseInterrupt(const gb1::CPU&, gb1::u8 num) override {
        std::cout << "CPU: INT $" << gb1::to_hex(num, 2) << " requested" << std::endl;
    }

    void FulldivUpdate(const gb1::Timer&, gb1::u32 new_fulldiv) override {
        std::cout << "TIMER: fulldiv update -> $" << gb1::to_hex(new_fulldiv, 8) << std::endl;
    }

    void TimaUpdate(const gb1::Timer&, gb1::u8 new_tima) override {
        std::cout << "TIMER: TIMA update -> $" << gb1::to_hex(new_tima, 2) << std::endl;
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: gb1-cpu-trace <rom>" << std::endl;
        return 1;
    }

    TraceObserver trace_observer;
    gb1::MachineObserver m_observer;
    m_observer.cpu = &trace_observer;
    m_observer.timer = &trace_observer;
    gb1::sdl::SDLFrontend frontend(m_observer);

    if (!frontend.LoadGame(argv[1])) {
        std::cerr << "error loading ROM" << std::endl;
        return 1;
    }

    return frontend.Run();
}
