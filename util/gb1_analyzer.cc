// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "core/Base.hpp"
#include "core/cpu/CPUObserver.hpp"
#include "core/Frontend.hpp"
#include "core/io/IOObserver.hpp"

#include "frontend/sdl/SDLFrontend.hpp"

struct BlockInfo {
    gb1::u16 end_addr;
    std::set<gb1::u16> read_addrs, write_addrs;
    std::map<gb1::u16, int> exit_counts;

    BlockInfo() : end_addr(0) {}
};

struct Block {
    gb1::u16 start_addr;
    std::unique_ptr<BlockInfo> info;

    explicit Block(gb1::u16 start_addr)
        : start_addr(start_addr), info(std::make_unique<BlockInfo>())
    {}

    bool operator<(const Block &rhs) const {
        return start_addr < rhs.start_addr;
    }
};

class AnalyzerObserver : public gb1::CPUObserver, public gb1::IOObserver {
    gb1::u16 start_addr_;
    std::set<Block> blocks_;

    const Block& GetBlock() {
        Block b(start_addr_);
        auto p = blocks_.insert(std::move(b));
        return *(p.first);
    }
public:
    AnalyzerObserver(gb1::u16 start_addr) : start_addr_(start_addr) {}

    void BeforeExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) override {
        switch (ins.op) {
            case gb1::OP_JR:
            case gb1::OP_JP:
            case gb1::OP_CALL:
            case gb1::OP_RET:
            case gb1::OP_RETI:
            case gb1::OP_RST:
                GetBlock().info->end_addr = cpu.GetRegs().pc;
                break;
            default:
                break;
        }
    }

    void AfterExecute(const gb1::CPU &cpu, const gb1::Instruction &ins) override {
        switch (ins.op) {
            case gb1::OP_JR:
            case gb1::OP_JP:
            case gb1::OP_CALL:
            case gb1::OP_RET:
            case gb1::OP_RETI:
            case gb1::OP_RST:
                GetBlock().info->exit_counts[cpu.GetRegs().pc] += 1;
                start_addr_ = cpu.GetRegs().pc;
                break;
            default:
                break;
        }
    }

    void Read(const gb1::IO&, gb1::u16 addr, gb1::u8, bool) override {
        if ((addr & 0xFF00) == 0xFF00) {
            GetBlock().info->read_addrs.insert(addr);
        }
    }

    void Write(const gb1::IO&, gb1::u16 addr, gb1::u8, bool) override {
        if ((addr & 0xFF00) == 0xFF00) {
            GetBlock().info->write_addrs.insert(addr);
        }
    }

    decltype(blocks_.begin()) begin() { return blocks_.begin(); }
    decltype(blocks_.end()) end() { return blocks_.end(); }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: gb1-analyzer <rom>" << std::endl;
        return 1;
    }

    AnalyzerObserver observer(0x100);
    gb1::MachineObserver m_observer;
    m_observer.cpu = &observer;
    m_observer.io = &observer;
    gb1::sdl::SDLFrontend frontend(m_observer);

    if (!frontend.LoadGame(argv[1])) { return 1; }

    frontend.Run();

    std::cout << "strict digraph flowgraph {" << std::endl;
    for (const auto &block : observer) {
        std::string label = "\"";
        for (auto read_addr : block.info->read_addrs) {
            label += "R";
            label += gb1::to_hex(read_addr, 4);
            label += ", ";
        }
        for (auto read_addr : block.info->read_addrs) {
            label += "W";
            label += gb1::to_hex(read_addr, 4);
            label += ", ";
        }
        if (label.size() > 2) {
            label.pop_back();
            label.pop_back();
        }
        label += "\"";

        std::cout << "  n" << gb1::to_hex(block.start_addr, 4) << "[xlabel=" << label << "];" << std::endl;
        for (auto p : block.info->exit_counts) {
            std::cout << "  n" << gb1::to_hex(block.start_addr, 4)
                      << " -> n" << gb1::to_hex(p.first, 4)
                      << "[weight=" << p.second << ",label=" << p.second << "]"
                      << ";" << std::endl;
        }
    }
    std::cout << "}" << std::endl;
}
