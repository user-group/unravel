#ifndef _ansi_parse_h
#define _ansi_parse_h
/*
*****************************************************************
*                                                               *
*  Unravel Project: Develop a program slicer for ANSI C         *
*                                                               *
*  Developed at N.I.S.T. Computer Systems Lab by:               *
*                                                               *
*      D. Wallace     Project Leader                            *
*                                                               *
*      J. Lyle, J. Graham, K. Gallagher, D. Binkley & J. Poole  *
*                                                               *
*****************************************************************
*/
#define PARSE_H_SCCS_ID "	@(#)ansi_parse.h	1.7  10/26/94 "
#include <stdio.h>
#define YYSTYPE yyunion
#define MAX_NEST 40
#define True 1
#define False 0
typedef struct aa token_at, *token_at_ptr;
typedef struct bb string_node, *string_ptr;
typedef struct cc token_rec, *token_ptr;
typedef struct dd token_desc_rec, *token_desc_ptr;
typedef struct ee decl_rec, *decl_ptr;
typedef struct ff type_rec, *type_ptr;
typedef struct gg stab_rec, *stab_ptr;
typedef struct hh var_ste_rec, *var_ste_ptr;
typedef struct ii symbol_table_entries;
typedef struct jj type_ste_rec, *type_ste_ptr;
typedef struct kk tree_rec, *tree_ptr;
typedef union ll yyunion;
typedef struct mm tag_ste_rec, *tag_ste_ptr;
typedef struct nn stmt_rec, *stmt_ptr;
typedef struct oo list_rec, *list_ptr;
typedef struct pp proc_rec, *proc_ptr;

typedef struct ptr_tab_struct ptr_tab_rec, *ptr_tab_ptr;
typedef struct addr_tab_struct addr_tab_rec, *addr_tab_ptr;
typedef struct label_struct label_rec, *label_ptr;
typedef struct chain_struct chain_rec, *chain_ptr;

struct chain_struct
{
    int chain_id;
    int is_global;
    int current_seq;
    int no_output;
    int is_array;
    type_ste_ptr type;
    var_ste_ptr fields;
};

struct label_struct
{
    char *label;
    int node;
    list_ptr gotos;
    label_ptr next;
};
struct addr_tab_struct
{
    int proc_id;
    int var_id;
    int addr;
    addr_tab_ptr next;
};

struct ptr_tab_struct
{
    int ptr_id;
    int id;
    int scope;
    int length;
    int is_used_as_ptr;
    int field_id;
    token_ptr token;
    var_ste_ptr sym_tab;
    ptr_tab_ptr parent;
    ptr_tab_ptr next;
};

union ll
{ /* parser value stack entry				*/
    stmt_ptr stmt;
    token_ptr token;
    tree_ptr tree;
    int op;
};

struct aa
{ /* token_at,*token_at_ptr			*/
    int line_start, col_start, line_end, col_end;
};

struct bb
{ /* string_node,*string_ptr			*/
    char *text;
    string_ptr next;
};

struct dd
{ /* token_desc_rec,*token_desc_ptr	*/
    int *any;
    decl_ptr decl;      /* if token is -> type declaration */
    type_ptr type_desc; /* if -> var being declared		*/
};

struct cc
{                        /* T O K E N    R E C O R D		*/
                         /*   typedef struct cc token_rec,*token_ptr	*/
    char *text;          /* text of this token		*/
    token_at at;         /* location in source file	*/
    token_ptr next;      /* next token in a list		*/
    token_desc_rec desc; /* description of token		*/
    int deref_level;     /* deref count in xpr	*/
};

struct ee
{ /* decl_rec,*decl_ptr;					*/
    unsigned int decl_flags;
    token_ptr vars;
    token_ptr tag;
    token_ptr members;
    var_ste_ptr member_ste;
    int offset;
};
#define NO_STYLE 0
#define OLD_STYLE 1
#define NEW_STYLE 2
#define OLD_STYLE_VOID 3

struct ff
{ /* type_rec,*type_ptr			*/
    int is;
    int style;
    token_ptr formals;
    type_ptr next;
};
#define IS_SCALAR 0
#define IS_FUNC_RET 1
#define IS_ARRAY_OF 2
#define IS_PTR_TO 3

#define STE_NONE 0
#define STE_TYPEDEF 1
#define STE_TAGGED_STRUCT 2
#define STE_ANON_STRUCT 3

struct hh
{ /* STE for variable */
    token_ptr token;
    token_ptr type_decl;
    int detail;
    type_ste_ptr type_entry;
    tag_ste_ptr tag_entry;
    var_ste_ptr anon_entry;
    int id;
    addr_tab_ptr addr;
    var_ste_ptr next;
};

struct ii
{ /* S Y M B O L    T A B L E */
    var_ste_ptr var;
    type_ste_ptr type;
    tag_ste_ptr tag;
};

struct jj
{ /* STE for typedef */
    token_ptr token;
    token_ptr type_decl;
    int detail;
    type_ste_ptr type_entry;
    tag_ste_ptr tag_entry;
    var_ste_ptr anon_entry;
    type_ste_ptr next;
};

struct mm
{ /* STE for tagged struct */
    token_ptr tag;
    token_ptr type_desc;
    var_ste_ptr vars;
    tag_ste_ptr next;
};

struct kk
{ /* expression tree node 		*/
    int op_code;
    tree_ptr left, right;
    token_ptr token;
};
/* OP CODES */
#define UN_OP 1
#define BIN_OP 2
#define POINTER_OP 3
#define DEREF_OP 4
#define DOT_OP 5
#define ADDR_OP 6
#define CALL_OP 7
#define PRE_OP 8
#define POST_OP 9
#define CAST_OP 10
#define ARRAY_OP 11
#define ASGN_OP 12
#define RELASGN_OP 13
#define ID_OP 14
#define CON_OP 15
#define COMMA_OP 16
#define LEFTP_OP 17
#define RIGHTP_OP 18
#define ARG_OP 19
#define TYPENAME_OP 20

/* typedef struct nn stmt_rec,*stmt_ptr; */
struct nn
{
    int is_jump;
    int from, to, entry, exit;
    list_ptr breaks, continues, cases;
    stmt_ptr next, last;
};

struct oo
{ /* list of nodes (see stmt_rec		*/
    int node;
    list_ptr next;
    short is_marked, is_default_case;
};

struct pp
{                                                 /* list of procedures				*/
    int proc_id, exit_node, n_locals, entry_node; /* entry of -1 ==> called only	*/
    token_ptr type_decl;
    token_ptr proc;
    proc_ptr next;
    char static_or_extern;
    short has_return_xpr;
};

/*
struct gg {
        stab_ptr	next;
        union {
                struct {
                        char	*name;
                        int		node;
                } label;
                struct {
                        char	*name;
                        stab_ptr members;
                } tag;
                struct {
                        token_ptr	type;
                        token_ptr	decl;
                        int			id;
                        int			is_type;
                } var;
        } s;
};
*/

/* decl flags (types) */
#define FLAG_TYPE_CHAR (1 << 0)
#define FLAG_TYPE_SHORT (1 << 1)
#define FLAG_TYPE_INT (1 << 2)
#define FLAG_TYPE_LONG (1 << 3)
#define FLAG_TYPE_SIGNED (1 << 4)
#define FLAG_TYPE_UNSIGNED (1 << 5)
#define FLAG_TYPE_FLOAT (1 << 6)
#define FLAG_TYPE_DOUBLE (1 << 7)
#define FLAG_TYPE_CONST (1 << 8)
#define FLAG_TYPE_VOLATILE (1 << 9)
#define FLAG_TYPE_VOID (1 << 10)
#define FLAG_TYPE_US (1 << 11)
#define FLAG_TYPE_ENUM (1 << 12)
#define FLAG_TYPE_USER_NAME (1 << 13)
/* storage class flags */
#define FLAG_SC_TYPEDEF (1 << 14)
#define FLAG_SC_EXTERN (1 << 15)
#define FLAG_SC_STATIC (1 << 16)
#define FLAG_SC_AUTO (1 << 17)
#define FLAG_SC_REGISTER (1 << 18)
#define FLAG_TYPE_STRUCT (1 << 19)
#define FLAG_TYPE_UNION (1 << 20)

extern int n_opt, c_opt, p_opt, i_opt, a_opt, k_opt, /* output each created token	*/
    o_opt,                                           /* output file name follows		*/
    x_opt,                                           /* expression debugging    		*/
    z_opt,                                           /* LIF details to output file*/
    m_opt,                                           /* memory usage summary    		*/
    s_opt,                                           /* print symbol table      		*/
    lineno, stmtno, id_max, current_column, global_scope, in_local_decl, in_struct, in_union,
    in_parms, n_returns, proc_returns[5000];

extern int type_seen, in_init;
extern FILE *outfile, *h_file;
extern char *progname;
extern char the_current_file[2000];
extern YYSTYPE yylval;
extern proc_ptr current_proc;

string_ptr alloc_string();
token_ptr alloc_token(), create_token(), leftmost();
token_ptr fake_var_decl();
decl_ptr alloc_decl();
type_ptr alloc_type();
type_ptr make_abstract_type();
type_ste_ptr alloc_type_ste();
var_ste_ptr alloc_var_ste(), struct_decl(), look_up_id();
tag_ste_ptr alloc_tag_ste();
addr_tab_ptr alloc_addr_tab(), addr_tab;
int top_scope(), get_stmt_no(), xpr_gen(), find_proc_id;
int current_proc_id(), call_to(), is_malloc();
int current_global(), current_local();
int current_addr(), current_chain();
int is_var_array();
tree_ptr alloc_tree(), make_tree(), make_leaf();
stmt_ptr make_stmt(), alloc_stmt(), join_stmt(), if_stmt();
stmt_ptr while_stmt(), do_stmt(), for_stmt(), brace_stmt();
stmt_ptr break_stmt(), continue_stmt(), switch_stmt(), case_stmt();
stmt_ptr return_stmt(), fun_stmt(), stmt_label(), goto_stmt();
list_ptr alloc_list();
label_ptr alloc_label();
proc_ptr alloc_proc(), find_proc();
char *find_string();
ptr_tab_ptr look_for_ptr();
#endif /* _ansi_parse_h */
