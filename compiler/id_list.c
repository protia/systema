#include <string.h>

#include "common.h"

id_list_t *parse_id_list() {
    int done = 0;
    int count = 0;
    sym_t *sym;
    /* id_list: identifier (',' id_list | lambda) */
    id_list_t *top = alloc_id_list();
    id_list_t *cur = top;
    top->sublist = NULL;
    top->count = 0;
    top->sym = NULL;
    while (!done) {
        /* read identifier */
        get_lexeme();
        if (lex.type != LEX_IDENTIFIER) {
            print_err("expected identifier", NULL);
            unget_lexeme();
        } else if ((sym = get_sym(lex.val)) &&
                   sym->scope == get_scope() &&
                   !sym->decl) {
            /* symbol already defined! */
            print_err("symbol already defined", 0);
        } else {
            /* add to id list */
            if ((sym = get_sym(lex.val)) &&
                sym->scope == get_scope()) {
                /* already declared with some type */
                cur->sym = add_sym(lex.val, sym->type);
            } else {
                /* assume void until initialized */
                cur->sym = add_sym(lex.val, alloc_type());
            }
            cur->sublist = alloc_id_list();
            cur->sublist->count = 0;
            cur->sublist->sym = NULL;
            cur->sublist->sublist = NULL;
            cur = cur->sublist;
            count++;
        }
        /* read , */
        get_lexeme();
        if (strcmp(lex.val, ",")) {
            /* no more identifiers */
            unget_lexeme();
            done = 1;
        }
    }
    /* modify count */
    cur = top;
    while(cur) {
        cur->count = count;
        count--;
        cur = cur->sublist;
    }
    return top;
}

