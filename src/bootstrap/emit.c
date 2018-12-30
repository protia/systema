#include "common.h"

FILE *emit_fd;

int cur_section = -1;

void emit_init(FILE *fd) {
    emit_fd = fd;
}

void emit_line(char *line) {
    fprintf(emit_fd, "\t%s\n", line);
}

void emit_comment(char *comment) {
    fprintf(emit_fd, "\t# %s\n", comment);
}

void emit_section(int section) {
    if (cur_section != section) {
        cur_section = section;
        if (section == STORE_CODE) {
            fprintf(emit_fd, "\n.text\n");
        } else if (section == STORE_DATA) {
            fprintf(emit_fd, "\n.data\n");
        } else if (section == STORE_BSS) {
            fprintf(emit_fd, "\n.bss\n");
        } else if (section == STORE_RODATA) {
            fprintf(emit_fd, "\n.section \"rodata\", \"ax\"\n");        
        }
    }
}

void emit_local(char *sym) {
    fprintf(emit_fd, ".local %s\n", sym);
}

void emit_global(char *sym) {
    fprintf(emit_fd, ".global %s\n", sym);
}

void emit_label(char *lbl) {
    fprintf(emit_fd, "%s:", lbl);
}

void emit_data(type_t *type, expr_t *expr) {
    if (type->specifier == TYPE_BYTE) {
        fprintf(emit_fd, "\t.byte %d\n", expr->byte_literal_val);
    } else if (type->specifier == TYPE_HALF) {
        fprintf(emit_fd, "\t.half %d\n", expr->half_literal_val);
    } else if (type->specifier == TYPE_WORD) {
        fprintf(emit_fd, "\t.word %d\n", expr->word_literal_val);
    } else if (type->specifier == TYPE_DOBL) {
        fprintf(emit_fd, "\t.quad %lld\n", expr->long_literal_val);
    } else if (type->specifier == TYPE_PTR) {
        fprintf(emit_fd, "\t.quad %p\n", (void *) expr->long_literal_val);
    } else if (type->specifier == TYPE_ARRAY) {
        /* composite type */
    } else {
        /* composite type */
    }
}

void emit_string(char *str) {
    fprintf(emit_fd, "\t.string %s\n", str);
}

void emit_space(int space) {
    fprintf(emit_fd, "\t.space %d\n", space);
}

void emit_func_entry(char *stack_sym) {
    arch_func_entry(stack_sym);
}

void emit_func_leave() {
    arch_func_leave();
}

void emit_set(char *sym, int val) {
    fprintf(emit_fd, "\t.set   %s, %d\n", sym, val);
}

void emit_jmp(char *lbl) {
    arch_jmp(lbl);
}

int emit_get_reg(int usage, int indx) {
    return arch_get_reg(usage, indx);
}

int emit_get_reg_size(char *reg_name) {
    return arch_get_reg_size(reg_name);
}

void emit_load(expr_t *expr, int reg) {
    /* load expr into reg */
    int indx_reg = arch_get_reg(REG_IDX, 0);
    switch (expr->type->specifier) {
        case TYPE_BYTE:
            if (expr->literal) {
                /* is literal */
                arch_loadb_literal(expr->byte_literal_val, reg);
            } else {
                if (expr->indir) {
                    /* indirect load */
                    arch_pushl(indx_reg);
                    arch_loadl(expr->addr, indx_reg);
                    arch_loadb_indx(indx_reg, reg);
                    arch_popl(indx_reg);
                } else {
                    /* direct load */
                    arch_loadb(expr->addr, reg);
                }
            }
            break;
        case TYPE_HALF:
            if (expr->literal) {
                /* is literal */
                arch_loadh_literal(expr->half_literal_val, reg);
            } else {
                if (expr->indir) {
                    /* indirect load */
                    arch_pushl(indx_reg);
                    arch_loadl(expr->addr, indx_reg);
                    arch_loadh_indx(indx_reg, reg);
                    arch_popl(indx_reg);
                } else {
                    /* direct load */
                    arch_loadh(expr->addr, reg);
                }
            }
            break;
        case TYPE_WORD:
            if (expr->literal) {
                /* is literal */
                arch_loadw_literal(expr->word_literal_val, reg);
            } else {
                if (expr->indir) {
                    /* indirect load */
                    arch_pushl(indx_reg);
                    arch_loadl(expr->addr, indx_reg);
                    arch_loadw_indx(indx_reg, reg);
                    arch_popl(indx_reg);
                } else {
                    /* direct load */
                    arch_loadw(expr->addr, reg);
                }
            }
            break;
        case TYPE_DOBL:
            if (expr->literal) {
                /* is literal */
                arch_loadl_literal(expr->long_literal_val, reg);
            } else {
                if (expr->indir) {
                    /* indirect load */
                    arch_pushl(indx_reg);
                    arch_loadl(expr->addr, indx_reg);
                    arch_loadl_indx(indx_reg, reg);
                    arch_popl(indx_reg);
                } else {
                    /* direct load */
                    arch_loadl(expr->addr, reg);
                }
            }
            break;
        case TYPE_PTR:
            if (expr->literal) {
                /* is literal */
                if (expr->addr) {
                    /* pointer to string literal */
                    arch_leal(expr->addr, reg);
                } else {
                    /* integral value casted to pointer */
                    arch_loadl_literal(expr->long_literal_val, reg);
                }
            } else {
                if (expr->indir) {
                    /* indirect load */
                    arch_pushl(indx_reg);
                    arch_loadl(expr->addr, indx_reg);
                    arch_loadl_indx(indx_reg, reg);
                    arch_popl(indx_reg);
                } else {
                    /* direct load */
                    arch_loadl(expr->addr, reg);
                }
            }
            break;
        default:
            print_err("<bug> unsupported emit_load() type", 0);
            break;
    }
}

void emit_loadaddr(expr_t *expr, int reg) {
    arch_leal(expr->addr, reg);
}

void emit_store(int reg, expr_t *expr) {
    /* store reg into expr */
    int indx_reg = arch_get_reg(REG_IDX, 0);
    if (expr->literal) {
        print_err("<bug> emit_store() into literal", 0);
    }
    switch (expr->type->specifier) {
        case TYPE_BYTE:
            if (expr->indir) {
                /* indirect store */
                arch_pushl(indx_reg);
                arch_loadl(expr->addr, indx_reg);
                arch_storeb_indx(reg, indx_reg);
                arch_popl(indx_reg);
            } else {
                /* direct load */
                arch_storeb(reg, expr->addr);
            }
            break;
        case TYPE_HALF:
            if (expr->indir) {
                /* indirect store */
                arch_pushl(indx_reg);
                arch_loadl(expr->addr, indx_reg);
                arch_storeh_indx(reg, indx_reg);
                arch_popl(indx_reg);
            } else {
                /* direct load */
                arch_storeh(reg, expr->addr);
            }
            break;
        case TYPE_WORD:
            if (expr->indir) {
                /* indirect store */
                arch_pushl(indx_reg);
                arch_loadl(expr->addr, indx_reg);
                arch_storew_indx(reg, indx_reg);
                arch_popl(indx_reg);
            } else {
                /* direct load */
                arch_storew(reg, expr->addr);
            }
            break;
        case TYPE_DOBL:
            if (expr->indir) {
                /* indirect store */
                arch_pushl(indx_reg);
                arch_loadl(expr->addr, indx_reg);
                arch_storel_indx(reg, indx_reg);
                arch_popl(indx_reg);
            } else {
                /* direct load */
                arch_storel(reg, expr->addr);
            }
            break;
        case TYPE_PTR:
            if (expr->indir) {
                /* indirect store */
                arch_pushl(indx_reg);
                arch_loadl(expr->addr, indx_reg);
                arch_storel_indx(reg, indx_reg);
                arch_popl(indx_reg);
            } else {
                /* direct load */
                arch_storel(reg, expr->addr);
            }
            break;
        default:
            print_err("<bug> unsupported emit_store() type", 0);
            break;
    }
}

void emit_extend_signed(type_t *src_type, type_t *dest_type, int reg) {
    int from = src_type->specifier;
    int to = dest_type->specifier;
    /* signed extension */
    if (from == TYPE_BYTE && to == TYPE_HALF) {
        arch_extbh(reg);
    } else if (from == TYPE_BYTE && to == TYPE_WORD) {
        arch_extbw(reg);
    } else if (from == TYPE_BYTE && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_extbl(reg);
    } else if (from == TYPE_HALF && to == TYPE_WORD) {
        arch_exthw(reg);
    } else if (from == TYPE_HALF && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_exthl(reg);    
    } else if (from == TYPE_WORD && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_extwl(reg);
    } else {
        /* do nothing */
    }
}

void emit_extend_unsigned(type_t *src_type, type_t *dest_type, int reg) {
    int from = src_type->specifier;
    int to = dest_type->specifier;
    /* unsigned extension */
    if (from == TYPE_BYTE && to == TYPE_HALF) {
        arch_extbh_zero(reg);
    } else if (from == TYPE_BYTE && to == TYPE_WORD) {
        arch_extbw_zero(reg);
    } else if (from == TYPE_BYTE && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_extbl_zero(reg);
    } else if (from == TYPE_HALF && to == TYPE_WORD) {
        arch_exthw_zero(reg);
    } else if (from == TYPE_HALF && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_exthl_zero(reg);    
    } else if (from == TYPE_WORD && (to == TYPE_DOBL || to == TYPE_PTR)) {
        arch_extwl_zero(reg);
    } else {
        /* do nothing */
    }
}

void emit_add(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_addb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_addh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_addw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_addl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_add(): invalid type", 0);
            break;
    }
}

void emit_sub(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_subb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_subh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_subw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_subl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_sub(): invalid type", 0);
            break;
    }
}

void emit_mul(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_mulb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_mulh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_mulw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_mull(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_mul(): invalid type", 0);
            break;
    }
}

void emit_div(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_divb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_divh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_divw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_divl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_div(): invalid type", 0);
            break;
    }
}

void emit_mod(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_modb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_modh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_modw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_modl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_mod(): invalid type", 0);
            break;
    }
}

void emit_not(type_t *type, int reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_notb(reg);
            break;
        case TYPE_HALF:
            arch_noth(reg);
            break;
        case TYPE_WORD:
            arch_notw(reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_notl(reg);
            break;
        default:
            print_err("<bug> arch_not(): invalid type", 0);
            break;
    }
}

void emit_and(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_andb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_andh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_andw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_andl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_and(): invalid type", 0);
            break;
    }
}

void emit_eor(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_eorb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_eorh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_eorw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_eorl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_eor(): invalid type", 0);
            break;
    }
}

void emit_xor(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_xorb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_xorh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_xorw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_xorl(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_xor(): invalid type", 0);
            break;
    }
}

void emit_sll(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_sllb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_sllh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_sllw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_slll(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_sll(): invalid type", 0);
            break;
    }
}

void emit_srl(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_srlb(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_srlh(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_srlw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_srll(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_srl(): invalid type", 0);
            break;
    }
}

void emit_sra(type_t *type, int src_reg, int dest_reg) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_srab(src_reg, dest_reg);
            break;
        case TYPE_HALF:
            arch_srah(src_reg, dest_reg);
            break;
        case TYPE_WORD:
            arch_sraw(src_reg, dest_reg);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_sral(src_reg, dest_reg);
            break;
        default:
            print_err("<bug> emit_sra(): invalid type", 0);
            break;
    }
}

void emit_beq(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_beqb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_beqh(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_beqw(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_beql(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_beq(): invalid type", 0);
            break;
    }
}

void emit_bne(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bneb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bneh(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bnew(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bnel(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_bne(): invalid type", 0);
            break;
    }
}

void emit_bgt(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bgtb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bgth(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bgtw(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bgtl(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_bgt(): invalid type", 0);
            break;
    }
}

void emit_bge(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bgeb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bgeh(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bgew(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bgel(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_bge(): invalid type", 0);
            break;
    }
}

void emit_blt(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bltb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_blth(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bltw(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bltl(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_blt(): invalid type", 0);
            break;
    }
}

void emit_ble(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bleb(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bleh(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_blew(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_blel(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_ble(): invalid type", 0);
            break;
    }
}

void emit_bgt_unsigned(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bgtb_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bgth_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bgtw_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bgtl_unsigned(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_bgt(): invalid type", 0);
            break;
    }
}

void emit_bge_unsigned(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bgeb_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bgeh_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bgew_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bgel_unsigned(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_bge(): invalid type", 0);
            break;
    }
}

void emit_blt_unsigned(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bltb_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_blth_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_bltw_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bltl_unsigned(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_blt(): invalid type", 0);
            break;
    }
}

void emit_ble_unsigned(type_t *type, int reg1, int reg2, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bleb_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_HALF:
            arch_bleh_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_WORD:
            arch_blew_unsigned(reg1, reg2, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_blel_unsigned(reg1, reg2, lbl);
            break;
        default:
            print_err("<bug> emit_ble(): invalid type", 0);
            break;
    }
}

void emit_bze(type_t *type, int reg, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bzeb(reg, lbl);
            break;
        case TYPE_HALF:
            arch_bzeh(reg, lbl);
            break;
        case TYPE_WORD:
            arch_bzew(reg, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bzel(reg, lbl);
            break;
        default:
            print_err("<bug> emit_bze(): invalid type", 0);
            break;
    }
}

void emit_bnz(type_t *type, int reg, char *lbl) {
    switch(type->specifier) {
        case TYPE_BYTE:
            arch_bnzb(reg, lbl);
            break;
        case TYPE_HALF:
            arch_bnzh(reg, lbl);
            break;
        case TYPE_WORD:
            arch_bnzw(reg, lbl);
            break;
        case TYPE_DOBL:
        case TYPE_PTR:
            arch_bnzl(reg, lbl);
            break;
        default:
            print_err("<bug> emit_bnz(): invalid type", 0);
            break;
    }
}

void emit_adjust_stack(int stack_size) {
    arch_adjust_stack(stack_size);
}

void emit_loadarg(int arg, int reg) {
    arch_loadarg(arg, reg);
}

void emit_pusharg(int size, int reg, int arg) {
    arch_pusharg(reg, arg);
}

void emit_poparg(int arg, int reg) {
    arch_poparg(arg, reg);
}

void emit_call(expr_t *expr) {
    if (expr->indir) {
        arch_call_indir(expr->addr);
    } else {
        arch_call(expr->addr);
    }
}

void emit_def_m4_macro(char *macro_name) {
    fprintf(emit_fd, "define(`%s',`", macro_name);
}

void emit_fed_m4_macro() {
    fprintf(emit_fd, "')");
}

void emit_use_m4_macro(char *macro_name) {
    fprintf(emit_fd, "%s`'", macro_name);    
}

void emit_nop() {
    arch_nop();
}

