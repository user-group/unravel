/*

This software was developed by employees of the National Institute 
of Standards and Technology (NIST), an agency of the Federal 
Government and is being made available as a public service. Pursuant 
to title 17 United States Code Section 105, works of NIST employees 
are not subject to copyright protection in the United States.  This 
software may be subject to foreign copyright.  Permission in the 
United States and in foreign countries, to the extent that NIST may 
hold copyright, to use, copy, modify, create derivative works, and 
distribute this software and its documentation without fee is hereby 
granted on a non-exclusive basis, provided that this notice and 
disclaimer of warranty appears in all copies. 

THE SOFTWARE IS PROVIDED 'AS IS' WITHOUT ANY WARRANTY OF ANY KIND, 
EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED 
TO, ANY WARRANTY THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS, 
ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE, AND FREEDOM FROM INFRINGEMENT, AND ANY WARRANTY THAT THE 
DOCUMENTATION WILL CONFORM TO THE SOFTWARE, OR ANY WARRANTY THAT THE 
SOFTWARE WILL BE ERROR FREE.  IN NO EVENT SHALL NIST BE LIABLE FOR 
ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, DIRECT, INDIRECT, SPECIAL 
OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN ANY 
WAY CONNECTED WITH THIS SOFTWARE, WHETHER OR NOT BASED UPON WARRANTY, 
CONTRACT, TORT, OR OTHERWISE, WHETHER OR NOT INJURY WAS SUSTAINED BY 
PERSONS OR PROPERTY OR OTHERWISE, AND WHETHER OR NOT LOSS WAS 
SUSTAINED FROM, OR AROSE OUT OF THE RESULTS OF, OR USE OF, THE 
SOFTWARE OR SERVICES PROVIDED HEREUNDER.

*/

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

