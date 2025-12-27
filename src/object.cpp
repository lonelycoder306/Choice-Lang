#include "../include/object.h"
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
using namespace Object;

// Base.

Base::Base() :
    type(OBJ_INVALID) {}

Base::Base(ObjType type) :
    type(type) {}

std::string Base::printType()
{
    return "OBJECT";
}

void Base::emit(std::ofstream& os)
{
    os.put(static_cast<char>(OBJ_BASE));
}

#define emitBytes(obj, type) \
    do { \
        os.put(static_cast<char>(obj)); \
        char bytes[sizeof(type)]; \
        std::memcpy(&bytes[0], &value, sizeof(type)); \
        os.write(bytes, sizeof(type)); \
    } while (false)

// Int.

Int::Int() :
	Base(OBJ_INT), value(0) {}

Int::Int(int64_t value) :
	Base(OBJ_INT), value(value) {}

std::string Int::print()
{
	return std::to_string(this->value);
}

std::string Int::printType()
{
	return "INT64";
}

void Int::emit(std::ofstream& os)
{
    emitBytes(OBJ_INT, int64_t);
}

// UInt.

UInt::UInt() :
	Base(OBJ_INT), value(0) {}

UInt::UInt(uint64_t value) :
	Base(OBJ_UINT), value(value) {}

std::string UInt::print()
{
	return std::to_string(this->value);
}

std::string UInt::printType()
{
	return "UINT64";
}

void UInt::emit(std::ofstream& os)
{
    emitBytes(OBJ_UINT, uint64_t);
}

// Dec.

Dec::Dec() :
	Base(OBJ_DEC), value(0.0) {}

Dec::Dec(double value) :
	Base(OBJ_DEC), value(value) {}

std::string Dec::print()
{
	return std::to_string(this->value);
}

std::string Dec::printType()
{
	return "DEC";
}

void Dec::emit(std::ofstream& os)
{
    emitBytes(OBJ_DEC, double);
}

// Bool.

Bool::Bool(bool value) :
    Base(OBJ_BOOL), value(value) {}

std::string Bool::print()
{
    return (this->value ? "true" : "false");
}

std::string Bool::printType()
{
    return "BOOL";
}

// String.

String::String() :
    Base(OBJ_STRING), value("") {} // Default initialize to empty string.

String::String(std::string_view& value) :
    Base(OBJ_STRING), value(value) {}

String String::makeString(const std::string& value)
{
    String str;
    str.alt = value;
    return str;
}

std::string String::print()
{
    return std::string(this->value);
}

std::string String::printType()
{
    return "STRING";
}

void String::emit(std::ofstream& os)
{
    os.put(static_cast<char>(OBJ_STRING));
    os.write(value.data(), value.size());
    os.put('\0');
}

// Null.

Null::Null() :
    Base(OBJ_NULL) {}

std::string Null::print()
{
    return "null";
}

std::string Null::printType()
{
    return "NULL";
}