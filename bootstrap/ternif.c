/* ternary if */
#include <string.h>

#include "common.h"

expr_t *parse_conditional() {
    expr_t *expr, *expr1, *expr2, *expr3;
    char *lbl1, *lbl2;
    int reg = emit_get_reg(REG_ACC, 0);
    /* conditional: logic_or [? expr : conditional] */
    expr = parse_logic_or();
    /* lookahead ternary if sign */
    get_lexeme();
    /* ternary if? */
    if (!strcmp(lex.val, "?")) {
        /* allocate labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* expr must be numerical */
        if (expr->type->specifier == TYPE_BYTE ||
            expr->type->specifier == TYPE_HALF ||
            expr->type->specifier == TYPE_WORD ||
            expr->type->specifier == TYPE_DOBL ||
            expr->type->specifier == TYPE_PTR) {
            /* branch if zero */
            emit_load(expr, reg);
            emit_bze(expr->type, reg, lbl1);
        } else {
            print_err("invalid type for ternary if condition", 0);
        }
        /* expect full expression! */
        expr1 = parse_expr();
        /* skip evaluation of expr2 */
        emit_jmp(lbl2);
        /* expect : sign */
        get_lexeme();
        if (strcmp(lex.val, ":")) {
            print_err("expected :", 0);
            unget_lexeme();            
        }
        /* lbl1 goes here */
        emit_label(lbl1);
        /* parse another conditional */
        expr2 = parse_conditional();
        /* lbl2 goes here */
        emit_label(lbl2);
        /* expr1 and expr2 types must match */
        if (!type_match(expr1->type, expr2->type, 1)) {
            print_err("ternary if expression types must match", 0);
        }
        /* allocate expr3 with the same type of expr1 (theoretically same as 2) */
        expr3 = alloc_expr();
        expr3->type = expr1->type;
        expr3->addr = get_new_addr(type_size(expr3->type));
        /* allocate other two labels */
        lbl1 = get_new_label();
        lbl2 = get_new_label();
        /* check expr0 value */
        if (expr->type->specifier == TYPE_BYTE ||
            expr->type->specifier == TYPE_HALF ||
            expr->type->specifier == TYPE_WORD ||
            expr->type->specifier == TYPE_DOBL ||
            expr->type->specifier == TYPE_PTR) {
            /* branch if zero */
            emit_load(expr, reg);
            emit_bze(expr->type, reg, lbl1);
        }
        /* copy expr1 into expr3 */
        expr3->lvalue = 1;
        do_assign(expr3, expr1);
        expr3->lvalue = 0;
        /* skip copying of expr2 */
        emit_jmp(lbl2);
        /* lbl1 goes here */
        emit_label(lbl1);
        /* copy expr2 into expr3 */
        expr3->lvalue = 1;
        do_assign(expr3, expr2);
        expr3->lvalue = 0;
        /* lbl2 goes here */
        emit_label(lbl2);
        /* return expr3 */
        expr = expr3;
    } else {
        /* miss */       
        unget_lexeme();
    }
    /* done */
    return expr;
}

