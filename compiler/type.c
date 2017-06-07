#include <stdlib.h>
#include <string.h>

#include "common.h"

void parse_func_type(type_t *type) {
    int done = 0;
    int count = 0;
    param_list_t *cur = alloc_param_list();
    param_list_t *top = cur;
    /* read as */
    get_lexeme();
    if (!strcmp(lex.val, "as")) {
        /* read ret type */
        type->rettype = parse_type();
        if (type->rettype->specifier != TYPE_BYTE &&
            type->rettype->specifier != TYPE_HALF &&
            type->rettype->specifier != TYPE_WORD &&
            type->rettype->specifier != TYPE_DOBL &&
            type->rettype->specifier != TYPE_PTR) {
            print_err("unsupported return type", 0);
            type->rettype = alloc_type();
        }
    } else {
        unget_lexeme();
    }
    /* ( */
    get_lexeme();
    if (strcmp(lex.val, "(")) {
        print_err("expected (", NULL);
        unget_lexeme();
    }    
    /* param list */
    get_lexeme();
    if (strcmp(lex.val, ")")) {    
        while (!done) {
            /* additional parameter */
            cur->sublist = alloc_param_list();
            cur->type = alloc_type();
            count++;
            /* read identifier */
            if (lex.type == LEX_IDENTIFIER) {
                /* copy identifier */
                cur->name = malloc(strlen(lex.val)+1);
                strcpy(cur->name, lex.val);

                /* read 'as' */
                get_lexeme();
                if (strcmp(lex.val, "as")) {
                    print_err("expected 'as'", NULL);
                    unget_lexeme();
                }
                /* read type */
                cur->type = parse_type();
                /* type must be complete */
                if (!cur->type->complete) {
                    print_err("incomplete parameter type", NULL);
                }
                /* read comma or ) */
                get_lexeme();
                if (!strcmp(lex.val, ")")) {
                    done = 1;
                } else if (!strcmp(lex.val, ",")) {
                    /* additional parameters are coming */
                    get_lexeme();
                } else {
                    print_err("expected , or )", NULL);
                    unget_lexeme();
                }
            } else if (!strcmp(lex.val, "...")) {
                cur->any  = 1;
                /* next lexeme must be ) */
                get_lexeme();
                if (strcmp(lex.val, ")")) {
                    print_err("expected  )", NULL);
                    unget_lexeme();
                }
                done = 1;                
            } else {
                print_err("expected identifier or ...", NULL);
                unget_lexeme();
                done = 1;
            }
            cur = cur->sublist;
        }
    }
    /* update type */
    type->param_list = top;
    /* recalculate size */
    cur = top;
    while (count) {
        cur->count = count--;
        cur = cur->sublist;
    }
}

type_t *parse_type() {
    /* type: type_specifier type_modifiers */
    int err = 0;
    int done = 0;
    int dim = 0;
    type_t *ptr = alloc_type();
    type_t *ptr2;
    expr_t *expr;
    /* type specifier */
    get_lexeme();
    if (!strcmp(lex.val, "void")) {
        /* void type */
        ptr->specifier = TYPE_VOID;
        ptr->complete = 0;
        ptr->subcount = 0;
        ptr->subtype = NULL;
    } else if (!strcmp(lex.val, "byte")) {
        /* byte type */
        ptr->specifier = TYPE_BYTE;
        ptr->complete = 1;
        ptr->subcount = 0;
        ptr->subtype = NULL;
    } else if (!strcmp(lex.val, "half")) {
        /* half type */
        ptr->specifier = TYPE_HALF;
        ptr->complete = 1;
        ptr->subcount = 0;
        ptr->subtype = NULL;
    } else if (!strcmp(lex.val, "word")) {
        /* word type */
        ptr->specifier = TYPE_WORD;
        ptr->complete = 1;
        ptr->subcount = 0;
        ptr->subtype = NULL;
    } else if (!strcmp(lex.val, "dobl")) {
        /* dobl type */
        ptr->specifier = TYPE_DOBL;
        ptr->complete = 1;
        ptr->subcount = 0;
        ptr->subtype = NULL;
    } else if (!strcmp(lex.val, "func")) {
        /* func type */
        ptr->specifier = TYPE_FUNC;
        ptr->complete = 0;
        ptr->subcount = 0;
        ptr->subtype = NULL;
        parse_func_type(ptr);
    } else {
        /* error? */
        unget_lexeme();
        ptr->specifier = TYPE_VOID;
        ptr->complete = 0;
        ptr->subcount = 0;
        ptr->subtype = NULL;
        print_err("expected byte, half, word, or dobl", 0);
        err = 1;
    }
    /* type modifiers */
    while(!done) {
        get_lexeme();
        if (!strcmp(lex.val, "[")) {
            /* array */
            ptr2 = alloc_type();
            ptr2->specifier = TYPE_ARRAY;
            ptr2->complete = 0;
            ptr2->subcount = 0;
            ptr2->subtype = ptr;
            ptr = ptr2;
            /* look ahead */
            get_lexeme();
            unget_lexeme();
            if (!strcmp(lex.val, "]")) {
                /* incompletely specified array */
            } else {
                /* parse expression */
                expr = parse_expr();
                /* perform some checks */
                if (!expr->literal) {
                    /* expression must be literal */
                    print_err("array size must be literal", 0);
                    dim = 0;
                } else if (expr->type->specifier != TYPE_WORD) {
                    /* expression must be integer */
                    print_err("array size must be an integer", 0);
                    dim = 0;
                } else if (!ptr->subtype->complete) {
                    /* inner array must be completely specified */
                    print_err("inner array must be completely specified", 0);
                    dim = 0;
                } else {
                    /* evaluate array size! */
                    ptr2->complete = 1;
                    /*TODO*/
                    /*ptr2->subcount = eval_int(expr->val);*/
                }
            }
            /* expected ] */
            get_lexeme();
            if (strcmp(lex.val, "]")) {
                print_err("expected ]", 0);
            }
        } else if (!strcmp(lex.val, "*")) {
            /* pointer */
            ptr2 = alloc_type();
            ptr2->specifier = TYPE_PTR;
            ptr2->complete = 1;
            ptr2->subcount = 1;
            ptr2->subtype = ptr;
            ptr = ptr2;
        } else {
            /* done */
            unget_lexeme();
            done = 1;
        }
    }
    /* finished */
    return ptr;
}
