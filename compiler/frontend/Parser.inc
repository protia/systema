#ifndef __PARSER_INC
#define __PARSER_INC

/* Utils library */
#include <Utils.inc>

/* AST definitions */
#include "AST.inc"

/* internal functions */
dec ParserError      : func(Msg     : @StringT)                 ;
dec ParserLex        : func(LexType : Int32T  ) returns @LexemeT;
dec ParserKeyword    : func(Keyword : @StringT) returns @LexemeT;
dec ParserOperator   : func(Operator: @StringT) returns @LexemeT;

/* AST creation functions */
dec ParserPar        : func() returns @ASTParNodeT     ;
dec ParserParList    : func() returns @ASTParListNodeT ;
dec ParserType       : func() returns @ASTTypeNodeT    ;
dec ParserFactor     : func() returns @ASTExprNodeT    ;
dec ParserUnaryPost  : func() returns @ASTExprNodeT    ;
dec ParserUnaryPre   : func() returns @ASTExprNodeT    ;
dec ParserMulDiv     : func() returns @ASTExprNodeT    ;
dec ParserAddSub     : func() returns @ASTExprNodeT    ;
dec ParserShift      : func() returns @ASTExprNodeT    ;
dec ParserRelation   : func() returns @ASTExprNodeT    ;
dec ParserEquality   : func() returns @ASTExprNodeT    ;
dec ParserBitwiseAnd : func() returns @ASTExprNodeT    ;
dec ParserBitwiseXor : func() returns @ASTExprNodeT    ;
dec ParserBitwiseOr  : func() returns @ASTExprNodeT    ;
dec ParserLogicAnd   : func() returns @ASTExprNodeT    ;
dec ParserLogicOr    : func() returns @ASTExprNodeT    ;
dec ParserConditional: func() returns @ASTExprNodeT    ;
dec ParserAssign     : func() returns @ASTExprNodeT    ;
dec ParserExpr       : func() returns @ASTExprNodeT    ;
dec ParserExprList   : func() returns @ASTStmtNodeT    ;
dec ParserWhile      : func() returns @ASTStmtNodeT    ;
dec ParserFor        : func() returns @ASTStmtNodeT    ;
dec ParserWhen       : func() returns @ASTWhenNodeT    ;
dec ParserCase       : func() returns @ASTStmtNodeT    ;
dec ParserIf         : func() returns @ASTStmtNodeT    ;
dec ParserStmtList   : func() returns @ASTStmtListNodeT;
dec ParserFunc       : func() returns @ASTFuncNodeT    ;
dec ParseDim         : func() returns @ASTDimNodeT     ;
dec ParserDimList    : func() returns @ASTDimListNodeT ;
dec ParserFile       : func() returns @ASTFileNodeT    ;

/* Init functions */
dec ParserInit   : func();
dec ParserPlay   : func();
dec ParserCleanup: func();

#endif