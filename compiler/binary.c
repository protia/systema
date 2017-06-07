/* binary operations */
#include <string.h>

#include "common.h"

expr_t *do_binary_op(expr_t *op1, char *op, expr_t *op2) {
    expr_t *expr, *expr0, *expr1;
    char *lbl1, *lbl2, *lbl3;
    int reg1 = emit_get_reg(REG_ACC, 0);
    int reg2 = emit_get_reg(REG_ACC, 1);
    /* allocate container for the result */
    expr = alloc_expr();
    /* different cases according to operand types */
    if (op1->type->specifier >= TYPE_BYTE && 
        op1->type->specifier <= TYPE_DOBL &&
        op2->type->specifier >= TYPE_BYTE &&
        op2->type->specifier <= TYPE_DOBL) {
        /* two operands are integral */
        /* final type is the largest of them */
        if (op1->type->specifier > op2->type->specifier) {
            expr->type = op1->type;
            op2 = type_cast(op2, expr->type); /* implicit cast */
        } else {
            expr->type = op2->type;
            op1 = type_cast(op1, expr->type); /* implicit cast */
        }
        /* for comparison operations, resultant type is word */
        if (!strcmp(op, ">") || !strcmp(op, ">=") ||
            !strcmp(op, "<") || !strcmp(op, "<=") ||
            !strcmp(op, "==") || !strcmp(op, "!=")) {
            /* result type is word */
            expr->type = alloc_type();
            expr->type->specifier = TYPE_WORD;
            /* allocate expressions for 0 and 1 literals */
            expr0 = alloc_expr();
            expr1 = alloc_expr();
            expr0->literal = 1;
            expr1->literal = 1;
            expr0->type->specifier = TYPE_WORD;
            expr1->type->specifier = TYPE_WORD;
            expr0->word_literal_val = 0;
            expr1->word_literal_val = 1;
            /* allocate labels */
            lbl1 = get_new_label();
            lbl2 = get_new_label();
            lbl3 = get_new_label();
        }
        /* do operation */
        if (op1->literal && op2->literal) {
            /* literals */
            expr->literal = 1;
            
        } else {
            /* allocate space for result */
            expr->addr = get_new_addr(type_size(expr->type));
            /* load both operands into machine regs */
            emit_load(op1, reg1);
            emit_load(op2, reg2);
            /* perform operation */
            if (!strcmp(op, "+")) {
                emit_add(op1->type, reg2, reg1);
            } else if (!strcmp(op, "-")) {
                emit_sub(op1->type, reg2, reg1);
            } else if (!strcmp(op, "*")) {
                emit_mul(op1->type, reg2, reg1);
            } else if (!strcmp(op, "/")) {
                emit_div(op1->type, reg2, reg1);
            } else if (!strcmp(op, "%")) {
                emit_mod(op1->type, reg2, reg1);
            } else if (!strcmp(op, "==")) {
                emit_beq(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);              
            } else if (!strcmp(op, "!=")) {
                emit_bne(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);
            } else if (!strcmp(op, ">")) {
                emit_bgt(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, ">=")) {
                emit_bge(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, "<")) {
                emit_blt(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, "<=")) {
                emit_ble(op1->type, reg1, reg2, lbl2);
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);
            } else if (!strcmp(op, "|")) {
                emit_eor(op1->type, reg2, reg1);
            } else if (!strcmp(op, "&")) {
                emit_and(op1->type, reg2, reg1);
            } else if (!strcmp(op, "^")) {
                emit_xor(op1->type, reg2, reg1);
            } else if (!strcmp(op, ">>")) {
                emit_sra(op1->type, reg2, reg1);
            } else if (!strcmp(op, "<<")) {
                emit_sll(op1->type, reg2, reg1);
            } else {
                print_err("<bug> do_binary_op(): invalid op", 0);
            }  
            emit_store(reg1, expr);
        }
    } else if ((op1->type->specifier == TYPE_PTR  &&
                op2->type->specifier >= TYPE_BYTE &&
                op2->type->specifier <= TYPE_DOBL ||
                op2->type->specifier == TYPE_PTR  &&
                op1->type->specifier >= TYPE_BYTE &&
                op1->type->specifier <= TYPE_DOBL) &&
               (!strcmp(op, "+") || !strcmp(op, "-"))) {
        /* one operand is a pointer, the other is integral */
    
    
    } else {
        /* unsupported types for operation */
        print_err("unsupported types for '%s'", op);
    }
    
    return expr;
}

expr_t *parse_muldiv() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* muldiv: unary_pre (('*'|'/'|'%') muldiv | lambda) */
    expr1 = parse_unary_pre();
    /* lookahead */
    get_lexeme();
    /* *, /, or % ? */
    while (!strcmp(lex.val, "*") || 
           !strcmp(lex.val, "/") || 
           !strcmp(lex.val, "%")) {
        /* mul/div/mod operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_unary_pre();
        /* generate code for mul/div/mod operation */
        expr1 = do_binary_op(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_addsub() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* addsub: muldiv (('+'|'-') addsub | lambda) */
    expr1 = parse_muldiv();
    /* lookahead */
    get_lexeme();
    /* + or - ? */
    while (!strcmp(lex.val, "+") || !strcmp(lex.val, "-")) {
        /* add/sub operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_muldiv();
        /* generate code for add/sub operation */
        expr1 = do_binary_op(expr1, op, expr2);
        /* get next */
        get_lexeme(); 
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_shift() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* shift: addsub (('<<'|'>>') shift | lambda) */
    expr1 = parse_addsub();
    /* lookahead */
    get_lexeme();
    /* >> or << ? */
    while (!strcmp(lex.val, ">>") || !strcmp(lex.val, "<<")) {
        /* shift operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_addsub();
        /* generate code for shifting */
        expr1 = do_binary_op(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_relation() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* relation: shift (('>'|'>='|'<'|'<=') relation | lambda) */
    expr1 = parse_shift();
    /* lookahead */
    get_lexeme();
    /* relop ? */
    while (!strcmp(lex.val, ">") || 
           !strcmp(lex.val, ">=") || 
           !strcmp(lex.val, "<") || 
           !strcmp(lex.val, "<=")) {
        /* relation operator */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_shift();
        /* generate code for relation operator */
        expr1 = do_binary_op(expr1, op, expr2);
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_equality() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* equality: relation (('=='|'!=') equality | lambda) */
    expr1 = parse_relation();
    /* lookahead */
    get_lexeme();
    /* == or != ? */
    while (!strcmp(lex.val, "==") || !strcmp(lex.val, "!=")) {
        /* equality */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_relation();
        /* generate code for equality */
        expr1 = do_binary_op(expr1, op, expr2);        
        /* get next */
        get_lexeme();        
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_and() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_and: equality ('&' bitwise_and | lambda) */
    expr1 = parse_equality();
    /* lookahead */
    get_lexeme();
    /* &? */
    while (!strcmp(lex.val, "&")) {
        /* bitwise and */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_equality();
        /* generate code for bitwise AND'ing */
        expr1 = do_binary_op(expr1, op, expr2);        
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_xor() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_xor: bitwise_and ('^' bitwise_xor | lambda) */
    expr1 = parse_bitwise_and();
    /* lookahead */
    get_lexeme();
    /* ^? */
    while (!strcmp(lex.val, "^")) {
        /* bitwise xor */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_bitwise_and();
        /* generate code for bitwise XOR'ing */
        expr1 = do_binary_op(expr1, op, expr2); 
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}

expr_t *parse_bitwise_or() {
    expr_t *expr, *expr1, *expr2;
    char op[10];
    /* bitwise_or: bitwise_xor ('|' bitwise_or | lambda) */
    expr1 = parse_bitwise_xor();
    /* lookahead */
    get_lexeme();
    /* |? */
    while (!strcmp(lex.val, "|")) {
        /* bitwise or */
        strcpy(op, lex.val);
        /* parse second operand */
        expr2 = parse_bitwise_xor();
        /* generate code for bitwise OR'ing */
        expr1 = do_binary_op(expr1, op, expr2); 
        /* move to next */
        get_lexeme();
    }
    /* no more */
    unget_lexeme();
    expr = expr1;
    /* done */
    return expr;
}
