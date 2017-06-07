#include <stdlib.h>
#include <string.h>

#include "common.h"

int parse_stmt_list();

int parse_if() {
    expr_t *expr;
    char *lbl2 = get_new_label();
    char *lbl1 = get_new_label();
    int reg = emit_get_reg(REG_ACC, 0);
    int done = 0;

    /* if */
    get_lexeme();
    
    /* expr */
    expr = parse_expr();
    
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
        print_err("invalid type for if condition", 0);
    }
    
    /* then */
    get_lexeme();
    if (strcmp(lex.val, "then")) {
        unget_lexeme();
        print_err("expected then", 0);
    }
    
    /* statements */
    parse_stmt_list();
    
    /* lookahead */
    get_lexeme();
    unget_lexeme();
    
    /* else/elseif? */
    if(strcmp(lex.val, "endif")) {
        /* jmp to end of if */
        emit_jmp(lbl2);
        
        /* lbl1 goes here */
        emit_label(lbl1);
        
        /* either elsif, else, or endif */
        while (!done) {
            get_lexeme();
            if (!strcmp(lex.val, "elsif") || !strcmp(lex.val, "else") ) {  
                /* allocate new label */
                lbl1 = get_new_label();
                /* for elseif, evaluate condition */
                if (!strcmp(lex.val, "elsif")) {
                    /* expr */
                    expr = parse_expr();
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
                        print_err("invalid type for if condition", 0);
                    }    
                    /* then */
                    get_lexeme();
                    if (strcmp(lex.val, "then")) {
                        unget_lexeme();
                        print_err("expected then", 0);
                    }
                } else {
                    /* done */
                    done = 1;
                }
                /* statements */
                parse_stmt_list();
                /* jmp to end of if */
                emit_jmp(lbl2);    
                /* lbl1 goes here */
                emit_label(lbl1);
            } else {
                unget_lexeme();
                done = 1;
            }
        }
        /* label of final */
        emit_label(lbl2);
    } else {
        /* label of final */
        emit_label(lbl1);
    }
    /* endif */
    get_lexeme();
    if (strcmp(lex.val, "endif")) {
        print_err("expected endif", 0);
        unget_lexeme();
    }
}

int parse_case() {

}

int parse_for() {

}

int parse_while() {

}

int parse_stmt_list() {
    int done = 0;
    while(!done) {
        get_lexeme();
        if (!strcmp(lex.val, "endf") ||
            !strcmp(lex.val, "elsif") ||
            !strcmp(lex.val, "else") ||
            !strcmp(lex.val, "endif") ||
            !strcmp(lex.val, "esac") ||
            !strcmp(lex.val, "next") ||
            !strcmp(lex.val, "return") ||
            !strcmp(lex.val, "done") ||
            lex.type == EOF) {
            /* done */
            unget_lexeme();
            done = 1;
        } else {
            if (!strcmp(lex.val, "if")) {
                /* if condition */
                unget_lexeme();
                parse_if();
            } else if (!strcmp(lex.val, "case")) {
                /* case */
                unget_lexeme();
                parse_case();
            } else if (!strcmp(lex.val, "for")) {
                /* for loop */
                unget_lexeme();
                parse_for();
            } else if (!strcmp(lex.val, "while")) {
                /* while loop */
                unget_lexeme();
                parse_while();
            } else {
                /* expression list */
                unget_lexeme();
                parse_expr_list();
            }
            /* semicolon */
            get_lexeme();
            if (strcmp(lex.val, ";")) {
                print_err("expected ;", 0);
            }
        }
    }
}

expr_t *parse_func() {
    int done = 0;
    int arg_count = 0;
    int reg = emit_get_reg(REG_ACC, 0);
    sym_t *sym;
    expr_t *expr;
    type_t *type;
    param_list_t *param_list;

    /* function! */
    type = parse_type();
    
    /* check header validity */
    if (type->specifier == TYPE_FUNC) {
    
        /* put code generated in func macro */
        emit_def_m4_macro("func");
    
        /* enter func scope */
        enter_scope();
    
        /* initialize stack frame */
        init_stack_frame();

        /* loop over parameters */
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
                } else if (param_list->type->specifier == TYPE_RECORD) {
                    /* record passed */
                } else {
                    /* invalid type */
                    print_err("invalid parameter types",0);
                } 
            }
            /* next parameter */
            param_list = param_list->sublist;
        }

        /* evaluate dims */
        parse_dim_list();

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
        if (strcmp(lex.val, "endf")) {
            print_err("expected endf", 0);
            unget_lexeme();
        }

        /* function termination */
        emit_func_leave();
        
        /* code generation done */    
        emit_fed_m4_macro();
    
        /* function entry point */
        emit_func_entry();
    
        /* function code */
        emit_use_m4_macro("func");

        /* delete symbols of current scope */
        del_syms();

        /* exit func scope */
        leave_scope();

        /* evaluate expr as function */
        expr = alloc_expr();
        expr->literal         = 1;
        expr->type            = type;
        expr->type->complete  = 1;
        
    } else {
    
        /* invalid function header */
        expr = alloc_expr();
        
    }

    /* done */
    return expr;
}
