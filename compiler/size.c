/* calculate type size, used by func, dim, and sizeof */

#include "common.h"

int type_size(type_t *type) {
    int size;
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
    }
    return size;
}
