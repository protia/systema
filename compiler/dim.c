#include <stdlib.h>
#include <string.h>

#include "common.h"

int parse_dim_keyword() {
    /* dim_keyword: def | dec | typ */
    int dim_type = 0;
    get_lexeme();
    if (!strcmp(lex.val, "def")) {
        /* define */
        dim_type = DIM_DEF;
    } else if (!strcmp(lex.val, "dec")) {
        /* declare */
        dim_type = DIM_DEC;
    } else if (!strcmp(lex.val, "typ")) {
        /* type */
        dim_type = DIM_TYP;
    } else {
        /* expected dim keyword */
        print_err("expected def, dec, or typ keyword.", 0);
        /* assume default */
        dim_type = DIM_DEF;
    }
    return dim_type;
}

int parse_local_keyword() {
    /* local_keyword: 'local' | lambda */
    int ret;
    get_lexeme();
    if (!strcmp(lex.val, "local")) {
        ret = 1;
    } else {
        unget_lexeme();
        ret = 0;    
    }
    return ret;
}

type_t *parse_as_type() {
    /* dim_type: 'as' type | lambda */
    type_t *type = NULL;
    get_lexeme();
    if (!strcmp(lex.val, "as")) {
        /* parse type */
        type = parse_type(1);
    } else {
        unget_lexeme();
    }
    return type;
}

expr_list_t *parse_ass() {
    /* ass: '=' expr_list | lambda */
    expr_list_t *ret = NULL;
    get_lexeme();
    if (!strcmp(lex.val, "=")) {
        /* parse expr_list */
        ret = parse_expr_list();
    } else {
        unget_lexeme();
    }
    return ret;
}

int parse_dim_stmt() {
    int err = 0, i;
    type_t *type;
    expr_list_t *expr_list, *eptr;
    id_list_t *id_list, *iptr;
    int *storage;
    type_t **types;
    int local = 0;
    int dim_type;
    /* dim_stmt: dim_keyword local_keyword id_list as_type dim_ass ;
     */
     
    /* put all generated code in m4 macro */
    if (!get_scope()) {
        emit_def_m4_macro("def");
    }
    
    /* 1: encounter dim keyword */
    dim_type = parse_dim_keyword();

    /* 2: encounter local keyword */
    local = parse_local_keyword();
    
    /* word local cannot be used within stack scope */
    if (get_scope() && local) {
        print_err("cannot use 'local' in this scope", 0);
        err = 1;
    }
    
    /* local cannot be used with dec and typ */
    if ((dim_type == DIM_TYP || dim_type == DIM_DEC) && local) {
        print_err("uage of 'local' with dec/typ is meaningless", 0);
        err = 1;
    }

    /* 3: id_list */
    id_list = parse_id_list();

    /* id_list size can't be zero */
    if (id_list->count == 0) {
        print_err("at least one identifier must be specified", 0); 
        err = 1;
    }

    /* 4: parse type */
    type = parse_as_type();

    /* 5: parse ass */
    expr_list = parse_ass();
    
    /* code generation done */
    if (!get_scope()) {
        emit_fed_m4_macro();
    }
    
    /* if no type & no expr_list, generate error */
    if (!type && !expr_list) {
        print_err("either type or initial value should be specified", 0);
        err = 1;
    }
    
    /* if expr_list is specified, check size */
    if (expr_list && expr_list->count != id_list->count) {
        print_err("expression list size <> identifiers", 0); 
        err = 1;       
    }

    /* dec and typ cannot have initializers */
    if ((dim_type == DIM_TYP || dim_type == DIM_DEC) && expr_list) {
        print_err("dec/typ cannot have initializers", 0); 
        err = 1;  
    }

    /* if both type and expr_list is specified, types must be consistent */
    if (!err && type && expr_list) {
        eptr = expr_list;
        while(eptr->count) {
            err += !type_match(eptr->expr->type, type, 1);
            eptr = eptr->sublist;
        }
        if (err) {
            print_err("types are inconsistent", 0);
        }
    }
    
    /* types must be completely specified if def */
    if (!err && dim_type == DIM_DEF) {
        if (type) {
            /* get type from type */
            if (!type->complete) {
                print_err("type must be completely specified", 0);
                err = 1;
            }
        } else {
            /* get type from expr_list */
            eptr = expr_list;
            while(eptr->count) {
                if (!eptr->expr->type->complete) {
                    print_err("expression types must be complete", 0);
                }
                eptr = eptr->sublist;
            }
            
        }
    }
    
    /* determine types */
    if (!err) {
        types = malloc(id_list->count * sizeof(types[0]));
        eptr = expr_list;
        iptr = id_list;
        for (i = 0; i < id_list->count; i++) {
            if (type) {
                iptr->sym->type = types[i] = type;
                iptr = iptr->sublist;
            } else {
                iptr->sym->type = types[i] = eptr->expr->type;
                eptr = eptr->sublist;
                iptr = iptr->sublist;
            }
        }
    }

    /* determine storage */
    if (!err) {
        storage = malloc(id_list->count * sizeof(storage[0]));
        for (i = 0; i < id_list->count; i++) {
            if (get_scope()) {
                storage[i] = STORE_STACK;
            } else {
                /* if type[i] is func, store in code, else: */
                if (types[i]->specifier == TYPE_FUNC) {
                    storage[i] = STORE_CODE;
                } else if (expr_list) {
                    storage[i] = STORE_DATA;
                } else {
                    storage[i] = STORE_BSS;
                }
            }
        }
    }
    
    /* if storage is not stack, all data must be literal */
    if (!err && expr_list) {
        eptr = expr_list;
        i = 0;
        while(!err && eptr->count) {
            if (storage[i] != STORE_STACK &&
                eptr->expr->literal != 1) {
                print_err("expression list must be literals", 0);
                err = 1;
            }
            eptr = eptr->sublist;
            i++;
        }
    }
    
    /* perform action */
    i = 0;
    eptr = expr_list;
    iptr = id_list;
    if (!err) {
        for (i = 0; i < id_list->count; i++) {
            if (dim_type == DIM_DEF) {
                /* definition */
                if (storage[i] == STORE_STACK) {
                    /* stack storage */
                    iptr->sym->val = get_new_addr(type_size(types[i]));
                    
                    /* initialized? */
                    /* TODO */
                    
                    /* set symbol type */
                    iptr->sym->type = types[i];
                } else {
                    /* enter section */
                    emit_section(storage[i]);
                    
                    /* local? */
                    if (local) {
                        emit_local(iptr->sym->name);
                    } else {
                        emit_global(iptr->sym->name);
                    }

                    /* print label */
                    emit_label(iptr->sym->name);
                                        
                    /* set value of symbol */
                    iptr->sym->val = iptr->sym->name;
                    
                    /* if initialized, print value, else print space */
                    if (types[i]->specifier == TYPE_FUNC) {
                    
                    } else if (storage[i] == STORE_DATA) {
                        emit_data(types[i], eptr->expr);
                    } else {
                        emit_space(type_size(types[i]));                    
                    }
                    
                    /* set symbol type */
                    iptr->sym->type = types[i];
                }
            } else if (dim_type == DIM_DEC) {
                /* just declare */
                iptr->sym->type = types[i];
                iptr->sym->val  = iptr->sym->name;
                iptr->sym->decl = 1;
            } else if (dim_type == DIM_TYP) {
                /* declare as type */
                iptr->sym->type = alloc_type();
                iptr->sym->type->specifier = TYPE_TYPEOF;
                iptr->sym->type->complete = 0;
                iptr->sym->type->subcount = 1;
                iptr->sym->type->subtype = types[i];
            }
            if (eptr)
                eptr = eptr->sublist;
            iptr = iptr->sublist;
        }
    }
    
    /* generate code */
    if (!get_scope()) {
        emit_use_m4_macro("def");
    }
    
    /* encounter ; */
    get_lexeme();
    if (strcmp(lex.val, ";")) {
        /* didn't find ; */
        print_err("expected ;", NULL);
        unget_lexeme();
    }

    /* done */
    return 0;
}

int parse_dim_list() {
    /* dim_list: dim_stmt dim_list | lambda */
    int done = 0;
    while (!done) {
        /* look ahead next lexeme */
        get_lexeme();
        unget_lexeme();
        /* done? */
        if (strcmp(lex.val, "def") &&
            strcmp(lex.val, "dec") &&
            strcmp(lex.val, "typ")) {
            done = 1;
        } else {
            /* parse dim_stmt */
            parse_dim_stmt();
        }
    }
    return 0;
}
