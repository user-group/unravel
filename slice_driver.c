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

#include "sets.h"
#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static  char    sccsid[] = "@(#)slice_driver.c	1.4  8/16/95";
static bit_set *slices, active;

extern int c_opt;
extern int v_opt;
int n_opt;

extern int read_k_file(char *name);
extern int read_link_file(char *name);
extern void verify_input(int i);
extern void print_all_active(bit_set slices[], bit_set active);

int make_criterion(char *file_name, int line_number, char *var_spec, 
                   int *file, int *stmt, int *proc, int *var);

void trace(int proc, int stmt) 
{ 
    printf("at stmt %d (line %d) in %s\n", stmt, 0, procs[proc].proc_name); 
}

void verify_criterion(int file, int stmt, int proc, int var)
{
    if ((file < 0) || (file >= n_files)) {
        fprintf(stderr, "file %d is out of range [0..%d]", file, n_files - 1);
        exit(1);
    }
    if ((proc < 0) || (proc > n_procs)) {
        fprintf(stderr, "proc %d is out of range [1..%d]", proc, n_procs);
        exit(1);
    }
    if (proc) {
        if ((var <= 0) || (var > procs[proc].n_locals)) {
            fprintf(stderr, "var %d is out of range [1..%d]", var, procs[proc].n_locals);
            exit(1);
        }
    } else {
        if ((var <= 0) || (var > n_globals)) {
            fprintf(stderr, "var %d is out of range [1..%d]", var, n_globals);
            exit(1);
        }
    }
    if ((stmt < 0) || (stmt > files[file].n_stmts)) {
        fprintf(stderr, "stmt %d is out of range [1..%d]", stmt, files[file].n_stmts);
        exit(1);
    }
}

void do_slice(int file, int stmt, int proc, int var) 
{
    int i;
    int stmt_proc;

    clear_active();
    for (i = 1; i <= n_procs; i++) {
        if (procs[i].file_id == file)
            if ((stmt >= procs[i].entry) && (stmt <= procs[i].exit)) {
                stmt_proc = i;
                break;
            }
    }
    if ((stmt_proc < 1) || (stmt_proc > n_procs)) {
        printf("stmt %d not found for file %s\n", stmt, files[file].name);
        return;
    }
    printf("%s%s@%d %s'%s:\n",
        files[file].name,
        procs[stmt_proc].proc_name,
        stmt,
        proc ? procs[proc].proc_name : "global",
        proc ? procs[proc].locals[var].name : globals[var].name);
    slice(file, stmt_proc, stmt, proc, var, slices, active);
    print_slices1(n_opt, slices, active);
    /*
    printf ("file %d stmt %d proc %d var %d\n",file,stmt,proc,var);
    status = 1;
    clear_bit_set (slices[file]);
    slice_tree (slices,proc,active);
    while (status){
            status = slice_pass (slices[file],proc,0);
            printf ("status %d\n",status);
    }
    */
    if (v_opt)
        print_all_active(slices, active);
}

int main(int np, char **p) 
{
    int status;
    int i, fx;
    int file, stmt, proc, var;
    char file_name[1000], var_spec[1000];
    int line_number;
    int is_err;
    int l_opt = 0;

    v_opt = 0;
    if (np < 2)
        exit(1);
    fx = 0;
    for (i = 1; i < np; i++) {
        if (p[i][0] == '-') {
            if (strcmp("-v", p[i]) == 0)
                v_opt = 1;
            else if (strcmp("-n", p[i]) == 0)
                n_opt = 1;
            else if (strcmp("-l", p[i]) == 0)
                l_opt = 1;
            else if (strcmp("-t", p[i]) == 0)
                slice_hook = trace;
            else if (strcmp("-c", p[i]) == 0)
                c_opt = 1;
            else
                fprintf(stderr, "%s: option %s not recognized\n", p[0], p[i]);
        } else
            fx = i;
    }
    i            = strlen(p[fx]);
    p[fx][i - 1] = '\0';
    status       = read_k_file(p[fx]);
    if (status)
        printf("%s: K file status %d\n", p[fx], status);
    status = read_link_file(p[fx]);
    if (status)
        printf("%s: LINK file status %d\n", p[fx], status);
    if (v_opt)
        verify_input(l_opt);
    slices = (bit_set *)malloc(n_files * sizeof(bit_set));
    for (i = 0; i < n_files; i++)
        slices[i] = create_bit_set(1 + files[i].n_stmts);
    active = create_bit_set(1 + n_procs);
    printf("\nvar spec -- locals: proc'var, global: 'var\n\n");
    printf("Enter criterion --\n");
    printf("    file name line number variable spec: ");
    /*
    while (scanf ("%d%d%d%d",&file,&stmt,&proc,&var) != EOF){
    */
    while (scanf("%s%d%s", file_name, &line_number, var_spec) != EOF) {
        is_err = make_criterion(file_name, line_number, var_spec, &file, &stmt, &proc, &var);
        if (is_err) {
        } else {
            verify_criterion(file, stmt, proc, var);
            do_slice(file, stmt, proc, var);
        }
        printf("Enter criterion -- file name line number variable spec: ");
    }
    return 0;
}

