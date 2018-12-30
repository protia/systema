#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>

/* register usage */
#define REG_ACC 0
#define REG_IDX 1

/* dim types */
#define DIM_TYP 0
#define DIM_DEC 1
#define DIM_DEF 2
    
/* storage classes */
#define STORE_CODE   0
#define STORE_RODATA 1
#define STORE_DATA   2
#define STORE_BSS    3
#define STORE_STACK  4
#define STORE_REG    5

/* lexeme types */
#define LEX_KEYWORD       0
#define LEX_IDENTIFIER    1
#define LEX_CHAR_LITERAL  2
#define LEX_STR_LITERAL   3
#define LEX_INT_LITERAL   4
#define LEX_OP            5
#define LEX_EOF           6
#define LEX_INVALID       7

/* type specifiers */
#define TYPE_VOID    0
#define TYPE_BYTE    1
#define TYPE_HALF    2
#define TYPE_WORD    3
#define TYPE_DOBL    4
#define TYPE_FUNC    5
#define TYPE_RECORD  6
#define TYPE_PTR     7
#define TYPE_ARRAY   8
#define TYPE_TYPEOF  9

typedef struct lexeme {
    char val[1024];
    int  size;
    int  type;
} lexeme_t;

typedef struct type {
    int                specifier;
    int                complete;
    int                subcount;
    struct type       *subtype;
    struct type       *rettype;
    struct param_list *param_list;
} type_t;

typedef struct sym {
    struct sym *next;
    char       *name;
    char       *val;
    type_t     *type;
    int         scope;
    int         decl;
} sym_t;

typedef struct expr {
    int        literal;
    int        lvalue;
    int        indir;
    char      *addr;
    type_t    *type;
    char       byte_literal_val;
    short      half_literal_val;
    int        word_literal_val;
    long long  long_literal_val;
} expr_t;

typedef struct loc {
    int   specifier;
    char *reg_name;
    int   reg_size;
} loc_t;

typedef struct expr_list {
    int               count;
    expr_t           *expr;
    struct expr_list *sublist;
} expr_list_t;

typedef struct id_list {
    int             count;
    sym_t          *sym;
    struct id_list *sublist;
} id_list_t;

typedef struct param_list {
    int                 count;
    char               *name;
    type_t             *type;
    struct param_list  *sublist;
    int                 any;
} param_list_t;

/* current lexeme */
extern lexeme_t lex;

/* util.c */
void itoa(char *arr, int num);

/* io.c */
void io_init(FILE *fd);
char *get_cur_line();
int get_off();
char *get_file_name();
int get_lineno();
int get_col();
int next_char();
void unread_char();

/* err.c */
void print_err(char *err_msg, char *data);
int get_err();

/* lex.c */
void get_lexeme();
void unget_lexeme();

/* arch.c */
void arch_sp_fmt(char *str, int offset);
int arch_get_reg(int usage, int indx);
int arch_get_reg_size(char *reg_name);
void arch_func_entry(char *stack_sym);
void arch_func_leave();
void arch_jmp(char *lbl);
void arch_loadb_literal(char literal, int reg);
void arch_loadh_literal(short literal, int reg);
void arch_loadw_literal(long literal, int reg);
void arch_loadl_literal(long long literal, int reg);
void arch_loadb(char *addr, int reg);
void arch_loadh(char *addr, int reg);
void arch_loadw(char *addr, int reg);
void arch_loadl(char *addr, int reg);
void arch_loadb_indx(int indx_reg, int reg);
void arch_loadh_indx(int indx_reg, int reg);
void arch_loadw_indx(int indx_reg, int reg);
void arch_loadl_indx(int indx_reg, int reg);
void arch_leal(char *addr, int reg);
void arch_storeb(int reg, char *addr);
void arch_storeh(int reg, char *addr);
void arch_storew(int reg, char *addr);
void arch_storel(int reg, char *addr);
void arch_storeb_indx(int reg, int indx_reg);
void arch_storeh_indx(int reg, int indx_reg);
void arch_storew_indx(int reg, int indx_reg);
void arch_storel_indx(int reg, int indx_reg);
void arch_extbh(int reg);
void arch_extbw(int reg);
void arch_extbl(int reg);
void arch_exthw(int reg);
void arch_exthl(int reg);
void arch_extwl(int reg);
void arch_extbh_zero(int reg);
void arch_extbw_zero(int reg);
void arch_extbl_zero(int reg);
void arch_exthw_zero(int reg);
void arch_exthl_zero(int reg);
void arch_extwl_zero(int reg);
void arch_addb(int src_reg, int dest_reg);
void arch_addh(int src_reg, int dest_reg);
void arch_addw(int src_reg, int dest_reg);
void arch_addl(int src_reg, int dest_reg);
void arch_subb(int src_reg, int dest_reg);
void arch_subh(int src_reg, int dest_reg);
void arch_subw(int src_reg, int dest_reg);
void arch_subl(int src_reg, int dest_reg);
void arch_mulb(int src_reg, int dest_reg);
void arch_mulh(int src_reg, int dest_reg);
void arch_mulw(int src_reg, int dest_reg);
void arch_mull(int src_reg, int dest_reg);
void arch_divb(int src_reg, int dest_reg);
void arch_divh(int src_reg, int dest_reg);
void arch_divw(int src_reg, int dest_reg);
void arch_divl(int src_reg, int dest_reg);
void arch_modb(int src_reg, int dest_reg);
void arch_modh(int src_reg, int dest_reg);
void arch_modw(int src_reg, int dest_reg);
void arch_modl(int src_reg, int dest_reg);
void arch_notb(int src_reg);
void arch_noth(int src_reg);
void arch_notw(int src_reg);
void arch_notl(int src_reg);
void arch_andb(int src_reg, int dest_reg);
void arch_andh(int src_reg, int dest_reg);
void arch_andw(int src_reg, int dest_reg);
void arch_andl(int src_reg, int dest_reg);
void arch_eorb(int src_reg, int dest_reg);
void arch_eorh(int src_reg, int dest_reg);
void arch_eorw(int src_reg, int dest_reg);
void arch_eorl(int src_reg, int dest_reg);
void arch_xorb(int src_reg, int dest_reg);
void arch_xorh(int src_reg, int dest_reg);
void arch_xorw(int src_reg, int dest_reg);
void arch_xorl(int src_reg, int dest_reg);
void arch_sllb(int src_reg, int dest_reg);
void arch_sllh(int src_reg, int dest_reg);
void arch_sllw(int src_reg, int dest_reg);
void arch_slll(int src_reg, int dest_reg);
void arch_srlb(int src_reg, int dest_reg);
void arch_srlh(int src_reg, int dest_reg);
void arch_srlw(int src_reg, int dest_reg);
void arch_srll(int src_reg, int dest_reg);
void arch_srab(int src_reg, int dest_reg);
void arch_srah(int src_reg, int dest_reg);
void arch_sraw(int src_reg, int dest_reg);
void arch_sral(int src_reg, int dest_reg);
void arch_beqb(int reg1, int reg2, char *lbl);
void arch_beqh(int reg1, int reg2, char *lbl);
void arch_beqw(int reg1, int reg2, char *lbl);
void arch_beql(int reg1, int reg2, char *lbl);
void arch_bneb(int reg1, int reg2, char *lbl);
void arch_bneh(int reg1, int reg2, char *lbl);
void arch_bnew(int reg1, int reg2, char *lbl);
void arch_bnel(int reg1, int reg2, char *lbl);
void arch_bgtb(int reg1, int reg2, char *lbl);
void arch_bgth(int reg1, int reg2, char *lbl);
void arch_bgtw(int reg1, int reg2, char *lbl);
void arch_bgtl(int reg1, int reg2, char *lbl);
void arch_bgeb(int reg1, int reg2, char *lbl);
void arch_bgeh(int reg1, int reg2, char *lbl);
void arch_bgew(int reg1, int reg2, char *lbl);
void arch_bgel(int reg1, int reg2, char *lbl);
void arch_bltb(int reg1, int reg2, char *lbl);
void arch_blth(int reg1, int reg2, char *lbl);
void arch_bltw(int reg1, int reg2, char *lbl);
void arch_bltl(int reg1, int reg2, char *lbl);
void arch_bleb(int reg1, int reg2, char *lbl);
void arch_bleh(int reg1, int reg2, char *lbl);
void arch_blew(int reg1, int reg2, char *lbl);
void arch_blel(int reg1, int reg2, char *lbl);
void arch_bgtb_unsigned(int reg1, int reg2, char *lbl);
void arch_bgth_unsigned(int reg1, int reg2, char *lbl);
void arch_bgtw_unsigned(int reg1, int reg2, char *lbl);
void arch_bgtl_unsigned(int reg1, int reg2, char *lbl);
void arch_bgeb_unsigned(int reg1, int reg2, char *lbl);
void arch_bgeh_unsigned(int reg1, int reg2, char *lbl);
void arch_bgew_unsigned(int reg1, int reg2, char *lbl);
void arch_bgel_unsigned(int reg1, int reg2, char *lbl);
void arch_bltb_unsigned(int reg1, int reg2, char *lbl);
void arch_blth_unsigned(int reg1, int reg2, char *lbl);
void arch_bltw_unsigned(int reg1, int reg2, char *lbl);
void arch_bltl_unsigned(int reg1, int reg2, char *lbl);
void arch_bleb_unsigned(int reg1, int reg2, char *lbl);
void arch_bleh_unsigned(int reg1, int reg2, char *lbl);
void arch_blew_unsigned(int reg1, int reg2, char *lbl);
void arch_blel_unsigned(int reg1, int reg2, char *lbl);
void arch_bzeb(int reg, char *lbl);
void arch_bzeh(int reg, char *lbl);
void arch_bzew(int reg, char *lbl);
void arch_bzel(int reg, char *lbl);
void arch_bnzb(int reg, char *lbl);
void arch_bnzh(int reg, char *lbl);
void arch_bnzw(int reg, char *lbl);
void arch_bnzl(int reg, char *lbl);
void arch_pushb(int reg);
void arch_pushh(int reg);
void arch_pushw(int reg);
void arch_pushl(int reg);
void arch_popb(int reg);
void arch_poph(int reg);
void arch_popw(int reg);
void arch_popl(int reg);
void arch_adjust_stack(int stack_size);
void arch_loadarg(int arg, int reg);
void arch_pusharg(int reg, int arg);
void arch_poparg(int arg, int reg);
void arch_call(char *func_addr);
void arch_call_indir(char *func_addr);
void arch_nop();

/* stack.c */
void  init_stack_frame();
int   get_stack_size();
char *get_new_addr(int size);
char *get_new_param(int size);
char *get_new_label();

/* scope.c */
void enter_scope();
void leave_scope();
int get_scope();

/* alloc.c */
type_t *alloc_type();
sym_t *alloc_sym();
expr_t *alloc_expr();
loc_t *alloc_loc();
expr_list_t *alloc_expr_list();
id_list_t *alloc_id_list();
param_list_t *alloc_param_list();

/* emit.c */
void emit_init(FILE *fd);
void emit_line(char *line);
void emit_comment(char *comment);
void emit_section(int section);
void emit_local(char *sym);
void emit_global(char *sym);
void emit_label(char *lbl);
void emit_data(type_t *type, expr_t *expr);
void emit_string(char *str);
void emit_space(int space);
void emit_func_entry(char *stack_sym);
void emit_func_leave();
void emit_set(char *sym, int val);
void emit_jmp(char *lbl);
int  emit_get_reg(int usage, int indx);
int  emit_get_reg_size(char *reg_name);
void emit_load(expr_t *expr, int reg);
void emit_loadaddr(expr_t *expr, int reg);
void emit_store(int reg, expr_t *expr);
void emit_extend_signed(type_t *src_type, type_t *dest_type, int reg);
void emit_extend_unsigned(type_t *src_type, type_t *dest_type, int reg);
void emit_add(type_t *type, int src_reg, int dest_reg);
void emit_sub(type_t *type, int src_reg, int dest_reg);
void emit_mul(type_t *type, int src_reg, int dest_reg);
void emit_div(type_t *type, int src_reg, int dest_reg);
void emit_mod(type_t *type, int src_reg, int dest_reg);
void emit_not(type_t *type, int src_reg);
void emit_and(type_t *type, int src_reg, int dest_reg);
void emit_eor(type_t *type, int src_reg, int dest_reg);
void emit_xor(type_t *type, int src_reg, int dest_reg);
void emit_sll(type_t *type, int src_reg, int dest_reg);
void emit_srl(type_t *type, int src_reg, int dest_reg);
void emit_sra(type_t *type, int src_reg, int dest_reg);
void emit_beq(type_t *type, int reg1, int reg2, char *lbl);
void emit_bne(type_t *type, int reg1, int reg2, char *lbl);
void emit_bgt(type_t *type, int reg1, int reg2, char *lbl);
void emit_bge(type_t *type, int reg1, int reg2, char *lbl);
void emit_blt(type_t *type, int reg1, int reg2, char *lbl);
void emit_ble(type_t *type, int reg1, int reg2, char *lbl);
void emit_bgt_unsigned(type_t *type, int reg1, int reg2, char *lbl);
void emit_bge_unsigned(type_t *type, int reg1, int reg2, char *lbl);
void emit_blt_unsigned(type_t *type, int reg1, int reg2, char *lbl);
void emit_ble_unsigned(type_t *type, int reg1, int reg2, char *lbl);
void emit_bze(type_t *type, int reg, char *lbl);
void emit_bnz(type_t *type, int reg, char *lbl);
void emit_adjust_stack(int stack_size);
void emit_loadarg(int arg, int reg);
void emit_pusharg(int size, int reg, int arg);
void emit_poparg(int arg, int reg);
void emit_call(expr_t *expr);
void emit_def_m4_macro(char *macro_name);
void emit_fed_m4_macro();
void emit_use_m4_macro(char *macro_name);
void emit_nop();

/* symtab.c */
sym_t *get_sym(char *name);
sym_t *add_sym(char *name, type_t *type);
void del_syms();

/* unsigned.c */
int set_unsignedf(int newval);
void reset_unsignedf(int oldval);
int get_unsignedf();

/* size.c */
int type_size(type_t *type);

/* match.c */
int type_match(type_t *type1, type_t *type2, int strict);

/* cast.c */
expr_t *type_cast(expr_t *before, type_t *new_type);

/* text.c */
char *add_str_literal(char *str_literal);
void str_literal_flush();

/* literal.c */
void literal_type_cast(expr_t *before, expr_t *after);
void literal_do_unary(expr_t *res, char *op, expr_t *op1);
void literal_do_binary(expr_t *expr, expr_t *op1, char *op, expr_t *op2);
int eval_literal_int(char *str);

/* do.c */
void do_assign(expr_t *dest, expr_t *src);
expr_t *do_unary(expr_t *expr, char *op, int post);
expr_t *do_binary(expr_t *op1, char *op, expr_t *op2);

/* record.c */
void parse_record_header(type_t *type);

/*func.c */
expr_t *parse_func_call(expr_t *func);
void parse_func_header(type_t *type);
void parse_func(sym_t *sym);

/* type.c */
type_t *parse_type();

/* block.c */
void parse_stmt_list();

/* factor.c */
expr_t *parse_factor();

/* math.c */
expr_t *parse_bitwise_or();

/* logic.c */
expr_t *parse_logic_or();

/* ternif.c */
expr_t *parse_conditional();

/* ass.c */
expr_t *parse_assign();

/* expr.c */
expr_t *parse_expr();

/* expr_list.c */
expr_list_t *parse_expr_list();

/* id_list.c */
id_list_t *parse_id_list();

/* dim.c */
void parse_dim_list();

/* parse.c */
void parse_file();

#endif /* __COMMON_H */

