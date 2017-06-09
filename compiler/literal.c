/* special handling for literals */
#include <string.h>

#include "common.h"

/* literal types:
 * integer literal
 * pointer literal
 * text literal
 */

void literal_type_cast(expr_t *before, expr_t *after) {
    /* get types */
    int from = before->type->specifier;
    int to  = after->type->specifier;
    /* perform casting */
    if (from == TYPE_BYTE && to == TYPE_BYTE) {
        /* byte to byte casting */
        after->byte_literal_val = before->byte_literal_val;
    } else if (from == TYPE_BYTE && to == TYPE_HALF) {
        /* byte to half casting */
        after->half_literal_val = before->byte_literal_val;
    } else if (from == TYPE_BYTE && to == TYPE_WORD) {
        /* byte to word casting */
        after->word_literal_val = before->byte_literal_val;
    } else if (from == TYPE_BYTE && to == TYPE_DOBL) {
        /* byte to long casting */
        after->long_literal_val = before->byte_literal_val;
    } else if (from == TYPE_BYTE && to == TYPE_PTR) {
        /* byte to ptr casting */
        after->long_literal_val = before->byte_literal_val;
    } else if (from == TYPE_HALF && to == TYPE_BYTE) {
        /* half to byte casting */
        after->byte_literal_val = before->half_literal_val;
    } else if (from == TYPE_HALF && to == TYPE_HALF) {
        /* half to half casting */
        after->half_literal_val = before->half_literal_val;
    } else if (from == TYPE_HALF && to == TYPE_WORD) {
        /* half to word casting */
        after->word_literal_val = before->half_literal_val;
    } else if (from == TYPE_HALF && to == TYPE_DOBL) {
        /* half to long casting */
        after->long_literal_val = before->half_literal_val;
    } else if (from == TYPE_HALF && to == TYPE_PTR) {
        /* half to ptr casting */
        after->long_literal_val = before->half_literal_val;
    } else if (from == TYPE_WORD && to == TYPE_BYTE) {
        /* word to byte casting */
        after->byte_literal_val = before->word_literal_val;
    } else if (from == TYPE_WORD && to == TYPE_HALF) {
        /* word to half casting */
        after->half_literal_val = before->word_literal_val;
    } else if (from == TYPE_WORD && to == TYPE_WORD) {
        /* word to word casting */
        after->word_literal_val = before->word_literal_val;
    } else if (from == TYPE_WORD && to == TYPE_DOBL) {
        /* word to dobl casting */
        after->long_literal_val = before->word_literal_val;
    } else if (from == TYPE_WORD && to == TYPE_PTR) {
        /* word to ptr casting */
        after->long_literal_val = before->word_literal_val;
    } else if (from == TYPE_DOBL && to == TYPE_BYTE) {
        /* long to byte casting */
        after->byte_literal_val = before->long_literal_val;
    } else if (from == TYPE_DOBL && to == TYPE_HALF) {
        /* long to half casting */
        after->half_literal_val = before->long_literal_val;
    } else if (from == TYPE_DOBL && to == TYPE_WORD) {
        /* long to word casting */
        after->word_literal_val = before->long_literal_val;
    } else if (from == TYPE_DOBL && to == TYPE_DOBL) {
        /* long to dobl casting */
        after->long_literal_val = before->long_literal_val;
    } else if (from == TYPE_DOBL && to == TYPE_PTR) {
        /* long to ptr casting */
        after->long_literal_val = before->long_literal_val;
    } else if (from == TYPE_PTR  && to == TYPE_BYTE) {
        /* ptr to byte casting */
        after->byte_literal_val = before->long_literal_val;
    } else if (from == TYPE_PTR  && to == TYPE_HALF) {
        /* ptr to half casting */
        after->half_literal_val = before->long_literal_val;
    } else if (from == TYPE_PTR  && to == TYPE_WORD) {
        /* ptr to word casting */
        after->word_literal_val = before->long_literal_val;
    } else if (from == TYPE_PTR  && to == TYPE_DOBL) {
        /* ptr to dobl casting */
        after->long_literal_val = before->long_literal_val;
    } else if (from == TYPE_PTR  && to == TYPE_PTR) {
        /* ptr to ptr casting */
        after->long_literal_val = before->long_literal_val;
    } else {
        print_err("unsupported types for casting\n", 0);
    }
}

void literal_do_binary(expr_t *expr, expr_t *op1, char *op, expr_t *op2) {

    /* expects op1 and op2 to be of the same type [or ptr with long],
     * and expr to be already initialized with the target type
     */

#define RESB   expr->byte_literal_val
#define RESH   expr->half_literal_val
#define RESW   expr->word_literal_val
#define RESD   expr->long_literal_val

#define OP1B   op1->byte_literal_val
#define OP1H   op1->half_literal_val
#define OP1W   op1->word_literal_val
#define OP1D   op1->long_literal_val

#define OP2B   op2->byte_literal_val
#define OP2H   op2->half_literal_val
#define OP2W   op2->word_literal_val
#define OP2D   op2->long_literal_val

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
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B > OP2B; break;
            case TYPE_HALF: RESH = OP1H > OP2H; break;
            case TYPE_WORD: RESW = OP1W > OP2W; break;
            case TYPE_DOBL: RESD = OP1D > OP2D; break;
            case TYPE_PTR:  RESD = OP1D > OP2D; break;
        }
    } else if (!strcmp(op, ">=")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B >= OP2B; break;
            case TYPE_HALF: RESH = OP1H >= OP2H; break;
            case TYPE_WORD: RESW = OP1W >= OP2W; break;
            case TYPE_DOBL: RESD = OP1D >= OP2D; break;
            case TYPE_PTR:  RESD = OP1D >= OP2D; break;
        }
    } else if (!strcmp(op, "<")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B < OP2B; break;
            case TYPE_HALF: RESH = OP1H < OP2H; break;
            case TYPE_WORD: RESW = OP1W < OP2W; break;
            case TYPE_DOBL: RESD = OP1D < OP2D; break;
            case TYPE_PTR:  RESD = OP1D < OP2D; break;
        }
    } else if (!strcmp(op, "<=")) {
        switch(op1->type->specifier) {
            case TYPE_BYTE: RESB = OP1B <= OP2B; break;
            case TYPE_HALF: RESH = OP1H <= OP2H; break;
            case TYPE_WORD: RESW = OP1W <= OP2W; break;
            case TYPE_DOBL: RESD = OP1D <= OP2D; break;
            case TYPE_PTR:  RESD = OP1D <= OP2D; break;
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

