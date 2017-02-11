// Copyright 2017 oneup

#include <array>
#include <fstream>
#include <string>
#include <vector>

#include "core/Base.hpp"
#include "core/Frontend.hpp"
#include "core/Machine.hpp"

class BlarggTesterFrontend : public gb1::Frontend {
    std::array<std::array<gb1::u8, 40>, 144> last_frame_;
    gb1::i64 nframes_;
public:
    BlarggTesterFrontend() : nframes_(0) {
        for (auto &line : last_frame_) {
            line.fill(0);
        }
    }
    BlarggTesterFrontend(const BlarggTesterFrontend&) = delete;
    BlarggTesterFrontend(BlarggTesterFrontend&&) = delete;
    BlarggTesterFrontend& operator=(const BlarggTesterFrontend&) = delete;
    BlarggTesterFrontend& operator=(BlarggTesterFrontend&&) = delete;

    gb1::Button InputJoypad() override { return gb1::Button(0); }
    void OutputAudioFrame(gb1::i16, gb1::i16) override {}
    void OutputVideoFrame(std::array<std::array<gb1::u8, 40>, 144>&& frame) override {
        last_frame_ = std::move(frame);
        ++nframes_;
    }

    const std::array<std::array<gb1::u8, 40>, 144>& GetLastFrame() const {
        return last_frame_;
    }
    gb1::i64 GetFrameCount() { return nframes_; }
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
    std::cerr << "usage: " << progname << " test-rom-path num-frames expected-output-path" << std::endl;
}

struct Args {
    int frames;
    std::string rom_path, exp_path;
};

static bool ParseArgs(int argc, char **argv, Args &args) {
    if (!argc) { Usage("blargg-tester"); return false; }
    else if (argc != 4) { Usage(argv[0]); return false; }

    args.rom_path = argv[1];

    try {
        args.frames = std::stoi(argv[2], nullptr, 0);
    } catch(...) {
        Usage(argv[0]);
        std::cerr << "invalid number of frames" << std::endl;
        return false;
    }

    args.exp_path = argv[3];

    return true;
}

static bool LoadExpectedOutput(
    const std::string &path,
    std::array<std::array<gb1::u8, 40>, 144> &expected
) {
    std::ifstream fin(path);
    for (auto &line : expected) {
        line.fill(0);

        std::array<char, 320> data;
        if (!fin.read(data.data(), data.size())) {
            std::cerr << "Xxx" << std::endl;
            return false; }

        auto j=0, shift=0;
        for (auto i=0u; i < data.size(); i += 2) {
            gb1::u8 dot = 0;
            switch (data[i]) {
                case ' ': dot = 0x00; break;
                case '.': dot = 0x01; break;
                case ':': dot = 0x02; break;
                case 'O': dot = 0x03; break;
                default: { std::cerr << "y" << std::endl; return false;}
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
            std::cerr <<" z" << std::endl;
            std::cerr << bool(fin) << ' ' << unsigned(newline) << std::endl;
            return false; }
    }

    return true;
}

int main(int argc, char **argv) {
    Args args;
    if (!ParseArgs(argc, argv, args)) { return 1; }

    BlarggTesterFrontend frontend;
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

    while (frontend.GetFrameCount() < args.frames) {
        m.Tick();
    }

    /*std::array<char, 4> palette{{' ','.',':','O'}};
    for (const auto &line : expected) {
        for (auto dot : line) {
            for (auto i = 0u; i < 4; ++i) {
                std::cout << palette[dot & 0x03];
                std::cout << palette[dot & 0x03];
                dot >>= 2;
            }
        }
        std::cout << std::endl;
    }*/

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
}
