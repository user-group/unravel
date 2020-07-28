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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lif.h"


/*
static char sccsid[] = "@(#)slink.c	1.4  4/26/95";
*/
int v_opt            = 0;

typedef struct local_struct local_rec, *local_ptr;
typedef struct addr_struct addr_rec, *addr_ptr;
typedef struct chain_struct chain_rec, *chain_ptr;
typedef struct field_struct field_rec, *field_ptr;
typedef struct global_struct global_rec, *global_ptr;
typedef struct file_struct file_rec, *file_ptr;
typedef struct proc_struct proc_rec, *proc_ptr;
typedef struct proc_list_struct proc_list_rec, *proc_list;
typedef struct h_struct h_rec, *h_ptr;
typedef struct var_struct var_rec, *var_ptr;

struct h_struct {
    char *name;
    var_ptr vars;
    int n_v;
    h_ptr next;
};

struct var_struct {
    int id;
    var_ptr next;
};

struct local_struct {
    int local, global;
    char *text;
    char *flags;
    local_ptr next;
};

struct addr_struct {
    int addr, pid, id, new_id;
    int new_addr, new_pid;
};

struct chain_struct {
    int old, new, id, new_id;
    int pid;
    int done;
    field_ptr field;
};

struct field_struct {
    int chain, fid;
    int offset;
    char *flags;
    field_ptr next;
};

struct global_struct {
    int new;
    char *text;
    char *flags;
    int is_static;
};

struct proc_list_struct {
    proc_list next;
    proc_ptr proc;
};

struct file_struct {
    int n_procs, n_stmts;
    int n_globals, n_addrs, n_chains;
    int n_lines, n_chars;
    char *name;
    proc_ptr procs;
    global_ptr globals;
    chain_ptr chains;
    addr_ptr addrs;
};

struct proc_struct {
    int proc_id, new_id, exit, n_locals, entry;
    char *name;
    char s_or_x;
    local_ptr localx;
};

int last_global, last_proc, last_addr, last_chain;

void print_procs(p) proc_ptr p[];
{
    int i;

    printf("\n\nP R O C S\n");
    for (i = 1; i <= last_proc; i++) {
        printf("%3d %3d %3d %c %s %d\n",
            i,
            p[i]->proc_id,
            p[i]->new_id,
            p[i]->s_or_x,
            p[i]->name,
            p[i]->entry);
    }
}

void print_globals(global_rec g[])
{
    int i;

    printf("\n\nG L O B A L S\n");
    for (i = 1; i <= last_global; i++) {
        printf(
            "%3d %3d %c %s %s\n", i, g[i].new, g[i].is_static ? 'S' : ' ', g[i].text, g[i].flags);
    }
}

int chains_are_equal(chain_ptr a, chain_ptr b)
{
    field_ptr af, bf;

    if (!a || !b)
        return 0;
    if (a->pid)
        return 0; /* KLUDGE */
    if ((a->new_id != b->new_id) || (a->pid != b->pid))
        return 0;
    af = a->field;
    bf = b->field;

    while (af && bf) {
        if (af->fid != bf->fid)
            return 0;
        if (af->offset != bf->offset)
            return 0;
        af = af->next;
        bf = bf->next;
    }
    if (af || bf)
        return 0;
    return 1;
}

void get_lif_files(int n, file_rec f[], char *dir)
{
    int i, j;
    int pid = 0, pix;
    int code, id, chain, fid, offset, addr, scope;
    char buff[2000], name[2000], flags[2000];
    FILE *lif_file;
    char *right, *extern_x;
    field_ptr field, at;
    local_ptr new_local;

    for (i = 0; i < n; i++) {
        sprintf(buff, "%s/%sLIF", dir, f[i].name);
        lif_file = fopen(buff, "r");
        if (!lif_file) {
            fprintf(stderr, "slink: could not open %s\n", buff);
            exit(1);
        }
        f[i].addrs  = (addr_ptr)malloc((1 + f[i].n_addrs) * sizeof(addr_rec));
        f[i].chains = (chain_ptr)malloc((1 + f[i].n_chains) * sizeof(chain_rec));
        for (j = 0; j <= f[i].n_chains; j++) {
            f[i].chains[j].old   = 0;
            f[i].chains[j].new   = 0;
            f[i].chains[j].id    = 0;
            f[i].chains[j].pid   = 0;
            f[i].chains[j].field = NULL;
            f[i].chains[j].done  = 0;
        }
        while (fgets(buff, 2000, lif_file)) {
            sscanf(buff, "%d", &code);
            switch (code) {
            case LIF_CHAIN:
            case LIF_GCHAIN:
                sscanf(buff, "%*d(%d,%d", &chain, &id);
                f[i].chains[chain].old   = chain;
                f[i].chains[chain].new   = 0;
                f[i].chains[chain].id    = id;
                f[i].chains[chain].pid   = code == LIF_CHAIN ? pid : 0;
                f[i].chains[chain].field = NULL;
                break;
            case LIF_FIELD:
                field = (field_ptr)malloc(sizeof(field_rec));
                sscanf(buff, "%*d(%d,%d,%d%[^\n]", &chain, &fid, &offset, flags);
                field->chain  = chain;
                field->offset = offset;
                field->fid    = fid;
                field->next   = NULL;
                field->flags  = malloc(strlen(flags) + 1);
                strcpy(field->flags, flags);
                if (f[i].chains[chain].field) {
                    if (f[i].chains[chain].field->fid > fid) {
                        field->next              = f[i].chains[chain].field;
                        f[i].chains[chain].field = field;
                    } else {
                        at = f[i].chains[chain].field;
                        while ((at->next) && (at->fid < fid))
                            at = at->next;

                        if (at->fid > fid) {
                            field->next = at->next;
                            at->next    = field;
                        } else {
                            field->next = at->next;
                            at->next    = field;
                        }
                    }
                } else
                    f[i].chains[chain].field = field;
                break;
            case LIF_ADDRESS:
                sscanf(buff, "%*d(%d,%d,%d", &addr, &scope, &id);
                f[i].addrs[addr].addr     = addr;
                f[i].addrs[addr].new_addr = 0;
                f[i].addrs[addr].pid      = scope;
                f[i].addrs[addr].id       = id;
                break;
            case LIF_LOCAL_ID:
                sscanf(buff, "%*d(%d,%[^,)]%[^\n]", &id, name, flags);
                right    = strchr(flags, ')');
                extern_x = strchr(flags, 'X');
                if ((extern_x) && (extern_x - flags < right - flags)) {
                    new_local              = (local_ptr)malloc(sizeof(local_rec));
                    new_local->next        = f[i].procs[pix].localx;
                    f[i].procs[pix].localx = new_local;
                    new_local->local       = id;
                    new_local->text        = malloc(strlen(name) + 1);
                    new_local->flags       = malloc(strlen(flags) + 1);
                    strcpy(new_local->text, name);
                    strcpy(new_local->flags, flags);
                    new_local->global = 0;
                }
                break;
            case LIF_GLOBAL_ID:
                sscanf(buff, "%*d(%d,%[^,)]%[^\n]", &id, name, flags);
                f[i].globals[id].text  = malloc(strlen(name) + 1);
                f[i].globals[id].flags = malloc(strlen(flags) + 1);
                strcpy(f[i].globals[id].flags, flags);
                strcpy(f[i].globals[id].text, name);
                break;
            case LIF_PROC_START:
                sscanf(buff, "%*d(%*d,%d", &pid);
                for (pix = 0; pix < f[i].n_procs; pix++)
                    if (f[i].procs[pix].proc_id == pid)
                        break;
                break;
            case LIF_PROC_END:
                pid = 0;
                break;
            case LIF_FORMAL_ID:
            case LIF_FILE:
            case LIF_REF:
            case LIF_DEF:
            case LIF_GREF:
            case LIF_GDEF:
            case LIF_CALL_START:
            case LIF_ACTUAL_SEP:
            case LIF_CALL_END:
            case LIF_RETURN:
            case LIF_GOTO:
            case LIF_SUCC:
            case LIF_REQUIRES:
            case LIF_SOURCE_MAP:
            case LIF_CREF:
            case LIF_CDEF:
            case LIF_AREF:
            default:
                break;
            }
        }
        fclose(lif_file);
    }
}

void get_t_files(FILE *sys, int n, file_rec f[], char *dir)
{
    int   i, j, len, nr;
    char  buff[2000];
    FILE *t_file;
    int   t_globals = 0;
    int   warning   = 1;

    fscanf(sys, "%s", buff);
    for (i = 0; i < n; i++) {
        fscanf(sys, "%s", buff);
        f[i].name = malloc(1 + strlen(buff));
        strcpy(f[i].name, buff);
        len = strlen(buff);
        if (len > 2)
            f[i].name[len - 1] = '\0';
    }
    /*
    k = printf ("FILES : ");
    for (i = 0; i < n; i++){
            if (k == 0) printf ("\t");
            k += printf ("%s ",f[i].name);
            if (k > 30){
                    printf ("\n");
                    k = 0;
            }
    }
    if (k) printf ("\n");
    */

    fclose(sys);
    /*
    fprintf(t_file,"%d\n",n_procs);
    p = procs;
    while (p){
            fprintf (t_file,"%3d %4d %4d %4d %c %s\n",
                    p->proc_id,p->entry_node,
                    p->exit_node,p->n_locals,
                    p->static_or_extern,p->proc->text);
            p = p -> next;
    }
    */
    for (i = 0; i < n; i++) {
        sprintf(buff, "%s/%sT", dir, f[i].name);
        t_file  = fopen(buff, "r");
        warning = 1;
        if (!t_file) {
            fprintf(stderr, "slink: couldn't open %s\n", buff);
            exit(1);
        } else {
            fscanf(t_file, "%d %d", &f[i].n_procs, &f[i].n_stmts);
            f[i].procs = (proc_ptr)malloc(f[i].n_procs * sizeof(proc_rec));
            for (j = 0; j < f[i].n_procs; j++) {
                nr = fscanf(t_file,
                    "%d %d %d %d %c %s",
                    &f[i].procs[j].proc_id,
                    &f[i].procs[j].entry,
                    &f[i].procs[j].exit,
                    &f[i].procs[j].n_locals,
                    &f[i].procs[j].s_or_x,
                    buff);
                if ((nr != 6) && warning) {
                    warning = 0;
                    fprintf(stderr, "Warning (slink): %sT corrupt\n", f[i].name);
                }
                f[i].procs[j].name = malloc(strlen(buff) + 1);
                strcpy(f[i].procs[j].name, buff);
                f[i].procs[j].localx = NULL;
            }
            nr = fscanf(t_file,
                "%d%d%d %d %*d %d",
                &f[i].n_globals,
                &f[i].n_chains,
                &f[i].n_addrs,
                &f[i].n_lines,
                &f[i].n_chars);
            if ((nr != 5) && warning) {
                warning = 0;
                fprintf(
                    stderr, "Warning (slink): %sT corrupt (counts from wc missing)\n", f[i].name);
            }
            fclose(t_file);
            f[i].globals = (global_ptr)malloc((1 + f[i].n_globals) * sizeof(global_rec));
            for (j = 0; j < (1 + f[i].n_globals); j++) {
                f[i].globals[j].new = 0;
            }
            t_globals += f[i].n_globals;
        }
    }
}

int scan_system(FILE *sys, char *target)
{
    char buff[2000], name[2000];
    int n = 0;

    while (fgets(buff, 2000, sys)) {
        if (buff[0] == 'M') {
            sscanf(buff, "MAIN %s %d", name, &n);
            if (strcmp(name, target) == 0)
                return n;
        }
    }
    return 0;
}

void print_files(int n, file_rec f[])
{
    int i, j;
    int k;
    field_ptr field;
    local_ptr localx;

    for (i = 0; i < n; i++) {
        printf("\nFile: %s  globals %d addrs %d chains %d procs %d\n",
            f[i].name,
            f[i].n_globals,
            f[i].n_addrs,
            f[i].n_chains,
            f[i].n_procs);
        printf("Globals:\n");
        for (j = 0; j < f[i].n_globals; j++) {
            if (f[i].globals[j + 1].text) {
                printf("%3d => %3d %s <%s>\n",
                    j + 1,
                    f[i].globals[j + 1].new,
                    f[i].globals[j + 1].text,
                    f[i].globals[j + 1].flags);
            }
        }
        printf("Chains:\n");
        for (j = 1; j <= f[i].n_chains; j++) {
            if (j != f[i].chains[j].old)
                printf("Chain skipped\n");
            else {
                printf("chain %3d -> %3d pid %3d id %3d (%d) fields: ",
                    f[i].chains[j].old,
                    f[i].chains[j].new,
                    f[i].chains[j].pid,
                    f[i].chains[j].id,
                    f[i].chains[j].new_id);
                field = f[i].chains[j].field;
                while (field) {
                    printf(" (%d,%d)", field->fid, field->offset);
                    field = field->next;
                }
                printf("\n");
            }
        }
        printf("Addrs:\n");
        for (j = 1; j <= f[i].n_addrs; j++) {
            printf("%3d => %3d %4d %4d\n",
                f[i].addrs[j].addr,
                f[i].addrs[j].new_addr,
                f[i].addrs[j].pid,
                f[i].addrs[j].id);
        }
        printf("Procs:\n");
        for (j = 0; j < f[i].n_procs; j++) {
            printf("%3d => %3d %c %4d %s\n",
                f[i].procs[j].proc_id,
                f[i].procs[j].new_id,
                f[i].procs[j].s_or_x,
                f[i].procs[j].entry,
                f[i].procs[j].name);
            localx = f[i].procs[j].localx;
            if (localx) {
                k = printf("\tLocal externs: ");
                while (localx) {
                    k += printf("(%d => %d %s %s) ",
                        localx->local,
                        localx->global,
                        localx->text,
                        localx->flags);
                    if (k > 50) {
                        printf("\n");
                        k = 0;
                    }
                    localx = localx->next;
                }
                if (k)
                    printf("\n");
            }
        }
    }
}

int pid_to_index(int n, file_rec f[], int i, int pid)
{
    int pix;

    for (pix = 0; pix < f[i].n_procs; pix++)
        if (f[i].procs[pix].proc_id == pid)
            return pix;
    return -1;
}

int find_proc(int np, proc_ptr *procs, char *name)
{
    int i;
    for (i = 1; i <= np; i++) {
        if (strcmp(name, procs[i]->name) == 0)
            if (procs[i]->s_or_x == 'X')
                return i;
    }
    return 0;
}

int find_global(int ng, global_rec g[], char *name) 
{
    int i;
    int k;

    for (i = 0, k = 1; i < ng; i++, k++) {
        if (strcmp(name, g[k].text) == 0)
            if (!g[k].is_static)
                return k;
    }
    return 0;
}

proc_ptr *assign_procs(int n_files, file_rec f[]) 
{
    int total = 0;
    int i, j, last, at;
    proc_ptr *procs;

    last = 0;
    for (i = 0; i < n_files; i++)
        total += f[i].n_procs;
    procs = (proc_ptr *)malloc(sizeof(proc_ptr) * (++total));
    for (i = 0; i < total; i++)
        procs[i] = NULL;
    for (j = 0; j < f[0].n_procs; j++) {
        procs[f[0].procs[j].proc_id] = &f[0].procs[j];
        f[0].procs[j].new_id         = f[0].procs[j].proc_id;
        if (f[0].procs[j].proc_id > last)
            last = f[0].procs[j].proc_id;
    }
    for (i = 1; i < n_files; i++) {
        for (j = 0; j < f[i].n_procs; j++) {
            if (f[i].procs[j].s_or_x == 'S')
                at = 0;
            else
                at = find_proc(last, procs, f[i].procs[j].name);
            if (at) {
                f[i].procs[j].new_id = at;
                if (f[i].procs[j].entry != -1)
                    procs[at] = &f[i].procs[j];
            } else {
                last++;
                procs[last]          = &f[i].procs[j];
                f[i].procs[j].new_id = last;
            }
        }
    }
    last_proc = last;
    return procs;
}

int otn_globals(file_rec f[], int fid, int old)
{ 
    return f[fid].globals[old].new; 
}

int find_global_addr(int n, addr_ptr *a, int var)
{
    int i;
    int gvar;

    for (i = 1; i <= n; i++) {
        gvar = a[i]->new_id;
        if ((a[i]->pid == 0) && (gvar == var))
            return i;
    }
    return 0;
}

addr_ptr *assign_addrs(int n_files, file_rec f[]) 
{
    int total = 0;
    addr_ptr *addrs;
    int i, j, last = 0;
    int pix, real_id;
    int addr_found;
    local_ptr lx;

    for (i = 0; i < n_files; i++)
        total += f[i].n_addrs;
    addrs = (addr_ptr *)malloc(sizeof(addr_ptr) * (++total));
    /*
    for (j = 1; j <= f[0].n_addrs; j++){
            addrs[j] = &f[0].addrs[j];
            addrs[j]->new_addr = j;
    }
    */
    last = 0;
    for (i = 0; i < n_files; i++) {
        for (j = 1; j <= f[i].n_addrs; j++) {
            real_id = 0;
            if (f[i].addrs[j].pid) {
                for (pix = 0; pix < f[i].n_procs; pix++)
                    if (f[i].procs[pix].proc_id == f[i].addrs[j].pid)
                        break;
                f[i].addrs[j].new_pid = f[i].procs[pix].new_id;
                if ((pix < f[i].n_procs) && (f[i].procs[pix].localx)) {
                    lx = f[i].procs[pix].localx;
                    while (lx) {
                        if (lx->local == f[i].addrs[j].id) {
                            real_id              = lx->global;
                            f[i].addrs[j].new_id = real_id;
                            addr_found           = find_global_addr(last, addrs, real_id);
                            if (addr_found) {
                                f[i].addrs[j].new_addr = addr_found;
                            } else {
                                last++;
                                f[i].addrs[j].new_addr = last;
                                addrs[last]            = &f[i].addrs[j];
                            }
                            break;
                        }
                        lx = lx->next;
                    }
                    if (lx == NULL) {
                        last++;
                        f[i].addrs[j].new_addr = last;
                        addrs[last]            = &f[i].addrs[j];
                    }
                } else {
                    last++;
                    f[i].addrs[j].new_addr = last;
                    addrs[last]            = &f[i].addrs[j];
                }
            } else { /* global var */
                real_id              = f[i].addrs[j].id;
                real_id              = otn_globals(f, i, real_id);
                f[i].addrs[j].new_id = real_id;
                addr_found           = find_global_addr(last, addrs, real_id);
                if (addr_found) {
                    f[i].addrs[j].new_addr = addr_found;
                } else {
                    last++;
                    f[i].addrs[j].new_addr = last;
                    addrs[last]            = &f[i].addrs[j];
                }
            }
        }
    }
    /*
    printf ("addrs assigned:\n");
    for (i = 1; i <= last; i++){
            printf ("%2d %d %d %d=>%d %d\n",i,addrs[i]->id,addrs[i]->new_id,
                    addrs[i]->addr,addrs[i]->new_addr,
                    addrs[i]->pid);
    }
    */
    last_addr = last;
    return addrs;
}

int really_global(file_rec f[], int i, int j, int id) 
{
    local_ptr lx;

    lx = f[i].procs[j].localx;
    if (!lx)
        return 0;
    while (lx) {
        if (lx->local == id)
            return lx->global;
        lx = lx->next;
    }
    return 0;
}

chain_ptr *shrink_chains(int n_files, file_rec f[]) 
{
    int i, j, k, global, pid, id, at = 0;
    chain_ptr *c = NULL, *new;
    int max      = 0;
    int delta    = 3;

    for (i = 0; i < n_files; i++) {
        for (j = 1; j <= f[i].n_chains; j++) {
            if (!f[i].chains[j].pid) {
                f[i].chains[j].new_id = f[i].globals[f[i].chains[j].id].new;
            } else {
                pid    = f[i].chains[j].pid;
                id     = f[i].chains[j].id;
                global = really_global(f, i, pid_to_index(n_files, f, i, pid), id);
                if (global) {
                    f[i].chains[j].pid    = 0;
                    f[i].chains[j].new_id = global;
                } else
                    f[i].chains[j].new_id = f[i].chains[j].id;
            }
            for (k = 1; k <= at; k++) {
                if (chains_are_equal(c[k], &f[i].chains[j])) {
                    f[i].chains[j].new = c[k]->new;
                    break;
                }
            }
            if (f[i].chains[j].new == 0) {
                ++at;
                f[i].chains[j].new = at;
                if (at >= max) {
                    max += delta;
                    new = (chain_ptr *)malloc(max * sizeof(chain_ptr));
                    for (k = 1; k < at; k++)
                        new[k] = c[k];
                    if (c)
                        free(c);
                    c = new;
                }
                c[at] = &f[i].chains[j];
            }
        }
    }
    last_chain = at;
    return c;
}

global_ptr assign_globals(int n_files, file_rec f[])
{
    int total = 0;
    int i, j, k = 0, last, at;
    global_rec *globals;
    char *right, *static_s;
    local_ptr localx;

    for (i = 0; i < n_files; i++)
        for (j = 0; j < f[i].n_procs; j++)
            if (f[i].procs[j].localx) {
                localx = f[i].procs[j].localx;
                while (localx) {
                    total++;
                    localx = localx->next;
                }
            }
    for (i = 0; i < n_files; i++)
        total += f[i].n_globals;
    globals = (global_ptr)malloc(sizeof(global_rec) * (++total));
    /*
    printf ("%d globals found (total)\n",total);
    for (j = 0; j < f[0].n_globals; j++){
            k = j + 1;
            globals[k].new = k;
            globals[k].text = f[0].globals[k].text;
            globals[k].flags = f[0].globals[k].flags;
            f[0].globals[k].new = k;
    }
    last = k;
    */
    last = 0;
    for (i = 0; i < n_files; i++) {
        for (j = 0, k = 1; j < f[i].n_globals; j++, k++) {
            right    = strchr(f[i].globals[k].flags, ')');
            static_s = strchr(f[i].globals[k].flags, 'S');
            if ((static_s) && (static_s - f[i].globals[k].flags < right - f[i].globals[k].flags))
                at = 0;
            else
                at = find_global(last, globals, f[i].globals[k].text);
            if (at) {
                f[i].globals[k].new = at;
            } else {
                last++;
                globals[last].is_static = (static_s != NULL);
                globals[last].new       = last;
                globals[last].text      = f[i].globals[k].text;
                globals[last].flags     = f[i].globals[k].flags;
                f[i].globals[k].new     = last;
            }
        }
    }
    for (i = 0; i < n_files; i++) {
        for (j = 0; j < f[i].n_procs; j++)
            if (f[i].procs[j].localx) {
                localx = f[i].procs[j].localx;
                while (localx) {
                    for (k = 1; k <= f[i].n_globals; k++) {
                        if (0 == strcmp(f[i].globals[k].text, localx->text)) {
                            /* found */
                            localx->global = f[i].globals[k].new;
                            break;
                        }
                    }
                    if (!localx->global) {
                        at = find_global(last, globals, localx->text);
                        if (at)
                            localx->global = at;
                        else {
                            last++;
                            globals[last].is_static = 0;
                            globals[last].new       = last;
                            globals[last].text      = localx->text;
                            globals[last].flags     = localx->flags;
                            localx->global          = last;
                        }
                    }
                    localx = localx->next;
                }
            }
        last_global = last;
    }
    return globals;
}

void merge_lif(int        n, 
               file_ptr   f, 
               char      *dir, 
               global_ptr globals, 
               proc_ptr  *procs, 
               addr_ptr  *addrs, 
               chain_ptr *chains) 
{
    char out_name[2000], lif_name[2000];
    
    FILE *out_file, *lif_file;
    int i, id, pid, px, code, node, chain;
    int gid;
    char flags[2000], buff[2000];
    field_ptr field;

    sprintf(out_name, "%s/%sLINK", dir, f[0].name);
    out_file = fopen(out_name, "w");
    if (!out_file) {
        fprintf(stderr, "could not create output file: %s\n", out_name);
        exit(1);
    }
    for (i = 1; i <= last_global; i++) {
        fprintf(out_file,
            "%d(%d,%s%s\n",
            LIF_GLOBAL_ID,
            globals[i].new,
            globals[i].text,
            globals[i].flags);
    }
    for (i = 0; i < n; i++) {
        fprintf(out_file, "%d(%d,%s)\n", LIF_FILE, i, f[i].name);
        sprintf(lif_name, "%s/%sLIF", dir, f[i].name);
        lif_file = fopen(lif_name, "r");
        if (!lif_file) {
            fprintf(stderr, "could not open file: %s\n", lif_name);
        }
        while (fgets(buff, 2000, lif_file)) {
            if (sscanf(buff, "%d", &code) == 0)
                code = 0;
            switch (code) {
            case LIF_GLOBAL_ID:
                break;

            case LIF_PROC_START:
            case LIF_CALL_START:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &pid, flags);
                px = pid_to_index(n, f, i, pid);
                fprintf(out_file, "%d(%d,%d%s\n", code, node, f[i].procs[px].new_id, flags);
                break;

            case LIF_GREF:
            case LIF_GDEF:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &id, flags);
                fprintf(out_file, "%d(%d,%d%s\n", code, node, f[i].globals[id].new, flags);
                break;
            case LIF_STRUCT:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &id, flags);
                if (node) {
                    px = pid_to_index(n, f, i, node);
                    fprintf(out_file, "%d(%d,%d%s\n", code, f[i].procs[px].new_id, id, flags);
                } else {
                    fprintf(out_file, "%d(%d,%d%s\n", code, node, f[i].globals[id].new, flags);
                }
                break;
            case LIF_AREF:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &id, flags);
                fprintf(out_file, "%d(%d,%d%s\n", code, node, f[i].addrs[id].new_addr, flags);
                break;

            case LIF_REF:
            case LIF_DEF:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &id, flags);
                gid = really_global(f, i, pid_to_index(n, f, i, pid), id);
                if (gid) {
                    fprintf(out_file,
                        "%d(%d,%d%s\n",
                        code == LIF_REF ? LIF_GREF : LIF_GDEF,
                        node,
                        gid,
                        flags);
                } else {
                    fputs(buff, out_file);
                }
                break;

            case LIF_FIELD:
            case LIF_ADDRESS:
                break;
            case LIF_CHAIN:
            case LIF_GCHAIN:
                sscanf(buff, "%*d(%d,%d%[^\n]", &chain, &id, flags);
                chain = f[i].chains[chain].new;
                if (chains[chain]->done)
                    break;
                chains[chain]->done = 1;
                if (code == LIF_CHAIN)
                    gid = really_global(f, i, pid_to_index(n, f, i, pid), id);
                else
                    gid = 0;
                fprintf(out_file,
                    "%d(%d,%d%s\n",
                    gid ? LIF_GCHAIN : code,
                    chain,
                    gid ? gid : chains[chain]->new_id,
                    flags);
                field = chains[chain]->field;
                while (field) {
                    fprintf(out_file,
                        "%d(%d,%d,%d%s\n",
                        LIF_FIELD,
                        chain,
                        field->fid,
                        field->offset,
                        field->flags);
                    field = field->next;
                }

                break;

            case LIF_CREF:
            case LIF_CDEF:
                sscanf(buff, "%*d(%d,%d%[^\n]", &node, &id, flags);
                fprintf(out_file, "%d(%d,%d%s\n", code, node, f[i].chains[id].new, flags);
                break;

            case LIF_LOCAL_ID:
            case LIF_FILE:
            case LIF_PROC_END:
            case LIF_FORMAL_ID:
            case LIF_ACTUAL_SEP:
            case LIF_CALL_END:
            case LIF_RETURN:
            case LIF_GOTO:
            case LIF_SUCC:
            case LIF_REQUIRES:
            case LIF_SOURCE_MAP:
            default:
                fputs(buff, out_file);
            }
        }
        fclose(lif_file);
    }
    for (i = 1; i <= last_addr; i++) {
        if (addrs[i]->pid) {
            id  = addrs[i]->id;
            pid = addrs[i]->new_pid;
        } else {
            id  = addrs[i]->new_id;
            pid = 0;
        }
        fprintf(out_file, "%d(%d,%d,%d)\n", LIF_ADDRESS, i, pid, id);
    }
    fclose(out_file);
}

var_ptr find_var(var_ptr list, int id)
{
    while (list) {
        if (list->id == id)
            return list;
        list = list->next;
    }
    return NULL;
}

int look_up_var(int i, file_rec f[], char *name) 
{
    int k;

    for (k = 1; k <= f[i].n_globals; k++) {
        if (strcmp(name, f[i].globals[k].text) == 0)
            return f[i].globals[k].new;
    }
    return 0;
}

h_ptr find_h(h_ptr list, char *name) 
{
    while (list) {
        if (strcmp(list->name, name) == 0)
            return list;
        list = list->next;
    }
    return NULL;
}

void merge_head(int n, file_rec f[], char *dir, proc_ptr procs[], global_rec g[]) 
{
    char buff[2000], name[2000];
    FILE *h_file, *k_file;
    h_ptr new_h, h_list = NULL;
    h_ptr at_h = NULL;
    var_ptr v;
    int i, var, n_h = 0;
    

    sprintf(buff, "%s/%sK", dir, f[0].name);
    k_file = fopen(buff, "w");
    if (!k_file) {
        fprintf(stderr, "could not create %s\n", buff);
        exit(1);
    }
    for (i = 0; i < n; i++) {
        sprintf(buff, "%s/%sH", dir, f[i].name);
        h_file = fopen(buff, "r");
        if (!h_file) {
            fprintf(stderr, "Warning (slink): could not open %s\n", buff);
            exit(1);
        }
        while (fgets(buff, 2000, h_file)) {
            if ((buff[0] == '@') && (strlen(buff) > 2)) {
                sscanf(buff, "@ %[^\n]", name);
                at_h = find_h(h_list, name);
                if (!at_h) {
                    new_h       = (h_ptr)malloc(sizeof(h_rec));
                    new_h->next = h_list;
                    new_h->vars = NULL;
                    new_h->n_v  = 0;
                    new_h->name = malloc(strlen(name) + 1);
                    strcpy(new_h->name, name);
                    h_list = new_h;
                    at_h   = new_h;
                }
            } else {
                sscanf(buff, "\t%[^\n]", name);
                var = look_up_var(i, f, name);
                if (var && at_h && (!find_var(at_h->vars, var))) {
                    v          = (var_ptr)malloc(sizeof(var_rec));
                    v->id      = var;
                    v->next    = at_h->vars;
                    at_h->vars = v;
                    at_h->n_v++;
                }
            }
        }
        fclose(h_file);
    }
    new_h = h_list;
    while (new_h) {
        if (new_h->n_v)
            n_h++;
        new_h = new_h->next;
    }
    fprintf(k_file,
        "globals %d procs %d addrs %d chains %d headers %d files %d\n",
        last_global,
        last_proc,
        last_addr,
        last_chain,
        n_h,
        n);
    for (i = 1; i <= last_proc; i++) {
        fprintf(k_file,
            "%3d %4d %4d %4d %c %s\n",
            procs[i]->new_id,
            procs[i]->entry,
            procs[i]->exit,
            procs[i]->n_locals,
            procs[i]->s_or_x,
            procs[i]->name);
    }
    fprintf(k_file, "\n");
    for (i = 0; i < n; i++) {
        fprintf(k_file,
            "%3d %3d %5d %4d %6d %s\n",
            i,
            f[i].n_procs,
            f[i].n_stmts,
            f[i].n_lines,
            f[i].n_chars,
            f[i].name);
    }
    new_h = h_list;
    while (new_h) {
        if (new_h->n_v) {
            v = new_h->vars;
            fprintf(k_file, "%d %s\n", new_h->n_v, new_h->name);
            while (v) {
                fprintf(k_file, "\t%3d %s\n", v->id, g[v->id].text);
                v = v->next;
            }
        }
        new_h = new_h->next;
    }
    fclose(k_file);
}

int main(int np, char **p) 
{
    FILE      *sys;
    int        n_files;
    file_ptr   files;
    char      *dir;
    char       buff[2000];
    global_ptr globals;
    proc_ptr  *procs;
    chain_ptr *chains;
    addr_ptr  *addrs;
    int        i, fx;
    char      *slash, *main_name;

    if ((np < 2) || (np > 3)) {
        fprintf(stderr, "%s: wrong number of parms\n", p[0]);
        fprintf(stderr, "%s usage: %s [-v] main_file_name.c\n", p[0], p[0]);
        exit(1);
    }
    fx = 0;
    for (i = 1; i < np; i++) {
        if (p[i][0] == '-') {
            if (strcmp("-v", p[i]) == 0)
                v_opt = 1;
            else {
                fprintf(stderr, "%s: option (%s) is not valid\n", p[0], p[i]);
                fprintf(stderr, "%s usage: %s [-v] main_file_name.c\n", p[0], p[0]);
                exit(1);
            }
        } else
            fx = i;
    }
    if ((fx == 0) || ((np == 3) && !v_opt)) {
        fprintf(stderr, "%s: one file name required\n", p[0]);
        fprintf(stderr, "%s usage: %s [-v] main_file_name.c\n", p[0], p[0]);
        exit(1);
    }
    slash = strrchr(p[fx], '/');
    if (slash) {
        dir       = p[fx];
        main_name = slash + 1;
        *slash    = '\0';
    } else {
        dir       = ".";
        main_name = p[fx];
    }
    sprintf(buff, "%s/SYSTEM", dir);
    sys = fopen(buff, "r");
    if (!sys) {
        fprintf(stderr, "%s: no 'SYSTEM' map file\n", p[0]);
        fprintf(stderr, "%s: run 'map' before 'slink'\n", p[0]);
        exit(1);
    }
    n_files = scan_system(sys, main_name);
    if (n_files) {
        files = (file_ptr)malloc(n_files * sizeof(file_rec));
        get_t_files(sys, n_files, files, dir);
        get_lif_files(n_files, files, dir);
        globals = assign_globals(n_files, files);
        procs   = assign_procs(n_files, files);
        addrs   = assign_addrs(n_files, files);
        chains  = shrink_chains(n_files, files);
        if (v_opt) {
            print_files(n_files, files);
            print_globals(globals);
            print_procs(procs);
        }
        merge_lif(n_files, files, dir, globals, procs, addrs, chains);
        merge_head(n_files, files, dir, procs, globals);
    } else {
        fprintf(stderr, "%s: %s is not a main program source file\n", p[0], main_name);
        exit(1);
    }
    /* exit(0); */
    return 0;
}

