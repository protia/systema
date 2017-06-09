/* operation code generation */
#include <string.h>

#include "common.h"

void do_assign(expr_t *dest, expr_t *src) {
    int reg = emit_get_reg(REG_ACC, 0);
    /* dest operand must be an lvalue */
    if (!dest->lvalue) {
        print_err("operand must be lvalue", 0);
    } else if (!type_match(src->type, dest->type, 1)) {
        print_err("operands must be of the same type", 0);
    } else {
        /* check if casting is needed */
        if (src->type->specifier != dest->type->specifier) {
            src = type_cast(src, dest->type);
        }
        /* perform assignment */
        if ((dest->type->specifier >= TYPE_BYTE && 
             dest->type->specifier <= TYPE_DOBL) ||
             dest->type->specifier == TYPE_PTR) {
             emit_load(src, reg);
             emit_store(reg, dest);
         } else {
            print_err("types for assignment are not supported", 0);
         }
    }
}

expr_t *do_binary(expr_t *op1, char *op, expr_t *op2) {
    expr_t *expr, *expr0, *expr1;
    expr_t *op_ptr, *op_incr, *subtype_size;
    expr_t *ptr_as_long, *add_val;
    type_t *type;
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
        } else if (op1->type->specifier < op2->type->specifier) {
            expr->type = op2->type;
            op1 = type_cast(op1, expr->type); /* implicit cast */
        } else {
            expr->type = op1->type; /* any type of them */
        }
        /* for comparison operations, resultant type is word */
        if (!strcmp(op, ">") || !strcmp(op, ">=") ||
            !strcmp(op, "<") || !strcmp(op, "<=") ||
            !strcmp(op, "=") || !strcmp(op, "!=")) {
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
            /* do binary operations on literals */
            literal_do_binary(expr, op1, op, op2);
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
            } else if (!strcmp(op, "=")) {
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
                print_err("<bug> do_binary(): invalid op", 0);
            }  
            emit_store(reg1, expr);
        }
    } else if (((op1->type->specifier == TYPE_PTR  &&
                 op2->type->specifier >= TYPE_BYTE &&
                 op2->type->specifier <= TYPE_DOBL) ||
                (op2->type->specifier == TYPE_PTR  &&
                 op1->type->specifier >= TYPE_BYTE &&
                 op1->type->specifier <= TYPE_DOBL)) &&
               (!strcmp(op, "+") || !strcmp(op, "-"))) {
    
        /* one operand is a pointer, the other is integral */
        if (op1->type->specifier == TYPE_PTR) {
            op_ptr = op1;
            op_incr = op2;
        } else {
            op_ptr = op2;
            op_incr = op1;
            /* this swapping can't happen in subtraction */
            if (!strcmp(op, "-")) {
                print_err("can't subtract a pointer from integer", 0);
            }
        }
                
        /* get the size of the subtype of the ptr */
        subtype_size = alloc_expr();
        subtype_size->literal = 1;
        subtype_size->type->specifier = TYPE_DOBL;
        subtype_size->long_literal_val=type_size(op_ptr->type->subtype);

        /* special treatment for void pointers */
        if (op_ptr->type->subtype->specifier == TYPE_VOID) {
            subtype_size->long_literal_val = 0;
        }

        /* subtype must be complete */
        if (!subtype_size->long_literal_val) {
            print_err("pointer subtype must be complete or void", 0);
        }

        /* casting needed for second operand? */
        if (op_incr->type->specifier != TYPE_DOBL) {
            /* cast to long */
            type = alloc_type();
            type->specifier = TYPE_DOBL;
            type->complete = 1;
            op_incr = type_cast(op_incr, type);
        }
        
        /* cast the pointer operand to long */
        ptr_as_long = type_cast(op_ptr, op_incr->type);

        /* multiply op_incr by pointer subtype size */
        add_val = do_binary(op_incr, "*", subtype_size);
        
        /* add add_val to ptr_as_long */
        expr = do_binary(ptr_as_long, op, add_val);

        /* final ptr type should reflect the original ptr */
        expr->type = op_ptr->type;
    
    } else if (op1->type->specifier == TYPE_PTR &&
               op2->type->specifier == TYPE_PTR) {

        /* operation on two pointers */
        if (!type_match(op1->type, op2->type, 1)) {
            print_err("pointers must point to the same type", NULL);
        }
        
        /* get the size of the subtype of the ptr */
        subtype_size = alloc_expr();
        subtype_size->literal = 1;
        subtype_size->type->specifier = TYPE_DOBL;
        subtype_size->long_literal_val=type_size(op1->type->subtype);

        /* subtype must be complete */
        if (!subtype_size->long_literal_val) {
            print_err("pointer subtype must be complete or void", 0);
        }

        /* now cast the two pointers to integers */
        type = alloc_type();
        type->specifier = TYPE_DOBL;
        type->complete = 1;
        op1 = type_cast(op1, type);
        op2 = type_cast(op2, type);
        
        /* perform the operation */
        if (!strcmp(op, "-") ||
            !strcmp(op, "<") ||
            !strcmp(op, "<=") ||
            !strcmp(op, ">") ||
            !strcmp(op, ">=") ||
            !strcmp(op, "=") ||
            !strcmp(op, "!=")) {
            
            /* do the operation normally */
            expr = do_binary(op1, op, op2);

            /* for sub operation, divide the result by ptr size */
            if (!strcmp(op, "-")) {
                expr = do_binary(expr, "/", subtype_size);
            }
            
        } else {
        
            /* operation not supported */
            print_err("invalid operation '%s' on pointers", op);
            
        }

    } else {
        /* unsupported types for operation */
        print_err("unsupported types for '%s'", op);
    }
    
    return expr;
}
