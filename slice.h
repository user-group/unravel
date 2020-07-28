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

#ifndef _slice_h
#define _slice_h
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

# include "sets.h"
# define SLICE_SCCS_ID "	@(#)slice.h	1.5  9/29/94 "

typedef struct id_struct    id_rec,   *id_ptr;
typedef struct cx_struct	cx_rec,	  *cx_ptr;
typedef struct file_struct  file_rec, *file_ptr;
typedef struct proc_struct  proc_rec, *proc_ptr;
typedef struct call_struct  call_rec, *call_ptr;
typedef struct stmt_struct  stmt_rec, *stmt_ptr;
typedef struct head_struct  head_rec, *head_ptr;
typedef struct addr_struct  addr_rec, *addr_ptr;
typedef struct chain_struct chain_rec,*chain_ptr;
typedef struct field_struct field_rec,*field_ptr;
typedef struct actual_struct actual_rec,*actual_ptr;

struct cx_struct {
	int		level;
	call_ptr *branch;
	cx_ptr	next_this_level,
			next_this_site;
};

struct proc_struct {
	int		entry,exit,n_locals;
	int		n_formals;
	id_ptr	locals;
	char	*proc_name;
	int		file_id;
	call_ptr calls;
	call_ptr called_by;
	cx_ptr	contexts;
	set_ptr	return_stmts;
	bit_set	global_defs,global_idefs,local_idefs,cdefs;
	bit_set lib_arefs;
	id_set_ptr	other_defs;
	short	returns_xpr;
	char	s_or_x;
};

struct head_struct {
	int		n;
	int		*ids;
	char	*header_file;
	char	**names;
};

struct actual_struct {
	int		actual_number;
	var_ptr	actuals;
	actual_ptr next;
};

struct call_struct {
	int		pid;
	int		stmt;
	actual_ptr actuals;
	call_ptr next,next_this_stmt;
};

struct addr_struct {
	int		pid,id;
};

struct chain_struct {
	int		id,pid;
	field_ptr fields;
};

struct field_struct {
	int			fid;
	int			offset;
	char		*name;
	field_ptr	next;
};

struct stmt_struct {
	var_ptr	refs,defs;
	set_ptr	succ,requires;
	bit_set	active_global,active_local;
	id_set_ptr active_other;
	int		froml,tol;
	call_ptr calls;
	short	fromc,toc;
	short	joins;
	short	is_return;
	short	visit_next;
};

struct file_struct {
	int		n_procs,n_stmts;
	int		n_lines,n_chars;
	char	*name;
	stmt_ptr stmts;
	char	*text;
	char	**src_index;
};

struct id_struct {
	char	*name;
	int		ptr_id;
	int		offset;
	unsigned int is_formal : 1;
	unsigned int is_pointer : 1;
	unsigned int is_array : 1;
};

extern int 		n_globals,n_procs,n_addrs,n_chains,n_heads,n_files;

extern proc_ptr	procs;
extern file_ptr	files;
extern id_ptr	globals;
extern head_ptr	headers;
extern chain_ptr chains;
extern addr_ptr	addrs;
set_ptr ptr_points_to(),var_points_to();
int line_to_stmt();
int abandon_slice;
char *var_name();
void	(*slice_hook)();
void	(*slice_pass_hook)();
char	**get_slice_list ();

int find_slice ();
void clear_active(void);
void print_slices1(int print_nodes, bit_set slices[], bit_set active);
void slice (int file, int proc, int stmt, int var_proc, int var, bit_set final_slice_sets[], bit_set active);

void set_criteria (int file, int stmt_proc, int stmt, int var_proc, int var);
	
#endif /* _slice_h */

