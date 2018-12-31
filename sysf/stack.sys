#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int stack_size = 0;
int last_label = 0;

void init_stack_frame() {
    stack_size = 0;
}

int get_stack_size() {
    return stack_size;
}

char *get_new_addr(int size) {
    char *ret = malloc(100);
    stack_size = (stack_size + size + 7)&(~7);
    arch_sp_fmt(ret, -stack_size);
    return ret;
}

char *get_new_label() {
    char *ret = malloc(100);
    ret[0] = '.';
    ret[1] = 'L';
    itoa(&ret[2], last_label++);
    return ret;
}

