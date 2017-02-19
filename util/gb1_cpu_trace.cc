// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "frontend/sdl/SDLFrontend.hpp"

class TraceObserver : public gb1::CPUObserver {
public:
    void AfterExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) {
        std::string log = to_string(ins);
        while (log.length() < 60) { log += " "; }
        log += to_string(cpu.GetRegs());

        std::cout << log << std::endl;
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
    gb1::sdl::SDLFrontend frontend(m_observer);

    if (!frontend.LoadGame(argv[1])) {
        std::cerr << "error loading ROM" << std::endl;
        return 1;
    }

    return frontend.Run();
}
