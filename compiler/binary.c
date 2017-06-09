/* binary operations */
#include <string.h>

#include "common.h"

expr_t *parse_muldiv() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* muldiv: unary_pre (('*'|'/'|'%') muldiv | lambda) */
    expr1 = parse_unary_pre();
    /* lookahead */
    get_lexeme();
    /* *, /, or % ? */
    while (!strcmp(lex.val, "*") || 
           !strcmp(lex.val, "/") || 
           !strcmp(lex.val, "%")) {
        /* mul/div/mod operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_unary_pre();
        /* generate code for mul/div/mod operation */
        expr1 = do_binary(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_addsub() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* addsub: muldiv (('+'|'-') addsub | lambda) */
    expr1 = parse_muldiv();
    /* lookahead */
    get_lexeme();
    /* + or - ? */
    while (!strcmp(lex.val, "+") || !strcmp(lex.val, "-")) {
        /* add/sub operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_muldiv();
        /* generate code for add/sub operation */
        expr1 = do_binary(expr1, op, expr2);
        /* get next */
        get_lexeme(); 
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_shift() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* shift: addsub (('<<'|'>>') shift | lambda) */
    expr1 = parse_addsub();
    /* lookahead */
    get_lexeme();
    /* >> or << ? */
    while (!strcmp(lex.val, ">>") || !strcmp(lex.val, "<<")) {
        /* shift operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_addsub();
        /* generate code for shifting */
        expr1 = do_binary(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_relation() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* relation: shift (('>'|'>='|'<'|'<=') relation | lambda) */
    expr1 = parse_shift();
    /* lookahead */
    get_lexeme();
    /* relop ? */
    while (!strcmp(lex.val, ">") || 
           !strcmp(lex.val, ">=") || 
           !strcmp(lex.val, "<") || 
           !strcmp(lex.val, "<=")) {
        /* relation operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_shift();
        /* generate code for relation operator */
        expr1 = do_binary(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_equality() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* equality: relation (('=='|'!=') equality | lambda) */
    expr1 = parse_relation();
    /* lookahead */
    get_lexeme();
    /* == or != ? */
    while (!strcmp(lex.val, "=") || !strcmp(lex.val, "!=")) {
        /* equality */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_relation();
        /* generate code for equality */
        expr1 = do_binary(expr1, op, expr2);        
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_and() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_and: equality ('&' bitwise_and | lambda) */
    expr1 = parse_equality();
    /* lookahead */
    get_lexeme();
    /* &? */
    while (!strcmp(lex.val, "&")) {
        /* bitwise and */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_equality();
        /* generate code for bitwise AND'ing */
        expr1 = do_binary(expr1, op, expr2);        
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_xor() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_xor: bitwise_and ('^' bitwise_xor | lambda) */
    expr1 = parse_bitwise_and();
    /* lookahead */
    get_lexeme();
    /* ^? */
    while (!strcmp(lex.val, "^")) {
        /* bitwise xor */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_bitwise_and();
        /* generate code for bitwise XOR'ing */
        expr1 = do_binary(expr1, op, expr2); 
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_or() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_or: bitwise_xor ('|' bitwise_or | lambda) */
    expr1 = parse_bitwise_xor();
    /* lookahead */
    get_lexeme();
    /* |? */
    while (!strcmp(lex.val, "|")) {
        /* bitwise or */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_bitwise_xor();
        /* generate code for bitwise OR'ing */
        expr1 = do_binary(expr1, op, expr2); 
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}
