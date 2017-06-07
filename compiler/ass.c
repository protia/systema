/* assignment operation */
#include <string.h>

#include "common.h"

expr_t *parse_assign() {
    expr_t *expr, *expr1, *expr2;
    /* assign: conditional ('=' assign | lambda) */
    expr1 = parse_conditional();
    /* lookahead */
    get_lexeme();
    /* =? */
    if (!strcmp(lex.val, "=")) {
        /* assignment */
        expr2 = parse_assign();
        /* generate code for assignment */
        
        /* evaluate expr1 */
    } else {
        unget_lexeme();
        expr = expr1;
    }
    /* done */
    return expr;
}
