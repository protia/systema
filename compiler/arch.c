#include <stdio.h>
#include <string.h>

#include "common.h"

extern FILE *emit_fd; /* from emit.c */

typedef struct {
    char *name;
    int  size;
} reg_t;

static reg_t regs[1000];
int regs_init = 0;

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

int arch_get_reg_size(char *reg_name) {
    int i = 0;
    if (!regs_init) {
        /* initialize */
        regs[i].name =    "%al"; regs[i++].size = 1;
        regs[i].name =    "%bl"; regs[i++].size = 1;
        regs[i].name =    "%cl"; regs[i++].size = 1;
        regs[i].name =    "%dl"; regs[i++].size = 1;
        regs[i].name =    "%ah"; regs[i++].size = 1;
        regs[i].name =    "%bh"; regs[i++].size = 1;
        regs[i].name =    "%ch"; regs[i++].size = 1;
        regs[i].name =    "%dh"; regs[i++].size = 1;
        regs[i].name =   "%sil"; regs[i++].size = 1;
        regs[i].name =   "%dil"; regs[i++].size = 1;
        regs[i].name =   "%bpl"; regs[i++].size = 1;
        regs[i].name =   "%spl"; regs[i++].size = 1;
        regs[i].name =   "%r8b"; regs[i++].size = 1;
        regs[i].name =   "%r9b"; regs[i++].size = 1;
        regs[i].name =  "%r10b"; regs[i++].size = 1;
        regs[i].name =  "%r11b"; regs[i++].size = 1;
        regs[i].name =  "%r12b"; regs[i++].size = 1;
        regs[i].name =  "%r13b"; regs[i++].size = 1;
        regs[i].name =  "%r14b"; regs[i++].size = 1;
        regs[i].name =  "%r15b"; regs[i++].size = 1;
        
        regs[i].name =    "%ax"; regs[i++].size = 2;
        regs[i].name =    "%bx"; regs[i++].size = 2;
        regs[i].name =    "%cx"; regs[i++].size = 2;
        regs[i].name =    "%dx"; regs[i++].size = 2;
        regs[i].name =    "%si"; regs[i++].size = 2;
        regs[i].name =    "%di"; regs[i++].size = 2;
        regs[i].name =    "%bp"; regs[i++].size = 2;
        regs[i].name =    "%sp"; regs[i++].size = 2;
        regs[i].name =   "%r8w"; regs[i++].size = 2;
        regs[i].name =   "%r9w"; regs[i++].size = 2;
        regs[i].name =  "%r10w"; regs[i++].size = 2;
        regs[i].name =  "%r11w"; regs[i++].size = 2;
        regs[i].name =  "%r12w"; regs[i++].size = 2;
        regs[i].name =  "%r13w"; regs[i++].size = 2;
        regs[i].name =  "%r14w"; regs[i++].size = 2;
        regs[i].name =  "%r15w"; regs[i++].size = 2;
    
        regs[i].name =   "%eax"; regs[i++].size = 4;
        regs[i].name =   "%ebx"; regs[i++].size = 4;
        regs[i].name =   "%ecx"; regs[i++].size = 4;
        regs[i].name =   "%edx"; regs[i++].size = 4;
        regs[i].name =   "%esi"; regs[i++].size = 4;
        regs[i].name =   "%edi"; regs[i++].size = 4;
        regs[i].name =   "%ebp"; regs[i++].size = 4;
        regs[i].name =   "%esp"; regs[i++].size = 4;
        regs[i].name =   "%r8d"; regs[i++].size = 4;
        regs[i].name =   "%r9d"; regs[i++].size = 4;
        regs[i].name =  "%r10d"; regs[i++].size = 4;
        regs[i].name =  "%r11d"; regs[i++].size = 4;
        regs[i].name =  "%r12d"; regs[i++].size = 4;
        regs[i].name =  "%r13d"; regs[i++].size = 4;
        regs[i].name =  "%r14d"; regs[i++].size = 4;
        regs[i].name =  "%r15d"; regs[i++].size = 4;
        
        regs[i].name =   "%rax"; regs[i++].size = 8;
        regs[i].name =   "%rbx"; regs[i++].size = 8;
        regs[i].name =   "%rcx"; regs[i++].size = 8;
        regs[i].name =   "%rdx"; regs[i++].size = 8;
        regs[i].name =   "%rsi"; regs[i++].size = 8;
        regs[i].name =   "%rdi"; regs[i++].size = 8;
        regs[i].name =   "%rbp"; regs[i++].size = 8;
        regs[i].name =   "%rsp"; regs[i++].size = 8;
        regs[i].name =    "%r8"; regs[i++].size = 8;
        regs[i].name =    "%r9"; regs[i++].size = 8;
        regs[i].name =   "%r10"; regs[i++].size = 8;
        regs[i].name =   "%r11"; regs[i++].size = 8;
        regs[i].name =   "%r12"; regs[i++].size = 8;
        regs[i].name =   "%r13"; regs[i++].size = 8;
        regs[i].name =   "%r14"; regs[i++].size = 8;
        regs[i].name =   "%r15"; regs[i++].size = 8;
        
        regs[i].name =    "%cs"; regs[i++].size = 2;
        regs[i].name =    "%ds"; regs[i++].size = 2;
        regs[i].name =    "%es"; regs[i++].size = 2;
        regs[i].name =    "%fs"; regs[i++].size = 2;
        regs[i].name =    "%gs"; regs[i++].size = 2;
        regs[i].name =    "%ss"; regs[i++].size = 2;
        
        regs[i].name =   "%cr0"; regs[i++].size = 8;
        regs[i].name =   "%cr1"; regs[i++].size = 8;
        regs[i].name =   "%cr2"; regs[i++].size = 8;
        regs[i].name =   "%cr3"; regs[i++].size = 8;
        regs[i].name =   "%cr4"; regs[i++].size = 8;
        regs[i].name =   "%cr5"; regs[i++].size = 8;
        regs[i].name =   "%cr6"; regs[i++].size = 8;
        regs[i].name =   "%cr7"; regs[i++].size = 8;
        regs[i].name =   "%cr8"; regs[i++].size = 8;
        
        regs[i].name =   "%dr0"; regs[i++].size = 8;
        regs[i].name =   "%dr1"; regs[i++].size = 8;
        regs[i].name =   "%dr2"; regs[i++].size = 8;
        regs[i].name =   "%dr3"; regs[i++].size = 8;
        regs[i].name =   "%dr6"; regs[i++].size = 8;
        regs[i].name =   "%dr7"; regs[i++].size = 8;
        
        regs[i].name =   "%tr3"; regs[i++].size = 4;
        regs[i].name =   "%tr4"; regs[i++].size = 4;
        regs[i].name =   "%tr5"; regs[i++].size = 4;
        regs[i].name =   "%tr6"; regs[i++].size = 4;
        regs[i].name =   "%tr7"; regs[i++].size = 4;
        
        regs[i].name =   "%mm0"; regs[i++].size = 8;
        regs[i].name =   "%mm1"; regs[i++].size = 8;
        regs[i].name =   "%mm2"; regs[i++].size = 8;
        regs[i].name =   "%mm3"; regs[i++].size = 8;
        regs[i].name =   "%mm4"; regs[i++].size = 8;
        regs[i].name =   "%mm5"; regs[i++].size = 8;
        regs[i].name =   "%mm6"; regs[i++].size = 8;
        regs[i].name =   "%mm7"; regs[i++].size = 8;
        
        regs[i].name = "%xmm00"; regs[i++].size = 16;
        regs[i].name = "%xmm01"; regs[i++].size = 16;
        regs[i].name = "%xmm02"; regs[i++].size = 16;
        regs[i].name = "%xmm03"; regs[i++].size = 16;
        regs[i].name = "%xmm04"; regs[i++].size = 16;
        regs[i].name = "%xmm05"; regs[i++].size = 16;
        regs[i].name = "%xmm06"; regs[i++].size = 16;
        regs[i].name = "%xmm07"; regs[i++].size = 16;
        regs[i].name = "%xmm08"; regs[i++].size = 16;
        regs[i].name = "%xmm09"; regs[i++].size = 16;
        regs[i].name = "%xmm10"; regs[i++].size = 16;
        regs[i].name = "%xmm11"; regs[i++].size = 16;
        regs[i].name = "%xmm12"; regs[i++].size = 16;
        regs[i].name = "%xmm13"; regs[i++].size = 16;
        regs[i].name = "%xmm14"; regs[i++].size = 16;
        regs[i].name = "%xmm15"; regs[i++].size = 16;
        
        regs[i].name = NULL;
        regs[i].size = 0;
        regs_init = 1;
    }
    /* look for register */
    i = 0;
    while (regs[i].name && strcmp(regs[i].name, reg_name)) {
        i++;    
    }
    return regs[i].size;
}

void arch_func_entry(char *stack_sym) {
    fprintf(emit_fd, "\tpushq  %%rbp\n");
    fprintf(emit_fd, "\tmovq   %%rsp, %%rbp\n");
    fprintf(emit_fd, "\tlea    -%s(%%rbp), %%rsp\n", stack_sym);
}

void arch_func_leave() {
    fprintf(emit_fd, "\tmovq   %%rbp, %%rsp\n");
    fprintf(emit_fd, "\tpopq   %%rbp\n");
    fprintf(emit_fd, "\tret\n");
}

void arch_jmp(char *lbl) {
    fprintf(emit_fd, "\tjmp    %s\n", lbl);
}

void arch_loadb_literal(char literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\tmovb   $%d, %s\n", literal, reg_name);
}

void arch_loadh_literal(short literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovw   $%d, %s\n", literal, reg_name);
}

void arch_loadw_literal(long literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovl   $%ld, %s\n", literal, reg_name);
}

void arch_loadl_literal(long long literal, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovq   $%lld, %s\n", literal, reg_name);
}

void arch_loadb(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\tmovb   %s, %s\n", addr, reg_name);
}

void arch_loadh(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovw   %s, %s\n", addr, reg_name);
}

void arch_loadw(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovl   %s, %s\n", addr, reg_name);
}

void arch_loadl(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovq   %s, %s\n", addr, reg_name);
}

void arch_loadb_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\tmovb   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadh_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovw   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadw_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovl   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_loadl_indx(int indx_reg, int reg) {
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovq   (%s), %s\n", indx_reg_name, reg_name);
}

void arch_leal(char *addr, int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tleaq   %s, %s\n", addr, reg_name);
}

void arch_storeb(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\tmovb   %s, %s\n", reg_name, addr);
}

void arch_storeh(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovw   %s, %s\n", reg_name, addr);
}

void arch_storew(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovl   %s, %s\n", reg_name, addr);
}

void arch_storel(int reg, char *addr) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovq   %s, %s\n", reg_name, addr);
}

void arch_storeb_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 1);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "\tmovb   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storeh_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "\tmovw   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storew_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "\tmovl   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_storel_indx(int reg, int indx_reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    char *indx_reg_name = arch_get_reg_name(indx_reg, 8);
    fprintf(emit_fd, "\tmovq   %s, (%s)\n", reg_name, indx_reg_name);
}

void arch_extbh(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovsbw %s, %s\n", reg_name1, reg_name2);
}

void arch_extbw(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovsbl %s, %s\n", reg_name1, reg_name2);
}

void arch_extbl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovsbq %s, %s\n", reg_name1, reg_name2);
}

void arch_exthw(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovswl %s, %s\n", reg_name1, reg_name2);
}

void arch_exthl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovswq %s, %s\n", reg_name1, reg_name2);
}

void arch_extwl(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 4);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovslq %s, %s\n", reg_name1, reg_name2);
}

void arch_extbh_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tmovzbw %s, %s\n", reg_name1, reg_name2);
}

void arch_extbw_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovzbl %s, %s\n", reg_name1, reg_name2);
}

void arch_extbl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 1);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovzbq %s, %s\n", reg_name1, reg_name2);
}

void arch_exthw_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovzwl %s, %s\n", reg_name1, reg_name2);
}

void arch_exthl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 2);
    char *reg_name2 = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tmovzwq %s, %s\n", reg_name1, reg_name2);
}

void arch_extwl_zero(int reg) {
    char *reg_name1 = arch_get_reg_name(reg, 4);
    char *reg_name2 = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tmovl   %s, %s\n", reg_name1, reg_name2);
}

void arch_addb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\taddb   %s, %s\n", src_name, dest_name);
}

void arch_addh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\taddw   %s, %s\n", src_name, dest_name);
}

void arch_addw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\taddl   %s, %s\n", src_name, dest_name);
}

void arch_addl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\taddq   %s, %s\n", src_name, dest_name);
}

void arch_subb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\tsubb   %s, %s\n", src_name, dest_name);
}

void arch_subh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\tsubw   %s, %s\n", src_name, dest_name);
}

void arch_subw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\tsubl   %s, %s\n", src_name, dest_name);
}

void arch_subl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\tsubq   %s, %s\n", src_name, dest_name);
}

void arch_mulb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  1);
    fprintf(emit_fd, "\tmulb   %s\n", src_name);
}

void arch_mulh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  2);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tmulw   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_mulw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  4);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tmull   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_mull(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name  = arch_get_reg_name(src_reg,  8);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tmulq   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_divb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 1);
    fprintf(emit_fd, "\tmovsbw %%al, %%ax\n"); /* sign extend */
    fprintf(emit_fd, "\tdivb   %s\n", src_name);
}

void arch_divh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 2);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcwd\n"); /* sign extend */
    fprintf(emit_fd, "\tdivw   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_divw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 4);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcdq\n"); /* sign extend */
    fprintf(emit_fd, "\tdivl   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_divl(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 8);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcqo\n"); /* sign extend */
    fprintf(emit_fd, "\tdivq   %s\n", src_name);
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_modb(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 1);
    fprintf(emit_fd, "\tmovsbw %%al, %%ax\n"); /* sign extend */
    fprintf(emit_fd, "\tdivb   %s\n", src_name);
    fprintf(emit_fd, "\tmovb   %%ah, %%al");
}

void arch_modh(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 2);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcwd\n"); /* sign extend */
    fprintf(emit_fd, "\tdivw   %s\n", src_name);
    fprintf(emit_fd, "\tmovw   %%dx, %%ax");
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_modw(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 4);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcdq\n"); /* sign extend */
    fprintf(emit_fd, "\tdivl   %s\n", src_name);
    fprintf(emit_fd, "\tmovl   %%edx, %%eax");
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_modl(int src_reg, int dest_reg) {
    /* src_reg is guaranteed to be BX, while dest_reg is AX */
    char *src_name = arch_get_reg_name(src_reg, 8);
    fprintf(emit_fd, "\tpushq  %%rdx\n");
    fprintf(emit_fd, "\tcqo\n"); /* sign extend */
    fprintf(emit_fd, "\tdivq   %s\n", src_name);
    fprintf(emit_fd, "\tmovq   %%rdx, %%rax");
    fprintf(emit_fd, "\tpopq   %%rdx\n");
}

void arch_notb(int reg) {
    char *reg_name = arch_get_reg_name(reg,  1);
    fprintf(emit_fd, "\tnotb   %s\n", reg_name);
}

void arch_noth(int reg) {
    char *reg_name = arch_get_reg_name(reg,  2);
    fprintf(emit_fd, "\tnotw   %s\n", reg_name);
}

void arch_notw(int reg) {
    char *reg_name = arch_get_reg_name(reg,  4);
    fprintf(emit_fd, "\tnotl   %s\n", reg_name);
}

void arch_notl(int reg) {
    char *reg_name = arch_get_reg_name(reg,  8);
    fprintf(emit_fd, "\tnotq   %s\n", reg_name);
}

void arch_andb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\tandb   %s, %s\n", src_name, dest_name);
}

void arch_andh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\tandw   %s, %s\n", src_name, dest_name);
}

void arch_andw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\tandl   %s, %s\n", src_name, dest_name);
}

void arch_andl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\tandq   %s, %s\n", src_name, dest_name);
}

void arch_eorb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\torb    %s, %s\n", src_name, dest_name);
}

void arch_eorh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\torw    %s, %s\n", src_name, dest_name);
}

void arch_eorw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\torl    %s, %s\n", src_name, dest_name);
}

void arch_eorl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\torq    %s, %s\n", src_name, dest_name);
}

void arch_xorb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\txorb   %s, %s\n", src_name, dest_name);
}

void arch_xorh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  2);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\txorw   %s, %s\n", src_name, dest_name);
}

void arch_xorw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  4);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\txorl   %s, %s\n", src_name, dest_name);
}

void arch_xorl(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  8);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\txorq   %s, %s\n", src_name, dest_name);
}

void arch_sllb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshlb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_sllh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshlw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_sllw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshll   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_slll(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshlq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srlb(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshrb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srlh(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshrw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srlw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshrl   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srll(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tshrq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srab(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 1);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tsarb   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_srah(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 2);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tsarw   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_sraw(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 4);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tsarl   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_sral(int src_reg, int dest_reg) {
    char *src_name  = arch_get_reg_name(src_reg,  1);
    char *dest_name = arch_get_reg_name(dest_reg, 8);
    fprintf(emit_fd, "\tpushw  %%cx\n");
    fprintf(emit_fd, "\tmovb   %s, %%cl\n", src_name);
    fprintf(emit_fd, "\tsarq   %%cl, %s\n", dest_name);
    fprintf(emit_fd, "\tpopw   %%cx\n");
}

void arch_beqb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tje     %s\n", lbl);
}

void arch_beqh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tje     %s\n", lbl);
}

void arch_beqw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tje     %s\n", lbl);
}

void arch_beql(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tje     %s\n", lbl);
}

void arch_bneb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjne    %s\n", lbl);
}

void arch_bneh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjne    %s\n", lbl);
}

void arch_bnew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjne    %s\n", lbl);
}

void arch_bnel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjne    %s\n", lbl);
}

void arch_bgtb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjg     %s\n", lbl);
}

void arch_bgth(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjg     %s\n", lbl);
}

void arch_bgtw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjg     %s\n", lbl);
}

void arch_bgtl(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjg     %s\n", lbl);
}

void arch_bgeb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjge    %s\n", lbl);
}

void arch_bgeh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjge    %s\n", lbl);
}

void arch_bgew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjge    %s\n", lbl);
}

void arch_bgel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjge    %s\n", lbl);
}

void arch_bltb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjl     %s\n", lbl);
}

void arch_blth(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjl     %s\n", lbl);
}

void arch_bltw(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjl     %s\n", lbl);
}

void arch_bltl(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjl     %s\n", lbl);
}

void arch_bleb(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjle    %s\n", lbl);
}

void arch_bleh(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjle    %s\n", lbl);
}

void arch_blew(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjle    %s\n", lbl);
}

void arch_blel(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjle    %s\n", lbl);
}

void arch_bgtb_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tja     %s\n", lbl);
}

void arch_bgth_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tja     %s\n", lbl);
}

void arch_bgtw_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tja     %s\n", lbl);
}

void arch_bgtl_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tja     %s\n", lbl);
}

void arch_bgeb_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjae    %s\n", lbl);
}

void arch_bgeh_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjae    %s\n", lbl);
}

void arch_bgew_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjae    %s\n", lbl);
}

void arch_bgel_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjae    %s\n", lbl);
}

void arch_bltb_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjb     %s\n", lbl);
}

void arch_blth_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjb     %s\n", lbl);
}

void arch_bltw_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjb     %s\n", lbl);
}

void arch_bltl_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjb     %s\n", lbl);
}

void arch_bleb_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 1);
    char *reg2_name = arch_get_reg_name(reg2, 1);
    fprintf(emit_fd, "\tcmpb   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjbe    %s\n", lbl);
}

void arch_bleh_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 2);
    char *reg2_name = arch_get_reg_name(reg2, 2);
    fprintf(emit_fd, "\tcmpw   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjbe    %s\n", lbl);
}

void arch_blew_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 4);
    char *reg2_name = arch_get_reg_name(reg2, 4);
    fprintf(emit_fd, "\tcmpl   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjbe    %s\n", lbl);
}

void arch_blel_unsigned(int reg1, int reg2, char *lbl) {
    char *reg1_name = arch_get_reg_name(reg1, 8);
    char *reg2_name = arch_get_reg_name(reg2, 8);
    fprintf(emit_fd, "\tcmpq   %s, %s\n", reg2_name, reg1_name);
    fprintf(emit_fd, "\tjbe    %s\n", lbl);
}

void arch_bzeb(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\ttestb  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjz     %s\n", lbl);
}

void arch_bzeh(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\ttestw  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjz     %s\n", lbl);
}

void arch_bzew(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\ttestl  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjz     %s\n", lbl);
}

void arch_bzel(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\ttestq  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjz     %s\n", lbl);
}

void arch_bnzb(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 1);
    fprintf(emit_fd, "\ttestb  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjnz    %s\n", lbl);
}

void arch_bnzh(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\ttestw  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjnz    %s\n", lbl);
}

void arch_bnzw(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\ttestl  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjnz    %s\n", lbl);
}

void arch_bnzl(int reg, char *lbl) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\ttestq  %s, %s\n", reg_name, reg_name);
    fprintf(emit_fd, "\tjnz    %s\n", lbl);
}

void arch_pushb(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tpushw  %s\n", reg_name);
}

void arch_pushh(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tpushw  %s\n", reg_name);
}

void arch_pushw(int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tpushl  %s\n", reg_name);
}

void arch_pushl(int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tpushq  %s\n", reg_name);
}

void arch_popb(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tpopw   %s\n", reg_name);
}

void arch_poph(int reg) {
    char *reg_name = arch_get_reg_name(reg, 2);
    fprintf(emit_fd, "\tpopw   %s\n", reg_name);
}

void arch_popw(int reg) {
    char *reg_name = arch_get_reg_name(reg, 4);
    fprintf(emit_fd, "\tpopl   %s\n", reg_name);
}

void arch_popl(int reg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    fprintf(emit_fd, "\tpopq   %s\n", reg_name);
}

void arch_adjust_stack(int stack_size) {
    fprintf(emit_fd, "\tlea    -%d(%%rbp), %%rsp\n", stack_size);
}

void arch_loadarg(int arg, int reg) {    
    char *reg_name = arch_get_reg_name(reg, 8);
    if (arg == 0) {
        fprintf(emit_fd, "\tmovq   %%rdi, %s\n", reg_name);    
    } else if (arg == 1) {
        fprintf(emit_fd, "\tmovq   %%rsi, %s\n", reg_name);  
    } else if (arg == 2) {
        fprintf(emit_fd, "\tmovq   %%rdx, %s\n", reg_name);  
    } else if (arg == 3) {
        fprintf(emit_fd, "\tmovq   %%rcx, %s\n", reg_name);  
    } else if (arg == 4) {
        fprintf(emit_fd, "\tmovq   %%r8, %s\n", reg_name);  
    } else if (arg == 5) {
        fprintf(emit_fd, "\tmovq   %%r9, %s\n", reg_name);  
    } else {
        fprintf(emit_fd, "\tmovq   %d(%%rbp), %s\n", (arg-4)*8, reg_name);
    }
}

void arch_pusharg(int reg, int arg) {
    char *reg_name = arch_get_reg_name(reg, 8);
    if (arg == 0) {
        fprintf(emit_fd, "\tmovq   %s, %%rdi\n", reg_name);    
    } else if (arg == 1) {
        fprintf(emit_fd, "\tmovq   %s, %%rsi\n", reg_name);  
    } else if (arg == 2) {
        fprintf(emit_fd, "\tmovq   %s, %%rdx\n", reg_name);  
    } else if (arg == 3) {
        fprintf(emit_fd, "\tmovq   %s, %%rcx\n", reg_name);  
    } else if (arg == 4) {
        fprintf(emit_fd, "\tmovq   %s, %%r8\n", reg_name);  
    } else if (arg == 5) {
        fprintf(emit_fd, "\tmovq   %s, %%r9\n", reg_name);  
    } else {
        fprintf(emit_fd, "\tpushq  %s\n", reg_name);
    }
}

void arch_poparg(int arg, int reg) {
    if (arg > 5) {
        fprintf(emit_fd, "\tadd   $8, %%esp\n");
    }
}

void arch_call(char *func_addr) {
    fprintf(emit_fd, "\tmovl   $0, %%eax\n");
    fprintf(emit_fd, "\tcall   %s\n", func_addr);
}

void arch_call_indir(char *func_addr) {
    fprintf(emit_fd, "\tmovl   $0, %%eax\n");
    fprintf(emit_fd, "\tcall   *%s\n", func_addr);
}

void arch_nop() {
    fprintf(emit_fd, "\tnop\n");
}

