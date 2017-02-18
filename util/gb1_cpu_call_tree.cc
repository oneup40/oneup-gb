// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/Frontend.hpp"

class DummyFrontend : public gb1::Frontend {
public:
    gb1::Button InputJoypad() override { return gb1::Button(0); }
    void OutputAudioFrame(gb1::i16, gb1::i16) override {}
    void OutputVideoFrame(std::array<std::array<gb1::u8, 40>, 144>&&) override {}
};

class CallTreeObserver : public gb1::CPUObserver {
    int indent_ = 0;

    void PrintIndent() {
        for (auto i=0; i < indent_; ++i) { std::cerr << "  "; }
    }
public:
    void AfterExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) {
        if (ins.op == gb1::OP_CALL && ins.taken) {
            PrintIndent();
            std::cerr << gb1::to_hex(cpu.GetRegs().pc, 4) << std::endl;
            ++indent_;
        } else if (ins.op == gb1::OP_RET && ins.taken) {
            --indent_;
        }
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: gb1-cpu-call-tree <rom>" << std::endl;
        return 1;
    }

    DummyFrontend frontend;
    CallTreeObserver observer;
    gb1::Machine m(frontend, &observer);

    std::ifstream is(argv[1], std::ios::binary | std::ios::in);
    if (!is) { return false; }

    is.seekg(0, std::ios::end);
    auto size = is.tellg();
    is.seekg(0);

    std::vector<gb1::u8> data(size);
    is.read(reinterpret_cast<char*>(data.data()), data.size());
    if (!is) { return false; }

    if (!m.LoadGame(std::move(data))) {
        std::cerr << "error loading ROM" << std::endl;
        return 1;
    }

    while (true) { m.Tick(); }
    return 0;
}
