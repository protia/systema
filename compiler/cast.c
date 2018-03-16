#include "common.h"

expr_t *type_cast(expr_t *before, type_t *new_type) {
    expr_t *after = alloc_expr();
    int from, to;
    int reg = emit_get_reg(REG_ACC, 0);
    /* get types */
    from = before->type->specifier;
    to = new_type->specifier;
    /* perform casting */
    if (before->literal) {
        after->literal = 1;
        after->type    = new_type;
        literal_type_cast(before, after);
    } else if (type_size(before->type) == type_size(new_type)) {
        /* no need to perform casting */
        after->type    = new_type;
        after->addr    = before->addr;
        after->indir   = before->indir;
        after->literal = 0;
    } else {
        after->literal = 0;
        after->indir   = 0; /* indirection is solved */
        after->type    = new_type;
        after->addr    = get_new_addr(type_size(after->type));
        /* perform casting */
        if (((from>=TYPE_BYTE && from<=TYPE_DOBL) || from==TYPE_PTR) &&
            ((to>=TYPE_BYTE && to<=TYPE_DOBL) || to==TYPE_PTR)) {
            /* integer/ptr to integer/ptr casting */
            emit_load(before, reg);
            /* extend */
            if (get_unsignedf()) {
                emit_extend_unsigned(before->type, after->type, reg);
            } else {
                emit_extend_signed(before->type, after->type, reg);
            }
            /* store the casted value */
            emit_store(reg, after);
        } else {
            print_err("unsupported types for casting", 0);
        }
    }
    return after;
}

