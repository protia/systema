#ifndef __SEMANTIC_INC
#define __SEMANTIC_INC

/* Utils library */
#include <Utils.inc>

/* AST definitions */
#include "AST.inc"

/* error messages */
#define SERR_LOCAL_MEAN   "Usage of 'local' with dec/typ is meaningless."
#define SERR_LOCAL_SCOPE  "You can't use 'local' in this scope."
#define SERR_TYPE_DECL    "You have to specify some type."
#define SERR_ARR_LITERAL  "Array size must be literal."
#define SERR_ARR_COMPL    "Array subtype must be complete."
#define SERR_PAR_NAME     "Name is already used by another parameter."
#define SERR_PAR_COMPL    "Parameter type must be complete."
#define SERR_RET_COMPL    "Return type must be complete."
#define SERR_DEF_COMPL    "Definition type must be complete."
#define SERR_DEC_COMPL    "Declaration type must be complete."
#define SERR_TYPE_BUG     "Bug - Unknown Type."
#define SERR_FUNC_REC     "Functions in functions are currently not supported."
#define SERR_FUNC_IN      "You can't use 'in' with function definitions."
#define SERR_FUNC_INIT    "Function's can't be initialized that way."
#define SERR_FUNC_DEC     "Please use 'dec' for functions with no implementation."
#define SERR_FUNC_DEF     "Please use 'def' for functions with implementation."
#define SERR_STOR_SCOPE   "This storage class can't be used in this scope."
#define SERR_STOR_INIT    "This storage class can't hold initialized data."
#define SERR_INIT_LIT     "Initializer expression must be literal."
#define SERR_DEC_INIT     "You can't initialize a declaration with 'dec'."
#define SERR_DEC_LOC      "You can't specifiy a storage class with 'dec'."
#define SERR_TYP_LOC      "You can't specifiy a storage class with 'typ'."
#define SERR_DEF_BEF      "Identifier was defined before."
#define SERR_DEC_MIS      "Identifier was declared before with different type."
#define SERR_NOT_TYP      "Identifier was not declared as type."

/* internal functions */
dec SemanticType: func(Node: @ASTTypeNodeT);
dec SemanticDimList: func(Node: @ASTDimListNodeT);

/* Init functions */
dec SemanticInit: func();
dec SemanticPlay: func();
dec SemanticCleanup: func();

#endif /* __SEMANTIC_INC */