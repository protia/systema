#include <string.h>

#include "common.h"

expr_t *parse_expr() {
    expr_t *expr;
    /* expr: assign */
    expr = parse_assign();
    /* done */
    return expr;
}

