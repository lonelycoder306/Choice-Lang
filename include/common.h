#pragma once
#include "object.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
// #define ALT

class Token;
using vT    	= std::vector<Token>;
using vByte 	= std::vector<uint8_t>;
using vObj  	= std::vector<Object::BaseUP>;

using Byte  	= int8_t;
using Short 	= int16_t;
using Long  	= int32_t;

using UByte     = uint8_t;
using UShort    = uint16_t;
using ULong     = uint32_t;

using i8        = int8_t;
using i16       = int16_t;
using i32       = int32_t;
using i64       = int64_t;

using ui8       = uint8_t;
using ui16      = uint16_t;
using ui32      = uint32_t;
using ui64      = uint64_t;

constexpr int TAB_SIZE = 4;
// Whether we're running an externally loaded
// program or not.
extern bool external;
extern std::string file;