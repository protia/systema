#include <stdlib.h>
#include <string.h>

#include "common.h"

sym_t *head = NULL;

sym_t *get_sym(char *name) {
    sym_t *ptr = head;
    int done = 0;
    while (!done && ptr) {
        if (!strcmp(name, ptr->name)) {
            done = 1;
        } else {
            ptr = ptr->next;
        }
    }
    return ptr;
}

sym_t *add_sym(char *name, type_t *type) {
    sym_t *sym = alloc_sym();
    sym->next = head;
    sym->name = malloc(strlen(name)+1);
    strcpy(sym->name, name);
    sym->val = sym->name;
    sym->type = type;
    sym->scope = get_scope();
    head = sym;
    return sym;
}

void del_syms() {
    /* remove all symbols of current scope */
    while (head && head->scope == get_scope()) {
        head = head->next;
    }
}

