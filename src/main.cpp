#include "../include/bytecode.h"
#include "../include/common.h"
#ifndef ALT // Testing.
#include "../include/compiler.h"
#include "../include/disasm.h"
#endif
#include "../include/lexer.h"
#include "../include/main_utils.h"
#include "../include/tokprinter.h"
#include "../include/vm.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

// Testing.
#ifdef ALT
#include "../include/altcompiler.h"
#include "../include/altdisasm.h"
#define Compiler AltCompiler
#define Disassembler AltDisassembler
#endif

std::string file = "";
bool external = false;

enum ArgvOption
{
	// Show the tokens for the given 
	// script or REPL input.
	EMIT_TOKENS,

	// Compile and show the bytecode for the
	// given script or REPL input.
	EMIT_BYTECODE,

	// Compile and store the bytecode for the
	// given script in a file.
	CACHE_BYTECODE,

	// Load a bytecode file/program and run it.
	LOAD_PROGRAM,

	// Load a bytecode file/program and display
	// the bytecode held in it.
	DIS_PROGRAM,

	// Entire execution pipeline.
	// Scan, compile, and execute given program
	// or REPL input.
	EXECUTE
};

static std::unordered_map<std::string_view, ArgvOption> options = {
	{"-token", EMIT_TOKENS},
	{"-t", EMIT_TOKENS},

	{"-bytecode", EMIT_BYTECODE},
	{"-b", EMIT_BYTECODE},

	{"-cache", CACHE_BYTECODE},
	{"-c", CACHE_BYTECODE},

	{"-load", LOAD_PROGRAM},
	{"-l", LOAD_PROGRAM},

	{"-dis", DIS_PROGRAM},
	{"-d", DIS_PROGRAM}
};

static void runFile(const char* fileName, ArgvOption option = EXECUTE)
{
	if (!fileNameCheck(fileName))
	{
		std::cerr << "Invalid file name.\n";
		exit(65);
	}
	
	file = std::string(fileName);
	if (option == LOAD_PROGRAM)
	{
		optionLoad(fileName);
		return;
	}

	if (option == DIS_PROGRAM)
	{
		optionDis(fileName);
		return;
	}

	std::string code = readFile(fileName);

	// Performing tokenization outside.
	Lexer lexer;
	vT& tokens = lexer.tokenize(code);
	if (option == EMIT_TOKENS)
	{
		TokenPrinter printer(tokens);
		printer.printTokens();
		return;
	}

	// Perform compilation outside.
	Compiler compiler(tokens);
	ByteCode& chunk = compiler.compile();
	if (option == EMIT_BYTECODE)
	{
		Disassembler dis(chunk);
		dis.disassembleCode();
		return;
	}

	if (option == CACHE_BYTECODE)
	{
		optionCacheBytes(chunk, fileName);
		return;
	}

	// Execution logic.
	VM vm;
	vm.executeCode(chunk);
}

static void repl(ArgvOption option = EXECUTE)
{
	// All invalid options for REPL mode.
	if (option == CACHE_BYTECODE || option == LOAD_PROGRAM ||
		option == DIS_PROGRAM)
	{
		std::cerr << "Invalid command-line option for REPL mode.\n";
		exit(64);
	}
	
	file = "";

	std::string line;
	while (true)
	{
		std::cout << ">>> ";
		std::getline(std::cin, line);

		if (!line.empty())
		{
			Lexer lexer;
			vT& tokens = lexer.tokenize(line);
			if (option == EMIT_TOKENS)
			{
				TokenPrinter printer(tokens);
				printer.printTokens();
				continue;
			}

			Compiler compiler(tokens);
			ByteCode& chunk = compiler.compile();
			if (option == EMIT_BYTECODE)
			{
				Disassembler dis(chunk);
				dis.disassembleCode();
				continue;
			}

			VM vm;
			vm.executeCode(chunk);
		}
		else
			break;
	}
}

int main(int argc, const char* argv[])
{   
	if (argc == 3)
		runFile(argv[2], options[argv[1]]);
	else if (argc == 2)
	{
		auto it = options.find(argv[1]);
		if (it != options.end())
			repl(it->second);
		else
			runFile(argv[1]);
	}
	else
		repl();
	
	return 0;
}