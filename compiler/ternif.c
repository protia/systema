/* ternary if */
#include <string.h>

#include "common.h"

expr_t *parse_conditional() {
    expr_t *expr;
    /* unimplemented, conditional: logic_or */
    expr = parse_logic_or();
    /* done */
    return expr;
}
