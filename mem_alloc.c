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


#include "ansi_parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//static char sccsid[] = "@(#)mem_alloc.c	1.2  4/26/95";

static int n_strings = 0, n_text = 0, n_tokens = 0, n_types = 0, n_type_ste = 0, n_var_ste = 0,
           n_trees = 0, n_procs = 0, n_lists = 0, n_stmts = 0, n_tag_ste = 0, n_addr_tab = 0,
           n_labels = 0, n_decls = 0;

string_ptr alloc_string(char *s, string_ptr next)
{
    string_ptr new;
    unsigned size;

    n_strings++;
    size = strlen(s) + 1;
    n_text += size;
    new       = (string_ptr)malloc(sizeof(string_node));
    new->text = malloc(size);
    new->next = next;
    new->text = strcpy(new->text, s);

    return new;
}

stmt_ptr alloc_stmt(void) {
    n_stmts++;
    return (stmt_ptr)malloc(sizeof(stmt_rec));
}

tree_ptr alloc_tree(void) {
    n_trees++;
    return (tree_ptr)malloc(sizeof(tree_rec));
}

proc_ptr alloc_proc(void) {
    n_procs++;
    return (proc_ptr)malloc(sizeof(proc_rec));
}

list_ptr alloc_list(void) {
    n_lists++;
    return (list_ptr)malloc(sizeof(list_rec));
}

type_ptr alloc_type(void) {
    n_types++;
    return (type_ptr)malloc(sizeof(type_rec));
}

decl_ptr alloc_decl(void) {
    n_decls++;
    return (decl_ptr)malloc(sizeof(decl_rec));
}

token_ptr alloc_token(void) {
    token_ptr t;

    n_tokens++;

    t                 = (token_ptr)malloc(sizeof(token_rec));
    t->desc.decl      = NULL;
    t->desc.any       = NULL;
    t->desc.type_desc = NULL;
    return t;
}

type_ste_ptr alloc_type_ste(void) 
{
    n_type_ste++;
    return (type_ste_ptr)malloc(sizeof(type_ste_rec));
}

var_ste_ptr alloc_var_ste(void) 
{
    n_var_ste++;
    return (var_ste_ptr)malloc(sizeof(var_ste_rec));
}

tag_ste_ptr alloc_tag_ste(void) 
{
    n_tag_ste++;
    return (tag_ste_ptr)malloc(sizeof(tag_ste_rec));
}

addr_tab_ptr alloc_addr_tab(void) 
{
    n_addr_tab++;
    return (addr_tab_ptr)malloc(sizeof(addr_tab_rec));
}

label_ptr alloc_label(void) 
{
    n_labels++;
    return (label_ptr)malloc(sizeof(label_rec));
}

void alloc_summary(void) {
    printf("Memory allocation summary\n");
    printf("Records  Size  Total  Type\n");
    /*       123456   123  123456  abc... */
    printf("%6d %5d %7d  %s\n", n_text, 1, n_text, "Bytes in names");
    printf("%6d %5ld %7ld  %s\n",
        n_strings,
        sizeof(string_node),
        n_strings * sizeof(string_node),
        "String Records");
    printf("%6d %5ld %7ld  %s\n",
        n_tokens,
        sizeof(token_rec),
        n_tokens * sizeof(token_rec),
        "Token Records");
    printf("%6d %5ld %7ld  %s\n",
        n_decls,
        sizeof(decl_rec),
        n_decls * sizeof(decl_rec),
        "Decl Records");
    printf("%6d %5ld %7ld  %s\n",
        n_stmts,
        sizeof(stmt_rec),
        n_stmts * sizeof(stmt_rec),
        "Stmt Records");
    printf("%6d %5ld %7ld  %s\n",
        n_lists,
        sizeof(list_rec),
        n_lists * sizeof(list_rec),
        "List Records");
    printf("%6d %5ld %7ld  %s\n",
        n_procs,
        sizeof(proc_rec),
        n_procs * sizeof(proc_rec),
        "Proc Records");
    printf("%6d %5ld %7ld  %s\n",
        n_trees,
        sizeof(tree_rec),
        n_trees * sizeof(tree_rec),
        "Tree Records");
    printf("%6d %5ld %7ld  %s\n",
        n_types,
        sizeof(type_rec),
        n_types * sizeof(type_rec),
        "Type Records");
    printf("%6d %5ld %7ld  %s\n",
        n_type_ste,
        sizeof(type_ste_rec),
        n_type_ste * sizeof(type_ste_rec),
        "Type_ste Records");
    printf("%6d %5ld %7ld  %s\n",
        n_var_ste,
        sizeof(var_ste_rec),
        n_var_ste * sizeof(var_ste_rec),
        "Var_ste Records");
    printf("%6d %5ld %7ld  %s\n",
        n_tag_ste,
        sizeof(tag_ste_rec),
        n_tag_ste * sizeof(tag_ste_rec),
        "Tag_ste Records");
    printf("%6d %5ld %7ld  %s\n",
        n_addr_tab,
        sizeof(addr_tab_rec),
        n_addr_tab * sizeof(addr_tab_rec),
        "Addr_tab Records");
    printf("%6d %5ld %7ld  %s\n",
        n_labels,
        sizeof(label_rec),
        n_labels * sizeof(label_rec),
        "Label Records");
}

