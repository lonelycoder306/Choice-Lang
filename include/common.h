#pragma once
#include "object.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#define COMP_AST

#define GETV(variant, type) std::get<type>(variant)

using i8        = int8_t;
using i16       = int16_t;
using i32       = int32_t;
using i64       = int64_t;

using ui8       = uint8_t;
using ui16      = uint16_t;
using ui32      = uint32_t;
using ui64      = uint64_t;

using Hash      = uint32_t;

class Token;
using vT    	= std::vector<Token>;
using vByte 	= std::vector<ui8>;
using vObj  	= std::vector<Object::BaseUP>;

constexpr int TAB_SIZE = 4;
// Whether we're running an externally loaded
// program or not.
extern bool external;
extern std::string file;