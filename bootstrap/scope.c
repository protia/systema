#include "common.h"

int scope = 0;

void enter_scope() {
    scope++;
}

void leave_scope() {
    scope--;
}

int get_scope() {
    return scope;
}

