#include <stdlib.h>
#include <string.h>

#include "common.h"

void parse_if() {
    expr_t *expr;
    char *lbl2 = get_new_label();
    char *lbl1 = get_new_label();
    int reg = emit_get_reg(REG_ACC, 0);
    int done = 0;
    int has_else = 0;

    /* if */
    get_lexeme();
    
    /* expr */
    emit_comment("IF: CONDITION EXPRESSION EVALUATION");
    expr = parse_expr();
    
    /* expr must be numerical */
    if (expr->type->specifier == TYPE_BYTE ||
        expr->type->specifier == TYPE_HALF ||
        expr->type->specifier == TYPE_WORD ||
        expr->type->specifier == TYPE_DOBL ||
        expr->type->specifier == TYPE_PTR) {
        /* branch if zero */
        emit_comment("IF: BRANCH IF COND EXPR == ZERO");
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
    emit_comment("IF: TRUE STATEMENT BLOCK");
    parse_stmt_list();
    
    /* jmp to end of if */
    emit_jmp(lbl2);
    
    /* lookahead */
    get_lexeme();
    unget_lexeme();
    
    /* else/elseif? */
    if(strcmp(lex.val, "endif")) {
        /* either elsif, else, or endif */
        while (!done) {
            get_lexeme();
            if (!strcmp(lex.val, "elsif") || !strcmp(lex.val, "else")) {            
                /* lbl1 goes here */
                emit_label(lbl1);
                /* allocate new label */
                lbl1 = get_new_label();
                /* for elseif, evaluate condition */
                if (!strcmp(lex.val, "elsif")) {
                    /* expr */
                    emit_comment("IF: ELSIF CONDITION EXPRESSION");
                    expr = parse_expr();
                    /* expr must be numerical */
                    if (expr->type->specifier == TYPE_BYTE ||
                        expr->type->specifier == TYPE_HALF ||
                        expr->type->specifier == TYPE_WORD ||
                        expr->type->specifier == TYPE_DOBL ||
                        expr->type->specifier == TYPE_PTR) {
                        /* branch if zero */
                        emit_comment("IF: BRANCH IF COND EXPR == ZERO");
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
                    has_else = 1;
                }
                /* statements */
                if (!has_else) {
                    emit_comment("IF: ELSIF STATEMENT BLOCK");
                } else {
                    emit_comment("IF: ELSE STATEMENT BLOCK");
                }
                parse_stmt_list();
                /* jmp to end of if */
                if (!has_else) {
                    emit_jmp(lbl2); 
                }
            } else {
                unget_lexeme();
                done = 1;
            }
        }
        /* else encountered? */
        if (!has_else) {
            emit_comment("IF: PSEUDO ELSE CLAUSE");
            emit_label(lbl1);
            emit_nop();
        }
    
        /* label of final */
        emit_label(lbl2);
    } else {
        /* enough playing */
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
                emit_comment("EXPRESSION STATEMENT");
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
