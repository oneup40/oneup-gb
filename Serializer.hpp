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
//	 0) Booleans are stored as little endian uint64: 0 = false, nonzero = true
//   1) All unsigned integer primitives are stored as little endian uint64.
//   2) All signed integer primitives are stored as little endian int64.
//   3) All float primitives are stored as IEEE 754 double.
//   4) Strings are stored as <char count> followed by <n> chars, then padded
//      to a multiple of 8 bytes.
//   5) Structures are stored by storing all their members.
//   6) Containers are stored as <item count> followed by <n> items.
//	 7) Fixed size arrays are stored as <n> items (no item count).
//
// There is also no support yet for including any sort of protocol version.

namespace gblr {

// forward declarations

class Serializer;
class Deserializer;

static inline Serializer& operator<<(Serializer &s, bool val);
static inline Serializer& operator<<(Serializer &s, u8 val);
static inline Serializer& operator<<(Serializer &s, u16 val);
static inline Serializer& operator<<(Serializer &s, u32 val);
static inline Serializer& operator<<(Serializer &s, u64 val);
static inline Serializer& operator<<(Serializer &s, i8 val);
static inline Serializer& operator<<(Serializer &s, i16 val);
static inline Serializer& operator<<(Serializer &s, i32 val);
static inline Serializer& operator<<(Serializer &s, i64 val);
static inline Serializer& operator<<(Serializer &s, float val);
static inline Serializer& operator<<(Serializer &s, double val);
template <typename T> static inline Serializer& operator<<(Serializer &s, const std::basic_string<T> &val);
template <typename T> static inline Serializer& operator<<(Serializer &s, const std::vector<T> &val);
template <typename T, size_t N> static inline Serializer& operator<<(Serializer &s, const std::array<T, N> &val);

static inline Deserializer& operator>>(Deserializer &d, bool &val);
static inline Deserializer& operator>>(Deserializer &d, u8 &val);
static inline Deserializer& operator>>(Deserializer &d, u16 &val);
static inline Deserializer& operator>>(Deserializer &d, u32 &val);
static inline Deserializer& operator>>(Deserializer &d, u64 &val);
static inline Deserializer& operator>>(Deserializer &d, i8 &val);
static inline Deserializer& operator>>(Deserializer &d, i16 &val);
static inline Deserializer& operator>>(Deserializer &d, i32 &val);
static inline Deserializer& operator>>(Deserializer &d, i64 &val);
static inline Deserializer& operator>>(Deserializer &d, float &val);
static inline Deserializer& operator>>(Deserializer &d, double &val);
template <typename T> static inline Deserializer& operator>>(Deserializer &d, std::basic_string<T> &val);
template <typename T> static inline Deserializer& operator>>(Deserializer &d, std::vector<T> &val);
template <typename T, size_t N> static inline Deserializer& operator>>(Deserializer &d, std::array<T, N> &val);

class Serializer {
	std::ostream *os_;

	template <typename T>
	void DoWrite(const T& val) { os_->write(reinterpret_cast<const char*>(&val), sizeof(val)); }
public:
	explicit Serializer(std::ostream *os) : os_(os) {}
	Serializer(const Serializer&) = delete;
	Serializer(Serializer&&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	Serializer& operator=(Serializer&&) = delete;

	explicit operator bool() { return static_cast<bool>(*os_); }

	Serializer& SerializeBool(bool val) {
		u64 raw = htole<u64>(val);
		DoWrite(raw);

		return *this;
	}

	template <typename T>
	Serializer& SerializeUint(T val) {
		static_assert(sizeof(u64) == 8, "Need 8-byte u64");
		u64 raw = htole<u64>(val);
		DoWrite(raw);

		return *this;
	}

	template <typename T>
	Serializer& SerializeInt(T val) {
		static_assert(sizeof(i64) == 8, "Need 8-byte i64");
		i64 raw = htole<i64>(val);
		DoWrite(raw);

		return *this;
	}

	template <typename T>
	Serializer& SerializeFloat(T val) {
		static_assert(sizeof(double) == 8, "Need 8-byte double");
		static_assert(std::numeric_limits<double>::is_iec559, "Need IEEE 754 double");
		DoWrite<double>(val);

		return *this;
	}

	template <typename T>
	Serializer& SerializeString(const T& val) {
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
	Serializer& SerializeContainer(const T& val) {
		(*this) << val.size();

		for (const auto &item : val) {
			(*this) << item;
		}

		return *this;
	}

	template <typename T>
	Serializer& SerializeArray(const T& val) {
		for (const auto &item : val) {
			(*this) << item;
		}

		return *this;
	}

	Serializer& Start(u64 code) { return (*this) << code; }
};

class Deserializer {
	std::istream *is_;

	template <typename T>
	void DoRead(T *val) { is_->read(reinterpret_cast<char*>(val), sizeof(*val)); }
public:
	explicit Deserializer(std::istream *is) : is_(is) {}
	Deserializer(const Deserializer&) = delete;
	Deserializer(Deserializer&&) = delete;
	Deserializer& operator=(const Deserializer&) = delete;
	Deserializer& operator=(Deserializer&&) = delete;

	explicit operator bool() { return static_cast<bool>(*is_); }

	Deserializer& DeserializeBool(bool *val) {
		u64 raw = 0;
		DoRead(&raw);
		*val = letoh(raw);

		return *this;
	}

	template <typename T>
	Deserializer& DeserializeUint(T *val) {
		static_assert(sizeof(u64) == 8, "Need 8-byte u64");

		u64 raw = 0;
		DoRead(&raw);
		*val = letoh(raw);

		return *this;
	}

	template <typename T>
	Deserializer& DeserializeInt(T *val) {
		static_assert(sizeof(i64) == 8, "Need 8-byte i64");

		i64 raw = 0;
		DoRead(&raw);
		*val = letoh(raw);

		return *this;
	}

	template <typename T>
	Deserializer& DeserializeFloat(T *val) {
		static_assert(sizeof(double) == 8, "Need 8-byte double");
		static_assert(std::numeric_limits<double>::is_iec559, "Need IEEE 754 double");

		double raw = 0;
		DoRead(&raw);
		*val = raw;

		return *this;
	}

	template <typename T>
	Deserializer& DeserializeString(T *val) {
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
	Deserializer& DeserializeContainer(T *val) {
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
	Deserializer& DeserializeArray(T* val) {
		for (auto &item : *val) {
			(*this) >> item;
		}

		return *this;
	}

	Deserializer& Start(u64 code) {
		u64 extracted = 0;
		(*this) >> extracted;
		if (!*this) { return *this; }

		if (extracted != code) { is_->setstate(std::ios_base::failbit); }
		return *this;
	}
};

static inline Serializer& operator<<(Serializer &s, bool val)   { return s.SerializeBool(val); }

static inline Serializer& operator<<(Serializer &s, u8 val) 	{ return s.SerializeUint(val); }
static inline Serializer& operator<<(Serializer &s, u16 val) 	{ return s.SerializeUint(val); }
static inline Serializer& operator<<(Serializer &s, u32 val) 	{ return s.SerializeUint(val); }
static inline Serializer& operator<<(Serializer &s, u64 val) 	{ return s.SerializeUint(val); }

static inline Serializer& operator<<(Serializer &s, i8 val) 	{ return s.SerializeInt(val); }
static inline Serializer& operator<<(Serializer &s, i16 val) 	{ return s.SerializeInt(val); }
static inline Serializer& operator<<(Serializer &s, i32 val) 	{ return s.SerializeInt(val); }
static inline Serializer& operator<<(Serializer &s, i64 val) 	{ return s.SerializeInt(val); }

static inline Serializer& operator<<(Serializer &s, float val)	{ return s.SerializeFloat(val); }
static inline Serializer& operator<<(Serializer &s, double val)	{ return s.SerializeFloat(val); }

template <typename T>
static inline Serializer& operator<<(Serializer &s, const std::basic_string<T> &val)	{ return s.SerializeString(val); }

template <typename T>
static inline Serializer& operator<<(Serializer &s, const std::vector<T> &val)		{ return s.SerializeContainer(val); }

template <typename T, size_t N>
static inline Serializer& operator<<(Serializer &s, const std::array<T, N> &val)	{ return s.SerializeArray(val); }

static inline Deserializer& operator>>(Deserializer &d, bool &val)   { return d.DeserializeBool(&val); }

static inline Deserializer& operator>>(Deserializer &d, u8 &val) 	{ return d.DeserializeUint(&val); }
static inline Deserializer& operator>>(Deserializer &d, u16 &val) { return d.DeserializeUint(&val); }
static inline Deserializer& operator>>(Deserializer &d, u32 &val) { return d.DeserializeUint(&val); }
static inline Deserializer& operator>>(Deserializer &d, u64 &val) { return d.DeserializeUint(&val); }

static inline Deserializer& operator>>(Deserializer &d, i8 &val) 	{ return d.DeserializeInt(&val); }
static inline Deserializer& operator>>(Deserializer &d, i16 &val) { return d.DeserializeInt(&val); }
static inline Deserializer& operator>>(Deserializer &d, i32 &val) { return d.DeserializeInt(&val); }
static inline Deserializer& operator>>(Deserializer &d, i64 &val) { return d.DeserializeInt(&val); }

static inline Deserializer& operator>>(Deserializer &d, float &val)		{ return d.DeserializeFloat(&val); }
static inline Deserializer& operator>>(Deserializer &d, double &val)	{ return d.DeserializeFloat(&val); }

template <typename T>
static inline Deserializer& operator>>(Deserializer &d, std::basic_string<T> &val)	{ return d.DeserializeString(&val); }

template <typename T>
static inline Deserializer& operator>>(Deserializer &d, std::vector<T> &val)	{ return d.DeserializeContainer(&val); }

template <typename T, size_t N>
static inline Deserializer& operator>>(Deserializer &d, std::array<T, N> &val)	{ return d.DeserializeArray(&val); }
}	// namespace gblr

