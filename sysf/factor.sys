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
    expr->type->subcount = 1;
    expr->type->subtype = alloc_type();
    expr->type->subtype->specifier = TYPE_ARRAY;
    expr->type->subtype->subcount = 0;
    expr->type->subtype->complete = 0;
    expr->type->subtype->subtype = alloc_type();
    expr->type->subtype->subtype->specifier = TYPE_BYTE;
    expr->type->subtype->subtype->complete = 1;
    expr->type->subtype->subtype->subcount = 0;
    expr->type->subtype->subtype->subtype = NULL;
    expr->literal = 1;
    expr->addr = add_str_literal(lex.val);
    return expr;
}

void parse_assembly() {
    char *line;
    /* parse assembly */
    get_lexeme(); 
    /* parse ( */
    get_lexeme();
    if (strcmp(lex.val, "(")) {
        print_err("expected (", 0);
        unget_lexeme();
    }
    /* parse string literal */
    get_lexeme();
    if (lex.type != LEX_STR_LITERAL) {
        print_err("expected string literal", 0);
        unget_lexeme();
    } else {
        line = malloc(strlen(lex.val)+1);
        strcpy(line, lex.val+1);
        line[strlen(line) - 1] = 0;
        emit_line(line);
    }
    /* parse ) */
    get_lexeme();
    if (strcmp(lex.val, ")")) {
        print_err("expected )", 0);
        unget_lexeme();
    }
}

expr_t *parse_factor() {
    int unsignedf;
    /* factor: parent | unsigned | assembly | identifier |
               int_literal | str_literal */
    expr_t *expr;
    /* look ahead */
    get_lexeme();
    unget_lexeme();
    /* parse according to type */
    if (!strcmp(lex.val, "(")) {
        /* parenthesis */
        expr = parse_parent();
    } else if (!strcmp(lex.val, "unsigned")) {
        /* unsigned expression */
        unsignedf = set_unsignedf(1);
        get_lexeme(); /* unsigned */
        expr = parse_expr();
        reset_unsignedf(unsignedf);
    } else if (!strcmp(lex.val, "assembly")) {
        /* inline assembly */
        parse_assembly();
        expr = alloc_expr();
    } else if (lex.type == LEX_IDENTIFIER) {
        /* encountered identifier */
        expr = parse_identifier();
    } else if (lex.type == LEX_INT_LITERAL || 
               lex.type == LEX_CHAR_LITERAL) {
        /* integer literal */
        expr = parse_int_literal();
    } else if (lex.type == LEX_STR_LITERAL) {
        /* string literal */
        expr = parse_str_literal();
    } else {
        print_err("expected identifier, literal, or (", 0);
        expr = alloc_expr();
        get_lexeme();
    }
    return expr;
}

