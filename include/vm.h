#pragma once
#include "bytecode.h"
#include "common.h"
#include <vector>

class VM
{
    private:
        vByte::const_iterator ip;

        void fetch();
        void decode();
        void execute();
    
    public:
        VM() = default;
        void executeCode(const ByteCode& code);
};