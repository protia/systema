/* unary operations */
#include <string.h>

#include "common.h"

expr_t *parse_unary_post() {
    expr_t *expr, *expr1, *backup, *res;
    type_t *type;
    int done = 0;
    int reg = emit_get_reg(REG_ACC, 0);
    /* unary_post: factor post_op* */
    expr = parse_factor();
    /* literal 1 */
    expr1 = alloc_expr();
    expr1->literal = 1;
    expr1->type->specifier = TYPE_WORD;
    expr1->word_literal_val = 1;
    /* loop over post operators */
    get_lexeme();
    done = 0;
    while(!done) {
        if (!strcmp(lex.val, "++")) {
            /* copy expr into another one */
            backup = alloc_expr();
            backup->type = expr->type;
            backup->addr = get_new_addr(type_size(expr->type));
            backup->lvalue = 1;
            /* copy into addr */
            do_assign(backup, expr);
            /* post increment */
            res = do_binary(expr, "+", expr1);
            /* perform assignment */
            do_assign(expr, res);
            /* return the backed up value */
            expr = backup;
            /* expr is no longer an lvalue */
            expr->lvalue = 0;
        } else if (!strcmp(lex.val, "--")) {
            /* copy expr into another one */
            backup = alloc_expr();
            backup->type = expr->type;
            backup->addr = get_new_addr(type_size(expr->type));
            backup->lvalue = 1;
            /* copy into addr */
            do_assign(backup, expr);
            /* post increment */
            res = do_binary(expr, "-", expr1);
            /* perform assignment */
            do_assign(expr, res);
            /* return the backed up value */
            expr = backup;
            /* expr is no longer an lvalue */
            expr->lvalue = 0;
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
            /* array subscript */
            if (expr->type->specifier == TYPE_ARRAY) {
                /* read array index */
                expr1 = parse_expr();
                /* read ] */
                get_lexeme();
                if (strcmp(lex.val, "]")) {
                    unget_lexeme();
                    print_err("expected ]", 0);
                }
                /* subscript must be integer */
                if (expr1->type->specifier >= TYPE_BYTE &&
                    expr1->type->specifier <= TYPE_DOBL) {
                    /* load the address of the first element in the 
                     * array to a pointer to array type
                     * then perform pointer arithmetic
                     * ex: to evaluate arr[i]
                     *        let p pointer = &arr[0]
                     *            p += i
                     *            return *p as indirect lvalue var 
                     */
                    res = alloc_expr();
                    res->type->specifier = TYPE_PTR;
                    res->type->complete = 1;
                    res->type->subtype = expr->type->subtype;
                    /* p = &arr[0] */
                    if (!expr->indir) {
                        res->addr = get_new_addr(type_size(res->type));
                        emit_loadaddr(expr, reg);
                        emit_store(reg, res);
                    } else {
                        res->addr = expr->addr;
                    }
                    /* p += i */
                    res = do_binary(res, "+", expr1);
                    /* now res holds the destined address */
                    res->type = res->type->subtype;
                    res->lvalue = 1;
                    res->indir = 1;
                    /* return res */
                    expr = res;                    
                } else {
                    print_err("array subscript must be integer", 0);
                    expr->type = expr->type->subtype;
                }
            } else {
                /* must be applied to an array type */
                print_err("[] can't be applied to a non-array type", 0);
            }
        } else if (!strcmp(lex.val, ".")) {
            /* record member */
        } else if (!strcmp(lex.val, "->")) {
            /* (ptr to record).member */
        } else if (!strcmp(lex.val, "as")) {
            /* type casting! */
            type = parse_type();
            expr = type_cast(expr, type);
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
    expr_t *expr, *tmp;
    int reg = emit_get_reg(REG_ACC, 0);
    /* unary_pre: (unary_op unary_pre | unary_post) */
    get_lexeme();
    if (!strcmp(lex.val, "++")) {
        /* pre increment */
        expr = parse_unary_pre();
        /* generate code for unary ++ */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "--")) {
        /* pre decrement */
        expr = parse_unary_pre();
        /* generate code for unary -- */
        
        /* set expr */
                
    } else if (!strcmp(lex.val, "+")) {
        /* unary plus */
        expr = parse_unary_pre();
        /* generate code for unary + */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "-")) {
        /* unary minus */
        expr = parse_unary_pre();
        /* generate code for unary - */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "!")) {
        /* unary not */
        expr = parse_unary_pre();
        /* generate code for unary ! */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "~")) {
        /* unary bitwise not */
        expr = parse_unary_pre();
        /* generate code for unary ~ */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "*")) {
        /* unary dereference */
        expr = parse_unary_pre();
        /* dereference a pointer */
        if (expr->type->specifier == TYPE_PTR) {
            tmp = alloc_expr();
            tmp->type   = expr->type;
            tmp->addr   = get_new_addr(type_size(expr->type));
            /* store pointer value in tmp's addr */
            emit_load(expr, reg);
            emit_store(reg, tmp);
            /* set tmp type to that of pointed expr and mark indirect */
            tmp->type = tmp->type->subtype;
            tmp->indir  = 1;
            tmp->lvalue = expr->lvalue;
            /* return tmp */
            expr = tmp;
        } else {
            print_err("can't dereference non lvalue expressions", 0);
        }
        
    } else if (!strcmp(lex.val, "&")) {
        /* unary address of */
        expr = parse_unary_pre();
        /* load the address of the expression
         * in a temporary variable
         */
        if (expr->lvalue) {
            tmp = alloc_expr();
            tmp->type->specifier = TYPE_PTR;
            tmp->type->complete = 1;
            tmp->type->subtype = expr->type;
            /* p = &var */
            if (!expr->indir) {
                tmp->addr = get_new_addr(type_size(expr->type));
                emit_loadaddr(expr, reg);
                emit_store(reg, tmp);
            } else {
                tmp->addr = expr->addr;
            }
            /* return tmp */
            expr = tmp;
        } else {
            print_err("can't reference a non lvalue expressions", 0);
        }
    } else {
        /* no unary operator */
        unget_lexeme();
        expr = parse_unary_post();
    }
    
    /* no more */
    unget_lexeme();
    /* done */
    return expr;
}
