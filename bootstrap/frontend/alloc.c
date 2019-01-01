#include <stdlib.h>

#include "common.h"

type_t *alloc_type() {
    type_t *type = malloc(sizeof(type_t));
    type->specifier  = TYPE_VOID;
    type->complete   = 0;
    type->subcount   = 0;
    type->subtype    = NULL;
    type->rettype    = NULL;
    type->param_list = 0;
    return type;
}

sym_t *alloc_sym() {
    sym_t *sym = malloc(sizeof(sym_t));
    sym->next  = NULL;
    sym->name  = "__INITNAME__";
    sym->val   = "__INITVAL__";
    sym->type  = alloc_type();
    sym->scope = 0;
    sym->decl  = 0;
    return sym;
}

expr_t *alloc_expr() {
    expr_t *expr  = malloc(sizeof(expr_t));
    expr->addr    = "__INITADDR__";
    expr->literal = 0;
    expr->lvalue  = 0;
    expr->indir   = 0;
    expr->type    = alloc_type();
    return expr;
}

loc_t *alloc_loc() {
    loc_t *loc     = malloc(sizeof(loc_t));
    loc->specifier = 0;
    loc->reg_name  = "__INITREG__";
    loc->reg_size  = 0;
    return loc;
}

expr_list_t *alloc_expr_list() {
    expr_list_t *expr_list  = malloc(sizeof(expr_list_t));
    expr_list->count   = 0;
    expr_list->expr    = NULL;
    expr_list->sublist = NULL;
    return expr_list;
}

id_list_t *alloc_id_list() {
    id_list_t *id_list = malloc(sizeof(id_list_t));
    id_list->count   = 0;
    id_list->sym     = NULL;
    id_list->sublist = NULL;
    return id_list;
}

param_list_t *alloc_param_list() {
    param_list_t *param_list = malloc(sizeof(param_list_t));
    param_list->count   = 0;    
    param_list->name    = "__INITNAME__";
    param_list->type    = NULL;
    param_list->sublist = NULL;
    param_list->any     = 0;
    return param_list;
}

