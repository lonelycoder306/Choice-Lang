#pragma once
#include "token.h"
#include <string_view>

class Error
{
	protected:
		std::string message;

	public:
		Error() = default;
		Error(const std::string& message);
};

class LexError : public Error
{
	private:
		char errorChar;
		ui16 line;
		ui8 position;

	public:
		LexError() = default;
		LexError(char c, ui16 line, ui8 position,
			const std::string& message);

		void report();
};

class CompileError : public Error
{
	private:
		Token token;

	public:
		CompileError(const Token& token, const std::string& message);

		void report();
};

class RuntimeError : public Error {};
class TypeError : public Error {}; // For static type-checking.
class CodeError : public Error {}; // For invalid externally-loaded byte-code.