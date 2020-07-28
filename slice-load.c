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
#include "lif.h"
#include "slice.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
static char sccsid[] = "@(#)slice-load.c	1.6  8/16/95";
*/

int v_opt;
int c_opt = 0;

extern void count_vars(void);
extern int chain_resolve_get(int *id, int *pid);
extern void chain_resolve_set(int def, int proc);
extern void bit_on(bit_set s, int id);
extern int offset_check(int pid, int id, int off);
extern int add_to_id_set(id_set_ptr *head, int pid, int id);
extern void flow(void);
/*
*********************************************************************
*                                                                   *
* slice load -- routines to read K and Link files                   *
*                                                                   *
*********************************************************************
*/
int n_globals, n_procs, n_addrs, n_chains, n_heads, n_files;
/*
*********************************************************************
*                                                                   *
*********************************************************************
*/

proc_ptr procs;
file_ptr files;
head_ptr headers;
chain_ptr chains;
addr_ptr addrs;
id_ptr globals;

/*
*********************************************************************
*                                                                   *
* return true if procedure pid is a library procedure               *
* (a library procedure has no body definition)                      *
*                                                                   *
*********************************************************************
*/
int
is_lib_proc(int pid)
{
    return procs[pid].entry == -1;
}

char *
var_name(int pid, int id)
{
    if (id <= 0)
        return "<no name: id is zero>";
    if (pid)
    {
        if ((pid < 0) || (pid > n_procs))
            return "<no name: pid out of range>";
        if (id > procs[pid].n_locals)
            return "<no name: local id too large>";
        else
            return procs[pid].locals[id].name;
    }
    else
    {
        if (id > n_globals)
            return "<no name: global id too large>";
        return globals[id].name;
    }
}

set_ptr
var_points_to(int pid, int id)
{
    int ptr_id;

    if (pid)
        ptr_id = procs[pid].locals[id].ptr_id;
    else
        ptr_id = globals[id].ptr_id;

    if (ptr_id)
        return ptr_points_to(ptr_id);

    return NULL;
}
/*
*********************************************************************
*  malloc n bytes, print fail message if not enough space           *
*********************************************************************
*/
char *
call_malloc(int n, char *fail)
{
    char *chunk;
    /*
    static t = 0;

    t += n;
    printf ("allocate %d bytes [%d] verify(%d) (%s)\n",n,t,
            malloc_verify(),fail);
    fprintf(stdin);
    */
    chunk = malloc(n);
    if (chunk)
        return chunk;

    fprintf(stderr, "Slice file load out of memory: %s\n", fail);
    exit(1);
}

void
get_visit_order(char *dir, file_ptr file)
{
    char mess[2000];
    FILE *v_file;
    int k, n, succ;

    sprintf(mess, "%s%sV", dir, file->name);
    v_file = fopen(mess, "r");
    if (v_file)
    {
        n = 0;
        while (fgets(mess, 2000, v_file))
        {
            k = sscanf(mess, "%d", &succ);
            if (k == 1)
            {
                /*
                printf ("visit %2d after %2d\n",n,succ);
                */
                if ((succ <= 0) || (succ > file->n_stmts))
                {
                    printf("visit error at %d %d in %s\n", succ, n, file->name);
                }
                file->stmts[succ].visit_next = n;
                n                            = succ;
            }
        }
        fclose(v_file);
    }
}

/*
*********************************************************************
*                                                                   *
*  read the file dir_main/file->name (should be a C source file)    *
*  note that the "c" in name.c is missing and must be supplied      *
*  an index is built to allow assess by line number to the source   *
*                                                                   *
*********************************************************************
*/
char mess[2000], dir[2000], *at, *start;

void
get_source(char *dir_main, file_ptr file)
{
    char mess[4000], dir[2000], *at, *start;
    int fd, i;

    sprintf(mess, "allocating %s source", file->name);
    file->text = call_malloc(file->n_chars + 1, mess);
    strcpy(dir, dir_main);
    at = strrchr(dir, '/');
    if (at)
        *(at + 1) = '\0';
    else
        strcpy(dir, "");
    sprintf(mess, "%s%sc", dir, file->name);
    fd = open(mess, O_RDONLY);
    if (fd == -1)
    {
        printf("could not open source: %s\n", mess);
        exit(1);
    }
    read(fd, file->text, file->n_chars);
    file->text[file->n_chars] = '\0';
    close(fd);
    sprintf(mess, "allocating %s source index", file->name);
    file->src_index = (char **)call_malloc((file->n_lines + 1) * sizeof(char *), mess);
    at              = file->text;
    start           = at;
    i               = 1;
    while (*at)
    {
        if (*at == '\n')
        {
            file->src_index[i] = start;
            start              = at + 1;
            i++;
        }
        at++;
    }
    get_visit_order(dir, file);
}

/*
*********************************************************************
*                                                                   *
* Called by read_K_file to create stmt records for a file           *
*                                                                   *
*********************************************************************
*/
void
create_stmts(file_ptr file)
{
    char mess[2000];
    int i;

    sprintf(mess, "%d stmts in %s", file->n_stmts, file->name);
    file->stmts = (stmt_ptr)call_malloc((1 + file->n_stmts) * sizeof(stmt_rec), mess);
    for (i = 1; i <= file->n_stmts; i++)
    {
        file->stmts[i].refs = file->stmts[i].defs = NULL;
        file->stmts[i].requires = file->stmts[i].succ = NULL;
        file->stmts[i].active_global                  = create_bit_set(n_globals + 1);
        file->stmts[i].active_other                   = NULL;
        file->stmts[i].joins                          = 0;
        file->stmts[i].is_return                      = 0;
        file->stmts[i].calls                          = NULL;
        file->stmts[i].visit_next                     = i - 1;
    }
}

/*
*********************************************************************
*                                                                   *
*  Read file.K -- then allocate space for objects:                  *
*       need space for: global variables, pointer chains,           *
*          addresses, procedures, local variables, lists of         *
*          variables declared in header files, source file recs     *
*                                                                   *
*********************************************************************
*/
int
read_k_file(char *name)
{
    char k_file_name[2000];
    FILE *k_file;
    int i, j, ix, nx, ok;
    char buff[2000], mess[4000];

    sprintf(k_file_name, "%sK", name);
    k_file = fopen(k_file_name, "r");
    if (!k_file)
        return 1;

    ok = fscanf(k_file,
        "%*s %d %*s %d %*s %d %*s %d %*s %d %*s %d",
        &n_globals,
        &n_procs,
        &n_addrs,
        &n_chains,
        &n_heads,
        &n_files);
    if (ok != 6)
        return 1;
    /*
    printf ("globals %d  procs %d  addrs %d  chains  %d",
            n_globals,n_procs,n_addrs,n_chains);
    printf ("  headers %d  files %d\n",n_heads,n_files);
    */

    chains =
        (chain_ptr)call_malloc((1 + n_chains) * sizeof(chain_rec), "allocating space for chains");

    for (i = 1; i <= n_chains; i++)
    {
        chains[i].fields = NULL;
    }

    addrs = (addr_ptr)call_malloc((1 + n_addrs) * sizeof(addr_rec), "allocating space for addrs");

    procs = (proc_ptr)call_malloc((1 + n_procs) * sizeof(proc_rec), "allocating space for procs");

    for (i = 0; i < n_procs; i++)
    {
        fscanf(k_file, "%d", &ix);
        fscanf(k_file,
            "%d %d %d %c %s",
            &procs[ix].entry,
            &procs[ix].exit,
            &procs[ix].n_locals,
            &procs[ix].s_or_x,
            buff);
        procs[ix].global_defs  = NULL;
        procs[ix].lib_arefs    = NULL;
        procs[ix].other_defs   = NULL;
        procs[ix].global_idefs = NULL;
        procs[ix].cdefs        = NULL;
        procs[ix].local_idefs  = NULL;
        procs[ix].return_stmts = NULL;
        procs[ix].n_formals    = 0;
        procs[ix].returns_xpr  = 0;
        procs[ix].calls        = NULL;
        procs[ix].contexts     = NULL;
        procs[ix].called_by    = NULL;
        procs[ix].proc_name    = call_malloc(strlen(buff) + 1, " space for proc name");
        strcpy(procs[ix].proc_name, buff);
        sprintf(mess, "%d locals in %s\n", procs[ix].n_locals, buff);
        procs[ix].locals = (id_ptr)call_malloc((1 + procs[ix].n_locals) * sizeof(id_rec), mess);
        for (j = 1; j <= procs[ix].n_locals; j++)
        {
            procs[ix].locals[j].offset = 0;
        }
    }
    files = (file_ptr)call_malloc(n_files * sizeof(file_rec), "space for file records");
    for (i = 0; i < n_files; i++)
    {
        fscanf(k_file,
            "%*d %d %d %d %d %s",
            &files[i].n_procs,
            &files[i].n_stmts,
            &files[i].n_lines,
            &files[i].n_chars,
            buff);
        files[i].name = call_malloc(strlen(buff) + 1, "space for file name");
        strcpy(files[i].name, buff);
        create_stmts(&files[i]);
        get_source(name, &files[i]);
    }

    headers = (head_ptr)call_malloc((n_heads) * sizeof(head_rec), "space for header file records");
    for (i = 0; i < n_heads; i++)
    {
        fscanf(k_file, "%d %s", &headers[i].n, buff);
        nx                     = headers[i].n;
        headers[i].header_file = call_malloc(strlen(buff) + 1, "space for header file name");
        strcpy(headers[i].header_file, buff);
        headers[i].ids = (int *)call_malloc(nx * sizeof(int), "space for header file id indices");
        headers[i].names =
            (char **)call_malloc(nx * sizeof(char *), "space for header file name pointers");
        for (j = 0; j < headers[i].n; j++)
        {
            fscanf(k_file, "%d %s", &headers[i].ids[j], buff);
            headers[i].names[j] = call_malloc(strlen(buff) + 1, "space for header file id names");
            strcpy(headers[i].names[j], buff);
        }
    }

    globals = (id_ptr)call_malloc((1 + n_globals) * sizeof(id_rec), "space for global records");
    for (i = 1; i <= n_globals; i++)
        globals[i].offset = 0;
    fclose(k_file);
    return 0;
}

/*
*********************************************************************
*   This section is obsolete                                        *
*********************************************************************
print_context (cx)
        cx_ptr	cx;
{
        int			i,pid;
        call_ptr	call;

        printf ("level %d: ",cx->level);
        for (i = 0; i < cx->level; i++){
                call = cx->branch[i];
                pid = call->pid;
                printf ("%s ",procs[pid].proc_name);
        }
        printf ("\n");
}

int is_recursive (branch,level)
        call_ptr	branch[];
        int			level;
{
        int			i;

        for (i = 0; i < level; i++)
                if (branch[i]->pid == branch[level]->pid) return 1;
        return 0;
}

generate_call_contexts()
{
        int		i;
        int		main_pid;
        int		level,called;
        cx_ptr		nx,cx,new;
        call_ptr	base,call;
        int			n = 0;

        level = 1;
        cx = NULL;
        for (i = 1; i <= n_procs; i++){
                if (strcmp("main",procs[i].proc_name) == 0) main_pid = i;
        }
        call = procs[main_pid].calls;
        while (call){
                called = call->pid;
                if (procs[called].entry != -1){
                        new = (cx_ptr) call_malloc (sizeof(cx_rec),"call context");
                        new -> branch = (call_ptr *) call_malloc
                                (level*sizeof(call_ptr),"call context");
                        new -> next_this_level = cx;
                        cx = new;
                        new -> level = level;
                        new -> next_this_site = procs[called].contexts;
                        procs[called].contexts = new;
                        new -> branch[0] = call;
                        print_context(new);
                        n++;
                }
                call = call -> next;
        }
        nx = cx;
        while (cx){
                cx = NULL;
                level++;
                while (nx){
                        base = nx->branch[level-2];
                        call = procs[base->pid].calls;
                        if (!is_recursive(nx->branch,level-2))while (call){
                                called = call->pid;
                                if (procs[called].entry != -1){
                                        new = (cx_ptr) call_malloc (sizeof(cx_rec),"call context");
                                        new -> branch = (call_ptr *) call_malloc
                                                (level*sizeof(call_ptr),"call context");
                                        new -> next_this_level = cx;
                                        cx = new;
                                        new -> level = level;
                                        new -> next_this_site = procs[called].contexts;
                                        procs[called].contexts = new;
                                        for (i = 0; i < level-1; i++)
                                                new->branch[i] = nx -> branch[i];
                                        new -> branch[level-1] = call;
                                        print_context(new);
                                        n++;
                                }
                                call = call -> next;
                        }
                        nx = nx -> next_this_level;
                }
                nx = cx;
        }
        printf ("%d calling contexts\n",n);
}
*/

/*
*********************************************************************
*  build a list of calling procs for each proc                      *
*********************************************************************
*/
void
set_called_by(void)
{
    int i;
    int to_pid;
    call_ptr called, by;

    for (i = 1; i <= n_procs; i++)
    {
        called = procs[i].calls;
        while (called)
        {
            to_pid                  = called->pid;
            by                      = (call_ptr)call_malloc(sizeof(call_rec), "called by records");
            by->next                = procs[to_pid].called_by;
            procs[to_pid].called_by = by;
            by->pid                 = i;
            by->stmt                = called->stmt;
            by->actuals             = called->actuals;
            called                  = called->next;
        }
    }
    /*
    for (i = 1; i <= n_procs; i++){
            by = procs[i].called_by;
            printf ("%s called by . . .\n",procs[i].proc_name);
            while (by){
                    printf ("   %s at %d\n",procs[by->pid].proc_name,by->stmt);
                    by = by -> next;
            }
    }
    */
}

/*
*********************************************************************
*  LIF code identifies structures by a non-zero offset field        *
*  However only a top level structure is identified in LIF code.    *
*  mark_sub_structs finds structures declared within other structs  *
*********************************************************************
*/
void
mark_sub_structs(id_ptr vars, int n_vars, int id, int offset)
{
    int i, j, off;
    char *at, *dot;

    if (offset > n_vars)
    {
        fprintf(stderr, "offset > n_vars (%d > %d)\n", offset, n_vars);
        abort();
    }
    /*
    *********************************************************************
    *                                                                   *
    G L O B A L S
      1   1   alph               alph is structure with offset 5
      2   2   alph.aaa
      3   3   alph.b1            need to find alph.b1 (offset 3)
      4   4   alph.b1.bbb
      5   5   alph.b1.g1         and alph.b1.g1 (offset 1)
      6   6   alph.b1.g1.ggg
    *                                                                   *
    *********************************************************************
    */
    at = strrchr(vars[id + 1].name, '.'); /* find rightmost . */
    if (!at)
        return;
    for (i = 2; i <= offset; i++)
    {
        dot = strrchr(vars[id + i].name, '.');
        /* test for new level of members by asking if
           position of rightmost dot has changed */
        if ((dot - vars[id + i].name) != (at - vars[id + 1].name))
        {
            j = i + 1;
            if (j <= offset)
                dot = strrchr(vars[id + j].name, '.');
            while ((j <= offset) && (dot - vars[id + j].name) != (at - vars[id + 1].name))
            {
                j++;
                if (j <= offset)
                    dot = strrchr(vars[id + j].name, '.');
            }
            off = j - i; /* j marks extent of change i.e. offset */
            mark_sub_structs(vars, n_vars, id + i - 1, off);
            vars[id + i - 1].offset = off;
            i                       = j - 1;
        }
    }
}

/*
pr_structs()
{
        int		i;
        int		proc;
        id_ptr	locals;

        for (proc = 1; proc <= n_procs; proc++){
                locals = procs[proc].locals;
                for (i = 1; i <= procs[proc].n_locals;i++){
                        printf ("%-10s %3d %5d %3d %s\n",
                                procs[proc].proc_name?procs[proc].proc_name:"<NIL>"
                                ,proc,i,locals[i].offset,
                                locals[i].name?locals[i].name:"<NIL>");
                        fflush(stdout);
                }
        }
        for (i = 1; i <= n_globals; i++){
                printf ("%-10s %3d %5d %3d %s\n",
                        "global var",0,i,globals[i].offset,
                        globals[i].name?globals[i].name:"<NIL>");
        }
}
*/

void
close_gdefs(void)
{
    int more, result, proc;
    call_ptr call;
    int var, chain;
    set_ptr objs;
    id_set_ptr non_local_defs;
    int aix;
    int id, pid, addr;
    int old;
    /* int		v_opt = 1; */

    for (proc = 1; proc <= n_procs; proc++)
    {
        more = 1;
        if (v_opt)
            printf("proc %s\n", procs[proc].proc_name);
        if (!is_lib_proc(proc))
            while (more)
            {
                more = 0;
                if (procs[proc].cdefs)
                {
                    chain = -1;
                    while ((chain = get_next_member(procs[proc].cdefs, chain)) >= 0)
                    {
                        chain_resolve_set(chain, proc);
                        while (chain_resolve_get(&id, &pid))
                        {
                            if (v_opt)
                                printf("got %s\n", var_name(pid, id));
                            fflush(stdout);
                            if (c_opt)
                                printf("chain %d resolves to (%d,%d) %s %c\n",
                                    chain,
                                    pid,
                                    id,
                                    var_name(pid, id),
                                    (pid ? procs[pid].locals[id].is_pointer
                                         : globals[id].is_pointer)
                                        ? 'P'
                                        : 'S');
                            if (offset_check(pid, id, 0))
                            {
                                if (pid)
                                {
                                    if (pid != proc)
                                        more += add_to_id_set(&procs[proc].other_defs, pid, id);
                                }
                                else
                                {
                                    if (!procs[proc].global_defs)
                                        procs[proc].global_defs = create_bit_set(n_globals + 1);
                                    if (!is_bit_on(procs[proc].global_defs, id))
                                    {
                                        bit_on(procs[proc].global_defs, id);
                                        more++;
                                    }
                                }
                            }
                            else
                            {
                                if (v_opt)
                                    printf("id range error\n");
                            }
                        }
                    }
                }
                if (procs[proc].global_idefs)
                {
                    var = -1;
                    while ((var = get_next_member(procs[proc].global_idefs, var)) >= 0)
                    {
                        objs = var_points_to(0, var);
                        while (objs)
                        {
                            aix = objs->id;
                            if (addrs[aix].pid == 0)
                            {
                                if (!procs[proc].global_defs)
                                    procs[proc].global_defs = create_bit_set(n_globals + 1);
                                if (!is_bit_on(procs[proc].global_defs, addrs[aix].id))
                                {
                                    bit_on(procs[proc].global_idefs, addrs[aix].id);
                                    bit_on(procs[proc].global_defs, addrs[aix].id);
                                    more++;
                                }
                            }
                            else
                            {
                                old = more;
                                more += add_to_id_set(
                                    &procs[proc].other_defs, addrs[aix].pid, addrs[aix].id);
                            }
                            objs = objs->next;
                        }
                    }
                }
                if (procs[proc].local_idefs)
                {
                    var = -1;
                    while ((var = get_next_member(procs[proc].local_idefs, var)) >= 0)
                    {
                        objs = var_points_to(proc, var);
                        while (objs)
                        {
                            aix = objs->id;
                            if (addrs[aix].pid == 0)
                            {
                                if (!procs[proc].global_idefs)
                                    procs[proc].global_idefs = create_bit_set(n_globals + 1);
                                if (!is_bit_on(procs[proc].global_idefs, addrs[aix].id))
                                {
                                    if (!procs[proc].global_defs)
                                        procs[proc].global_defs = create_bit_set(n_globals + 1);
                                    bit_on(procs[proc].global_defs, addrs[aix].id);
                                    bit_on(procs[proc].global_idefs, addrs[aix].id);
                                    more++;
                                }
                                else
                                {
                                    if (addrs[aix].pid == proc)
                                        bit_on(procs[proc].local_idefs, addrs[aix].id);
                                }
                            }
                            else
                            {
                                old = more;
                                more += add_to_id_set(
                                    &procs[proc].other_defs, addrs[aix].pid, addrs[aix].id);
                            }
                            objs = objs->next;
                        }
                    }
                }
                if (procs[proc].lib_arefs)
                {
                    addr = -1;
                    while ((addr = get_next_member(procs[proc].lib_arefs, addr)) >= 0)
                    {
                        if (addrs[addr].pid == 0)
                        {
                            bit_on(procs[proc].global_defs, addrs[addr].id);
                        }
                    }
                }
                if (v_opt)
                    printf("closing %s %d changes\n", procs[proc].proc_name, more);
            }
    }
    more = 1;
    while (more)
    {
        more = 0;
        for (proc = 1; proc <= n_procs; proc++)
        {
            if (!is_lib_proc(proc))
            {
                call = procs[proc].called_by;
                while (call)
                {
                    if (procs[proc].global_defs)
                    {
                        if (!procs[call->pid].global_defs)
                            procs[call->pid].global_defs = create_bit_set(n_globals + 1);
                        result =
                            cunion_bit_set(procs[call->pid].global_defs, procs[proc].global_defs);
                        more += result;
                        non_local_defs = procs[proc].other_defs;
                        while (non_local_defs)
                        {
                            if (non_local_defs->pid != call->pid)
                                more += add_to_id_set(&procs[call->pid].other_defs,
                                    non_local_defs->pid,
                                    non_local_defs->id);
                            non_local_defs = non_local_defs->next;
                        }
                    }
                    call = call->next;
                }
            }
        }
    }
}

/*
*********************************************************************
* scan ids for structures, then call mark_sub_structs to mark       *
* structures declared within other structures                       *
*********************************************************************
*/
void
scan_structs(void)
{
    int i;
    int proc;
    id_ptr locals;

    for (i = 1; i <= n_globals; i += 1 + globals[i].offset)
    {
        if (globals[i].offset)
        {
            mark_sub_structs(globals, n_globals, i, globals[i].offset);
        }
    }

    for (proc = 1; proc <= n_procs; proc++)
    {
        locals = procs[proc].locals;
        for (i = 1; i <= procs[proc].n_locals; i += 1 + locals[i].offset)
        {
            if (locals[i].offset)
            {
                mark_sub_structs(locals, procs[proc].n_locals, i, locals[i].offset);
            }
        }
    }
}

/*
*********************************************************************
*                                                                   *
*  Read the LINK file with the LIF codes for all source files       *
*  merged together by slink.                                        *
*                                                                   *
*********************************************************************
*/
int
read_link_file(char *name)
{
    char l_file_name[2000];
    FILE *l_file;
    int code, id, pid, stmt, level, chain;
    int cur_proc = 0, cur_file = 0, i, n, addr;
    int fl, fc, tl, tc, field_seq, offset, from, to;
    char id_name[2000], buff[2000], flags[2000];
    id_ptr cur_id;
    var_ptr *add_to;
    field_ptr at_field, field;
    call_ptr call_stack[200], call;
    actual_ptr actual;
    int top_call = 0, sep_stack[200];
    set_ptr new_return;

    sprintf(l_file_name, "%sLINK", name);
    l_file = fopen(l_file_name, "r");
    if (!l_file)
        return 1;
    while (fgets(buff, 2000, l_file))
    {
        sscanf(buff, "%d", &code);
        switch (code)
        {
        case LIF_PROC_START:
            sscanf(buff, "%*d(%*d,%d", &cur_proc);
            procs[cur_proc].file_id = cur_file;
            for (i = procs[cur_proc].entry; i <= procs[cur_proc].exit; i++)
            {
                files[cur_file].stmts[i].active_local =
                    create_bit_set(procs[cur_proc].n_locals + 1);
            }
            break;
        case LIF_PROC_END:
            cur_proc = 0;
            break;
        case LIF_FORMAL_ID:
            procs[cur_proc].n_formals++;
        case LIF_LOCAL_ID:
        case LIF_GLOBAL_ID:
            sscanf(buff, "%*d(%d,%[^,)]%s", &id, id_name, flags);
            if (code == LIF_GLOBAL_ID)
                cur_id = globals;
            else
                cur_id = procs[cur_proc].locals;
            cur_id[id].name = call_malloc(strlen(id_name) + 1, "space for id name");
            strcpy(cur_id[id].name, id_name);
            if (code == LIF_FORMAL_ID)
                cur_id[id].is_formal = 1;
            else
                cur_id[id].is_formal = 0;
            if (strchr(flags, 'P'))
                cur_id[id].is_pointer = 1;
            else
                cur_id[id].is_pointer = 0;
            if (strchr(flags, 'A'))
                cur_id[id].is_array = 1;
            else
                cur_id[id].is_array = 0;
            break;
        case LIF_STRUCT:
            sscanf(buff, "%*d(%d,%d,%d", &pid, &id, &offset);
            if (pid)
                procs[pid].locals[id].offset = offset;
            else
                globals[id].offset = offset;
            break;
        case LIF_FILE:
            sscanf(buff, "%*d(%d", &cur_file);
            break;
        case LIF_REF:
        case LIF_DEF:
        case LIF_GREF:
        case LIF_GDEF:
        case LIF_CREF:
        case LIF_CDEF:
        case LIF_AREF:
            n = sscanf(buff, "%*d(%d,%d,%d", &stmt, &id, &level);
            if (n == 2)
                level = 0;
            if (top_call)
            {
                if (!call->actuals)
                {
                    actual                = (actual_ptr)call_malloc(sizeof(actual_rec), "actual");
                    actual->next          = NULL;
                    actual->actuals       = NULL;
                    call->actuals         = actual;
                    actual->actual_number = 1;
                }
                else
                    actual = call->actuals;
                actual->actuals = add_to_var_set(actual->actuals, id, level, code);
                if ((code == LIF_AREF) && is_lib_proc(call->pid))
                {
                    if (procs[cur_proc].lib_arefs == NULL)
                        procs[cur_proc].lib_arefs = create_bit_set(n_addrs + 1);
                    bit_on(procs[cur_proc].lib_arefs, id);
                }
            }
            else
            {
                if (code == LIF_GDEF)
                {
                    if (procs[cur_proc].global_defs == NULL)
                        procs[cur_proc].global_defs = create_bit_set(n_globals + 1);
                    if (level)
                    {
                        if (procs[cur_proc].global_idefs == NULL)
                            procs[cur_proc].global_idefs = create_bit_set(n_globals + 1);
                        bit_on(procs[cur_proc].global_idefs, id);
                    }
                    else
                        bit_on(procs[cur_proc].global_defs, id);
                }
                else if (code == LIF_DEF)
                {
                    if (level)
                    {
                        if (procs[cur_proc].local_idefs == NULL)
                            procs[cur_proc].local_idefs =
                                create_bit_set(procs[cur_proc].n_locals + 1);
                        bit_on(procs[cur_proc].local_idefs, id);
                    }
                }
                else if (code == LIF_CDEF)
                {
                    if (procs[cur_proc].cdefs == NULL)
                        procs[cur_proc].cdefs = create_bit_set(n_chains + 1);
                    bit_on(procs[cur_proc].cdefs, id);
                }
                if ((code == LIF_DEF) || (code == LIF_GDEF) || (code == LIF_CDEF))
                    add_to = &files[cur_file].stmts[stmt].defs;
                else
                    add_to = &files[cur_file].stmts[stmt].refs;
                *add_to = add_to_var_set(*add_to, id, level, code);
            }
            break;
        case LIF_SOURCE_MAP:
            sscanf(buff, "%*d(%d,%d,%d,%d,%d", &stmt, &fl, &fc, &tl, &tc);
            files[cur_file].stmts[stmt].froml = fl;
            files[cur_file].stmts[stmt].fromc = fc;
            files[cur_file].stmts[stmt].tol   = tl;
            files[cur_file].stmts[stmt].toc   = tc;
            break;
        case LIF_CHAIN:
        case LIF_GCHAIN:
            sscanf(buff, "%*d(%d,%d", &chain, &id);
            chains[chain].id  = id;
            chains[chain].pid = LIF_GCHAIN == code ? 0 : cur_proc;
            break;
        case LIF_ADDRESS:
            sscanf(buff, "%*d(%d,%d,%d", &addr, &pid, &id);
            addrs[addr].id  = id;
            addrs[addr].pid = pid;
            break;
        case LIF_FIELD:
            sscanf(buff, "%*d(%d,%d,%d,%[^)]", &chain, &field_seq, &offset, id_name);
            field       = (field_ptr)call_malloc(sizeof(field_rec), "allocating field");
            field->next = chains[chain].fields;
            if (chains[chain].fields && (chains[chain].fields->fid < field_seq))
            {
                at_field = chains[chain].fields;
                while (at_field->next && (at_field->next->fid < field_seq))
                    at_field = at_field->next;
                field->next    = at_field->next;
                at_field->next = field;
            }
            else
            {
                field->next          = chains[chain].fields;
                chains[chain].fields = field;
            }
            field->offset = offset;
            field->fid    = field_seq;
            field->name   = call_malloc(strlen(id_name) + 1, "field name");
            strcpy(field->name, id_name);
            break;
        case LIF_SUCC:
            sscanf(buff, "%*d(%d,%d", &stmt, &id);
            files[cur_file].stmts[stmt].joins++;
            files[cur_file].stmts[stmt].succ = add_to_set(files[cur_file].stmts[stmt].succ, id);
            break;
        case LIF_REQUIRES:
            n = sscanf(buff, "%*d(%d,%d,%d", &stmt, &from, &to);
            if (n == 2)
                to = from;
            for (i = from; i <= to; i++)
            {
                files[cur_file].stmts[i].requires =
                    add_to_set(files[cur_file].stmts[i].requires, stmt);
            }
            break;
        case LIF_CALL_START:
            sscanf(buff, "%*d(%d,%d", &stmt, &pid);
            top_call++;
            sep_stack[top_call]  = 1;
            call_stack[top_call] = call = (call_ptr)call_malloc(sizeof(call_rec), "call site");
            call->next                  = procs[cur_proc].calls;
            procs[cur_proc].calls       = call;
            call->actuals               = NULL;
            call->stmt                  = stmt;
            call->pid                   = pid;
            call->next_this_stmt        = files[cur_file].stmts[stmt].calls;
            files[cur_file].stmts[stmt].calls = call;
            break;
        case LIF_ACTUAL_SEP:
            sep_stack[top_call]++;
            actual                = (actual_ptr)call_malloc(sizeof(actual_rec), "actual");
            actual->next          = call->actuals;
            call->actuals         = actual;
            actual->actuals       = NULL;
            actual->actual_number = sep_stack[top_call];
            break;
        case LIF_CALL_END:
            top_call--;
            call = call_stack[top_call];
            break;
        case LIF_RETURN:
            sscanf(buff, "%*d(%d,%d", &stmt, &id);
            files[cur_file].stmts[stmt].is_return = 1;
            procs[cur_proc].returns_xpr |= id;
            new_return                   = new_set;
            new_return->id               = stmt;
            new_return->next             = procs[cur_proc].return_stmts;
            procs[cur_proc].return_stmts = new_return;
            break;
        case LIF_GOTO:
            break;
        }
    }

    fclose(l_file);

    /*
    generate_call_contexts();
    pr_structs();
    */
    set_called_by();
    scan_structs();
    count_vars();
    flow();
    close_gdefs();

    return 0;
}

void
print_var_set(proc_ptr p, var_ptr s)
{
    field_ptr at_field;
    int i;

    while (s)
    {
        /*
        printf ("  [code %d] ",s->code);
        */
        switch (s->code)
        {
        case LIF_REF:
        case LIF_DEF:
            for (i = 0; i < s->level; i++)
                printf("*");
            printf("%s ", p->locals[s->id].name);
            break;
        case LIF_GREF:
        case LIF_GDEF:
            for (i = 0; i < s->level; i++)
                printf("*");
            printf("%s ", globals[s->id].name);
            break;
        case LIF_AREF:
            printf("&%s ",
                addrs[s->id].pid ? p->locals[addrs[s->id].id].name : globals[addrs[s->id].id].name);
            break;
        case LIF_CREF:
        case LIF_CDEF:
            if (chains[s->id].pid && (&procs[chains[s->id].pid] != p))
            {
                fprintf(stderr,
                    "local chain %d on %s of %s used in %s\n",
                    s->id,
                    var_name(chains[s->id].pid, chains[s->id].id),
                    procs[chains[s->id].pid].proc_name,
                    p->proc_name);
            }
            printf("(%d)%s",
                s->id,
                chains[s->id].pid ? procs[chains[s->id].pid].locals[chains[s->id].id].name :
                                  /*
                                  p->locals[chains[s->id].id].name:
                                  */
                    globals[chains[s->id].id].name);
            at_field = chains[s->id].fields;
            while (at_field)
            {
                printf("->%s", at_field->name);
                at_field = at_field->next;
            }
            printf(" ");
            break;
        default:
            printf("%d(%d) ", s->id, s->code);
        }
        s = s->next;
    }
}

void
print_set(set_ptr s)
{
    while (s)
    {
        printf("%d ", s->id);
        s = s->next;
    }
}

void
print_proc_defs(int nc, int proc)
{
    int i = proc;
    int at;
    id_set_ptr others;

    if (procs[i].global_defs)
    {
        at = -1;
        while ((at = get_next_member(procs[i].global_defs, at)) >= 0)
        {
            if (nc == 0)
                printf("\t\t");
            nc += printf("%s ", globals[at].name);
            if (nc > 60)
            {
                nc = 0;
                printf("\n");
            }
        }
    }
    if (procs[i].other_defs)
    {
        printf(" others ");
        others = procs[i].other_defs;
        while (others)
        {
            if (nc == 0)
                printf("\t\t");
            nc += printf("%s ", procs[others->pid].locals[others->id].name);
            if (nc > 60)
            {
                nc = 0;
                printf("\n");
            }
            others = others->next;
        }
    }
    if (procs[i].global_idefs)
    {
        at = -1;
        while ((at = get_next_member(procs[i].global_idefs, at)) >= 0)
        {
            if (nc == 0)
                printf("\t\t");
            nc += printf("*%s ", globals[at].name);
            if (nc > 60)
            {
                nc = 0;
                printf("\n");
            }
        }
    }
    if (procs[i].local_idefs)
    {
        at = -1;
        while ((at = get_next_member(procs[i].local_idefs, at)) >= 0)
        {
            if (nc == 0)
                printf("\t\t");
            nc += printf("*%s ", procs[i].locals[at].name);
            if (nc > 60)
            {
                nc = 0;
                printf("\n");
            }
        }
    }
}

void
verify_input(int l_opt)
{
    int nc, i, j, fid;
    char *line;
    call_ptr call, cp;
    actual_ptr actual;
    field_ptr f;

    printf("Files\n");
    for (i = 0; i < n_files; i++)
    {
        printf("\t%-15s %3d %3d %5d %7d\n",
            files[i].name,
            files[i].n_procs,
            files[i].n_stmts,
            files[i].n_lines,
            files[i].n_chars);
    }
    printf("Procs\n");
    for (i = 1; i <= n_procs; i++)
        if (procs[i].entry != -1)
        {
            printf("\t%-20s %3d  %3d %c [%d..%d] (%s)\n",
                procs[i].proc_name,
                i,
                procs[i].n_locals,
                procs[i].s_or_x,
                procs[i].entry,
                procs[i].exit,
                files[procs[i].file_id].name);
        }
    printf("Procs global/non-local defs\n");
    for (i = 1; i <= n_procs; i++)
        if (procs[i].entry != -1)
        {
            nc = printf("\t%-20s ", procs[i].proc_name);
            print_proc_defs(nc, i);
            if (!procs[i].global_defs && !procs[i].other_defs && !procs[i].global_idefs &&
                !procs[i].local_idefs)
                printf("none");
            if (nc)
                printf("\n");
        }
    printf("Calls\n");
    for (i = 1; i <= n_procs; i++)
        if ((procs[i].entry != -1) && procs[i].calls)
        {
            printf("\t%-20s %3d  %3d %c [%d..%d] (%s)\n",
                procs[i].proc_name,
                i,
                procs[i].n_locals,
                procs[i].s_or_x,
                procs[i].entry,
                procs[i].exit,
                files[procs[i].file_id].name);
            call = procs[i].calls;
            while (call)
            {
                printf("\t%4d %s: (", call->stmt, procs[call->pid].proc_name);
                actual = call->actuals;
                while (actual)
                {
                    print_var_set(&procs[i], actual->actuals);
                    actual = actual->next;
                    if (actual)
                        printf(", ");
                }
                printf(")\n");
                call = call->next;
            }
        }
    printf("Library Functions\n");
    for (i = 1; i <= n_procs; i++)
        if (procs[i].entry == -1)
        {
            printf("\t%-20s %3d\n", procs[i].proc_name, i);
        }
    printf("Globals\n");
    for (i = 1; i <= n_globals; i++)
    {
        printf("\t%3d %c %c %c %s",
            i,
            globals[i].is_formal ? 'F' : ' ',
            globals[i].is_pointer ? 'P' : ' ',
            globals[i].is_array ? 'A' : ' ',
            globals[i].name);
        if (globals[i].offset)
            printf(" struct with %d members\n", globals[i].offset);
        else
            printf("\n");
    }
    printf("Locals\n");
    for (i = 1; i <= n_procs; i++)
        if (procs[i].entry != -1)
        {
            printf("\t%-20s \n", procs[i].proc_name);
            for (j = 1; j <= procs[i].n_locals; j++)
            {
                printf("\t\t%3d %c %c %c %s",
                    j,
                    procs[i].locals[j].is_formal ? 'F' : ' ',
                    procs[i].locals[j].is_pointer ? 'P' : ' ',
                    procs[i].locals[j].is_array ? 'A' : ' ',
                    procs[i].locals[j].name);
                if (procs[i].locals[j].offset)
                    printf(" struct with %d members\n", procs[i].locals[j].offset);
                else
                    printf("\n");
            }
        }
    printf("Addrs\n");
    for (i = 1; i <= n_addrs; i++)
    {
        printf("%4d ==> %s.%s (%d,%d)\n",
            i,
            addrs[i].pid ? procs[addrs[i].pid].proc_name : "global",
            addrs[i].pid ? procs[addrs[i].pid].locals[addrs[i].id].name : globals[addrs[i].id].name,
            addrs[i].pid,
            addrs[i].id);
    }
    printf("Chains\n");
    for (i = 1; i <= n_chains; i++)
    {
        printf("%4d ==> (%d,%d) (%s) %s",
            i,
            chains[i].pid,
            chains[i].id,
            chains[i].pid ? procs[chains[i].pid].proc_name : "global",
            chains[i].pid ? procs[chains[i].pid].locals[chains[i].id].name
                          : globals[chains[i].id].name);
        f = chains[i].fields;
        while (f)
        {
            printf("->%s", f->name);
            f = f->next;
        }
        printf("\n");
    }
    if (!l_opt)
        return;
    printf("Statements\n");
    for (i = 1; i <= n_procs; i++)
        if (procs[i].entry != -1)
        {
            printf("\t%-20s %3d  %3d %c [%d..%d] (%s)\n",
                procs[i].proc_name,
                i,
                procs[i].n_locals,
                procs[i].s_or_x,
                procs[i].entry,
                procs[i].exit,
                files[procs[i].file_id].name);
            fid = procs[i].file_id;
            for (j = procs[i].entry; j <= procs[i].exit; j++)
            {
                nc = printf("\t%4d @ %d  ", j, files[fid].stmts[j].froml);
                for (; nc < 12; nc++)
                    printf(" ");
                line = files[procs[i].file_id].src_index[files[fid].stmts[j].froml];
                while (*line != '\n')
                    printf("%c", *line++);
                if (files[fid].stmts[j].joins != 1)
                    printf("\n\t     %d pred", files[fid].stmts[j].joins);
                if (files[fid].stmts[j].calls)
                    printf("\n\t     Calls:");
                cp = files[fid].stmts[j].calls;
                while (cp)
                {
                    printf(" %s", procs[cp->pid].proc_name);
                    cp = cp->next_this_stmt;
                }
                printf("\n\t     Defs: ");
                print_var_set(&procs[i], files[fid].stmts[j].defs);
                printf("\n\t     Refs: ");
                if (files[fid].stmts[j].is_return)
                    printf("returns ");
                if (procs[i].returns_xpr)
                    printf("expression ");
                print_var_set(&procs[i], files[fid].stmts[j].refs);
                printf("\n\t     Succ: ");
                print_set(files[fid].stmts[j].succ);
                printf("  Requires: ");
                print_set(files[fid].stmts[j].requires);
                printf("  Visits next: %d", files[fid].stmts[j].visit_next);
                printf("\n");
            }
        }
}

/*
*********************************************************************
* find the proc_id for a given proc name                            *
* Note that the first name found is returned                        *
* procs declared static could be a problem?????                     *
*********************************************************************
*/
int
find_proc(char *name)
{
    int ix;

    for (ix = 1; ix <= n_procs; ix++)
    {
        if (0 == strcmp(procs[ix].proc_name, name))
        {
            return ix;
        }
    }
    return -1;
}

/*
*********************************************************************
* Translate a file name to a file_id                                *
*********************************************************************
*/
int
find_file(char *name)
{
    int ix;

    for (ix = 0; ix < n_files; ix++)
    {
        if (0 == strcmp(files[ix].name, name))
        {
            return ix;
        }
    }
    return -1;
}

/*
*********************************************************************
*                                                                   *
* Translate a variable name to a variable id                        *
* pid == 0 => global id                                             *
* pid > 0  => local id of procedure pid                             *
*                                                                   *
*********************************************************************
*/
int
find_var(int pid, char *name)
{
    int n, ix;
    id_ptr vars;

    if (pid)
    {
        vars = procs[pid].locals;
        n    = procs[pid].n_locals;
    }
    else
    {
        vars = globals;
        n    = n_globals;
    }
    for (ix = 1; ix <= n; ix++)
    {
        if (0 == strcmp(vars[ix].name, name))
        {
            return ix;
        }
    }
    return -1;
}

int
make_criterion(
    char *file_name, int line_number, char *var_spec, int *file, int *stmt, int *proc, int *var)
{
    int len;
    int n;
    char proc_name[1000], var_name[1000];

    len                = strlen(file_name);
    file_name[len - 1] = '\0';
    *file              = find_file(file_name);
    /*
    printf ("find file %s => %d\n",file_name,*file);
    */
    if (*file == -1)
    {
        fprintf(stderr, "file %sc not part of %sc system\n", file_name, files[0].name);
        return 1;
    }

    if (var_spec[0] == '\'')
    {
        *proc = 0;
        n     = sscanf(var_spec, "'%s", var_name);
    }
    else
    {
        n = sscanf(var_spec, "%[^']'%s", proc_name, var_name);
        if (n == 1)
        {
            *proc = 0;
            strcpy(var_name, proc_name);
        }
        else
            *proc = find_proc(proc_name);
    }
    /*
    printf ("find proc %s => %d\n",*proc?proc_name:"global",*proc);
    */
    if (*proc == -1)
    {
        fprintf(stderr, "proc %s not part of %sc system\n", proc_name, files[0].name);
        return 1;
    }
    *var = find_var(*proc, var_name);
    /*
    printf ("find var %s => %d\n",var_name,*var);
    */
    *stmt = line_to_stmt(*file, line_number);

    return 0;
}

/*
*********************************************************************
*  Translate a line number within a file to a node (stmt) number    *
*  If the line has no stmt (e.g., is blank) find a close stmt.      *
*********************************************************************
*/
int
line_to_stmt(int file, int line)
{
    int i;
    int best_stmt, best_line;

    for (i = 1; i <= files[file].n_stmts; i++)
    {
        if (files[file].stmts[i].froml == line)
            return i;
    }
    best_line = -1;
    best_stmt = 1;
    for (i = 1; i <= files[file].n_stmts; i++)
    {
        if (files[file].stmts[i].froml < line)
        {
            if (files[file].stmts[i].froml > best_line)
            {
                best_stmt = i;
                best_line = files[file].stmts[i].froml;
            }
        }
    }
    return best_stmt;
}
