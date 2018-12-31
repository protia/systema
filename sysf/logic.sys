#include <string.h>

#include "common.h"

expr_t *parse_logic_and() {
    expr_t *expr, *op1, *op2, *res;
    expr_t *expr0, *expr1;
    char *lbl1, *lbl2;
    int val1, val2;
    int err = 0, gen_code = 0;
    int reg = emit_get_reg(REG_ACC, 0);
    /* logic_and: bitwise_or ('and' logic_and | lambda) */
    op1 = parse_bitwise_or();
    /* literal 0 */
    expr0 = alloc_expr();
    expr0->literal = 1;
    expr0->type->specifier = TYPE_WORD;
    expr0->word_literal_val = 0;
    /* literal 1 */
    expr1 = alloc_expr();
    expr1->literal = 1;
    expr1->type->specifier = TYPE_WORD;
    expr1->word_literal_val = 1;
    /* lookahead */
    get_lexeme();
    /* and? */
    while (!strcmp(lex.val, "and")) {
        /* generate labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* container for result */
        res = alloc_expr();
        res->type->specifier = TYPE_WORD;
        res->type->complete = 1;
        /* check type 1 */
        if (op1->type->specifier != TYPE_BYTE &&
            op1->type->specifier != TYPE_HALF &&
            op1->type->specifier != TYPE_WORD &&
            op1->type->specifier != TYPE_DOBL &&
            op1->type->specifier != TYPE_PTR) {
            print_err("unsupported types for and", 0);
            err = 1;
        } else {
            if (op1->literal) {
                /* prepare val1 */
                op1 = type_cast(op1, res->type);
                val1 = op1->word_literal_val;
            } else {
                /* generate code */
                emit_load(op1, reg);
                emit_bze(op1->type, reg, lbl1);
            }
        }
        /* parse second expr */
        op2 = parse_bitwise_or();
        /* check type 2 */
        if (op2->type->specifier != TYPE_BYTE &&
            op2->type->specifier != TYPE_HALF &&
            op2->type->specifier != TYPE_WORD &&
            op2->type->specifier != TYPE_DOBL &&
            op2->type->specifier != TYPE_PTR) {
            print_err("unsupported types for and", 0);
            err = 1;
        } else if (!err) {
            if (op2->literal) {
                /* prepare val2 */
                op2 = type_cast(op2, res->type);
                val2  = op2->word_literal_val;
                if (op1->literal) {
                    /* both expressions are literals */
                    res->literal = 1;
                    res->word_literal_val = val1 && val2;
                    gen_code = 0;
                } else {
                    /* op1 is not literal but op2 is */
                    gen_code = 1;
                }
            } else {
                if (op1->literal) {
                    /* op1 is literal but op2 is not */
                    if (val1 == 1) {
                        gen_code = 1;
                    } else {
                        /* always false */
                        res->literal = 1;
                        res->word_literal_val = 0;
                        gen_code = 0;
                    }
                } else {
                    /* op1 is not literal and op2 is not */
                    gen_code = 1;
                }
            }
            /* generate code */
            if (gen_code) {
                res->addr = get_new_addr(type_size(res->type));
                emit_load(op2, reg);
                emit_bze(op2->type, reg, lbl1);
                emit_load(expr1, reg);
                emit_jmp(lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg);
                emit_label(lbl2);
                emit_store(reg, res);
            }
        }
        /* update op1 */
        op1 = res;
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = op1;
    /* done */
    return expr;
}

expr_t *parse_logic_or() {
    expr_t *expr, *op1, *op2, *res;
    expr_t *expr0, *expr1;
    char *lbl1, *lbl2;
    int val1, val2;
    int err = 0, gen_code = 0;
    int reg = emit_get_reg(REG_ACC, 0);
    /* logic_or: logic_and ('or' logic_or | lambda) */
    op1 = parse_logic_and();
    /* literal 0 */
    expr0 = alloc_expr();
    expr0->literal = 1;
    expr0->type->specifier = TYPE_WORD;
    expr0->word_literal_val = 0;
    /* literal 1 */
    expr1 = alloc_expr();
    expr1->literal = 1;
    expr1->type->specifier = TYPE_WORD;
    expr1->word_literal_val = 1;
    /* lookahead */
    get_lexeme();
    /* or? */
    while (!strcmp(lex.val, "or")) {
        /* generate labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* container for result */
        res = alloc_expr();
        res->type->specifier = TYPE_WORD;
        res->type->complete = 1;
        /* check type 1 */
        if (op1->type->specifier != TYPE_BYTE &&
            op1->type->specifier != TYPE_HALF &&
            op1->type->specifier != TYPE_WORD &&
            op1->type->specifier != TYPE_DOBL &&
            op1->type->specifier != TYPE_PTR) {
            print_err("unsupported types for and", 0);
            err = 1;
        } else {
            if (op1->literal) {
                /* prepare val1 */
                op1 = type_cast(op1, res->type);
                val1 = op1->word_literal_val;
            } else {
                /* generate code */
                emit_load(op1, reg);
                emit_bnz(op1->type, reg, lbl1);
            }
        }
        /* parse second expr */
        op2 = parse_bitwise_or();
        /* check type 2 */
        if (op2->type->specifier != TYPE_BYTE &&
            op2->type->specifier != TYPE_HALF &&
            op2->type->specifier != TYPE_WORD &&
            op2->type->specifier != TYPE_DOBL &&
            op2->type->specifier != TYPE_PTR) {
            print_err("unsupported types for and", 0);
            err = 1;
        } else if (!err) {
            if (op2->literal) {
                /* prepare val2 */
                op2 = type_cast(op2, res->type);
                val2  = op2->word_literal_val;
                if (op1->literal) {
                    /* both expressions are literals */
                    res->literal = 1;
                    res->word_literal_val = val1 || val2;
                    gen_code = 0;
                } else {
                    /* op1 is not literal but op2 is */
                    gen_code = 1;
                }
            } else {
                if (op1->literal) {
                    /* op1 is literal but op2 is not */
                    if (val1 == 0) {
                        gen_code = 1;
                    } else {
                        /* always true */
                        res->literal = 1;
                        res->word_literal_val = 1;
                        gen_code = 0;
                    }
                } else {
                    /* op1 is not literal and op2 is not */
                    gen_code = 1;
                }
            }
            /* generate code */
            if (gen_code) {
                res->addr = get_new_addr(type_size(res->type));
                emit_load(op2, reg);
                emit_bnz(op2->type, reg, lbl1);
                emit_load(expr0, reg);
                emit_jmp(lbl2);
                emit_label(lbl1);
                emit_load(expr1, reg);
                emit_label(lbl2);
                emit_store(reg, res);
            }
        }
        /* update op1 */
        op1 = res;
        /* move to next */
        get_lexeme();
    }
    /* no more */       
    unget_lexeme();
    expr = op1;
    /* done */
    return expr;
}

