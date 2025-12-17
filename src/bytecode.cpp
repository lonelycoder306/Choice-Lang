#include "../include/bytecode.h"
#include "../include/object.h"
#include "../include/opcodes.h"
#include <cstring>
#include <fstream>
#include <type_traits>
using namespace Object;

ByteCode::ByteCode() :
	block(0), pool(0) {}

ByteCode::ByteCode(const vByte& block) :
	block(block) {}

ByteCode::ByteCode(const vByte& block, vObj pool) :
	block(block), pool(std::move(pool)) {}

void ByteCode::addByte(ui8 byte)
{
	block.push_back(byte);
}

void ByteCode::addShort(ui16 bytes)
{
	addByte((bytes >> 8) & 0xff);
	addByte(bytes & 0xff);
}

void ByteCode::addLong(ui32 bytes)
{
	addByte((bytes >> 24) & 0xff);
	addByte((bytes >> 16) & 0xff);
	addByte((bytes >> 8) & 0xff);
	addByte(bytes & 0xff);
}

void ByteCode::loadReg(ui8 reg, ui8 op)
{
	addBytes(static_cast<ui8>(OP_LOAD_R), reg, op);
}

void ByteCode::loadRegConst(BaseUP constant, ui8 reg)
{
	// Must do them separately since addBytes
	// cannot handle an opcode and a regular byte together.
	addByte(OP_LOAD_R);
	// Destination first.
	addByte(reg);

	if (constant->type == OBJ_INT)
	{
		if (constant->size == 1)		ADD_SMALL_INT(constant, 8);
		else if (constant->size == 2)	ADD_SMALL_INT(constant, 16);
		else if (constant->size == 4)	ADD_SMALL_INT(constant, 32);
		else if (constant->size == 8)	ADD_SMALL_INT(constant, 64);
	}

	else if (constant->type == OBJ_UINT)
	{
		if (constant->size == 1)		ADD_SMALL_UINT(constant, 8);
		else if (constant->size == 2)	ADD_SMALL_UINT(constant, 16);
		else if (constant->size == 4)	ADD_SMALL_UINT(constant, 32);
		else if (constant->size == 8)
		{
			GET_CONST_T(constant, UInt<UINT_SIZE(64)>);
			if (temp->value <= INT64_MAX)
				ADD_SMALL_UINT(constant, 64);
		}
	}

	pool.push_back(std::move(constant));

	size_t size = pool.size();
	if (size - 1 < 256)
	{
		addByte(OP_BYTE_OPER);
		addByte(static_cast<ui8>(size - 1));
	}

	else if (size - 1 < 65536)
	{
		addByte(OP_SHORT_OPER);
		addShort(static_cast<ui16>(size - 1));
	}

	else
	{
		addByte(OP_LONG_OPER);
		addLong(static_cast<ui32>(size - 1));
	}
}

void ByteCode::cacheStream(std::ofstream& os) const
{
	os.write(reinterpret_cast<const char*>(block.data()), 
				block.size());

	// Hypothetical for constant pool.
	constexpr ui8 POOL_START = 100; // Beyond any opcodes we have.
	os.put(static_cast<char>(POOL_START));
	if (file != "")
		os.write(file.data(), file.length());
	os.put('\0');
	for (const BaseUP& constant : pool)
		constant->emit(os);
}

void ByteCode::clearCode()
{
	block.clear();
}

void ByteCode::clearPool()
{
	pool.clear();
}

void ByteCode::clear()
{
	clearCode();
	clearPool();
}