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
    
    /* done */
    emit_comment("IF: END IF");
    
    /* endif */
    get_lexeme();
    if (strcmp(lex.val, "endif")) {
        print_err("expected endif", 0);
        unget_lexeme();
    }

}

void parse_case() {

    char *lbl_final, *lbl_next = NULL, *lbl_sub;
    expr_t *case_expr;
    expr_list_t *expr_list;
    int done = 0, got_others = 0;
    int reg1 = emit_get_reg(REG_ACC, 0);
    int reg2 = emit_get_reg(REG_ACC, 1);

    /* parse 'case' */
    get_lexeme();
    
    /* allocate label for exit */
    lbl_final = get_new_label();
    
    /* parse case expression */
    emit_comment("CASE: EXPRESSION EVALUATION");
    case_expr = parse_expr();
    
    /* when */
    while (!done) {
        get_lexeme();
        if (strcmp(lex.val, "when")) {
            /* not when */
            unget_lexeme();
            done = 1;
        } else {
            /* when */
            if (lbl_next) {
                emit_label(lbl_next);
            }
            /* fetch expression list */
            get_lexeme();
            if (strcmp(lex.val, "others")) {
                /* when expr, expr, ... : */
                unget_lexeme();
                /* get new labels */
                lbl_sub  = get_new_label();            
                lbl_next = get_new_label();
                /* evaluate expression list */
                emit_comment("CASE: EVALUATE EXPRESSION LIST");
                expr_list = parse_expr_list();
                if (expr_list->count == 0) {
                    print_err("empty expression list for case when", 0);
                }
                /* branch if all expressions do not match main one */
                emit_comment("CASE: COMPARE AND BRANCH");
                while (expr_list->count) {
                    /* type cast */
                    if (expr_list->expr->type->specifier != 
                        case_expr->type->specifier) {
                        expr_list->expr = type_cast(expr_list->expr, 
                                                    case_expr->type);
                    }
                    /* emit comparison code */
                    emit_load(case_expr, reg1);
                    emit_load(expr_list->expr, reg2);
                    emit_beq(case_expr->type, reg1, reg2, lbl_sub);
                    /* next expression */
                    expr_list = expr_list->sublist;
                }
                emit_jmp(lbl_next);
                /* expect : */
                get_lexeme();
                if (strcmp(lex.val, ":")) {
                    unget_lexeme();
                    print_err("expected :", 0);
                }
                
                /* execute action */
                emit_label(lbl_sub);
                emit_comment("CASE: EXECUTE ACTION");
                parse_stmt_list();
                emit_jmp(lbl_final);
            } else {
                /* when others */
                got_others = 1;
                done = 1;
                /* expect : */
                get_lexeme();
                if (strcmp(lex.val, ":")) {
                    unget_lexeme();
                    print_err("expected :", 0);
                }
                /* execute action */
                emit_comment("CASE: OTHERS BLOCK");
                parse_stmt_list();   
            }
        }
    }
    
    /* if no (when others) is defined, define it */
    if (!got_others && lbl_next) {
        emit_label(lbl_next);
        emit_comment("CASE: PSEUDO OTHERS CLAUSE");
    }
    
    /* esac */
    get_lexeme();
    if (strcmp(lex.val, "esac")) {
        unget_lexeme();
        print_err("expected esac", 0);
    }
    
    /* done */
    emit_label(lbl_final);    
    emit_comment("CASE: ESAC");
    
}

void parse_for() {

    expr_t *index, *init, *limit, *step, *tmp;
    char *lbl1, *lbl2;
    int reg1 = emit_get_reg(REG_ACC, 0);
    int reg2 = emit_get_reg(REG_ACC, 1);
    int down = 0, unsign = 0;

    /* parse 'for' */
    get_lexeme();
    
    /* allocate labels */
    lbl1 = get_new_label();
    lbl2 = get_new_label();
    
    /* parse index */
    index = parse_expr();
    if (!index->lvalue) {
        print_err("index must be an lvalue", 0);
    } else if (index->type->specifier != TYPE_BYTE &&
               index->type->specifier != TYPE_HALF &&
               index->type->specifier != TYPE_WORD &&
               index->type->specifier != TYPE_DOBL &&
               index->type->specifier != TYPE_PTR) {
        print_err("index must be of numeric type", 0);           
    }
    
    /* from */
    get_lexeme();
    if (strcmp(lex.val, "from")) {
        unget_lexeme();
        print_err("expected from", 0);
    }
    
    /* read initializer */
    emit_comment("FOR: PEFROM INITIALIZATION");
    init = parse_expr();
    do_assign(index, init);
    
    /* condition evaluation */
    emit_label(lbl1);
    emit_comment("FOR: CONDITION EVALUATION");

    /* unsigned */
    get_lexeme();
    if (!strcmp(lex.val, "unsigned")) {
        unsign = 1;
    } else {
        unget_lexeme();
    }
    
    /* down */
    get_lexeme();
    if (!strcmp(lex.val, "down")) {
        down = 1;
    } else {
        unget_lexeme();
    }
    
    /* to */
    get_lexeme();
    if (strcmp(lex.val, "to")) {
        unget_lexeme();
        print_err("expected to", 0);
    }
    
    /* parse the condition */
    limit = parse_expr();
    
    /* code to check the condition */
    emit_comment("FOR: CHECK CONDITION");
    emit_load(index, reg1);
    emit_load(limit, reg2);
    if (down) {
        if (unsign) {
            emit_blt(index->type, reg1, reg2, lbl2);
        } else {
            emit_blt_unsigned(index->type, reg1, reg2, lbl2);
        }
    } else {
        if (unsign) {
            emit_bgt(index->type, reg1, reg2, lbl2);
        } else {
            emit_bgt_unsigned(index->type, reg1, reg2, lbl2);
        }
    }

    /* step? */
    get_lexeme();
    if (strcmp(lex.val, "step")) {
        unget_lexeme();
        step = alloc_expr();
        step->literal = 1;
        step->type->specifier = TYPE_WORD;
        if (!down) {
            step->word_literal_val = 1;
        } else {
            step->word_literal_val = -1;
        }
    } else {
        step = parse_expr();
        if (!step->literal) {
            print_err("for loop step must be literal", 0);
        }
    }
        
    /* do */
    get_lexeme();
    if (strcmp(lex.val, "do")) {
        unget_lexeme();
        print_err("expected do", 0);
    }
    
    /* statements */
    emit_comment("FOR: EXECUTE BLOCK");
    parse_stmt_list();
    
    
    /* increment */
    emit_comment("FOR: PERFORM INCREMENT");
    tmp = do_binary(index, "+", step);
    do_assign(index, tmp);
    
    /* jump back */
    emit_comment("FOR: JUMP BACK");
    emit_jmp(lbl1);
    
    /* loop keyword */
    get_lexeme();
    if (strcmp(lex.val, "loop")) {
        unget_lexeme();
        print_err("expected loop", 0);
    }
    
    /* done */
    emit_label(lbl2);
    emit_comment("FOR: LOOP DONE");

}

void parse_while() {

    expr_t *cond;
    char *lbl1, *lbl2;
    int reg1 = emit_get_reg(REG_ACC, 0);
    
    /* parse 'while' */
    get_lexeme();
    
    /* allocate labels */
    lbl1 = get_new_label();
    lbl2 = get_new_label();
    
    /* parse cond */
    emit_label(lbl1);
    emit_comment("WHILE: EVALUATE CONDITION");
    cond = parse_expr();
    if (cond->type->specifier != TYPE_BYTE &&
        cond->type->specifier != TYPE_HALF &&
        cond->type->specifier != TYPE_WORD &&
        cond->type->specifier != TYPE_DOBL &&
        cond->type->specifier != TYPE_PTR) {
        print_err("condition must be of numeric type", 0);           
    }
    
    /* check condition */
    emit_comment("WHILE: CHECK CONDITION");
    emit_load(cond, reg1);
    emit_bze(cond->type, reg1, lbl2);
    
    /* do keyword */
    get_lexeme();
    if (strcmp(lex.val, "do")) {
        unget_lexeme();
        print_err("expected do", 0);
    }
    
    /* process while body */
    emit_comment("WHILE: PROCESS BODY");
    parse_stmt_list();
    
    /* jump back */
    emit_comment("FOR: JUMP BACK");
    emit_jmp(lbl1);
    
    /* loop keyword */
    get_lexeme();
    if (strcmp(lex.val, "loop")) {
        unget_lexeme();
        print_err("expected loop", 0);
    }

    /* done */
    emit_label(lbl2);
    emit_comment("WHILE: LOOP DONE");

}

void parse_stmt_list() {
    int done = 0;
    while(!done) {
        get_lexeme();
        if (!strcmp(lex.val, "end") ||
            !strcmp(lex.val, "elsif") ||
            !strcmp(lex.val, "else") ||
            !strcmp(lex.val, "endif") ||
            !strcmp(lex.val, "when") ||
            !strcmp(lex.val, "esac") ||
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

