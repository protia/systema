#include <stdio.h>
#include <string.h>

#include "common.h"

extern FILE *emit_fd; /* from emit.c */

void arch_sp_fmt(char *str, int offset) {
    itoa(str, offset);
    strcat(str, "(%rbp)");
}

int arch_get_reg(int usage, int indx) {
    if (usage == REG_ACC) {
        /* return accumulator */
        return indx;
    } else if (usage == REG_IDX) {
        /* return index register */
        return 4+indx;
    } else {
        return -1;
    }
}

char *arch_get_reg_name(int reg, int size) {
    char *name = "";
    if (reg == 0) {
        if (size == 1) {
            name = "%al";
        } else if (size == 2) {
            name = "%ax";
        } else if (size == 4) {
            name = "%eax";
        } else if (size == 8) {
            name = "%rax";
        }
    } else if (reg == 1) {
        if (size == 1) {
            name = "%bl";
        } else if (size == 2) {
            name = "%bx";
        } else if (size == 4) {
            name = "%ebx";
        } else if (size == 8) {
            name = "%rbx";
        }
    } else if (reg == 2) {        
        if (size == 1) {
            name = "%cl";
        } else if (size == 2) {
            name = "%cx";
        } else if (size == 4) {
            name = "%ecx";
        } else if (size == 8) {
            name = "%rcx";
        }
    } else if (reg == 3) {        
        if (size == 1) {
            name = "%dl";
        } else if (size == 2) {
            name = "%dx";
        } else if (size == 4) {
            name = "%edx";
        } else if (size == 8) {
            name = "%rdx";
        }
    } else if (reg == 4) {        
        if (size == 2) {
            name = "%si";
        } else if (size == 4) {
            name = "%esi";
        } else if (size == 8) {
            name = "%rsi";
        }
    } else if (reg == 5) {        
        if (size == 2) {
            name = "%di";
        } else if (size == 4) {
            name = "%edi";
        } else if (size == 8) {
            name = "%rdi";
        }
    }
    return name;
}

void arch_func_entry() {
    fprintf(emit_fd, "    pushq  %%rbp\n");
    fprintf(emit_fd, "    movq   %%rsp, %%rbp\n");
}

void arch_func_leave() {
    fprintf(emit_fd, "    movq   %%rbp, %%rsp\n");
    fprintf(emit_fd, "    popq   %%rbp\n");
    fprintf(emit_fd, "    ret\n\n");
}

void arch_jmp(char *lbl) {
    fprintf(emit_fd, "    jmp    %s\n", lbl);
}

void arch_loadb_literal(char literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    movb   $%d, %s\n", literal, reg_name);
}

void arch_loadh_literal(short literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movw   $%d, %s\n", literal, reg_name);
}

void arch_loadw_literal(long literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movl   $%ld, %s\n", literal, reg_name);
}

void arch_loadl_literal(long long literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movq   $%lld, %s\n", literal, reg_name);
}

void arch_loadb(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    movb   %s, %s\n", addr, reg_name);
}

void arch_loadh(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movw   %s, %s\n", addr, reg_name);
}

void arch_loadw(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movl   %s, %s\n", addr, reg_name);
}

void arch_loadl(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movq   %s, %s\n", addr, reg_name);
}

void arch_loadb_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    movb   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadh_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movw   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadw_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movl   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadl_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movq   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_leal(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    leaq   %s, %s\n", addr, reg_name);
}

void arch_storeb(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    movb   %s, %s\n", reg_name, addr);
}

void arch_storeh(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movw   %s, %s\n", reg_name, addr);
}

void arch_storew(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movl   %s, %s\n", reg_name, addr);
}

void arch_storel(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movq   %s, %s\n", reg_name, addr);
}

void arch_storeb_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "    movb   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storeh_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "    movw   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storew_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "    movl   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storel_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "    movq   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_extbh(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movsbw %s, %s\n", reg_name1, reg_name2);
}

void arch_extbw(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movsbl %s, %s\n", reg_name1, reg_name2);
}

void arch_extbl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movsbq %s, %s\n", reg_name1, reg_name2);
}

void arch_exthw(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movswl %s, %s\n", reg_name1, reg_name2);
}

void arch_exthl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movswq %s, %s\n", reg_name1, reg_name2);
}

void arch_extwl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 4);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movslq %s, %s\n", reg_name1, reg_name2);
}

void arch_extbh_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    movzbw %s, %s\n", reg_name1, reg_name2);
}

void arch_extbw_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movzbl %s, %s\n", reg_name1, reg_name2);
}

void arch_extbl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movzbq %s, %s\n", reg_name1, reg_name2);
}

void arch_exthw_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    movzwl %s, %s\n", reg_name1, reg_name2);
}

void arch_exthl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movzwq %s, %s\n", reg_name1, reg_name2);
}

void arch_extwl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 4);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    movzlq %s, %s\n", reg_name1, reg_name2);
}

void arch_addb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    addb   %s, %s\n", src_name, dest_name);
}

void arch_addh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    addw   %s, %s\n", src_name, dest_name);
}

void arch_addw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    addl   %s, %s\n", src_name, dest_name);
}

void arch_addl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    addq   %s, %s\n", src_name, dest_name);
}

void arch_subb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    subb   %s, %s\n", src_name, dest_name);
}

void arch_subh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    subw   %s, %s\n", src_name, dest_name);
}

void arch_subw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    subl   %s, %s\n", src_name, dest_name);
}

void arch_subl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    subq   %s, %s\n", src_name, dest_name);
}

void arch_mulb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  1);
    fprintf(emit_fd, "    mulb   %s\n", src_name);
}

void arch_mulh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  2);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    mulw   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_mulw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  4);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    mull   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_mull(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  8);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    mulq   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_divb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 1);
    fprintf(emit_fd, "    movsbw %%al, %%ax\n"); /* sign extend */
    fprintf(emit_fd, "    divb   %s\n", src_name);
}

void arch_divh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 2);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cwd\n"); /* sign extend */
    fprintf(emit_fd, "    divw   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_divw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 4);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cdq\n"); /* sign extend */
    fprintf(emit_fd, "    divl   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_divl(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 8);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cqo\n"); /* sign extend */
    fprintf(emit_fd, "    divq   %s\n", src_name);
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_modb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 1);
    fprintf(emit_fd, "    movsbw %%al, %%ax\n"); /* sign extend */
    fprintf(emit_fd, "    divb   %s\n", src_name);
    fprintf(emit_fd, "    movb   %%ah, %%al");
}

void arch_modh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 2);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cwd\n"); /* sign extend */
    fprintf(emit_fd, "    divw   %s\n", src_name);
    fprintf(emit_fd, "    movw   %%dx, %%ax");
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_modw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 4);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cdq\n"); /* sign extend */
    fprintf(emit_fd, "    divl   %s\n", src_name);
    fprintf(emit_fd, "    movl   %%edx, %%eax");
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_modl(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 8);
    fprintf(emit_fd, "    pushq  %%rdx\n");
    fprintf(emit_fd, "    cqo\n"); /* sign extend */
    fprintf(emit_fd, "    divq   %s\n", src_name);
    fprintf(emit_fd, "    movq   %%rdx, %%rax");
    fprintf(emit_fd, "    popq   %%rdx\n");
}

void arch_andb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    andb   %s, %s\n", src_name, dest_name);
}

void arch_andh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    andw   %s, %s\n", src_name, dest_name);
}

void arch_andw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    andl   %s, %s\n", src_name, dest_name);
}

void arch_andl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    andq   %s, %s\n", src_name, dest_name);
}

void arch_eorb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    orb    %s, %s\n", src_name, dest_name);
}

void arch_eorh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    orw    %s, %s\n", src_name, dest_name);
}

void arch_eorw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    orl    %s, %s\n", src_name, dest_name);
}

void arch_eorl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    orq    %s, %s\n", src_name, dest_name);
}

void arch_xorb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    xorb   %s, %s\n", src_name, dest_name);
}

void arch_xorh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    xorw   %s, %s\n", src_name, dest_name);
}

void arch_xorw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    xorl   %s, %s\n", src_name, dest_name);
}

void arch_xorl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    xorq   %s, %s\n", src_name, dest_name);
}

void arch_sllb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shlb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_sllh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shlw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_sllw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shll   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_slll(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shlq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srlb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shrb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srlh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shrw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srlw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shrl   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srll(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    shrq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srab(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    sarb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_srah(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    sarw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_sraw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    sarl   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_sral(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "    pushw  %%cx\n");
    fprintf(emit_fd, "    movb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "    sarq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "    popw   %%cx\n");
}

void arch_beqb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    je     %s\n", lbl);
}

void arch_beqh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    je     %s\n", lbl);
}

void arch_beqw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    je     %s\n", lbl);
}

void arch_beql(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    je     %s\n", lbl);
}

void arch_bneb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jne    %s\n", lbl);
}

void arch_bneh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jne    %s\n", lbl);
}

void arch_bnew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jne    %s\n", lbl);
}

void arch_bnel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jne    %s\n", lbl);
}

void arch_bgtb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jg     %s\n", lbl);
}

void arch_bgth(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jg     %s\n", lbl);
}

void arch_bgtw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jg     %s\n", lbl);
}

void arch_bgtl(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jg     %s\n", lbl);
}

void arch_bgeb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jge    %s\n", lbl);
}

void arch_bgeh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jge    %s\n", lbl);
}

void arch_bgew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jge    %s\n", lbl);
}

void arch_bgel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jge    %s\n", lbl);
}

void arch_bltb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jl     %s\n", lbl);
}

void arch_blth(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jl     %s\n", lbl);
}

void arch_bltw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jl     %s\n", lbl);
}

void arch_bltl(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jl     %s\n", lbl);
}

void arch_bleb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "    cmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jl     %s\n", lbl);
}

void arch_bleh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "    cmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jle    %s\n", lbl);
}

void arch_blew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "    cmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jle    %s\n", lbl);
}

void arch_blel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "    cmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "    jle    %s\n", lbl);
}

void arch_bzeb(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    testb  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jz     %s\n", lbl);
}

void arch_bzeh(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    testw  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jz     %s\n", lbl);
}

void arch_bzew(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    testl  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jz     %s\n", lbl);
}

void arch_bzel(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    testq  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jz     %s\n", lbl);
}

void arch_bnzb(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "    testb  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jnz    %s\n", lbl);
}

void arch_bnzh(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    testw  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jnz    %s\n", lbl);
}

void arch_bnzw(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    testl  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jnz    %s\n", lbl);
}

void arch_bnzl(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    testq  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "    jnz    %s\n", lbl);
}

void arch_pushb(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    pushw  %s\n", reg_name);
}

void arch_pushh(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    pushw  %s\n", reg_name);
}

void arch_pushw(int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    pushl  %s\n", reg_name);
}

void arch_pushl(int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    pushq  %s\n", reg_name);
}

void arch_popb(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    popw   %s\n", reg_name);
}

void arch_poph(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "    popw   %s\n", reg_name);
}

void arch_popw(int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "    popl   %s\n", reg_name);
}

void arch_popl(int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "    popq   %s\n", reg_name);
}

void arch_adjust_stack(int stack_size) {
    fprintf(emit_fd, "    lea   -%d(%%rbp), %%rsp\n", stack_size);
}

void arch_loadarg(int arg, int reg) {    
    char *reg_name = arch_get_reg_name(reg, 8);
    if (arg == 0) {
        fprintf(emit_fd, "    movq   %%rdi, %s\n", reg_name);    
    } else if (arg == 1) {
        fprintf(emit_fd, "    movq   %%rsi, %s\n", reg_name);  
    } else if (arg == 2) {
        fprintf(emit_fd, "    movq   %%rdx, %s\n", reg_name);  
    } else if (arg == 3) {
        fprintf(emit_fd, "    movq   %%rcx, %s\n", reg_name);  
    } else if (arg == 4) {
        fprintf(emit_fd, "    movq   %%r8, %s\n", reg_name);  
    } else if (arg == 5) {
        fprintf(emit_fd, "    movq   %%r9, %s\n", reg_name);  
    } else {
        fprintf(emit_fd, "    movq   %d(%%rbp), %s\n", (arg-4)*8, reg_name);
    }
}

void arch_pusharg(int reg, int arg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    if (arg == 0) {
        fprintf(emit_fd, "    movq   %s, %%rdi\n", reg_name);    
    } else if (arg == 1) {
        fprintf(emit_fd, "    movq   %s, %%rsi\n", reg_name);  
    } else if (arg == 2) {
        fprintf(emit_fd, "    movq   %s, %%rdx\n", reg_name);  
    } else if (arg == 3) {
        fprintf(emit_fd, "    movq   %s, %%rcx\n", reg_name);  
    } else if (arg == 4) {
        fprintf(emit_fd, "    movq   %s, %%r8\n", reg_name);  
    } else if (arg == 5) {
        fprintf(emit_fd, "    movq   %s, %%r9\n", reg_name);  
    } else {
        fprintf(emit_fd, "    pushq  %s\n", reg_name);
    }
}

void arch_poparg(int arg, int reg) {
    if (arg > 5) {
        fprintf(emit_fd, "    add   $8, %%esp\n");
    }
}

void arch_call(char *func_addr) {
    fprintf(emit_fd, "    movl   $0, %%eax\n");
    fprintf(emit_fd, "    call   %s\n", func_addr);
}

void arch_call_indir(char *func_addr) {
    fprintf(emit_fd, "    movl   $0, %%eax\n");
    fprintf(emit_fd, "    call   *%s\n", func_addr);
}
