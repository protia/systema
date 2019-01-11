#ifndef __CODEGEN_INC
#define __CODEGEN_INC

/* Utils library */
#include <Utils.inc>

/* AST definitions */
#include "AST.inc"

/* Internal functions */
dec CodegenType: func(Node: @ASTTypeNodeT);
dec CodegenDimList: func(Node: @ASTDimListNodeT);

/* Init functions */
dec CodegenInit: func();
dec CodegenPlay: func();
dec CodegenCleanup: func();

#endif /* __CODEGEN_INC */