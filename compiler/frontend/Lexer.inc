#ifndef __LEXER_INC
#define __LEXER_INC

/* Utility library */
#include <Utils.inc>

/* Error module */
#include "Error.inc"

/* Lexeme types */
#define LEX_KEYWORD       0
#define LEX_IDENTIFIER    1
#define LEX_CHAR_LITERAL  2
#define LEX_STR_LITERAL   3
#define LEX_INT_LITERAL   4
#define LEX_OP            5
#define LEX_COMMENT       6
#define LEX_EOF           7
#define LEX_INVALID       8

/* Lexer states */
#define STATE_START       0
#define STATE_CHAR        1
#define STATE_STRING      2
#define STATE_COMMENT     3
#define STATE_OP          4
#define STATE_ALPHA       5
#define STATE_NUM         6
#define STATE_OCT         7
#define STATE_HEX         8
#define STATE_EOF         9

/* Lexer buffer */
#define LEX_MAX_BUF       4096

/* Lexeme Type */
typ LexemeT: record
(
   Next     : @LexemeT,
   Value    : @StringT,
   Size     : Int32T,
   Type     : Int32T,
   ErrorData: @ErrorDataT
);

/* Lexeme linked list */
dec FirstLexeme: @LexemeT;
dec LastLexeme:  @LexemeT;

/* Interface functions */
dec LexerInit   : func(FileName: @StringT);
dec LexerCleanup: func(                  );
dec LexerPlay   : func(                  );
dec LexerDebug  : func(                  );

#endif /* __LEXER_INC */
