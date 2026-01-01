#pragma once
#include "token.h"
#include <string_view>
#include <vector>

struct ClassState
{
    bool inClass;
    ui8 braceCount;
};

class Lexer
{
    private:
        std::string_view code;
        std::vector<Token> stream;
        ui16 line;
        ui8 column;
        ui32 start;
        ui32 current;
        ClassState state;
        // bool inGrouping;

        // Utilities.

        bool hitEnd(); // Check if we've reached the end.
        char advance(); // Move to next character.
        bool checkChar(char c); // Check next character.
        bool consumeChar(char c); // Only advance if char matches.
        void consumeChars(char c, int count = 1);
        char peekChar(int distance = 0);
        char previousChar(int distance = 0);
        TokenType identifierType();
        bool matchSequence(char c, int length);

        // Value conversion methods.

        i64 intValue(std::string_view text);
        double decValue(std::string_view text);
        bool boolValue(TokenType type);

        // Token makers.

        void makeToken(TokenType type);
        void numToken();
        void stringToken();
        void multiStringToken(); // For multi-line strings.
        void identifierToken();
        // For nested comments with ###.
        // Returns true if nested comment
        // was hit, false otherwise.
        bool checkHyperComment();
        void singleToken();
    
    public:
        Lexer();
        std::vector<Token>& tokenize(std::string_view code);
};