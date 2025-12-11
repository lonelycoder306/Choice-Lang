#pragma once
#include "token.h"
#include <string_view>

class Error
{
	protected:
		std::string_view message;

	public:
		Error() = default;
		Error(std::string_view message);
};

class LexError : public Error
{
	private:
		char errorChar;
		ui16 line;
		ui8 position;

	public:
		LexError() = default;
		LexError(char c, ui16 line, ui8 position, std::string_view message);

		void report();
};

class CompileError : public Error
{
	private:
		const Token& token;

	public:
		CompileError(const Token& token, std::string_view message);

		void report();
};

class RuntimeError : public Error {};
class TypeError : public Error {}; // For static type-checking.
class CodeError : public Error {}; // For invalid externally-loaded byte-code.