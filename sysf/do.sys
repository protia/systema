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

expr_t *do_unary(expr_t *expr, char *op, int post) {
    expr_t *ret, *expr0, *expr1, *tmp;
    int reg = emit_get_reg(REG_ACC, 0);
    char *lbl1, *lbl2;
    /* literal 0 */
    expr0 = alloc_expr();
    expr0->literal = 1;
    expr0->type->specifier = TYPE_WORD;
    expr0->word_literal_val = 0;
    /* literal 1 */
    expr1 = alloc_expr();
    expr1->literal = 1;
    expr1->type->specifier = TYPE_WORD;
    expr1->word_literal_val = 1;
    /* perform operation */
    if (!strcmp(op, "++")) {
        if (!post) {
            /* pre-increment */
            ret = do_binary(expr, "+", expr1);
            /* perform assignment */
            do_assign(expr, ret);
        } else {
            /* post-increment */
            ret = alloc_expr();
            ret->type = expr->type;
            ret->addr = get_new_addr(type_size(expr->type));
            ret->lvalue = 1;
            /* copy expr into ret */
            do_assign(ret, expr);
            /* post increment */
            tmp = do_binary(expr, "+", expr1);
            /* perform assignment */
            do_assign(expr, tmp);            
            /* ret is no longer an lvalue */
            ret->lvalue = 0;
        }
    } else if (!strcmp(op, "--")) {
        if (!post) {
            /* pre-decrement */
            ret = do_binary(expr, "-", expr1);
            /* perform assignment */
            do_assign(expr, ret);
        } else {
            /* post-decrement */
            ret = alloc_expr();
            ret->type = expr->type;
            ret->addr = get_new_addr(type_size(expr->type));
            ret->lvalue = 1;
            /* copy expr into ret */
            do_assign(ret, expr);
            /* post increment */
            tmp = do_binary(expr, "-", expr1);
            /* perform assignment */
            do_assign(expr, tmp); 
            /* ret is no longer an lvalue */
            ret->lvalue = 0;
        }
    } else if (!strcmp(op, "+")) {
        /* perform addition to zero */
        ret = do_binary(expr0, "+", expr);
    } else if (!strcmp(op, "-")) {
        /* perform subtraction from zero */
        ret = do_binary(expr0, "-", expr);
    } else if (!strcmp(op, "!")) {
        /* allocate a temporary holder */
        tmp = alloc_expr();
        tmp->type = expr->type;
        /* check type */
        if (expr->type->specifier >= TYPE_BYTE && 
            expr->type->specifier <= TYPE_DOBL) {
            if (expr->literal) {
                /* literal */
                tmp->literal = 1;
                literal_do_unary(tmp, op, expr);
            } else {
                /* allocate memory and label */
                tmp->addr = get_new_addr(type_size(tmp->type));
                lbl1 = get_new_label();
                lbl2 = get_new_label();
                /* expr = zero? */
                emit_load(expr, reg);
                emit_bze(expr->type, reg, lbl1);
                /* set acc to 0 */
                emit_load(expr0, reg);
                emit_jmp(lbl2);
                /* set acc to 1 */
                emit_label(lbl1);
                emit_load(expr1, reg);
                /* store acc into tmp */
                emit_label(lbl2);
                emit_store(reg, tmp);
            }
        } else {
            print_err("unsupported operation for the given type", 0);        
        }
        /* return tmp */
        ret = tmp;
    } else if (!strcmp(op, "~")) {
        /* allocate a temporary holder */
        tmp = alloc_expr();
        tmp->type = expr->type;
        /* check type */
        if (expr->type->specifier >= TYPE_BYTE && 
            expr->type->specifier <= TYPE_DOBL) {
            if (expr->literal) {
                /* literal */
                tmp->literal = 1;
                literal_do_unary(tmp, op, expr);
            } else {
                tmp->addr = get_new_addr(type_size(tmp->type));
                emit_load(expr, reg);
                emit_not(expr->type, reg);
                emit_store(reg, tmp);
            }
        } else {
            print_err("unsupported operation for the given type", 0);        
        }
        /* return tmp */
        ret = tmp;
    } else if (!strcmp(op, "&")) {
        /* allocate temporary variable */
        tmp = alloc_expr();
        tmp->type->specifier = TYPE_PTR;
        tmp->type->complete = 1;
        tmp->type->subtype = expr->type;
        /* expr must be an lvalue */
        if (expr->lvalue) {
            /* p = &var */
            if (!expr->indir) {
                tmp->addr = get_new_addr(type_size(expr->type));
                emit_loadaddr(expr, reg);
                emit_store(reg, tmp);
            } else {
                tmp->addr = expr->addr;
            }
        } else {
            print_err("can't reference a non lvalue expressions", 0);
        }
        /* return tmp */
        ret = tmp;
    } else if (!strcmp(op, "@")) {
        /* dereference a pointer */
        if (expr->type->specifier == TYPE_PTR) {
            /* allocate temporary variable */
            tmp = alloc_expr();
            tmp->type = expr->type;
            tmp->addr = get_new_addr(type_size(expr->type));
            /* store pointer value in tmp's addr */
            emit_load(expr, reg);
            emit_store(reg, tmp);
            /* set tmp type to that of pointed expr and mark indirect */
            tmp->type   = tmp->type->subtype;
            tmp->indir  = 1;
            tmp->lvalue = expr->lvalue;
            /* return res */
            ret = tmp;
        } else {
            print_err("can't dereference non lvalue expressions", 0);
        }
    } else {
        print_err("<bug> do_unary(): invalid op %s", op);
        ret = expr;
    } 
    /* done */
    return ret;
}

expr_t *do_binary(expr_t *op1, char *op, expr_t *op2) {
    expr_t *expr, *expr0, *expr1;
    expr_t *op_ptr, *op_incr, *subtype_size;
    expr_t *ptr_as_long, *add_val;
    type_t *type;
    param_list_t *cur;
    char *lbl1, *lbl2, *lbl3;
    int reg1 = emit_get_reg(REG_ACC, 0);
    int reg2 = emit_get_reg(REG_ACC, 1);
    int offset = 0, found = 0;
    /* container for the result */
    expr = alloc_expr();
    /* literal 0 */
    expr0 = alloc_expr();
    expr0->literal = 1;
    expr0->type->specifier = TYPE_WORD;
    expr0->word_literal_val = 0;
    /* literal 1 */
    expr1 = alloc_expr();
    expr1->literal = 1;
    expr1->type->specifier = TYPE_WORD;
    expr1->word_literal_val = 1;
    /* perform action */
    if (!strcmp(op, "[")) {
    
        /* array subscript */
        if (op1->type->specifier == TYPE_ARRAY) {
            /* subscript must be integer */
            if (op2->type->specifier >= TYPE_BYTE &&
                op2->type->specifier <= TYPE_DOBL) {
                /* load the address of the first element in the 
                 * array to a pointer to array's subtype
                 * then perform pointer arithmetic
                 * ex: to evaluate arr[i]
                 *        let p pointer = &arr[0]
                 *            p += i
                 *            return *p as indirect lvalue var 
                 */
                expr = alloc_expr();
                expr->type->specifier = TYPE_PTR;
                expr->type->complete = 1;
                expr->type->subtype = op1->type->subtype;
                /* p = &arr[0] */
                if (!op1->indir) {
                    expr->addr = get_new_addr(type_size(expr->type));
                    emit_loadaddr(op1, reg1);
                    emit_store(reg1, expr);
                } else {
                    expr->addr = op1->addr;
                }
                /* p += i */
                expr = do_binary(expr, "+", op2);
                /* now expr holds the destined address */
                expr->type = expr->type->subtype;
                expr->lvalue = 1;
                expr->indir = 1;              
            } else {
                print_err("array subscript must be integer", 0);
                expr = op1;
                expr->type = op1->type->subtype;
            }
        } else {
            /* must be applied to an array type */
            print_err("[] can't be applied to a non-array type", 0);
            expr = op1;
        }
        
    } else if (!strcmp(op, ".")) {

        /* record member resolution */
        if (op1->type->specifier == TYPE_RECORD) {
            /* op2 must be a member of the record */
            offset = 0;
            found = 0;
            cur = op1->type->param_list;
            while(cur->type) {
                if (!strcmp(cur->name, op2->addr)) {
                    found = 1;
                    break;
                }
                offset += type_size(cur->type);
                cur = cur->sublist;
            }
            /* found? */
            if (found) {
                /* load the address of the record to 
                 * to a pointer of type @byte,
                 * then perform pointer arithmetic
                 * ex: to evaluate record member
                 *        let p pointer = &arr[0]
                 *            p += member_offset
                 *            return *p as indirect lvalue var 
                 */
                expr = alloc_expr();
                expr->type->specifier = TYPE_PTR;
                expr->type->complete = 1;
                expr->type->subtype = alloc_type();
                expr->type->subtype->specifier = TYPE_BYTE;
                expr->type->subtype->complete = 1;
                expr->type->subtype->subcount = 0;
                expr->type->subtype->subtype = NULL;
                /* p = &arr[0] */
                if (!op1->indir) {
                    expr->addr = get_new_addr(type_size(expr->type));
                    emit_loadaddr(op1, reg1);
                    emit_store(reg1, expr);
                } else {
                    expr->addr = op1->addr;
                }
                /* store offset in op2 */
                op2->literal = 1;
                op2->type = alloc_type();
                op2->type->specifier = TYPE_WORD;
                op2->type->complete = 1;
                op2->type->subcount = 0;
                op2->type->subtype = NULL;
                op2->word_literal_val = offset;
                /* p += i */
                expr = do_binary(expr, "+", op2);
                /* now expr holds the destined address */
                expr->type = cur->type;
                expr->lvalue = 1;
                expr->indir = 1;              
            } else {
                print_err("cannot match record member", 0);
                expr = op1;
            }
        } else {
            /* must be applied to record type */
            print_err(". can't be applied to a non-record type", 0);
            expr = op1;
        }

    } else if (op1->type->specifier >= TYPE_BYTE && 
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
            !strcmp(op, "==") || !strcmp(op, "!=")) {
            /* result type is word */
            expr->type = alloc_type();
            expr->type->specifier = TYPE_WORD;
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
                if (get_unsignedf()) {
                    emit_bgt_unsigned(op1->type, reg1, reg2, lbl2);
                } else {
                    emit_bgt(op1->type, reg1, reg2, lbl2);
                }
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, ">=")) {
                if (get_unsignedf()) {
                    emit_bge_unsigned(op1->type, reg1, reg2, lbl2);
                } else {
                    emit_bge(op1->type, reg1, reg2, lbl2);
                }
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, "<")) {
                if (get_unsignedf()) {
                    emit_blt_unsigned(op1->type, reg1, reg2, lbl2);
                } else {
                    emit_blt(op1->type, reg1, reg2, lbl2);
                }
                emit_label(lbl1);
                emit_load(expr0, reg1);
                emit_jmp(lbl3);
                emit_label(lbl2);
                emit_load(expr1, reg1);
                emit_label(lbl3);    
            } else if (!strcmp(op, "<=")) {
                if (get_unsignedf()) {
                    emit_ble_unsigned(op1->type, reg1, reg2, lbl2);
                } else {
                    emit_ble(op1->type, reg1, reg2, lbl2);
                }
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
                if (get_unsignedf()) {
                    emit_srl(op1->type, reg2, reg1);
                } else {
                    emit_sra(op1->type, reg2, reg1);
                }
            } else if (!strcmp(op, "<<")) {
                emit_sll(op1->type, reg2, reg1);
            } else {
                print_err("<bug> do_binary(): invalid op %s", op);
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
            !strcmp(op, "==") ||
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
    /* done */
    return expr;
}

