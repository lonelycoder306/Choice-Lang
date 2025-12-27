#include "../include/main_utils.h"
#include "../include/bytecode.h"
#include "../include/common.h"
#include "../include/disasm.h"
#include "../include/object.h"
#include "../include/utils.h"
#include "../include/vm.h"
#include <functional>
#include <iostream>
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
using namespace Object;

std::string readFile(const char* fileName)
{
	std::ifstream file(fileName);
	if (file.fail())
	{
		std::cerr << "Failed to open file.\n";
		exit(66);
	}
	
	if (file.is_open())
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string fileString = buffer.str();
		file.close();
		return fileString;
	}

	std::cerr << "File is closed.\n";
	exit(66);
}

template<typename Size>
static Size reconstructBytes(vByte::const_iterator& it)
{
	Size value = 0;
	ui8 bytes[sizeof(Size)];
	for (size_t i = 0; i < sizeof(Size); i++)
		bytes[i] = *(it++);
	it--;
	std::memcpy(&value, &bytes[0], sizeof(Size));
	return value;
}

static BaseUP reconstructInt(vByte::const_iterator& it)
{
	return std::make_unique<Int>(reconstructBytes<i64>(it));
}

static BaseUP reconstructUInt(vByte::const_iterator& it)
{
	return std::make_unique<UInt>(reconstructBytes<ui64>(it));
}

static BaseUP reconstructDec(vByte::const_iterator& it)
{
	return std::make_unique<Dec>(reconstructBytes<double>(it));
}

static BaseUP reconstructString(vByte::const_iterator& it)
{
	std::string str;
	while (static_cast<char>(*it) != '\0')
	{
		str.push_back(static_cast<char>(*it));
		it++;
	}
	auto strPtr = std::make_unique<String>(String::makeString(str));
	// We have to explicitly make this assignment
	// after the above call to makeString() since
	// we're passing a temporary to std::make_unique,
	// which means its string_view becomes invalidated 
	// after the pointer is formed.
	// This keeps the view valid:
	strPtr->value = strPtr->alt;
	return strPtr;
}

// Reconstructor functions.
static std::unordered_map<ObjType, 
	std::function<BaseUP(vByte::const_iterator& it)>> recons = {
		{OBJ_INT,		reconstructInt},
		{OBJ_UINT,		reconstructUInt},
		{OBJ_DEC,		reconstructDec},
		{OBJ_STRING,	reconstructString}
};

vObj reconstructPool(const vByte& poolBytes)
{
	auto it = poolBytes.begin();
	vObj pool;

	while (it != poolBytes.end())
	{
		if (*it == OBJ_BASE)
			it++;
		else
		{
			auto pos = recons.find(ObjType(*it));
			if (pos != recons.end())
			{
				auto& func = pos->second;
				pool.push_back(func(++it));
			}
			else
				std::cout << "Error: byte is " << static_cast<int>(*it) << ".\n";
			it++;
		}
	}

	return pool;
}

ByteCode readCache(std::ifstream& fileIn)
{
	vByte codeBytes;
	std::string funcName;
	vByte poolBytes;

	if (fileIn.is_open())
	{
		int ch{0};

		// Build up bytecode.
		while ((ch = fileIn.get()) != EOF)
		{
			if (ch == 100)
				break;
			codeBytes.push_back(static_cast<ui8>(ch));
		}

		// fileIn.get(); // Skip POOL_START.

		// Store function name.
		while ((ch = fileIn.get()) != EOF)
		{
			if (static_cast<char>(ch) == '\0')
				break;
			funcName.push_back(static_cast<char>(ch));
		}

		bool extraNullBytes = false;
		while (ch == '\0') // In case of extra null bytes.
		{
			extraNullBytes = true;
			ch = fileIn.get();
		}
		if (extraNullBytes)
			fileIn.seekg(-1, std::ios::cur);

		while ((ch = fileIn.get()) != EOF)
			poolBytes.push_back(static_cast<ui8>(ch));

		file = funcName;
		return ByteCode(codeBytes, reconstructPool(poolBytes));
	}

	std::cerr << "File is closed.\n";
	exit(66);
}

void optionLoad(const char* fileName)
{	
	if (!ends_with(fileName, ".bch"))
	{
		std::cerr << "Invalid bytecode file.\n";
		exit(65);
	}

	external = true;

	std::ifstream program(fileName, std::ios::binary);
	if (program.fail())
	{
		std::cerr << "Failed to open file.\n";
		exit(66);
	}

	ByteCode chunk = readCache(program);
	VM vm;
	vm.executeCode(chunk);
}

void optionDis(const char* fileName)
{
	if (!ends_with(fileName, ".bch"))
	{
		std::cerr << "Invalid bytecode file.\n";
		exit(65);
	}

	external = true;

	std::ifstream program(fileName, std::ios::binary);
	if (program.fail())
	{
		std::cerr << "Failed to open file.\n";
		exit(66);
	}

	ByteCode chunk = readCache(program);
	Disassembler dis(chunk);
	dis.disassembleCode();
}

void optionCacheBytes(const ByteCode& chunk, const char* fileName)
{
	std::string outFile(fileName);
	// Cut off extension.
	outFile = outFile.substr(0, outFile.size() - 3);
	// Add new extension.
	outFile += ".bch";
	std::ofstream cacheFile(outFile, std::ios::binary);
	chunk.cacheStream(cacheFile);
}

bool fileNameCheck(std::string_view fileName)
{
	return (ends_with(fileName, ".ch") || ends_with(fileName, ".bch"));
}