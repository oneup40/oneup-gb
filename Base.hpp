#pragma once

// Copyright 2016 oneup

#include <iomanip>
#include <sstream>
#include <string>

#include <cstdint>

namespace gblr {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

namespace {

template <typename T>
std::string do_to_string(const T t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template <typename T>
static inline std::string do_to_hex(const T& t, size_t width) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(width) << std::setfill('0') << t;
    return oss.str();
}

template <>
inline std::string do_to_hex<u8>(const u8& t, size_t width) {
	std::ostringstream oss;
	size_t val = t;
	oss << std::hex << std::uppercase << std::setw(width) << std::setfill('0') << val;
	return oss.str();
}

}    // namespace (anonymous)

static inline std::string to_string(int val)                { return do_to_string(val); }
static inline std::string to_string(long val)               { return do_to_string(val); }
static inline std::string to_string(long long val)          { return do_to_string(val); }
static inline std::string to_string(unsigned val)           { return do_to_string(val); }
static inline std::string to_string(unsigned long val)      { return do_to_string(val); }
static inline std::string to_string(unsigned long long val)	{ return do_to_string(val); }
static inline std::string to_string(float val)              { return do_to_string(val); }
static inline std::string to_string(double val)             { return do_to_string(val); }
static inline std::string to_string(long double val)        { return do_to_string(val); }

static inline std::string to_hex(int val, size_t width)                 { return do_to_hex(val, width); }
static inline std::string to_hex(long val, size_t width)                { return do_to_hex(val, width); }
static inline std::string to_hex(long long val, size_t width)           { return do_to_hex(val, width); }
static inline std::string to_hex(unsigned val, size_t width)            { return do_to_hex(val, width); }
static inline std::string to_hex(unsigned long val, size_t width)       { return do_to_hex(val, width); }
static inline std::string to_hex(unsigned long long val, size_t width)  { return do_to_hex(val, width); }

#ifdef _MSC_VER
static inline u8 bswap8(u8 x)		{ return x; }
static inline u16 bswap16(u16 x)	{ return _byteswap_ushort(x); }
static_assert(sizeof(u16) == sizeof(unsigned short), "expected unsigned short == u16");
static inline u32 bswap32(u32 x)	{ return _byteswap_ulong(x); }
static_assert(sizeof(u32) == sizeof(unsigned long), "expected unsigned long == u32");
static inline u64 bswap64(u64 x)	{ return _byteswap_uint64(x); }
#else
static inline u8 bswap8(u8 x) 	    { return x; }
static inline u16 bswap16(u16 x)    { return __builtin_bswap16(x); }
static inline u32 bswap32(u32 x)    { return __builtin_bswap32(x); }
static inline u64 bswap64(u64 x)    { return __builtin_bswap64(x); }
#endif

template <typename T> static inline T bswap(T x);
template <> inline u8 bswap<>(u8 x)     { return bswap8(x); }
template <> inline u16 bswap<>(u16 x)   { return bswap16(x); }
template <> inline u32 bswap<>(u32 x)   { return bswap32(x); }
template <> inline u64 bswap<>(u64 x)   { return bswap64(x); }

#if (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) \
    || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) \
    || defined(__BIG_ENDIAN__)

template <typename T> static inline T htobe(T x) { return x; }
template <typename T> static inline T htole(T x) { return bswap(x); }
template <typename T> static inline T betoh(T x) { return x; }
template <typename T> static inline T letoh(T x) { return bswap(x); }

#elif (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN) \
    || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) \
    || defined(__LITTLE_ENDIAN__) \
	|| defined(_M_AMD64 )

template <typename T> static inline T htobe(T x) { return bswap(x); }
template <typename T> static inline T htole(T x) { return x; }
template <typename T> static inline T betoh(T x) { return bswap(x); }
template <typename T> static inline T letoh(T x) { return x; }

#else
#error "Unable to detect platform endianness"
#endif

constexpr static inline u64 eight_cc(u8 a=0, u8 b=0, u8 c=0, u8 d=0, u8 e=0, u8 f=0, u8 g=0, u8 h=0) {
	return u64(u8(a))
		| (u64(u8(b)) << 8)
		| (u64(u8(c)) << 16)
		| (u64(u8(d)) << 24)
		| (u64(u8(e)) << 32)
		| (u64(u8(f)) << 40)
		| (u64(u8(g)) << 48)
		| (u64(u8(h)) << 56);
}

}    // namespace gblr
