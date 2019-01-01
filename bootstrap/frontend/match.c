#include <string.h>

#include "common.h"

int type_match(type_t *type1, type_t *type2, int strict) {
    int ret;
    param_list_t *param_list1;
    param_list_t *param_list2;
    if (type1->specifier == TYPE_VOID || type2->specifier == TYPE_VOID) {
        /* both must be void */
        if (strict) {
            ret = type1->specifier == type2->specifier;
        } else {
            ret = 1;
        }
    } else if (type1->specifier >= TYPE_BYTE &&
               type1->specifier <= TYPE_DOBL) {
        /* type2 must also be integral */
        ret = type2->specifier >= TYPE_BYTE &&
              type2->specifier <= TYPE_DOBL;
   } else if (type1->specifier == TYPE_PTR &&
              type2->specifier == TYPE_PTR) {
        /* type2 must be pointer to same type */
        if (type1->subtype == type2->subtype) {
            return 1;
        } else {
            ret = type_match(type1->subtype, type2->subtype, 0);
        }
   } else if (type1->specifier == TYPE_ARRAY &&
              type2->specifier == TYPE_ARRAY) {
        if (strict) {
            ret = (type1->subcount == type2->subcount) &&
                  type_match(type1->subtype, type2->subtype, 1);
        } else {
            ret = type_match(type1->subtype, type2->subtype, 0);
        }
   } else if (type1->specifier == TYPE_RECORD &&
              type2->specifier == TYPE_RECORD) {
        param_list1 = type1->param_list;
        param_list2 = type2->param_list;
        if (param_list1->count == param_list2->count) {
            ret = 1;
            while (param_list1->sublist != NULL) {
                if (!type_match(param_list1->type,
                                param_list2->type,
                                strict)) {
                    ret = 0;
                }
                param_list1 = param_list1->sublist;
                param_list2 = param_list2->sublist;
            }
        } else {
            ret = 0;
        }
   } else {
        /* type checking fails */
        ret = 0;
   }
   return ret;
}

