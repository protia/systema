#include <stdlib.h>
#include <string.h>

#include "common.h"

void parse_if() {
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

void parse_case() {

}

void parse_for() {

}

void parse_while() {

}

void parse_stmt_list() {
    int done = 0;
    while(!done) {
        get_lexeme();
        if (!strcmp(lex.val, "end") ||
            !strcmp(lex.val, "elsif") ||
            !strcmp(lex.val, "else") ||
            !strcmp(lex.val, "endif") ||
            !strcmp(lex.val, "loop") ||
            !strcmp(lex.val, "return") ||
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
