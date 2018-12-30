#include <string.h>

#include "common.h"

expr_list_t *parse_expr_list() {
    /* expr_list: expr ',' expr_list | lambda */
    expr_list_t *expr_list = alloc_expr_list();
    /* next token is ;, ), or : ? */
    get_lexeme();
    unget_lexeme();
    if (!strcmp(lex.val, ";") || 
        !strcmp(lex.val, ")") ||
        !strcmp(lex.val, ":")) {
        /* empty expr list */
    } else {
        /* evaluate expr */
        expr_list->expr = parse_expr();
        /* read comma */
        get_lexeme();
        if (!strcmp(lex.val, ",")) {
            /* more expressions are following */
            expr_list->sublist = parse_expr_list();
        } else {
            /* we are done */
            unget_lexeme();
            expr_list->sublist = alloc_expr_list();
        }
        /* update count */
        expr_list->count = expr_list->sublist->count + 1;
    }
    return expr_list;
}

