#include <string.h>

#include "common.h"

/* literal types:
 * integer literal
 * pointer literal
 * text literal
 */

#define RESB   expr->byte_literal_val
#define RESH   expr->half_literal_val
#define RESW   expr->word_literal_val
#define RESD   expr->long_literal_val

#define OP1B   op1->byte_literal_val
#define OP1H   op1->half_literal_val
#define OP1W   op1->word_literal_val
#define OP1D   op1->long_literal_val

#define OP1BU  ((unsigned) op1->byte_literal_val)
#define OP1HU  ((unsigned) op1->half_literal_val)
#define OP1WU  ((unsigned) op1->word_literal_val)
#define OP1DU  ((unsigned) op1->long_literal_val)

#define OP2B   op2->byte_literal_val
#define OP2H   op2->half_literal_val
#define OP2W   op2->word_literal_val
#define OP2D   op2->long_literal_val

#define OP2BU  ((unsigned) op2->byte_literal_val)
#define OP2HU  ((unsigned) op2->half_literal_val)
#define OP2WU  ((unsigned) op2->word_literal_val)
#define OP2DU  ((unsigned) op2->long_literal_val)

void literal_type_cast(expr_t *before, expr_t *after) {
    /* get types */
    int from = before->type->specifier;
    int to  = after->type->specifier;
    long long holder; /* temporary holder */
    /* check types */
    if (((from < TYPE_BYTE || from > TYPE_DOBL) && from != TYPE_PTR) ||
        ((to   < TYPE_BYTE || to   > TYPE_DOBL) && to   != TYPE_PTR)) {
        print_err("unsupported types for literal casting\n", 0);
    } else {
        /* move from 'before' into 'holder' */
        if (get_unsignedf()) {
            if (from == TYPE_BYTE) {
                holder = (unsigned char) before->byte_literal_val;
            } else if (from == TYPE_HALF) {
                holder = (unsigned short) before->half_literal_val;
            } else if (from == TYPE_WORD) {
                holder = (unsigned int) before->word_literal_val;
            } else {
                holder = (unsigned long long) before->long_literal_val;
            }
        } else {
            if (from == TYPE_BYTE) {
                holder = before->byte_literal_val;
            } else if (from == TYPE_HALF) {
                holder = before->half_literal_val;
            } else if (from == TYPE_WORD) {
                holder = before->word_literal_val;
            } else {
                holder = before->long_literal_val;
            }
        }
        /* move from 'holder' into 'after' */
        if (to == TYPE_BYTE) {
            after->byte_literal_val = holder;
        } else if (to == TYPE_HALF) {
            after->half_literal_val = holder;
        } else if (to == TYPE_WORD) {
            after->word_literal_val = holder;
        } else {
            after->long_literal_val = holder;
        }
    }
}

void literal_do_unary(expr_t *expr, char *op, expr_t *op1) {
    /* expects expr to be already initialized with the target type */
    if (!strcmp(op, "!")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = !OP1B; break;
            case TYPE_HALF: RESH = !OP1H; break;
            case TYPE_WORD: RESW = !OP1W; break;
            case TYPE_DOBL: RESD = !OP1D; break;
            case TYPE_PTR:  RESD = !OP1D; break;
        }
    } else if (!strcmp(op, "~")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = ~OP1B; break;
            case TYPE_HALF: RESH = ~OP1H; break;
            case TYPE_WORD: RESW = ~OP1W; break;
            case TYPE_DOBL: RESD = ~OP1D; break;
            case TYPE_PTR:  RESD = ~OP1D; break;
        }
    } else {
        print_err("<bug>: literal_do_unary() invalid op\n", 0);
    }
}

void literal_do_binary(expr_t *expr, expr_t *op1, char *op, expr_t *op2) {

    /* expects op1 and op2 to be of the same type,
     * and expr to be already initialized with the target type
     */

    if (!strcmp(op, "+")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B + OP2B; break;
            case TYPE_HALF: RESH = OP1H + OP2H; break;
            case TYPE_WORD: RESW = OP1W + OP2W; break;
            case TYPE_DOBL: RESD = OP1D + OP2D; break;
            case TYPE_PTR:  RESD = OP1D + OP2D; break;
        }
    } else if (!strcmp(op, "-")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B - OP2B; break;
            case TYPE_HALF: RESH = OP1H - OP2H; break;
            case TYPE_WORD: RESW = OP1W - OP2W; break;
            case TYPE_DOBL: RESD = OP1D - OP2D; break;
            case TYPE_PTR:  RESD = OP1D - OP2D; break;
        }
    } else if (!strcmp(op, "*")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B * OP2B; break;
            case TYPE_HALF: RESH = OP1H * OP2H; break;
            case TYPE_WORD: RESW = OP1W * OP2W; break;
            case TYPE_DOBL: RESD = OP1D * OP2D; break;
            case TYPE_PTR:  RESD = OP1D * OP2D; break;
        }
    } else if (!strcmp(op, "/")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B / OP2B; break;
            case TYPE_HALF: RESH = OP1H / OP2H; break;
            case TYPE_WORD: RESW = OP1W / OP2W; break;
            case TYPE_DOBL: RESD = OP1D / OP2D; break;
            case TYPE_PTR:  RESD = OP1D / OP2D; break;
        }
    } else if (!strcmp(op, "%")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B % OP2B; break;
            case TYPE_HALF: RESH = OP1H % OP2H; break;
            case TYPE_WORD: RESW = OP1W % OP2W; break;
            case TYPE_DOBL: RESD = OP1D % OP2D; break;
            case TYPE_PTR:  RESD = OP1D % OP2D; break;
        }
    } else if (!strcmp(op, "=")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B == OP2B; break;
            case TYPE_HALF: RESH = OP1H == OP2H; break;
            case TYPE_WORD: RESW = OP1W == OP2W; break;
            case TYPE_DOBL: RESD = OP1D == OP2D; break;
            case TYPE_PTR:  RESD = OP1D == OP2D; break;
        }
    } else if (!strcmp(op, "!=")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B != OP2B; break;
            case TYPE_HALF: RESH = OP1H != OP2H; break;
            case TYPE_WORD: RESW = OP1W != OP2W; break;
            case TYPE_DOBL: RESD = OP1D != OP2D; break;
            case TYPE_PTR:  RESD = OP1D != OP2D; break;
        }
    } else if (!strcmp(op, ">")) {
        if (get_unsignedf()) {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1BU > OP2BU; break;
                case TYPE_HALF: RESH = OP1HU > OP2HU; break;
                case TYPE_WORD: RESW = OP1WU > OP2WU; break;
                case TYPE_DOBL: RESD = OP1DU > OP2DU; break;
                case TYPE_PTR:  RESD = OP1DU > OP2DU; break;
            }
        } else {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1B > OP2B; break;
                case TYPE_HALF: RESH = OP1H > OP2H; break;
                case TYPE_WORD: RESW = OP1W > OP2W; break;
                case TYPE_DOBL: RESD = OP1D > OP2D; break;
                case TYPE_PTR:  RESD = OP1D > OP2D; break;
            }
        }
    } else if (!strcmp(op, ">=")) {
        if (get_unsignedf()) {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1BU >= OP2BU; break;
                case TYPE_HALF: RESH = OP1HU >= OP2HU; break;
                case TYPE_WORD: RESW = OP1WU >= OP2WU; break;
                case TYPE_DOBL: RESD = OP1DU >= OP2DU; break;
                case TYPE_PTR:  RESD = OP1DU >= OP2DU; break;
            }
        } else {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1B >= OP2B; break;
                case TYPE_HALF: RESH = OP1H >= OP2H; break;
                case TYPE_WORD: RESW = OP1W >= OP2W; break;
                case TYPE_DOBL: RESD = OP1D >= OP2D; break;
                case TYPE_PTR:  RESD = OP1D >= OP2D; break;
            }
        }
    } else if (!strcmp(op, "<")) {
        if (get_unsignedf()) {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1BU < OP2BU; break;
                case TYPE_HALF: RESH = OP1HU < OP2HU; break;
                case TYPE_WORD: RESW = OP1WU < OP2WU; break;
                case TYPE_DOBL: RESD = OP1DU < OP2DU; break;
                case TYPE_PTR:  RESD = OP1DU < OP2DU; break;
            }
        } else {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1B < OP2B; break;
                case TYPE_HALF: RESH = OP1H < OP2H; break;
                case TYPE_WORD: RESW = OP1W < OP2W; break;
                case TYPE_DOBL: RESD = OP1D < OP2D; break;
                case TYPE_PTR:  RESD = OP1D < OP2D; break;
            }
        }
    } else if (!strcmp(op, "<=")) {
        if (get_unsignedf()) {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1BU <= OP2BU; break;
                case TYPE_HALF: RESH = OP1HU <= OP2HU; break;
                case TYPE_WORD: RESW = OP1WU <= OP2WU; break;
                case TYPE_DOBL: RESD = OP1DU <= OP2DU; break;
                case TYPE_PTR:  RESD = OP1DU <= OP2DU; break;
            }
        } else {
            switch(op1->type->specifier) {
                case TYPE_BYTE: RESB = OP1B <= OP2B; break;
                case TYPE_HALF: RESH = OP1H <= OP2H; break;
                case TYPE_WORD: RESW = OP1W <= OP2W; break;
                case TYPE_DOBL: RESD = OP1D <= OP2D; break;
                case TYPE_PTR:  RESD = OP1D <= OP2D; break;
            }
        }
    } else if (!strcmp(op, "|")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B | OP2B; break;
            case TYPE_HALF: RESH = OP1H | OP2H; break;
            case TYPE_WORD: RESW = OP1W | OP2W; break;
            case TYPE_DOBL: RESD = OP1D | OP2D; break;
            case TYPE_PTR:  RESD = OP1D | OP2D; break;
        }
    } else if (!strcmp(op, "&")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B & OP2B; break;
            case TYPE_HALF: RESH = OP1H & OP2H; break;
            case TYPE_WORD: RESW = OP1W & OP2W; break;
            case TYPE_DOBL: RESD = OP1D & OP2D; break;
            case TYPE_PTR:  RESD = OP1D & OP2D; break;
        }
    } else if (!strcmp(op, "^")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B ^ OP2B; break;
            case TYPE_HALF: RESH = OP1H ^ OP2H; break;
            case TYPE_WORD: RESW = OP1W ^ OP2W; break;
            case TYPE_DOBL: RESD = OP1D ^ OP2D; break;
            case TYPE_PTR:  RESD = OP1D ^ OP2D; break;
        }
    } else if (!strcmp(op, ">>")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B >> OP2B; break;
            case TYPE_HALF: RESH = OP1H >> OP2H; break;
            case TYPE_WORD: RESW = OP1W >> OP2W; break;
            case TYPE_DOBL: RESD = OP1D >> OP2D; break;
            case TYPE_PTR:  RESD = OP1D >> OP2D; break;
        }
    } else if (!strcmp(op, "<<")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B << OP2B; break;
            case TYPE_HALF: RESH = OP1H << OP2H; break;
            case TYPE_WORD: RESW = OP1W << OP2W; break;
            case TYPE_DOBL: RESD = OP1D << OP2D; break;
            case TYPE_PTR:  RESD = OP1D << OP2D; break;
        }
    } else {
        print_err("<bug>: literal_do_binary() invalid op\n", 0);
    }
}

int eval_literal_int(char *str) {
    int val = 0;
    int i;
    if (str[0] == '\'') {
        /* character */
        if (str[1] == '\\') {
            /* escaped */
            switch(str[2]) {
                case 'n':
                    val = '\n';
                    break;
                case 'r':
                    val = '\r';
                    break;
                case 't':
                    val = '\t';
                    break;
                case 'f':
                    val = '\f';
                    break;
                case 'v':
                    val = '\v';
                    break;
                default:
                    val = str[2];
            }
        } else {
            val = str[1];
        }
    } else if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        /* hexa */
        i = 2;
        while(str[i]) {
            if (str[i] >= '0' && str[i] <= '9') {
                val = val*16 + str[i] - '0';
            } else if (str[i] >= 'A' && str[i] <= 'F') {
                val = val*16 + str[i] - 'A' + 10;
            } else {
                val = val*16 + str[i] - 'a' + 10;
            }
            i++;
        }
    } else if (str[0] == '0' && (str[1] == 'o' || str[1] == 'O')) {
        /* octal */
        i = 2;
        while(str[i]) {
            val = val*8 + str[i] - '0';
            i++;
        }
    } else {
        /* decimal */
        i = 0;
        while (str[i]) {
            val = val*10 + str[i] - '0';
            i++;
        }
    }
    return val;
}

