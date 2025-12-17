#pragma once
#include "astnodes.h"
#include "bytecode.h"
#include "vartable.h"
#include "vm.h"
#include <memory>
using namespace AST::Statement;
using namespace AST::Expression;

// Using PIMPL idiom.
class ASTCompVarsWrapper;

class ASTCompiler
{
    #define DECL(type)  void compile##type(UP(type) node)
    #define DEF(type)   void ASTCompiler::compile##type(UP(type) node)
    #define COMPILE(type, node) \
        do { \
            type* ptr = static_cast<type*>(node.release()); \
            compile##type(UP(type)(ptr)); \
        } while (false)
    
    #define GET_TOK_V(token, type) GETV(token.content, type)
    #define VAL_PTR(val, type) std::make_unique<type>(val)
    #define TOK_VAL_PTR(token, origType, newType) \
		VAL_PTR(GET_TOK_V(token, origType), newType)

    #define GET_NUM(token)  GET_TOK_V(token, NumLiteral)
    #define GET_SIZE(token) GET_NUM(token).size
    #define GET_VAL(token, type) GETV(GET_NUM(token).value, type)

    #define INT_TYPE(size)  Int<i##size>
    #define UINT_TYPE(size) UInt<ui##size>
    
    private:
        ByteCode code;
        ui8 previousReg;
        ui8 currentReg;
        // The last register that contains a variable (initially 0).
        ui8 lastVarReg;
        ui8 scope; // Our current lexical scope depth.
        ASTCompVarsWrapper* varsWrapper;
        std::vector<std::vector<std::string>> varScopes;

        // Variables.

        void defVar(std::string name, ui8 reg, ui8 scope);
        ui8* getVarSlot(const Token& token, ui8 scope);
        ui8* getVarSlot(ExprUP& node, ui8 scope);
        void popScope(ui8 scope);

        // Registers.

        void freeReg();
        void reserveReg();

        // Declarations.

        DECL(VarDecl);
        DECL(FunDecl);
        DECL(ClassDecl);

        // Statements.

        DECL(ReturnStmt);
        DECL(LoopStmt);
        DECL(ExprStmt);
        DECL(BlockStmt);

        // Expressions.

        DECL(AssignExpr);
        DECL(LogicExpr);
        DECL(CompareExpr);
        DECL(BitExpr);
        DECL(ShiftExpr);
        DECL(BinaryExpr);
        DECL(UnaryExpr);
        DECL(CallExpr);
        DECL(VarExpr);
        DECL(LiteralExpr);

        void compileExpr(ExprUP& node);
        void compileStmt(StmtUP& node);

    public:
        ASTCompiler();
        ~ASTCompiler();

        ByteCode& compile(StmtVec& program);
};