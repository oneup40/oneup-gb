// Copyright 2017 oneup

#include "core/serial/Serial.hpp"

#include "core/Frontend.hpp"
#include "core/Machine.hpp"

namespace gb1 {

bool Serial::Tick() {
    if (!(sc_ & 0x80) || !bits_) { return true; }

    m_->frontend.OutputSerial((sb_ & 0x80) != 0);
    sb_ <<= 1;
    if (m_->frontend.InputSerial()) {
        sb_ |= 0x01;
    }

    --bits_;

    if (!bits_) {
        m_->Interrupt(0x08);
    }

    return true;
}

}   // namespace gb1
