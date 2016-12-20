#pragma once

// Copyright 2016 oneup

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Base.hpp"

// Assumptions:
//   1) Serialization is not a frequent occurrence (< 1 time per second)
//   2) There is not a large amount of data to serialize (< 32 MiB)
//
// As a result, this is *not* designed to be super fast or super small.
// Rather, it is designed to be super basic.
//
// Basic serialization protocol:
//     0) Booleans are stored as little endian uint64: 0 = false, nonzero = true
//   1) All unsigned integer primitives are stored as little endian uint64.
//   2) All signed integer primitives are stored as little endian int64.
//   3) All float primitives are stored as IEEE 754 double.
//   4) Strings are stored as <char count> followed by <n> chars, then padded
//      to a multiple of 8 bytes.
//   5) Structures are stored by storing all their members.
//   6) Containers are stored as <item count> followed by <n> items.
//     7) Fixed size arrays are stored as <n> items (no item count).
//
// There is also no support yet for including any sort of protocol version.

namespace gblr {

class Serializer {
    std::ostream *os_;

    template <typename T> void DoWrite(const T& val);

    Serializer& SerializeBool(bool val);
    template <typename T> Serializer& SerializeUint(T val);
    template <typename T> Serializer& SerializeInt(T val);
    template <typename T> Serializer& SerializeFloat(T val);
    template <typename T> Serializer& SerializeString(const T& val);
    template <typename T> Serializer& SerializeContainer(const T& val);
    template <typename T> Serializer& SerializeArray(const T& val);
public:
    explicit Serializer(std::ostream *os);
    Serializer(const Serializer&) = delete;
    Serializer(Serializer&&) = delete;
    Serializer& operator=(const Serializer&) = delete;
    Serializer& operator=(Serializer&&) = delete;

    explicit operator bool();
    void Fail();

    Serializer& Start(u64 code);

    Serializer& operator<<(bool val);

    Serializer& operator<<(u8 val);
    Serializer& operator<<(u16 val);
    Serializer& operator<<(u32 val);
    Serializer& operator<<(u64 val);

    Serializer& operator<<(i8 val);
    Serializer& operator<<(i16 val);
    Serializer& operator<<(i32 val);
    Serializer& operator<<(i64 val);

    Serializer& operator<<(float val);
    Serializer& operator<<(double val);

    template <typename T>
    Serializer& operator<<(const std::basic_string<T> &val);

    template <typename T>
    Serializer& operator<<(const std::vector<T> &val);

    template <typename T, size_t N>
    Serializer& operator<<(const std::array<T, N> &val);
};

class Deserializer {
    std::istream *is_;

    template <typename T>
    void DoRead(T *val);

    Deserializer& DeserializeBool(bool *val);
    template <typename T> Deserializer& DeserializeUint(T *val);
    template <typename T> Deserializer& DeserializeInt(T *val);
    template <typename T> Deserializer& DeserializeFloat(T *val);
    template <typename T> Deserializer& DeserializeString(T *val);
    template <typename T> Deserializer& DeserializeContainer(T *val);
    template <typename T> Deserializer& DeserializeArray(T* val);
public:
    explicit Deserializer(std::istream *is);
    Deserializer(const Deserializer&) = delete;
    Deserializer(Deserializer&&) = delete;
    Deserializer& operator=(const Deserializer&) = delete;
    Deserializer& operator=(Deserializer&&) = delete;

    explicit operator bool();
    void Fail();

    Deserializer& Start(u64 code);

    Deserializer& operator>>(bool &val);

    Deserializer& operator>>(u8 &val);
    Deserializer& operator>>(u16 &val);
    Deserializer& operator>>(u32 &val);
    Deserializer& operator>>(u64 &val);

    Deserializer& operator>>(i8 &val);
    Deserializer& operator>>(i16 &val);
    Deserializer& operator>>(i32 &val);
    Deserializer& operator>>(i64 &val);

    Deserializer& operator>>(float &val);
    Deserializer& operator>>(double &val);

    template <typename T>
    Deserializer& operator>>(std::basic_string<T> &val);

    template <typename T>
    Deserializer& operator>>(std::vector<T> &val);

    template <typename T, size_t N>
    Deserializer& operator>>(std::array<T, N> &val);
};

template <typename T>
void Serializer::DoWrite(const T& val) {
    os_->write(reinterpret_cast<const char*>(&val), sizeof(val));
}

template <typename T>
Serializer& Serializer::SerializeUint(T val) {
    static_assert(sizeof(u64) == 8, "Need 8-byte u64");
    u64 raw = htole<u64>(val);
    DoWrite(raw);

    return *this;
}

template <typename T>
Serializer& Serializer::SerializeInt(T val) {
    static_assert(sizeof(i64) == 8, "Need 8-byte i64");
    i64 raw = htole<i64>(val);
    DoWrite(raw);

    return *this;
}

template <typename T>
Serializer& Serializer::SerializeFloat(T val) {
    static_assert(sizeof(double) == 8, "Need 8-byte double");
    static_assert(std::numeric_limits<double>::is_iec559, "Need IEEE 754 double");
    DoWrite<double>(val);

    return *this;
}

template <typename T>
Serializer& Serializer::SerializeString(const T& val) {
    (*this) << val.size();

    size_t n = 0;
    for (auto ch : val) {
        n += sizeof(ch);
        DoWrite(ch);
    }

    while (n & 7) {
        n += sizeof(char);
        DoWrite<char>(0);
    }

    return *this;
}

template <typename T>
Serializer& Serializer::SerializeContainer(const T& val) {
    (*this) << val.size();

    for (const auto &item : val) {
        (*this) << item;
    }

    return *this;
}

template <typename T>
Serializer& Serializer::SerializeArray(const T& val) {
    for (const auto &item : val) {
        (*this) << item;
    }

    return *this;
}

template <typename T>
Serializer& Serializer::operator<<(const std::basic_string<T> &val) {
    return SerializeString(val);
}

template <typename T>
Serializer& Serializer::operator<<(const std::vector<T> &val) {
    return SerializeContainer(val);
}

template <typename T, size_t N>
Serializer& Serializer::operator<<(const std::array<T, N> &val) {
    return SerializeArray(val);
}

template <typename T>
void Deserializer::DoRead(T *val) {
    is_->read(reinterpret_cast<char*>(val), sizeof(*val));
}

template <typename T>
Deserializer& Deserializer::DeserializeUint(T *val) {
    static_assert(sizeof(u64) == 8, "Need 8-byte u64");

    u64 raw = 0;
    DoRead(&raw);
    *val = letoh(raw);

    return *this;
}

template <typename T>
Deserializer& Deserializer::DeserializeInt(T *val) {
    static_assert(sizeof(i64) == 8, "Need 8-byte i64");

    i64 raw = 0;
    DoRead(&raw);
    *val = letoh(raw);

    return *this;
}

template <typename T>
Deserializer& Deserializer::DeserializeFloat(T *val) {
    static_assert(sizeof(double) == 8, "Need 8-byte double");
    static_assert(std::numeric_limits<double>::is_iec559, "Need IEEE 754 double");

    double raw = 0;
    DoRead(&raw);
    *val = raw;

    return *this;
}

template <typename T>
Deserializer& Deserializer::DeserializeString(T *val) {
    u64 n = 0;
    (*this) >> n;
    if (!*this) { return *this; }

    val->resize(n / sizeof(typename T::value_type));
    for (auto &ch : *val) {
        DoRead(&ch);
    }

    char ch = 0;
    while (n & 7) {
        n += sizeof(char);
        DoRead(&ch);
    }

    return *this;
}

template <typename T>
Deserializer& Deserializer::DeserializeContainer(T *val) {
    u64 n;
    (*this) >> n;
    if (!*this) { return *this; }

    val->resize(n);
    for (auto &item : *val) {
        (*this) >> item;
    }

    return *this;
}

template <typename T>
Deserializer& Deserializer::DeserializeArray(T* val) {
    for (auto &item : *val) {
        (*this) >> item;
    }

    return *this;
}

template <typename T>
Deserializer& Deserializer::operator>>(std::basic_string<T> &val)    {
    return DeserializeString(&val);
}

template <typename T>
Deserializer& Deserializer::operator>>(std::vector<T> &val) {
    return DeserializeContainer(&val);
}

template <typename T, size_t N>
Deserializer& Deserializer::operator>>(std::array<T, N> &val)    {
    return DeserializeArray(&val);
}


}    // namespace gblr
