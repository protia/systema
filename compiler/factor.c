/* factor evaluation */

#include <stdlib.h>
#include <string.h>

#include "common.h"

expr_t *parse_parent() {
    expr_t *expr;
    get_lexeme(); /* ( */
    expr = parse_expr();
    get_lexeme(); /* ) */
    if (strcmp(lex.val, ")")) {
        print_err("expected )", 0);
        unget_lexeme();
    }
    return expr;
}

expr_t *parse_identifier() {
    /* create expression from identifier */
    expr_t *expr;
    sym_t *sym;
    get_lexeme();
    /* evaluate */
    expr = alloc_expr();
    sym = get_sym(lex.val); 
    if (!sym) {
        print_err("undeclared identifier: %s", lex.val);
        expr->addr = malloc(strlen(lex.val)+1);
        strcpy(expr->addr, lex.val);
     } else {
        expr->addr = malloc(strlen(sym->val)+1);
        strcpy(expr->addr, sym->val);
        expr->type = sym->type;
    }
    expr->lvalue = 1;
    return expr;
}

expr_t *parse_int_literal() {
    /* create literal expression from integer literal */
    expr_t *expr;
    get_lexeme();
    /* create expr structure */
    expr = alloc_expr();
    expr->type->specifier = TYPE_WORD;
    expr->type->complete  = 1;
    expr->literal         = 1;
    expr->word_literal_val = eval_literal_int(lex.val);
    return expr;
}

expr_t *parse_str_literal() {
    /* create literal expression from string literal */
    expr_t *expr;
    get_lexeme();
    /* create expr structure */
    expr = alloc_expr();
    expr->type->specifier = TYPE_PTR;
    expr->type->complete = 1;
    expr->type->subtype = alloc_type();
    expr->type->subtype->specifier = TYPE_ARRAY;
    expr->type->subtype->subtype = alloc_type();
    expr->type->subtype->subtype->specifier = TYPE_BYTE;
    expr->type->subtype->subtype->complete = 1;
    expr->literal = 1;
    expr->addr = get_new_label();
    emit_section(STORE_RODATA);
    emit_label(expr->addr);
    emit_string(lex.val);
    if (get_scope())
        emit_section(STORE_CODE);
    return expr;
}

expr_t *parse_factor() {
    /* factor: parent | identifier | int_literal | str_literal | func */
    expr_t *expr;
    /* look ahead */
    get_lexeme();
    unget_lexeme();
    /* parse according to type */
    if (!strcmp(lex.val, "(")) {
        /* parenthesis */
        expr = parse_parent();
    } else if (lex.type == LEX_IDENTIFIER) {
        /* encountered identifier */
        unget_lexeme();
        expr = parse_identifier();
    } else if (lex.type == LEX_INT_LITERAL || 
               lex.type == LEX_CHAR_LITERAL) {
        /* integer literal */
        unget_lexeme();
        expr = parse_int_literal();
    } else if (lex.type == LEX_STR_LITERAL) {
        /* string literal */
        unget_lexeme();
        expr = parse_str_literal();
    } else {
        print_err("expected identifier, literal, or (", 0);
        expr = alloc_expr();
        get_lexeme();
    }
    return expr;
}
