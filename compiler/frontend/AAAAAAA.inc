#ifndef __COMMON_H
#define __COMMON_H

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

/* location types */
/* FIXME: redundant */
#define LOC_REGISTER    0
#define LOC_STACK       1
#define LOC_CODE        2
#define LOC_RODATA      3
#define LOC_DATA        4
#define LOC_BSS         5

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

/* lexeme */
typ lexeme_t: record
(
    val : [1024 byte],
    size: four,
    type: four
);

/* type */
typ param_list_t;
typ type_t: record
(
    specifier : four,
    complete  : four,
    subcount  : four,
    subtype   : @type_t,
    rettype   : @type_t,
    param_list: @param_list_t
);

/* symbol */
typ sym_t: record
(
    next : @sym_t,
    name : @[byte],
    val  : @[byte],
    type : @type_t,
    scope: four,
    decl : four
);

/* expression */
typ expr_t: record
(
    literal          : four,
    lvalue           : four,
    indir            : four,
    addr             : @[byte],
    type             : @type_t,
    byte_literal_val : byte,
    half_literal_val : twin,
    word_literal_val : four,
    long_literal_val : eigh
);

/* list of expressions */
typ expr_list_t: record
(
    count  : four,
    expr   : @expr_t,
    sublist: @expr_list_t
);

/* memory location */
typ loc_t: record
(
    specifier: four,
    reg_name : @[byte],
    reg_size : four
);

/* list of identifiers */
typ id_list_t: record
(
    count  : four,
    sym    : @sym_t,
    sublist: @id_list_t
);

/* list of parameters */
typ param_list_t: record
(
    count  : four,
    name   : @[byte],
    type   : @type_t,
    sublist: @param_list_t,
    any    : four
);

/* register */
typ reg_t: record
(
    name: @[byte],
    size: four
);

/* string pool (managed by code generator) */
typ str_t: record
(
    next       : @str_t,
    lbl        : @[byte],
    str_literal: @[byte]
);

/* current lexeme */
dec lex: lexeme_t;

/* C functions */
#define NULL (0 as @void)
#define EOF  -1
#define stdin  __stdinp
#define stdout __stdoutp
#define stderr __stderrp
typ FILE;
dec __stdinp:  @FILE;
dec __stdoutp: @FILE;
dec __stderrp: @FILE;
dec fopen   : func(fname: @[byte], mode: @[byte]) returns @FILE;
dec fclose  : func(fd: @FILE) returns four;
dec printf  : func(fmt: @[byte], ...) returns four;
dec fprintf : func(fd: @FILE, fmt: @[byte], ...) returns four;
dec sprintf : func(str: @[byte], fmt: @[byte], count: four) returns four;
dec fgets   : func(str: @[byte], count: four, fd: @FILE) returns @[byte];
dec get_char: func(fd: @FILE) returns four;
dec isspace : func(chr: byte) returns four;
dec malloc  : func(size: four) returns @void;
dec itoa    : func(arr: @[byte], num: four);
dec strlen  : func(str: @[byte]) returns four;
dec strcpy  : func(str1: @[byte], str2: @[byte]) returns @[byte];
dec strcat  : func(str1: @[byte], str2: @[byte]) returns @[byte];
dec strcmp  : func(str1: @[byte], str2: @[byte]) returns four;
dec strtol  : func(str: @[byte], ptr: @@[byte], base: four) returns four;

/* alloc.sys */
dec alloc_type      : func() returns @type_t;
dec alloc_sym       : func() returns @sym_t;
dec alloc_expr      : func() returns @expr_t;
dec alloc_loc       : func() returns @loc_t;
dec alloc_expr_list : func() returns @expr_list_t;
dec alloc_id_list   : func() returns @id_list_t;
dec alloc_param_list: func() returns @param_list_t;

/* err.sys */
dec err_init : func();
dec print_err: func(err_msg: @[byte], msg: eigh);
dec get_err  : func() returns four;

/* symtab.sys */
dec symtab_init: func();
dec get_sym    : func(name: @[byte]) returns @sym_t;
dec add_sym    : func(name: @[byte], type: @type_t) returns @sym_t;
dec del_syms   : func();

/* lex.sys */
dec lexer_init   : func(fin: @FILE);
dec get_cur_line : func() returns @[byte];
dec get_off      : func() returns four;
dec get_file_name: func() returns @[byte];
dec get_lineno   : func() returns four;
dec get_col      : func() returns four;
dec get_char     : func() returns four;
dec next_char    : func() returns four;
dec unread_char  : func();
dec isop         : func(str: @[byte]) returns four;
dec iskeyword    : func(str: @[byte]) returns four;
dec isnum        : func(chr: byte) returns four;
dec isoct        : func(chr: byte) returns four;
dec ishex        : func(chr: byte) returns four;
dec isnondigit   : func(chr: byte) returns four;
dec get_lexeme   : func();
dec unget_lexeme : func();

/* stack.sys */
dec init_stack_frame  : func();
dec get_stack_size    : func() returns four;
dec get_new_addr      : func(size: four) returns @[byte];

/* arch.sys */
dec arch_init         : func();
dec arch_sp_fmt       : func(str: @[byte], offset: four);
dec arch_get_reg      : func(usage: four, indx: four) returns four;
dec arch_get_reg_name : func(reg: four, size: four) returns @[byte];
dec arch_get_reg_size : func(reg_name: @[byte]) returns four;
dec arch_func_entry   : func(stack_sym: @[byte]);
dec arch_func_leave   : func(stack_sym: @[byte]);
dec arch_jmp          : func(lbl: @[byte]);
dec arch_loadb_literal: func(literal: byte, reg: four);
dec arch_loadh_literal: func(literal: twin, reg: four);
dec arch_loadw_literal: func(literal: four, reg: four);
dec arch_loadl_literal: func(literal: eigh, reg: four);
dec arch_loadb        : func(addr: @[byte], reg: four);
dec arch_loadh        : func(addr: @[byte], reg: four);
dec arch_loadw        : func(addr: @[byte], reg: four);
dec arch_loadl        : func(addr: @[byte], reg: four);
dec arch_loadb_indx   : func(indx_reg: four, reg: four);
dec arch_loadh_indx   : func(indx_reg: four, reg: four);
dec arch_loadw_indx   : func(indx_reg: four, reg: four);
dec arch_loadl_indx   : func(indx_reg: four, reg: four);
dec arch_leal         : func(addr: @[byte], reg: four);
dec arch_storeb       : func(reg: four, addr: @[byte]);
dec arch_storeh       : func(reg: four, addr: @[byte]);
dec arch_storew       : func(reg: four, addr: @[byte]);
dec arch_storel       : func(reg: four, addr: @[byte]);
dec arch_storeb_indx  : func(reg: four, indx_reg: four);
dec arch_storeh_indx  : func(reg: four, indx_reg: four);
dec arch_storew_indx  : func(reg: four, indx_reg: four);
dec arch_storel_indx  : func(reg: four, indx_reg: four);
dec arch_extbh        : func(reg: four);
dec arch_extbw        : func(reg: four);
dec arch_extbl        : func(reg: four);
dec arch_exthw        : func(reg: four);
dec arch_exthl        : func(reg: four);
dec arch_extwl        : func(reg: four);
dec arch_extbh_zero   : func(reg: four);
dec arch_extbw_zero   : func(reg: four);
dec arch_extbl_zero   : func(reg: four);
dec arch_exthw_zero   : func(reg: four);
dec arch_exthl_zero   : func(reg: four);
dec arch_extwl_zero   : func(reg: four);
dec arch_addb         : func(src_reg: four, dest_reg: four);
dec arch_addh         : func(src_reg: four, dest_reg: four);
dec arch_addw         : func(src_reg: four, dest_reg: four);
dec arch_addl         : func(src_reg: four, dest_reg: four);
dec arch_subb         : func(src_reg: four, dest_reg: four);
dec arch_subh         : func(src_reg: four, dest_reg: four);
dec arch_subw         : func(src_reg: four, dest_reg: four);
dec arch_subl         : func(src_reg: four, dest_reg: four);
dec arch_mulb         : func(src_reg: four, dest_reg: four);
dec arch_mulh         : func(src_reg: four, dest_reg: four);
dec arch_mulw         : func(src_reg: four, dest_reg: four);
dec arch_mull         : func(src_reg: four, dest_reg: four);
dec arch_divb         : func(src_reg: four, dest_reg: four);
dec arch_divh         : func(src_reg: four, dest_reg: four);
dec arch_divw         : func(src_reg: four, dest_reg: four);
dec arch_divl         : func(src_reg: four, dest_reg: four);
dec arch_modb         : func(src_reg: four, dest_reg: four);
dec arch_modh         : func(src_reg: four, dest_reg: four);
dec arch_modw         : func(src_reg: four, dest_reg: four);
dec arch_modl         : func(src_reg: four, dest_reg: four);
dec arch_notb         : func(reg: four);
dec arch_noth         : func(reg: four);
dec arch_notw         : func(reg: four);
dec arch_notl         : func(reg: four);
dec arch_andb         : func(src_reg: four, dest_reg: four);
dec arch_andh         : func(src_reg: four, dest_reg: four);
dec arch_andw         : func(src_reg: four, dest_reg: four);
dec arch_andl         : func(src_reg: four, dest_reg: four);
dec arch_eorb         : func(src_reg: four, dest_reg: four);
dec arch_eorh         : func(src_reg: four, dest_reg: four);
dec arch_eorw         : func(src_reg: four, dest_reg: four);
dec arch_eorl         : func(src_reg: four, dest_reg: four);
dec arch_xorb         : func(src_reg: four, dest_reg: four);
dec arch_xorh         : func(src_reg: four, dest_reg: four);
dec arch_xorw         : func(src_reg: four, dest_reg: four);
dec arch_xorl         : func(src_reg: four, dest_reg: four);
dec arch_sllb         : func(src_reg: four, dest_reg: four);
dec arch_sllh         : func(src_reg: four, dest_reg: four);
dec arch_sllw         : func(src_reg: four, dest_reg: four);
dec arch_slll         : func(src_reg: four, dest_reg: four);
dec arch_srlb         : func(src_reg: four, dest_reg: four);
dec arch_srlh         : func(src_reg: four, dest_reg: four);
dec arch_srlw         : func(src_reg: four, dest_reg: four);
dec arch_srll         : func(src_reg: four, dest_reg: four);
dec arch_srab         : func(src_reg: four, dest_reg: four);
dec arch_srah         : func(src_reg: four, dest_reg: four);
dec arch_sraw         : func(src_reg: four, dest_reg: four);
dec arch_sral         : func(src_reg: four, dest_reg: four);
dec arch_beqb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_beqh         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_beqw         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_beql         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bneb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bneh         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bnew         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bnel         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgth         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtw         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtl         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgeb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgeh         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgew         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgel         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blth         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltw         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltl         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bleb         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bleh         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blew         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blel         : func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtb_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgth_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtw_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgtl_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgeb_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgeh_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgew_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bgel_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltb_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blth_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltw_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bltl_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bleb_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bleh_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blew_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_blel_unsigned: func(reg1: four, reg2: four, lbl: @[byte]);
dec arch_bzeb         : func(reg: four, lbl: @[byte]);
dec arch_bzeh         : func(reg: four, lbl: @[byte]);
dec arch_bzew         : func(reg: four, lbl: @[byte]);
dec arch_bzel         : func(reg: four, lbl: @[byte]);
dec arch_bnzb         : func(reg: four, lbl: @[byte]);
dec arch_bnzh         : func(reg: four, lbl: @[byte]);
dec arch_bnzw         : func(reg: four, lbl: @[byte]);
dec arch_bnzl         : func(reg: four, lbl: @[byte]);
dec arch_pushb        : func(reg: four);
dec arch_pushh        : func(reg: four);
dec arch_pushw        : func(reg: four);
dec arch_pushl        : func(reg: four);
dec arch_popb         : func(reg: four);
dec arch_poph         : func(reg: four);
dec arch_popw         : func(reg: four);
dec arch_popl         : func(reg: four);
dec arch_adjust_stack : func(stack_size: four);
dec arch_loadarg      : func(arg: four, reg: four);
dec arch_pusharg      : func(reg: four, arg: four);
dec arch_poparg       : func(arg: four, reg: four);
dec arch_call         : func(func_addr: @[byte]);
dec arch_call_indir   : func(func_addr: @[byte]);
dec arch_nop          : func();

/* emit.sys */
dec get_new_label       : func() returns @[byte];
dec add_str_literal     : func(str_literal: @[byte]) returns @[byte];
dec str_literal_flush   : func();
dec emit_init           : func(fd: @FILE);
dec emit_line           : func(line: @[byte]);
dec emit_comment        : func(comment: @[byte]);
dec emit_section        : func(section: four);
dec emit_local          : func(sym: @[byte]);
dec emit_global         : func(sym: @[byte]);
dec emit_label          : func(lbl: @[byte]);
dec emit_data           : func(type: @type_t, expr: @expr_t);
dec emit_string         : func(str: @[byte]);
dec emit_space          : func(space: four);
dec emit_func_entry     : func(stack_sym: @[byte]);
dec emit_func_leave     : func(stack_sym: @[byte]);
dec emit_set            : func(sym: @[byte], val: four);
dec emit_jmp            : func(lbl: @[byte]);
dec emit_get_reg        : func(usage: four, indx: four) returns four;
dec emit_get_reg_size   : func(reg_name: @[byte]) returns four;
dec emit_load           : func(expr: @expr_t, reg: four);
dec emit_loadaddr       : func(expr: @expr_t, reg: four);
dec emit_store          : func(reg: four, expr: @expr_t);
dec emit_extend_signed  : func(src_type: @type_t, dest_type: @type_t, reg: four);
dec emit_extend_unsigned: func(src_type: @type_t, dest_type: @type_t, reg: four);
dec emit_add            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_sub            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_mul            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_div            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_mod            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_not            : func(type: @type_t, reg: four);
dec emit_and            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_eor            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_xor            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_sll            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_srl            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_sra            : func(type: @type_t, src_reg: four, dest_reg: four);
dec emit_beq            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bne            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bgt            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bge            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_blt            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_ble            : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bgt_unsigned   : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bge_unsigned   : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_blt_unsigned   : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_ble_unsigned   : func(type: @type_t, reg1: four, reg2: four, lbl: @[byte]);
dec emit_bze            : func(type: @type_t, reg: four, lbl: @[byte]);
dec emit_bnz            : func(type: @type_t, reg: four, lbl: @[byte]);
dec emit_adjust_stack   : func(stack_size: four);
dec emit_loadarg        : func(arg: four, reg: four);
dec emit_pusharg        : func(size: four, reg: four, arg: four);
dec emit_poparg         : func(arg: four, reg: four);
dec emit_call           : func(expr: @expr_t);
dec emit_def_m4_macro   : func(macro_name: @[byte]);
dec emit_fed_m4_macro   : func();
dec emit_use_m4_macro   : func(macro_name: @[byte]);
dec emit_nop            : func();

/* scope.sys */
dec semantic_init: func();
dec enter_scope: func();
dec leave_scope: func();
dec get_scope  : func() returns four;

/* unsigned.sys */
dec set_unsignedf  : func(newval: four) returns four;
dec reset_unsignedf: func(oldval: four);
dec get_unsignedf  : func() returns four;

/* size.sys */
dec type_size: func(type: @type_t) returns four;

/* match.sys */
dec type_match: func(type1: @type_t, type2: @type_t, strict: four) returns four;

/* cast.sys */
dec type_cast: func(before: @expr_t, new_type: @type_t) returns @expr_t;

/* literal.sys */
dec literal_type_cast: func(before: @expr_t, after: @expr_t);
dec literal_do_unary : func(expr: @expr_t, op: @[byte], op1: @expr_t);
dec literal_do_binary: func(expr: @expr_t, op1 : @expr_t, op  : @[byte], op2 : @expr_t);
dec eval_literal_int : func(str: @[byte]) returns four;

/* do.sys */
dec do_assign    : func(dest: @expr_t, src: @expr_t);
dec do_unary     : func(expr: @expr_t, op: @[byte], post: four) returns @expr_t;
dec do_binary    : func(op1: @expr_t, op: @[byte], op2: @expr_t) returns @expr_t;

/* block.sys */
dec parse_if       : func();
dec parse_case     : func();
dec parse_for      : func();
dec parse_while    : func();
dec parse_stmt_list: func();

/* record.sys */
dec parse_record_header: func(type: @type_t);

/* func.sys */
dec push_recursively : func(expr_list: @expr_list_t, cur_arg: four);
dec pop_recursively  : func(expr_list: @expr_list_t, cur_arg: four);
dec parse_func_call  : func(func_expr: @expr_t) returns @expr_t;
dec parse_func_header: func(type: @type_t);
dec parse_func       : func(sym: @sym_t);

/* type.sys */
dec parse_type: func() returns @type_t;

/* factor.sys */
dec parse_parent     : func() returns @expr_t;
dec parse_identifier : func() returns @expr_t;
dec parse_int_literal: func() returns @expr_t;
dec parse_str_literal: func() returns @expr_t;
dec parse_assembly   : func();
dec parse_factor     : func() returns @expr_t;

/* math.sys */
dec parse_unary_post : func() returns @expr_t;
dec parse_unary_pre  : func() returns @expr_t;
dec parse_cast       : func() returns @expr_t;
dec parse_muldiv     : func() returns @expr_t;
dec parse_addsub     : func() returns @expr_t;
dec parse_shift      : func() returns @expr_t;
dec parse_relation   : func() returns @expr_t;
dec parse_equality   : func() returns @expr_t;
dec parse_bitwise_and: func() returns @expr_t;
dec parse_bitwise_xor: func() returns @expr_t;
dec parse_bitwise_or : func() returns @expr_t;

/* logic.sys */
dec parse_logic_and  : func() returns @expr_t;
dec parse_logic_or   : func() returns @expr_t;

/* ternif.sys */
dec parse_conditional: func() returns @expr_t;

/* ass.sys */
dec parse_assign: func() returns @expr_t;

/* expr.sys */
dec parse_expr: func() returns @expr_t;

/* expr_list.sys */
dec parse_expr_list: func() returns @expr_list_t;

/* id_list.sys */
dec parse_id_list: func() returns @id_list_t;

/* dim.sys */
dec parse_dim_keyword  : func() returns four;
dec parse_local_keyword: func() returns four;
dec parse_in_loc       : func() returns @loc_t;
dec parse_as_type      : func() returns @type_t;
dec parse_ass          : func() returns @expr_list_t;
dec parse_dim_stmt     : func() returns four;
dec parse_dim_list     : func();

/* parse.sys */
dec parse_file: func();

#endif /* __COMMON_H */

