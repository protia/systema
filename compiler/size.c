#include "common.h"

int type_size(type_t *type) {
    int size;
    param_list_t *cur;
    if (type->specifier == TYPE_VOID) {
        size = 0;
    } else if (type->specifier == TYPE_BYTE) {
        size = 1;
    } else if (type->specifier == TYPE_HALF) {
        size = 2;
    } else if (type->specifier == TYPE_WORD) {
        size = 4;
    } else if (type->specifier == TYPE_DOBL) {
        size = 8;
    } else if (type->specifier == TYPE_ARRAY) {
        size = type->subcount * type_size(type->subtype);
    } else if (type->specifier == TYPE_PTR) {
        size = 8;
    } else if (type->specifier == TYPE_RECORD) {
        cur = type->param_list;
        size = 0;
        while(cur->type) {
            size += type_size(cur->type);
            cur = cur->sublist;
        }
    } else {
        print_err("cannot get size for that type.", 0);
    }
    return size;
}

