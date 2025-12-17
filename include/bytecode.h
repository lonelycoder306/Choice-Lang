#pragma once
#include "common.h"
#include "object.h"
#include <fstream>
#include <vector>
using namespace Object;

class VM;
class Disassembler;

class ByteCode
{
    #define INT_SIZE(size) i##size
    #define UINT_SIZE(size) ui##size
    #define GET_CONST_T(constant, type) \
        type* temp = dynamic_cast<type*>(constant.get())
    #define CHECK_VAL_T(constant, type) \
        GET_CONST_T(constant, type); temp != nullptr
    #define ADD_IF_SMALL(value) \
        do { \
            if ((-3 < static_cast<i64>(value)) && (value < 3)) \
            { \
                addByte(static_cast<ui8>(value + 2)); \
                return; \
            } \
        } while (false)
    #define ADD_SMALL_INT(constant, size) \
        do { \
            GET_CONST_T(constant, Int<INT_SIZE(size)>); \
            ADD_IF_SMALL(temp->value); \
        } while (false)
    #define ADD_SMALL_UINT(constant, size) \
        do { \
            GET_CONST_T(constant, UInt<UINT_SIZE(size)>); \
            ADD_IF_SMALL(temp->value); \
        } while (false)

    private:
        vByte block;
        vObj pool;

    public:
        ByteCode();
        ByteCode(const vByte& block);
        ByteCode(const vByte& block, vObj pool);

        void addByte(ui8 byte);
        template<typename... Bytes>
        void addBytes(Bytes... bytes);
        // Using big endian.
        void addShort(ui16 bytes);
        void addLong(ui32 bytes);

        void loadReg(ui8 reg, ui8 op);
        void loadRegConst(BaseUP constant, ui8 reg);
        template<typename Op, typename... Bytes>
        void addOp(Op op, Bytes... opers);

        void cacheStream(std::ofstream& os) const;
        void clearCode();
        void clearPool();
        void clear();

        friend class VM;
        friend class Disassembler;
};

template<typename... Bytes>
void ByteCode::addBytes(Bytes... bytes)
{
    for (ui8 byte : {bytes...})
        addByte(byte);
}

// For testing.

template<typename Op, typename... Bytes>
void ByteCode::addOp(Op op, Bytes... opers)
{
    addByte(static_cast<ui8>(op));
    for (ui8 operand : {opers...})
        addByte(operand);
}