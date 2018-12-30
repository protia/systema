#include <stdlib.h>
#include <string.h>

#include "common.h"

void push_recursively(expr_list_t *expr_list, int cur_arg) {
    expr_t *expr;
    type_t *type, *longtype;
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
            if (type->specifier < TYPE_DOBL) {
                longtype = alloc_type();
                longtype->specifier = TYPE_DOBL;
                longtype->complete = 1;
                emit_extend_signed(expr->type, longtype, reg); /* FIXME */
            }
            emit_pusharg(type_size(type), reg, cur_arg);
        } else {
            /* array or record */
            print_err("passing of arrays/records is not supported", 0);
        }
    }
}

void pop_recursively(expr_list_t *expr_list, int cur_arg) {
    expr_t *expr;
    type_t *type;
    int arg_count;
    int reg = emit_get_reg(REG_ACC, 0);
    if (expr_list->count == 0) {
        /* done */
    } else {
        expr = expr_list->expr;
        type = expr->type;
        /* determine arg_count and pop out arguments */
        if (type->specifier == TYPE_BYTE ||
            type->specifier == TYPE_HALF ||
            type->specifier == TYPE_WORD ||
            type->specifier == TYPE_DOBL ||
            type->specifier == TYPE_PTR) {
            emit_poparg(cur_arg, reg);
            arg_count = 1;
        } else {
            arg_count  = 0;
        }
        /* next argument */
        pop_recursively(expr_list->sublist, cur_arg+arg_count);
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
                        expr_list->expr = type_cast(expr_list->expr, 
                                                    param_list->type);
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
        /* pop out arguments */
        pop_recursively(top, 0);
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

void parse_func_header(type_t *type) {
    /* parse function parameters as a record header */
    parse_record_header(type);
    /* return type */
    get_lexeme();
    if (!strcmp(lex.val, "returns")) {
        /* read ret type */
        type->rettype = parse_type();
        if (type->rettype->specifier != TYPE_BYTE &&
            type->rettype->specifier != TYPE_HALF &&
            type->rettype->specifier != TYPE_WORD &&
            type->rettype->specifier != TYPE_DOBL &&
            type->rettype->specifier != TYPE_PTR) {
            print_err("unsupported return type", 0);
            type->rettype = alloc_type();
        }
    } else {
        unget_lexeme();
    }
}

void parse_func(sym_t *sym) {
    int arg_count = 0;
    int reg = emit_get_reg(REG_ACC, 0);
    expr_t *expr;
    type_t *type;
    param_list_t *param_list;
    char *stack_sym;

    /* determine stack symbol */
    stack_sym = malloc(strlen(sym->name)+20);
    strcpy(stack_sym, ".");
    strcat(stack_sym, sym->name);
    strcat(stack_sym, ".frame");

    /* function entry point assembly code */
    emit_comment("FUNCTION ENTRY");
    emit_func_entry(stack_sym);

    /* enter a new scope level */
    enter_scope();

    /* initialize stack frame */
    init_stack_frame();

    /* get function header */
    type = sym->type;

    /* loop over function parameters */
    emit_comment("COPY FUNCTION PARAMETERS");
    param_list = type->param_list;
    while (param_list->count != 0 && !param_list->any) {
        /* add to symtab */
        if (get_sym(param_list->name)) {
            print_err("identifier already declared", 0);
        } else {
            /* add sym to symtab */
            sym = add_sym(param_list->name, param_list->type);
            /* set symbol value */
            sym->val = get_new_addr(type_size(param_list->type));
            /* create expr as container */
            expr = alloc_expr();
            expr->type = param_list->type;
            expr->addr = sym->val;
            /* load argument into variable */
            if (param_list->type->specifier == TYPE_BYTE ||
                param_list->type->specifier == TYPE_HALF ||
                param_list->type->specifier == TYPE_WORD ||
                param_list->type->specifier == TYPE_DOBL ||
                param_list->type->specifier == TYPE_PTR) {
                /* integral type */
                emit_loadarg(arg_count, reg);
                emit_store(reg, expr);
                arg_count++;
            } else if (param_list->type->specifier == TYPE_ARRAY) {
                /* array passed */
                /* TODO */
            } else if (param_list->type->specifier == TYPE_RECORD) {
                /* record passed */
                /* TODO */
            } else {
                /* invalid type */
                print_err("invalid parameter types",0);
            } 
        }
        /* next parameter */
        param_list = param_list->sublist;
    }

    /* evaluate dims */
    emit_comment("EVALUATE VARIABLE DEFINITIONS");
    parse_dim_list();

    /* parse begin */
    get_lexeme();
    if (strcmp(lex.val, "begin")) {
        print_err("expected 'begin'", 0);
        unget_lexeme();
    }

    /* evaluate statements */
    parse_stmt_list();

    /* evaluate return stmt if applicable */
    if (type->rettype) {
        /* return */
        get_lexeme();
        if (strcmp(lex.val, "return")) {
            print_err("expected return", 0);
            unget_lexeme();
        }
        /* evaluate expression */
        expr = parse_expr();
        /* type must be matching */
        if (!type_match(expr->type, type->rettype, 1)) {
            print_err("incompatible return type", 0);
        } else {
            /* needs casting? */
            if (expr->type->specifier != type->rettype->specifier) {
                expr = type_cast(expr, type->rettype);
            }
            /* load to accumulator */
            emit_load(expr, reg);
        }
        /* read ; */
        get_lexeme();
        if (strcmp(lex.val, ";")) {
            print_err("expected ;", 0);
            unget_lexeme();
        }
    }

    /* endf */
    get_lexeme();
    if (strcmp(lex.val, "end")) {
        print_err("expected end", 0);
        unget_lexeme();
    }

    /* function termination */
    emit_comment("FUNCTION EXIT");
    emit_func_leave();
    emit_set(stack_sym, get_stack_size());

    /* delete symbols of current scope */
    del_syms();

    /* exit func scope */
    leave_scope();

}

