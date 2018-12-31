#include <stdlib.h>
#include <string.h>

#include "common.h"

#define LOC_REGISTER    0
#define LOC_STACK       1
#define LOC_CODE        2
#define LOC_RODATA      3
#define LOC_DATA        4
#define LOC_BSS         5

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
        print_err("expected def, dec, or typ keyword", 0);
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

loc_t *parse_in_loc() {
    /* in_loc: 'in' loc | lambda */
    loc_t *loc = NULL;
    get_lexeme();
    if (!strcmp(lex.val, "in")) {
        /* location is specified */
        loc = alloc_loc();
        get_lexeme();
        if (!strcmp(lex.val, "code")) {
            loc->specifier = STORE_CODE;
        } else if (!strcmp(lex.val, "rodata")) {
            loc->specifier = STORE_RODATA;
        } else if (!strcmp(lex.val, "data")) {
            loc->specifier = STORE_DATA;        
        } else if (!strcmp(lex.val, "bss")) {
            loc->specifier = STORE_BSS;
        } else if (!strcmp(lex.val, "stack")) {
            loc->specifier = STORE_STACK;
        } else if (!strcmp(lex.val, "register")) {
            loc->specifier = STORE_REG;
            /* parse ( */
            get_lexeme();
            if (strcmp(lex.val, "(")) {
                print_err("expected (", 0);
                unget_lexeme();
            }
            /* parse string literal */
            get_lexeme();
            if (lex.type != LEX_STR_LITERAL) {
                print_err("expected string literal", 0);
                unget_lexeme();
                loc = NULL;
            } else {
                loc->reg_name = malloc(strlen(lex.val)+1);
                strcpy(loc->reg_name, lex.val+1);
                loc->reg_name[strlen(loc->reg_name) - 1] = 0;
                loc->reg_size = emit_get_reg_size(loc->reg_name);
                if (loc->reg_size == 0) {
                    print_err("no such register", 0);
                    loc = NULL;
                }
            }
            /* parse ) */
            get_lexeme();
            if (strcmp(lex.val, ")")) {
                print_err("expected )", 0);
                unget_lexeme();
            }
        } else {
            print_err("unknown location", 0);
            loc = NULL;
        }
    } else {
        unget_lexeme();
    }
    return loc;
}

type_t *parse_as_type() {
    /* dim_type: 'as' type | lambda */
    type_t *type = NULL;
    get_lexeme();
    if (strcmp(lex.val, "as") && strcmp(lex.val, ":")) {
        unget_lexeme();
        print_err("expected as or :", 0);
    }
    /* parse type */
    type = parse_type();
    /* done */
    return type;
}

expr_list_t *parse_ass() {
    /* ass: '=' expr_list | lambda */
    expr_list_t *ret = NULL;
    get_lexeme();
    if (!strcmp(lex.val, ":=")) {
        /* parse expr_list */
        ret = parse_expr_list();
    } else {
        unget_lexeme();
    }
    return ret;
}

int parse_dim_stmt() {
    int err = 0, i;
    loc_t *loc;
    type_t *type;
    expr_list_t *expr_list, *eptr;
    id_list_t *id_list, *iptr;
    expr_t *lvalue;
    int local = 0;
    int dim_type;
        
    /* dim_stmt: dim_keyword local_keyword id_list 
     *           (as_type in_loc dim_ass | func_body) ;
     */
    
    /* parse dim keyword */
    dim_type = parse_dim_keyword();

    /* parse local keyword */
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

    /* parse id_list */
    id_list = parse_id_list();

    /* id_list size can't be zero */
    if (id_list->count == 0) {
        print_err("at least one identifier must be specified", 0); 
        err = 1;
    }

    /* parse type */
    type = parse_as_type();

    /* function or variable? */
    if (dim_type != DIM_DEF || type->specifier != TYPE_FUNC) {

        /* type must be completely specified if def */
        if (dim_type == DIM_DEF) {
            if (!type->complete) {
                print_err("type must be completely specified", 0);
                err = 1;
            }
        }
        
        /* parse location */
        loc = parse_in_loc();
                
        /* parse assignment */
        expr_list = parse_ass();
        
        /* determine location */
        if (!loc) {
            /* no location specified, use default */
            loc = alloc_loc();
            if (get_scope()) {
                loc->specifier = STORE_STACK;
            } else if (expr_list) {
                loc->specifier = STORE_DATA;
            } else {
                loc->specifier = STORE_BSS;
            }
        } else if (!err) {
            /* scope defined by user, check it. */
            if (get_scope() && loc->specifier < STORE_STACK) {
                print_err("invalid location for internal variable", 0);
                err = 1;
            } else if (!get_scope() && loc->specifier >= STORE_STACK) {
                print_err("invalid location for external variable", 0);
                err = 1;
            } else if (dim_type != DIM_DEF) {
                print_err("location can't be specified with dec/typ", 0);
                err = 1;
            } else if (loc->specifier == STORE_BSS && expr_list) {
                print_err("bss can't hold initialized data", 0);
                err = 1;
            } else if (loc->specifier == STORE_REG) {
                /* match register with type */
                if (type->specifier==TYPE_BYTE && loc->reg_size==1) {
                    /* 1 BYTE ~ OK */
                } else if (type->specifier==TYPE_HALF && loc->reg_size==2) {
                    /* 2 BYTES ~ OK */
                } else if (type->specifier==TYPE_WORD && loc->reg_size==4) {
                    /* 4 BYTES ~ OK */
                } else if (type->specifier==TYPE_DOBL && loc->reg_size==8) {
                    /* 8 BYTES ~ OK */
                } else if (type->specifier==TYPE_PTR  && loc->reg_size==8) {
                    /* 8 BYTES ~ OK */
                } else {
                    print_err("type and register specifiers do not match",0);
                    printf("reg_size: %d\n", loc->reg_size);
                    err = 1;
                }
            }
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
        
        /* initializers must be literal */
        if (!err && expr_list) {
            eptr = expr_list;
            i = 0;
            while(!err && eptr->count) {
                if (eptr->expr->literal != 1) {
                    print_err("initializers must be literals", 0);
                    err = 1;
                }
                eptr = eptr->sublist;
                i++;
            }
        }
        
        /* check initializer types */
        if (!err && expr_list) {
            eptr = expr_list;
            while(eptr->count) {
                err += !type_match(eptr->expr->type, type, 1);
                /* implicit cast needed? */
                if (!err && eptr->expr->type->specifier != type->specifier) {
                    eptr->expr = type_cast(eptr->expr, type);
                }
                eptr = eptr->sublist;
            }
            if (err) {
                print_err("types are inconsistent", 0);
            }
        }
                
        /* do action! */
        iptr = id_list;
        eptr = expr_list;
        while(!err && iptr->count) {
            if (dim_type == DIM_DEF) {
                /* definition */
                if (get_scope()) {
                    /* in register? */
                    if (loc->specifier == STORE_REG) {
                        iptr->sym->val = loc->reg_name;
                    } else {
                        /* allocate stack space */
                        iptr->sym->val = get_new_addr(type_size(type));
                    }
                    /* initialized? */
                    if (expr_list) {
                        lvalue = alloc_expr();
                        lvalue->type = type;
                        lvalue->addr = iptr->sym->val;
                        lvalue->lvalue = 1;
                        do_assign(lvalue, eptr->expr);
                    }
                    /* set symbol type */
                    iptr->sym->type = type;
                } else {                
                    /* enter section */
                    emit_section(loc->specifier);
                    
                    /* local? */
                    if (local) {
                        emit_local(iptr->sym->name);
                    } else {
                        emit_global(iptr->sym->name);
                    }

                    /* print label */
                    emit_label(iptr->sym->name);
                                        
                    /* set addr of symbol to its lbl name */
                    iptr->sym->val = iptr->sym->name;
        
                    /* if initialized, print value, else print space */
                    if (expr_list) {
                        emit_data(type, eptr->expr);
                    } else {
                        emit_space(type_size(type));                    
                    }
                    
                    /* set symbol type */
                    iptr->sym->type = type;
                }
            } else if (dim_type == DIM_DEC) {
                    /* just declare */
                    iptr->sym->type = type;
                    iptr->sym->val  = iptr->sym->name;
                    iptr->sym->decl = 1;
            } else if (dim_type == DIM_TYP) {
                    /* declare as type */
                    iptr->sym->type = alloc_type();
                    iptr->sym->type->specifier = TYPE_TYPEOF;
                    iptr->sym->type->complete = 0;
                    iptr->sym->type->subcount = 1;
                    iptr->sym->type->subtype = type;
            }
            /* next element */
            iptr = iptr->sublist;
            if (expr_list) {
                eptr = eptr->sublist;
            }
        }

    } else {

        /* inline function are currently not supported */
        if (get_scope() != 0) {
            print_err("inline functions are currently not supported", 0); 
        }

        /* encounter : */
        get_lexeme();
        if (strcmp(lex.val, ":")) {
            /* didn't find : */
            print_err("expected :", NULL);
            unget_lexeme();
        }

        /* a function! */
        emit_section(STORE_CODE);
        emit_line("");

        /* only one identifier should be named */
        if (id_list->count > 1) {
            print_err("at most one identifier must be specified", 0); 
            err = 1;
        } else if (id_list->count == 1) {
            /* local? */
            if (local) {
                emit_local(id_list->sym->name);
            } else {
                emit_global(id_list->sym->name);
            }

            /* print labels */
            emit_label(id_list->sym->name);
            
            /* set addr of symbol directly to its lbl name */
            id_list->sym->val = id_list->sym->name;
        
            /* set type of symbol as func */
            id_list->sym->type = type;

            /* parse function body */
            parse_func(id_list->sym);
        }
        
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

void parse_dim_list() {
    /* dim_list: dim_stmt dim_list | lambda */
    int done = 0;
    while (!done) {
        /* look ahead next lexeme */
        get_lexeme();
        unget_lexeme();
        /* done? */
        if (strcmp(lex.val, "def") &&
            strcmp(lex.val, "dec") &&
            strcmp(lex.val, "typ") &&
            strcmp(lex.val, "sub")) {
            done = 1;
        } else {
            /* parse dim_stmt */
            parse_dim_stmt();
        }
    }
}

