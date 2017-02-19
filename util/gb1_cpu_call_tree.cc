// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/Frontend.hpp"
#include "core/io/IOObserver.hpp"

#include "frontend/sdl/SDLFrontend.hpp"

class DummyFrontend : public gb1::Frontend {
public:
    gb1::Button InputJoypad() override { return gb1::Button(0); }
    void OutputAudioFrame(gb1::i16, gb1::i16) override {}
    void OutputVideoFrame(std::array<std::array<gb1::u8, 40>, 144>&&) override {}
};

class CallTreeObserver : public gb1::CPUObserver, public gb1::IOObserver {
    int indent_ = 0;

    void PrintIndent() {
        for (auto i=0; i < indent_; ++i) { std::cerr << "  "; }
    }
public:
    void AfterExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) override {
        if (ins.op == gb1::OP_CALL && ins.taken) {
            PrintIndent();
            std::cerr << gb1::to_hex(cpu.GetRegs().pc, 4) << std::endl;
            ++indent_;
        } else if (ins.op == gb1::OP_RET && ins.taken) {
            --indent_;
        }
    }

    void Read(const gb1::IO&, gb1::u16 addr, gb1::u8 val, bool) override {
        if ((addr & 0xFF00) == 0xFF00) {
            PrintIndent();
            std::cerr << "Read $(" << gb1::to_hex(addr, 4) << ") -> $" << gb1::to_hex(val, 2) << std::endl;
        }
    }

    void Write(const gb1::IO&, gb1::u16 addr, gb1::u8 val, bool) override {
        if ((addr & 0xFF00) == 0xFF00) {
            PrintIndent();
            std::cerr << "Write $(" << gb1::to_hex(addr, 4) << ") = $" << gb1::to_hex(val, 2) << std::endl;
        }
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: gb1-cpu-call-tree <rom>" << std::endl;
        return 1;
    }

    CallTreeObserver observer;
    gb1::MachineObserver m_observer;
    m_observer.cpu = &observer;
    m_observer.io = &observer;
    gb1::sdl::SDLFrontend frontend(m_observer);

    if (!frontend.LoadGame(argv[1])) { return 1; }

    return frontend.Run();
}
