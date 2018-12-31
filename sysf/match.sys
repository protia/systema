#include <string.h>

#include "common.h"

int type_match(type_t *type1, type_t *type2, int strict) {
    int ret;
    if (type1->specifier == TYPE_VOID) {
        /* type2 must be void */
        ret = type2->specifier == TYPE_VOID;
    } else if (type1->specifier >= TYPE_BYTE && 
               type1->specifier <= TYPE_DOBL) {
        /* type2 must also be integral */
        ret = type2->specifier >= TYPE_BYTE && 
              type2->specifier <= TYPE_DOBL;
   } else if (type1->specifier == TYPE_PTR) {
        /* type2 must be pointer to same type */
        if (type2->specifier == TYPE_PTR) {
            ret = type_match(type1->subtype, type2->subtype, 1);
        } else {
            ret = 0;
        }
   } else if (type1->specifier == TYPE_ARRAY &&
              type2->specifier == TYPE_ARRAY) {
        if (strict) {
            ret = (type1->subcount == type2->subcount) &&
                  type_match(type1->subtype, type2->subtype, 1);
        } else {
            ret = type_match(type1->subtype, type2->subtype, 0);
        }
   } else {
        /* type checking fails */
        ret = 0;
   }
   return ret;
}

