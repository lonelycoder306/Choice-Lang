#include "../include/vm.h"
#include "../include/opcodes.h"
#include <cstring>
using namespace AST::Statement;
using namespace AST::Expression;

VM::VM() :
    registers(regSize), regSlot(0) {}

bool VM::checkNumOper(ui8 slot)
{
    if (registers[slot].index() == 0) // BaseUP.
    {
        ObjType type = std::get<BaseUP>(registers[slot])->type;
        if (IS_NUM(type))
            return true;
    }
    else if (registers[slot].index() == 1) // Integer.
        return true;

    return false;
}

bool VM::checkNumOpers(ui8 slot1, ui8 slot2)
{
    return (checkNumOper(slot1) && checkNumOper(slot2));
}

void VM::arithOper(Opcode oper)
{
    ui8 dest = *(++ip);
    ui8 oper1 = *(++ip);
    ui8 oper2 = *(++ip);

    // Temporary.
    (void) dest;
    (void) oper;

    if (checkNumOpers(oper1, oper2))
    {

    }
}

void VM::executeOp(Opcode op)
{
    switch (op)
    {
        // Basic values.

        case OP_ZERO:       registers[regSlot++] = 0;       ip++;   break;
        case OP_ONE:        registers[regSlot++] = 1;       ip++;   break;
        case OP_TWO:        registers[regSlot++] = 2;       ip++;   break;
        case OP_NEG_ONE:    registers[regSlot++] = -1;      ip++;   break;
        case OP_NEG_TWO:    registers[regSlot++] = -2;      ip++;   break;
        case OP_TRUE:       registers[regSlot++] = true;    ip++;   break;
        case OP_FALSE:      registers[regSlot++] = false;   ip++;   break;
        case OP_NULL:       registers[regSlot++] = Null();  ip++;   break;
        case OP_CONST:
        {

        }
        
        // Arithmetic operations.

        case OP_ADD:    arithOper(OP_ADD);  break;
        case OP_SUB:    arithOper(OP_SUB);  break;
        case OP_MULT:   arithOper(OP_MULT); break;
        case OP_DIV:    arithOper(OP_DIV);  break;
        case OP_NEGATE: ip++;   break;
        case OP_POWER:  ip++;   break;
        case OP_MOD:    arithOper(OP_MOD);  break;

        default: break;
    }
}

void VM::executeCode(const ByteCode& code)
{
    ip = code.block.begin();
    auto end = code.block.end();
    
    while (ip < end)
    {
        ui8 op = *ip;
        executeOp(static_cast<Opcode>(op));
    }
}