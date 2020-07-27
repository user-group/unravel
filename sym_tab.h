#ifndef _SYM_TAB_H_
#define _SYM_TAB_H_

# include "ansi_parse.h"

void start_local_decls(void);
void close_scope(void);
void open_scope(void);
void end_local_decl(void);
void start_typedef(void);
void end_typedef(void);
void insert_var_decl(token_ptr var);
void insert_struct_members (char * base, var_ste_ptr members, var_ste_ptr *st, int *off_set);
void list_procs(void);
void print_flags(int flags);

void merge_abstract_type(type_ptr old, type_ptr newtype);
void modify_type (token_ptr token, unsigned int flag, int style, token_ptr formals);
void decl (token_ptr type_desc, token_ptr var_list, int with_members);
void make_decl (token_ptr token, unsigned int flag);


#endif

