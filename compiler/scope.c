#include "common.h"

int scope = 0;

int enter_scope() {
    scope++;
}

int leave_scope() {
    scope--;
}

int get_scope() {
    return scope;
}
