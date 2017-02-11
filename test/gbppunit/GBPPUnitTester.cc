// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "core/Base.hpp"
#include "core/Frontend.hpp"
#include "core/Machine.hpp"

class GBPPunitTesterFrontend : public gb1::Frontend {
    std::array<std::array<gb1::u8, 40>, 144> last_frame_;
public:
    GBPPunitTesterFrontend() {
        for (auto &line : last_frame_) { line.fill(0); }
    }
    GBPPunitTesterFrontend(const GBPPunitTesterFrontend&) = delete;
    GBPPunitTesterFrontend(GBPPunitTesterFrontend&&) = delete;
    GBPPunitTesterFrontend& operator=(const GBPPunitTesterFrontend&) = delete;
    GBPPunitTesterFrontend& operator=(GBPPunitTesterFrontend&&) = delete;

    gb1::Button InputJoypad() override { return gb1::Button(0); }
    void OutputAudioFrame(gb1::i16, gb1::i16) override {}
    void OutputVideoFrame(std::array<std::array<gb1::u8, 40>, 144>&& frame) override {
        last_frame_ = std::move(frame);
    }

    const std::array<std::array<gb1::u8, 40>, 144>& GetLastFrame() const { return last_frame_; }
};

static bool LoadGame(const std::string &path, gb1::Machine &m) {
    std::ifstream is(path, std::ios::binary | std::ios::in);
    if (!is) { return false; }

    is.seekg(0, std::ios::end);
    auto size = is.tellg();
    is.seekg(0);

    std::vector<gb1::u8> data(size);
    is.read(reinterpret_cast<char*>(data.data()), data.size());
    if (!is) { return false; }

    return m.LoadGame(std::move(data));
}

static void Usage(const std::string &progname) {
    std::cerr << "usage: " << progname << " test-rom-path expected-output-path" << std::endl;
}

struct Args {
    std::string rom_path, exp_path;
};

static bool ParseArgs(int argc, char **argv, Args &args) {
    if (!argc) { Usage("gbppunit-tester"); return false; }
    else if (argc != 3) { Usage(argv[0]); return false; }

    args.rom_path = argv[1];
    args.exp_path = argv[2];

    return true;
}

static bool LoadExpectedOutput(
    const std::string &path,
    std::array<std::array<gb1::u8, 40>, 144> &expected)
{
    std::ifstream fin(path);
    for (auto &line : expected) {
        line.fill(0);

        std::array<char, 320> data;
        if (!fin.read(data.data(), data.size())) {
            return false;
        }

        auto j=0, shift=0;
        for (auto i=0u; i < data.size(); i += 2) {
            gb1::u8 dot = 0;
            switch (data[i]) {
                case ' ': dot = 0x00; break;
                case '.': dot = 0x01; break;
                case ':': dot = 0x02; break;
                case 'O': dot = 0x03; break;
                default: return false;
            }

            line[j] |= (dot << shift);

            shift += 2;
            if (shift >= 8) {
                ++j;
                shift = 0;
            }
        }

        char newline = 0;
        if (!fin.read(&newline, 1) || newline != '\n') {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    Args args;
    if (!ParseArgs(argc, argv, args)) { return 1; }

    GBPPunitTesterFrontend frontend;
    gb1::Machine m(frontend);

    if (!LoadGame(args.rom_path, m)) {
        std::cerr << "error loading game" << std::endl;
        return 1;
    }

    std::array<std::array<gb1::u8, 40>, 144> expected;
    if (!LoadExpectedOutput(args.exp_path, expected)) {
        std::cerr << "error loading expected output" << std::endl;
        return 1;
    }

    while (m.cpu.GetRegs().pc != 0x7FF0 && m.cpu.GetRegs().pc != 0x7FF8) {
        m.Tick();
    }

    if (m.cpu.GetRegs().pc == 0x7FF0) {
        std::cerr << "test failed" << std::endl;
        return 1;
    }

    /*
    std::array<char, 4> palette{{' ','.',':','O'}};
    for (const auto &line : frontend.GetLastFrame()) {
        for (auto dot : line) {
            for (auto i = 0u; i < 4; ++i) {
                std::cout << palette[dot & 0x03];
                std::cout << palette[dot & 0x03];
                dot >>= 2;
            }
        }
        std::cout << std::endl;
    }
    return 0;
    */

    if (frontend.GetLastFrame() == expected) { return 0; }
    else {
        std::cerr << "actual output does not match expected output" << std::endl;

        bool found = false;
        for (auto y=0ul; y < expected.size(); ++y) {
            const auto &exp_row = expected[y],
                       &act_row = frontend.GetLastFrame()[y];

            for (auto x=0ul; x < exp_row.size(); ++x) {
                if (exp_row[x] != act_row[x]) {
                    std::cerr << "first mismatch at (" << y << "," << x << ")" << std::endl;
                    found = true;
                    break;
                }
            }

            if (found) { break; }
        }

        return 1;
    }

    return 0;
}
