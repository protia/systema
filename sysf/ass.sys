#include <string.h>

#include "common.h"

expr_t *parse_assign() {
    expr_t *expr, *expr1;
    char op[10]; 
    /* assign: conditional ('=' assign | lambda) */
    expr = parse_conditional();
    /* lookahead */
    get_lexeme();
    /* =? */
    if (!strcmp(lex.val, ":=") ||
        !strcmp(lex.val, "+=") ||
        !strcmp(lex.val, "-=") ||
        !strcmp(lex.val, "*=") ||
        !strcmp(lex.val, "/=") ||
        !strcmp(lex.val, "%=") ||
        !strcmp(lex.val, "&=") ||
        !strcmp(lex.val, "|=") ||
        !strcmp(lex.val, "^=") ||
        !strcmp(lex.val, ">>=") ||
        !strcmp(lex.val, "<<=")) {
        /* assignment */
        strcpy(op, lex.val);
        op[strlen(op)-1] = 0;
        /* evaluate the value */
        expr1 = parse_assign();
        /* generate code for assignment */
        if (!strcmp(op, ":")) {
            /* normal assignment */
            do_assign(expr, expr1);
        } else {
            /* do operation, then assign */
            expr1 = do_binary(expr, op, expr1);
            do_assign(expr, expr1);
        }
    } else {
        unget_lexeme();
    }
    /* done */
    return expr;
}

