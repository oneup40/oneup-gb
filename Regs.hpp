#pragma once

// Copyright 2016 oneup

#include "Base.hpp"
#include "Serializer.hpp"

namespace gblr {

enum Flag : u8 {
    FLAG_Z = 0x80,
    FLAG_N = 0x40,
    FLAG_H = 0x20,
    FLAG_C = 0x10
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct Regs {    // TODO: endianness
    u16 pc = 0x100, sp = 0xfffe;
    union {
        u16 bc = 0x0013;
        struct {
            u8 c, b;
        };
    };
    union {
        u16 de = 0x00d8;
        struct {
            u8 e, d;
        };
    };
    union {
        u16 hl = 0x014d;
        struct {
            u8 l, h;
        };
    };
    union {
        u16 af = 0x01b0;
        struct {
            u8 f, a;
        };
    };

    bool getf(Flag flag) const { return f & flag; }
    void setf(Flag flag) { f |= flag; }
    void clrf(Flag flag) { f &= ~flag; }
    void setfb(Flag flag, bool state) {
        if (state) { setf(flag); }
        else { clrf(flag); }
    }
};
#pragma GCC diagnostic pop

static inline std::string to_string(const Regs &regs) {
    std::string s;

    s += "A=";
    s += to_hex(regs.a, 2);
    s += " ";

    s += "B=";
    s += to_hex(regs.b, 2);
    s += " ";

    s += "C=";
    s += to_hex(regs.c, 2);
    s += " ";

    s += "D=";
    s += to_hex(regs.d, 2);
    s += " ";

    s += "E=";
    s += to_hex(regs.e, 2);
    s += " ";

    s += "H=";
    s += to_hex(regs.h, 2);
    s += " ";

    s += "L=";
    s += to_hex(regs.l, 2);
    s += " ";

    s += "F=";
    s += to_hex(regs.f, 2);
    s += " ";

    s += "SP=";
    s += to_hex(regs.sp, 4);
    s += " ";

    s += "PC=";
    s += to_hex(regs.pc, 4);
    s += " ";

    s += (regs.getf(FLAG_Z)) ? 'Z' : '.';
    s += (regs.getf(FLAG_N)) ? 'N' : '.';
    s += (regs.getf(FLAG_H)) ? 'H' : '.';
    s += (regs.getf(FLAG_C)) ? 'C' : '.';
    s += "....";

    return s;
}

static inline Serializer& operator<<(Serializer &s, const Regs &reg) {
	return s << reg.pc << reg.sp << reg.bc << reg.de << reg.hl << reg.af;
}
static inline Deserializer& operator>>(Deserializer &d, Regs &reg) {
	return d >> reg.pc >> reg.sp >> reg.bc >> reg.de >> reg.hl >> reg.af;
}

}    // namespace gblr
