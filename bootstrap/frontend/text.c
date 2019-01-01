#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct str {
    struct str *next;
    char *lbl;
    char *str_literal;
} str_t;

static str_t *head = NULL;
int cur_lbl = 0;

char *add_str_literal(char *str_literal) {

    str_t *str = head;
    
    /* look for matching strings */
    while (str && strcmp(str_literal, str->str_literal)) {
        str = str->next;
    }

    /* not found? allocate one */
    if (!str) {
        str = malloc(sizeof(str_t));
        str->next = head;
        str->lbl = malloc(20);
        strcpy(str->lbl, ".S");
        itoa(&str->lbl[2], cur_lbl++);
        str->str_literal = malloc(strlen(str_literal)+1);
        strcpy(str->str_literal, str_literal); 
        head = str;
    }
    
    /* done */
    return str->lbl;

}

void str_literal_flush() {
    str_t *str = head;
    emit_section(STORE_RODATA);
    if (str) {
        head = str->next;
        str_literal_flush();
        emit_label(str->lbl);
        emit_string(str->str_literal);
    }
}

