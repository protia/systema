/* logic operations */
#include <string.h>

#include "common.h"

expr_t *parse_logic_and() {
    expr_t *expr, *expr1, *expr2, *expr3;
    char *lbl1, *lbl2;
    unsigned long long val1, val2;
    int err = 0, gen_code = 0;
    /* logic_and: bitwise_or ('&&' logic_and | lambda) */
    expr1 = parse_bitwise_or();
    /* lookahead */
    get_lexeme();
    /* &&? */
    while (!strcmp(lex.val, "&&")) {
        /* generate labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* container for result */
        expr3 = alloc_expr();
        expr3->type->specifier = TYPE_WORD;
        expr3->type->complete = 1;
        /* check type 1 */
        if (expr1->type->specifier != TYPE_BYTE &&
            expr1->type->specifier != TYPE_HALF &&
            expr1->type->specifier != TYPE_WORD &&
            expr1->type->specifier != TYPE_DOBL &&
            expr1->type->specifier != TYPE_PTR) {
            print_err("unsupported types for &&", 0);
            err = 1;
        } else {
            if (expr1->literal) {
                /*TODO*/
                /*val1 = eval_int(expr1->val);*/
            } else {
                /* generate code */
#if 0
                emit_branch_if_zero(expr1->type, expr1->addr, lbl1);
#endif
            }
        }
        /* parse second expr */
        expr2 = parse_bitwise_or();
        /* check type 2 */
        if (expr2->type->specifier != TYPE_BYTE &&
            expr2->type->specifier != TYPE_HALF &&
            expr2->type->specifier != TYPE_WORD &&
            expr2->type->specifier != TYPE_DOBL &&
            expr2->type->specifier != TYPE_PTR) {
            print_err("unsupported types for &&", 0);
            err = 1;
        } else if (!err) {
            if (expr2->literal) {
                /*TODO*/
                /*val2 = eval_int(expr1->val);*/
                if (expr1->literal) {
                    /* both expressions are literals */
                    expr3->literal = 1;
                    if (val1 && val2) {
                        //expr3->val = "1";
                    } else {
                        //expr3->val = "0";
                    }
                } else {
                    /* expr1 is not literal but expr2 is */
                    if (val2 == 1) {
                        expr3 = expr1;
                    } else {
                        /* always false */
                        //expr->val = "0";
                        expr->literal = 1;
                    }
                }
            } else {
                if (expr1->literal) {
                    /* expr1 is literal but expr2 is not */
                    if (val1) {
                        gen_code = 1;
                    } else {
                        /* always false */
                        expr3->literal = 1;
                        //expr3->val = "0";
                    }
                } else {
                    /* expr1 is not literal and expr2 is not */
                    gen_code = 1;
                }
            }
            /* generate code */
            if (gen_code) {
#if 0
                expr3->addr = get_new_addr(expr3->type);
                emit_branch_if_zero(expr2->type, expr2->addr, lbl1);
                emit_set_literal(expr3->type, 1, expr3->addr);
                emit_jmp(lbl2);
                emit_label(lbl1);
                emit_set_literal(expr3->type, 0, expr3->addr);
                emit_label(lbl2);
#endif
            }
        }
        /* update expr1 */
        expr1 = expr3;
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_logic_or() {
    expr_t *expr, *expr1, *expr2, *expr3;
    char *lbl1, *lbl2;
    unsigned long long val1, val2;
    int err = 0, gen_code = 0;
    /* logic_or: logic_and ('||' logic_or | lambda) */
    expr1 = parse_logic_and();
    /* lookahead */
    get_lexeme();
    /* ||? */
    while (!strcmp(lex.val, "||")) {
        /* generate labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* container for result */
        expr3 = alloc_expr();
        expr3->type->specifier = TYPE_WORD;
        expr3->type->complete = 1;
        /* check type 1 */
        if (expr1->type->specifier != TYPE_BYTE &&
            expr1->type->specifier != TYPE_HALF &&
            expr1->type->specifier != TYPE_WORD &&
            expr1->type->specifier != TYPE_DOBL &&
            expr1->type->specifier != TYPE_PTR) {
            print_err("unsupported types for ||", 0);
            err = 1;
        } else {
            if (expr1->literal) {
                /*TODO*/
                /*val1 = eval_int(expr1->val);*/
            } else {
                /* generate code */
#if 0
                emit_branch_if_nonzero(expr1->type, expr1->addr, lbl1);
#endif
            }
        }
        /* parse second expr */
        expr2 = parse_logic_and();
        /* check type 2 */
        if (expr2->type->specifier != TYPE_BYTE &&
            expr2->type->specifier != TYPE_HALF &&
            expr2->type->specifier != TYPE_WORD &&
            expr2->type->specifier != TYPE_DOBL &&
            expr2->type->specifier != TYPE_PTR) {
            print_err("unsupported types for ||", 0);
            err = 1;
        } else if (!err) {
            if (expr2->literal) {
                /*TODO*/
                /*val2 = eval_int(expr1->val);*/
                if (expr1->literal) {
                    /* both expressions are literals */
                    expr3->literal = 1;
                    if (val1 || val2) {
                        //expr3->val = "1";
                    } else {
                        //expr3->val = "0";
                    }
                } else {
                    /* expr1 is not literal but expr2 is */
                    if (val2 == 0) {
                        expr3 = expr1;
                    } else {
                        /* always true */
                        //expr->val = "1";
                        expr->literal = 1;
                    }
                }
            } else {
                if (expr1->literal) {
                    /* expr1 is literal but expr2 is not */
                    if (!val1) {
                        gen_code = 1;
                    } else {
                        /* always true */
                        expr3->literal = 1;
                        //expr3->val = "1";
                    }
                } else {
                    /* expr1 is not literal and expr2 is not */
                    gen_code = 1;
                }
            }
            /* generate code */
            if (gen_code) {
#if 0
                expr3->addr = get_new_addr(expr3->type);
                emit_branch_if_nonzero(expr2->type, expr2->addr, lbl1);
                emit_set_literal(expr3->type, 0, expr3->addr);
                emit_jmp(lbl2);
                emit_label(lbl1);
                emit_set_literal(expr3->type, 1, expr3->addr);
                emit_label(lbl2);
#endif
            }
        }
        /* update expr1 */
        expr1 = expr3;
        /* move to next */
        get_lexeme();
    }
    /* no more */       
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}
