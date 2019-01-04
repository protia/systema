#ifndef __AST_INC
#define __AST_INC

/* Utils library */
#include <Utils.inc>

/* We need the "Lexeme" definition */
#include "Lexer.inc"

/* AST node types */
#define AST_FILE          1
#define AST_DIM_LIST      2
#define AST_DIM           3
#define AST_FUNC          4
#define AST_STMT_LIST     5
#define AST_IF            6
#define AST_CASE          7
#define AST_WHEN          8
#define AST_FOR           9
#define AST_WHILE         10
#define AST_EXPR_LIST     11
#define AST_ASSIGN        12
#define AST_CONDITIONAL   13
#define AST_LOGIC_OR      14
#define AST_LOGIC_AND     15
#define AST_BIT_OR        16
#define AST_BIT_XOR       17
#define AST_BIT_AND       18
#define AST_EQUAL         29
#define AST_NOT_EQUAL     20
#define AST_GREATER       21
#define AST_GREATER_EQUAL 22
#define AST_LESS          23
#define AST_LESS_EQUAL    24
#define AST_SHIFT_LEFT    25
#define AST_SHIFT_RIGHT   26
#define AST_ADD           27
#define AST_SUB           28
#define AST_MUL           29
#define AST_DIV           30
#define AST_MOD           31
#define AST_PRE_INC       32
#define AST_PRE_DEC       33
#define AST_POSITIVE      34
#define AST_NEGATIVE      35
#define AST_NOT           36
#define AST_BIT_COMPL     37
#define AST_REFERENCE     38
#define AST_SIZEOF        39
#define AST_UNSIGNED      40
#define AST_POST_INC      41
#define AST_POST_DEC      42
#define AST_DEREFERENCE   43
#define AST_INDEX         44
#define AST_MEMBER        45
#define AST_CALL          46
#define AST_CAST          47
#define AST_IDENTIFIER    48
#define AST_INT_LITERAL   49
#define AST_CHAR_LITERAL  50
#define AST_STR_LITERAL   51
#define AST_ASSEMBLER     52
#define AST_TYPE          53
#define AST_PARAM_LIST    54
#define AST_PARAM         55
#define AST_NODE          56

/* all types */
typ ASTNodeT;
typ ASTParNodeT;
typ ASTParListNodeT;
typ ASTTypeNodeT;
typ ASTExprNodeT;
typ ASTAsmNodeT;
typ ASTStrNodeT;
typ ASTCharNodeT;
typ ASTIntNodeT;
typ ASTIdNodeT;
typ ASTCastNodeT;
typ ASTCallNodeT;
typ ASTMemberNodeT;
typ ASTIndexNodeT;
typ ASTDerefNodeT;
typ ASTPostDecNodeT;
typ ASTPostIncNodeT;
typ ASTUnsignedNodeT;
typ ASTSizeofNodeT;
typ ASTRefNodeT;
typ ASTBitComplNodeT;
typ ASTNotNodeT;
typ ASTNegativeNodeT;
typ ASTPositiveNodeT;
typ ASTPreDecNodeT;
typ ASTPreIncNodeT;
typ ASTModNodeT;
typ ASTDivNodeT;
typ ASTMulNodeT;
typ ASTSubNodeT;
typ ASTAddNodeT;
typ ASTShiftRightNodeT;
typ ASTShiftLeftNodeT;
typ ASTLessEqNodeT;
typ ASTLessNodeT;
typ ASTGreatEqNodeT;
typ ASTGreatNodeT;
typ ASTNotEqualNodeT;
typ ASTEqualNodeT;
typ ASTBitAndNodeT;
typ ASTBitXorNodeT;
typ ASTBitOrNodeT;
typ ASTLogicAndNodeT;
typ ASTLogicOrNodeT;
typ ASTCondNodeT;
typ ASTAssignNodeT;
typ ASTExprListNodeT;
typ ASTStmtNodeT;
typ ASTWhileNodeT;
typ ASTForNodeT;
typ ASTWhenNodeT;
typ ASTCaseNodeT;
typ ASTIfNodeT;
typ ASTStmtListNodeT;
typ ASTFuncNodeT;
typ ASTDimNodeT;
typ ASTDimListNodeT;
typ ASTFileNodeT;

/* General Syntax Tree Node */
typ ASTNodeT: record
(
   Type    : Int32T,
   Lexeme  : @LexemeT
);

/* Param Node */
typ ASTParNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTParNodeT,
   TypeNode    : @ASTTypeNodeT
);

/* Parameter List Node */
typ ASTParListNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   FirstPar    : @ASTParNodeT,
   LastPar     : @ASTParNodeT,
   Arbitrary   : @LexemeT
);

/* Type Node */
typ ASTTypeNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   SubType     : @ASTTypeNodeT,
   ArraySize   : @ASTExprNodeT,
   ParList     : @ASTParListNodeT,
   RetType     : @ASTTypeNodeT
);

/* Expr Node */
typ ASTExprNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTExprNodeT
);

/* Assembler Node */
typ ASTAsmNodeT: record
(
   Super       : ASTExprNodeT
);

/* String Literal Node */
typ ASTStrNodeT: record
(
   Super       : ASTExprNodeT
);

/* Char Literal Node */
typ ASTCharNodeT: record
(
   Super       : ASTExprNodeT
);

/* Integer Literal Node */
typ ASTIntNodeT: record
(
   Super       : ASTExprNodeT
);

/* Identifier Node */
typ ASTIdNodeT: record
(
   Super       : ASTExprNodeT
);

/* Type Casting Node */
typ ASTCastNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT,
   NewType     : @ASTTypeNodeT
);

/* Function Call Node */
typ ASTCallNodeT: record
(
   Super       : ASTExprNodeT,
   FuncExpr    : @ASTExprNodeT,
   ParList     : @ASTExprListNodeT
);

/* Member Node */
typ ASTMemberNodeT: record
(
   Super       : ASTExprNodeT,
   RecordExpr  : @ASTExprNodeT,
   MemberLex   : @LexemeT
);

/* Indexing Node */
typ ASTIndexNodeT: record
(
   Super       : ASTExprNodeT,
   ArrayExpr   : @ASTExprNodeT,
   IndexExpr   : @ASTExprNodeT
);

/* Dereference Node */
typ ASTDerefNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* PostDecr Node */
typ ASTPostDecNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* PostIncr Node */
typ ASTPostIncNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Unsigned Node */
typ ASTUnsignedNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Sizeof Node */
typ ASTSizeofNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Reference Node */
typ ASTRefNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Bitwise Complement Node */
typ ASTBitComplNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Not Node */
typ ASTNotNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Negative Node */
typ ASTNegativeNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Positive Node */
typ ASTPositiveNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Pre Decrement Node */
typ ASTPreDecNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Pre Increment Node */
typ ASTPreIncNodeT: record
(
   Super       : ASTExprNodeT,
   SubExpr     : @ASTExprNodeT
);

/* Mod Node */
typ ASTModNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Div Node */
typ ASTDivNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Mul Node */
typ ASTMulNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Sub Node */
typ ASTSubNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Add Node */
typ ASTAddNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Shift Right Node */
typ ASTShiftRightNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Shift Left Node */
typ ASTShiftLeftNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Less than or Equal Node */
typ ASTLessEqNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Less than Node */
typ ASTLessNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Greater than or Equal Node */
typ ASTGreatEqNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Greater than Node */
typ ASTGreatNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Not Equal Node */
typ ASTNotEqualNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Equal Node */
typ ASTEqualNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Bitwise AND Node */
typ ASTBitAndNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Bitwise XOR Node */
typ ASTBitXorNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Bitwise OR Node */
typ ASTBitOrNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Logic AND Node */
typ ASTLogicAndNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Logic OR Node */
typ ASTLogicOrNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Conditional Node */
typ ASTCondNodeT: record
(
   Super       : ASTExprNodeT,
   CondExpr    : @ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Assign Node */
typ ASTAssignNodeT: record
(
   Super       : ASTExprNodeT,
   LeftExpr    : @ASTExprNodeT,
   RightExpr   : @ASTExprNodeT
);

/* Expr List Node */
typ ASTExprListNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT,
   FirstExpr   : @ASTExprNodeT,
   LastExpr    : @ASTExprNodeT
);

/* Statement Node */
typ ASTStmtNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT
);

/* While Node */
typ ASTWhileNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT,
   CondExpr    : @ASTExprNodeT,
   StmtList    : @ASTStmtListNodeT
);

/* For Node */
typ ASTForNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT,
   CondExpr    : @ASTExprNodeT,
   StartExpr   : @ASTExprNodeT,
   EndExpr     : @ASTExprNodeT,
   StepExpr    : @ASTExprNodeT,
   StmtList    : @ASTStmtListNodeT,
   Down        : @LexemeT,
   Unsigned    : @LexemeT
);

/* When Node */
typ ASTWhenNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTWhenNodeT,
   ExprList    : @ASTExprListNodeT,
   StmtList    : @ASTStmtListNodeT
);

/* Case Node */
typ ASTCaseNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT,
   CondExpr    : @ASTExprNodeT,
   FirstWhen   : @ASTWhenNodeT,
   LastWhen    : @ASTWhenNodeT
);

/* If Node */
typ ASTIfNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTStmtNodeT,
   CondExpr    : @ASTExprNodeT,
   ThenStmts   : @ASTStmtListNodeT,
   ElseStmts   : @ASTStmtListNodeT
);

/* Statement List */
typ ASTStmtListNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   FirstStmt   : @ASTStmtNodeT,
   LastStmt    : @ASTStmtNodeT
);

/* Func Node */
typ ASTFuncNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   DimList     : @ASTDimListNodeT,
   StmtList    : @ASTStmtListNodeT,
   RetExpr     : @ASTExprNodeT
);

/* Dim Node */
typ ASTDimNodeT: record
(
   Type        : Int32T,
   Lexeme      : @LexemeT,
   Next        : @ASTDimNodeT,
   Local       : @LexemeT,
   Identifier  : @LexemeT,
   TypeNode    : @ASTTypeNodeT,
   Location    : @LexemeT,
   Initializer : @ASTExprNodeT,
   FuncNode    : @ASTFuncNodeT
);

/* Dim List Node */
typ ASTDimListNodeT: record
(
   Type    : Int32T,
   Lexeme  : @LexemeT,
   FirstDim: @ASTDimNodeT,
   LastDim : @ASTDimNodeT
);

/* File Node */
typ ASTFileNodeT: record
(
   Type    : Int32T,
   Lexeme  : @LexemeT,
   DimList : @ASTDimListNodeT
);

/* root node */
dec ASTRoot: @ASTNodeT;

/* interface functions */
dec ASTInit   : func();
dec ASTCleanup: func();

#endif /* __AST_INC */