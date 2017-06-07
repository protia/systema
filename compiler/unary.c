/* unary operations */
#include <string.h>

#include "common.h"

void do_unary_op() {

}

void push_recursively(expr_list_t *expr_list, int cur_arg) {
    expr_t *expr;
    type_t *type;
    int arg_count;
    int reg = emit_get_reg(REG_ACC, 0);
    if (expr_list->count == 0) {
        /* done */
    } else {
        expr = expr_list->expr;
        type = expr->type;
        /* determine arg_count */
        if (type->specifier == TYPE_BYTE ||
            type->specifier == TYPE_HALF ||
            type->specifier == TYPE_WORD ||
            type->specifier == TYPE_DOBL ||
            type->specifier == TYPE_PTR) {
            arg_count = 1;
        } else {
            arg_count  = 0;
        }
        /* next argument */
        push_recursively(expr_list->sublist, cur_arg+arg_count);
        /* push argument into stack */
        if (type->specifier == TYPE_BYTE ||
            type->specifier == TYPE_HALF ||
            type->specifier == TYPE_WORD ||
            type->specifier == TYPE_DOBL ||
            type->specifier == TYPE_PTR) {
            emit_load(expr, reg);
            emit_pusharg(reg, cur_arg);
        } else {
            /* array or record */
        }
    }
    
}

expr_t *parse_func_call(expr_t *func) {
    int done = 0, err = 0, i = 0;
    expr_t *expr;
    param_list_t *param_list = func->type->param_list;
    expr_list_t *expr_list, *top;
    int reg = emit_get_reg(REG_ACC, 0);
    /* read ( */
    get_lexeme();
    /* read par list */
    expr_list = parse_expr_list();
    top = expr_list;
    /* read ) */
    get_lexeme();
    if (strcmp(lex.val, ")")) {
        print_err("expected )", 0);
        unget_lexeme();
    }
    /* all parameters must match function definition */
    while(!done) {
        if (!expr_list->count || !param_list->count) {
            if (param_list->any) {
                param_list = param_list->sublist;
            }
            done = 1;
        } else {
            /* match parameter with type */
            i++;
            if (param_list->any) {
                /* any type is acceptable, really */
            } else {
                if (!type_match(param_list->type,expr_list->expr->type,1)) {
                    if (!err) {
                        print_err("function call: parameters invalid",0);
                    }
                    err = 1;
                } else {
                    /* implicit cast needed? */
                    if (param_list->type->specifier != 
                        expr_list->expr->type->specifier) {
                        expr_list->expr = type_cast(expr, param_list->type);
                    }
                }
            }
            /* move forward */
            expr_list = expr_list->sublist;
            if (!param_list->any) {
                param_list = param_list->sublist;
            }
        }
    }
    /* parameter count is valid? */
    if (expr_list->count) {
        print_err("too many parameters for function", 0);
        err = 1;
    } else if (param_list->count) {
        print_err("too few parameters for function", 0);
        err = 1;
    }
    if (!err) {
        /* push args to stack */
        push_recursively(top, 0);    
        /* issue call to function */ 
        emit_call(func);
    }
    /* ret value */
    expr = alloc_expr();
    if (func->type->rettype) {
        expr->type = func->type->rettype;
        expr->addr = get_new_addr(type_size(expr->type));
        emit_store(reg, expr);
    } else {
        expr->type->specifier = TYPE_VOID;
    }
    /* done */
    return expr;
}

expr_t *parse_unary_post() {
    expr_t *expr, *expr1;
    type_t *type;
    int done = 0;
    /* unary_post: factor post_op* */
    expr1 = parse_factor();
    /* loop over post operators */
    get_lexeme();
    done = 0;
    while(!done) {
        if (!strcmp(lex.val, "++")) {
            /* post increment */
        } else if (!strcmp(lex.val, "--")) {
            /* post decrement */
        } else if (!strcmp(lex.val, "(")) {
            /* function call */
            if (expr1->type->specifier != TYPE_FUNC) {
                print_err("error: call to a non-function", 0);
                done = 1;
            } else {
                unget_lexeme();
                expr1 = parse_func_call(expr1);
            }
        } else if (!strcmp(lex.val, "[")) {
            /* array subscript */
        } else if (!strcmp(lex.val, ".")) {
            /* record member */
        } else if (!strcmp(lex.val, "->")) {
            /* (ptr to record).member */
        } else if (!strcmp(lex.val, "as")) {
            /* type casting! */
            type = parse_type();
            expr1->type = type;
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
    /* nothing more to do */
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_unary_pre() {
    expr_t *expr, *expr1, *expr2;
    type_t *type;
    /* unary_pre: (unary_op unary_pre | unary_post) */
    get_lexeme();
    if (!strcmp(lex.val, "++")) {
        /* pre increment */
        expr1 = parse_unary_pre();
        /* generate code for unary ++ */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "--")) {
        /* pre decrement */
        expr1 = parse_unary_pre();
        /* generate code for unary -- */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "+")) {
        /* unary plus */
        expr1 = parse_unary_pre();
        /* generate code for unary + */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "-")) {
        /* unary minus */
        expr1 = parse_unary_pre();
        /* generate code for unary - */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "!")) {
        /* unary not */
        expr1 = parse_unary_pre();
        /* generate code for unary ! */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "~")) {
        /* unary bitwise not */
        expr1 = parse_unary_pre();
        /* generate code for unary ~ */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "*")) {
        /* unary dereference */
        expr1 = parse_unary_pre();
        /* generate code for deref */
        
        /* set expr */
        
    } else if (!strcmp(lex.val, "&")) {
        /* unary address of */
        expr1 = parse_unary_pre();
        /* generate address of */
        
        /* set expr */

    } else {
        /* no unary operator */
        unget_lexeme();
        expr1 = parse_unary_post();
    }
    
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}
