#include <stdlib.h>
#include <string.h>

#include "common.h"

void parse_record_header(type_t *type) {
    int done = 0;
    int count = 0;
    param_list_t *cur = alloc_param_list();
    param_list_t *top = cur;
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
                if (strcmp(lex.val, "as") && strcmp(lex.val, ":")) {
                    print_err("expected 'as' or :", NULL);
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
    /* update type structure */
    type->param_list = top;
    /* recalculate size */
    cur = top;
    while (count) {
        cur->count = count--;
        cur = cur->sublist;
    }
}

