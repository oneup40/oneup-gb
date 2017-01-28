// Copyright 2016 oneup

#include "Serializer.hpp"

namespace gb1 {

Serializer& Serializer::SerializeBool(bool val) {
    u64 raw = htole<u64>(val);
    DoWrite(raw);

    return *this;
}

Serializer::Serializer(std::ostream *os) : os_(os) {}

Serializer::operator bool() { return static_cast<bool>(*os_); }

void Serializer::Fail() { os_->setstate(std::ios_base::failbit); }

Serializer& Serializer::Start(u64 code) { return (*this) << code; }

Serializer& Serializer::operator<<(bool val)    { return SerializeBool(val); }

Serializer& Serializer::operator<<(u8 val)      { return SerializeUint(val); }
Serializer& Serializer::operator<<(u16 val)     { return SerializeUint(val); }
Serializer& Serializer::operator<<(u32 val)     { return SerializeUint(val); }
Serializer& Serializer::operator<<(u64 val)     { return SerializeUint(val); }

Serializer& Serializer::operator<<(i8 val)      { return SerializeInt(val); }
Serializer& Serializer::operator<<(i16 val)     { return SerializeInt(val); }
Serializer& Serializer::operator<<(i32 val)     { return SerializeInt(val); }
Serializer& Serializer::operator<<(i64 val)     { return SerializeInt(val); }

Serializer& Serializer::operator<<(float val)   { return SerializeFloat(val); }
Serializer& Serializer::operator<<(double val)  { return SerializeFloat(val); }

Deserializer& Deserializer::DeserializeBool(bool *val) {
    u64 raw = 0;
    DoRead(&raw);
    *val = letoh(raw) != 0;

    return *this;
}

Deserializer::Deserializer(std::istream *is) : is_(is) {}

Deserializer::operator bool() { return static_cast<bool>(*is_); }

void Deserializer::Fail() { is_->setstate(std::ios_base::failbit); }

Deserializer& Deserializer::Start(u64 code) {
    u64 extracted = 0;
    (*this) >> extracted;
    if (!*this) { return *this; }

    if (extracted != code) { Fail(); }
    return *this;
}

Deserializer& Deserializer::operator>>(bool &val)   { return DeserializeBool(&val); }

Deserializer& Deserializer::operator>>(u8 &val)     { return DeserializeUint(&val); }
Deserializer& Deserializer::operator>>(u16 &val)    { return DeserializeUint(&val); }
Deserializer& Deserializer::operator>>(u32 &val)    { return DeserializeUint(&val); }
Deserializer& Deserializer::operator>>(u64 &val)    { return DeserializeUint(&val); }

Deserializer& Deserializer::operator>>(i8 &val)     { return DeserializeInt(&val); }
Deserializer& Deserializer::operator>>(i16 &val)    { return DeserializeInt(&val); }
Deserializer& Deserializer::operator>>(i32 &val)    { return DeserializeInt(&val); }
Deserializer& Deserializer::operator>>(i64 &val)    { return DeserializeInt(&val); }

Deserializer& Deserializer::operator>>(float &val)  { return DeserializeFloat(&val); }
Deserializer& Deserializer::operator>>(double &val) { return DeserializeFloat(&val); }

}     // namespace gb1
