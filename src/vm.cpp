#include "../include/vm.h"
#include "../include/bytecode.h"

void VM::fetch()
{

}

void VM::decode()
{

}

void VM::execute()
{

}

void VM::executeCode(const ByteCode& code)
{
    ip = code.block.begin();
    
    while (ip != code.block.end())
    {
        // uint8_t instruction = *ip;
        // ip++;

        // switch (instruction)
        // {
            
        // }

        // fetch(); decode(); execute();
    }
}