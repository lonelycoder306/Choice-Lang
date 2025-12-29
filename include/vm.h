#pragma once
#include "astnodes.h"
#include "bytecode.h"
#include "common.h"
#include "object.h"
#include "opcodes.h"
#include "vartable.h"
#include <variant>
#include <vector>
using AST::Expression::VarExpr;
using RegVar = std::variant<BaseUP, int, bool, Null>;
class ASTCompiler;

class VM
{
    #define HAS_TYPE(type, val) std::holds_alternative<type>(val)
    #define OBJ_TYPE(obj)       std::get<BaseUP>(obj)->type

    private:
        vByte::const_iterator ip;
        static constexpr int regSize = 256;
        std::vector<RegVar> registers;
        ui8 regSlot;

        // Utilities.

        bool checkNumOper(ui8 slot);
        bool checkNumOpers(ui8 slot1, ui8 slot2);
        void arithOper(Opcode oper);

        // Variables.

        void executeOp(Opcode op);
    
    public:
        VM();
        void executeCode(const ByteCode& code);
        friend class ASTCompiler;
};