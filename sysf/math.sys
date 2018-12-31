#include <string.h>
#include <stdlib.h>

#include "common.h"

expr_t *parse_unary_post() {
    expr_t *expr, *expr1;
    int done = 0;
    char op[10];
    /* unary_post: factor post_op* */
    expr = parse_factor();
    /* loop over post operators */
    get_lexeme();
    done = 0;
    while(!done) {
        if (!strcmp(lex.val, "++") ||
            !strcmp(lex.val, "--") ||
            !strcmp(lex.val, "@")) {
            /* copy operator */
            strcpy(op, lex.val);
            /* perform operation */
            expr = do_unary(expr, op, 1);
        } else if (!strcmp(lex.val, "(")) {
            /* function call */
            if (expr->type->specifier != TYPE_FUNC) {
                print_err("error: call to a non-function", 0);
                done = 1;
            } else {
                unget_lexeme();
                expr = parse_func_call(expr);
            }
        } else if (!strcmp(lex.val, "[")) {
            /* read array index */
            expr1 = parse_expr();
            /* read ] */
            get_lexeme();
            if (strcmp(lex.val, "]")) {
                unget_lexeme();
                print_err("expected ]", 0);
            }
            /* resolve array subscript */
            expr = do_binary(expr, "[", expr1);
        } else if (!strcmp(lex.val, ".")) {
            /* record member */
            get_lexeme();
            if (lex.type != LEX_IDENTIFIER) {
                print_err("expected identifier", NULL);
                unget_lexeme();
            } else {
                /* enclose the lexeme in an expression */
                expr1 = alloc_expr();
                expr1->addr = malloc(strlen(lex.val)+1);
                strcpy(expr1->addr, lex.val);
                /* resolve record member operation */
                expr = do_binary(expr, ".", expr1);
            }
        } else {
            /* no more */
            unget_lexeme();
            done = 1;
        }
        /* next thing */
        if (!done) {
            get_lexeme();
        }
    }
    /* done */
    return expr;
}

expr_t *parse_unary_pre() {
    expr_t *expr;
    char op[10];
    /* unary_pre: (unary_op unary_pre | unary_post) */
    get_lexeme();
    if (!strcmp(lex.val, "++") ||
        !strcmp(lex.val, "--") ||
        !strcmp(lex.val, "+")  ||
        !strcmp(lex.val, "-")  ||
        !strcmp(lex.val, "!")  ||
        !strcmp(lex.val, "~")  ||
        !strcmp(lex.val, "&")) {
        /* copy operator */
        strcpy(op, lex.val);
        /* parse next unary_pre expression */
        expr = parse_unary_pre();
        /* perform operation */
        expr = do_unary(expr, op, 0);
    } else {
        /* no unary operator */
        unget_lexeme();
        expr = parse_unary_post();
    }
    /* done */
    return expr;
}

expr_t *parse_cast() {
    expr_t *expr;
    type_t *type;
    int done = 0;        
    /* cast: unary_pre (as type)* */
    expr = parse_unary_pre();
    /* perform all casts */
    while (!done) {
        get_lexeme();
        if (!strcmp(lex.val, "as")) {
            /* type casting! */
            type = parse_type();
            expr = type_cast(expr, type);  
        } else {
            unget_lexeme();
            done = 1;
        }
    }
    /* finished */
    return expr;
}

expr_t *parse_muldiv() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* muldiv: cast (('*'|'/'|'%') muldiv | lambda) */
    expr1 = parse_cast();
    /* lookahead */
    get_lexeme();
    /* *, /, or % ? */
    while (!strcmp(lex.val, "*") || 
           !strcmp(lex.val, "/") || 
           !strcmp(lex.val, "%")) {
        /* mul/div/mod operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_cast();
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
    while (!strcmp(lex.val, "==") || !strcmp(lex.val, "!=")) {
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

