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

#include "multislice.h"
#include "sets.h"
#include "slice.h"
#include <X11/Intrinsic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"

#define LIST_MAX 2000

/*
static  char    sccsid[] = "@(#)history.c	1.2  4/26/95";
*/
typedef struct history_struct history_rec, *history_ptr;

struct history_struct
{
    char *list_text;
    long file_index;
    int var_pid, var_id, file_id, stmt_id;
    int is_partial;
    history_ptr next;
};

static history_ptr history = NULL;
static char history_file[1000];
static char *list[LIST_MAX];

extern void bit_on(bit_set s, int id);
void SliceSet(MultiSliceWidget w, int line_from, int col_from, int line_to, int col_to, int redraw);
void SliceRedraw(MultiSliceWidget w);
void SliceClearAll(MultiSliceWidget w);
extern int is_id_valid(int var_pid, int var_id);

/* typedef void *Widget;
 */

void
clear_history(void)
{
    history = NULL;
}

void
setup_history(char *file)
{
    FILE *h_file;
    int var_pid, var_id, file_id, stmt_id, is_partial;
    int fid, s;
    history_ptr new, at;
    char text[1000];
    long addr;

    sprintf(history_file, "%sY", file);
    h_file = fopen(history_file, "r");
    if (h_file)
    {
        addr = ftell(h_file);
        while (EOF !=
               fscanf(h_file, "%d %d %d %d %d", &var_pid, &var_id, &file_id, &stmt_id, &is_partial))
        {
            if (!is_id_valid(var_pid, var_id))
            {
                fprintf(stderr, "pid %d var %d not valid\n", var_pid, var_id);
                fflush(stderr);
            }
            fscanf(h_file, "%d", &fid);
            while (fid != -1)
            {
                fscanf(h_file, "%d", &s);
                while (s != -1)
                {
                    fscanf(h_file, "%d", &s);
                }
                fscanf(h_file, "%d", &fid);
            }
            new             = (history_ptr)malloc(sizeof(history_rec));
            new->file_index = addr;
            new->var_pid    = var_pid;
            new->var_id     = var_id;
            new->file_id    = file_id;
            new->stmt_id    = stmt_id;
            new->is_partial = is_partial;
            new->next       = NULL;
            sprintf(text,
                "%s(%s) %d(%sc)%s",
                var_name(var_pid, var_id),
                var_pid ? procs[var_pid].proc_name : "global",
                files[file_id].stmts[stmt_id].froml,
                files[file_id].name,
                is_partial ? " (partial)" : "");
            new->list_text = malloc(strlen(text) + 1);
            strcpy(new->list_text, text);
            if (history)
            {
                at = history;
                while (at->next)
                    at = at->next;
                at->next = new;
            }
            else
                history = new;
            addr = ftell(h_file);
        }
        fclose(h_file);
    }
}

int find_slice(var_pid, var_id, file_id, stmt_id) int var_pid, var_id, file_id, stmt_id;
{
    history_ptr at;
    int ix = 0;

    at = history;
    while (at)
    {
        if ((at->var_pid == var_pid) && (at->var_id == var_id) && (at->file_id == file_id) &&
            (at->stmt_id == stmt_id) && !at->is_partial)
            return ix;
        ix++;
        at = at->next;
    }
    return -1;
}

void
load_slice(Widget w, int index, int n_files, bit_set slices[], int line_map[])
{
    history_ptr at;
    FILE *h_file;
    int i, fid, stmt;

    at = history;
    while (at && index)
    {
        at = at->next;
        index--;
    }
    if (index)
        return;

    h_file = fopen(history_file, "r");
    if (h_file)
    {
        SliceClearAll(w);
        fseek(h_file, at->file_index, 0);
        fscanf(h_file, "%*d %*d%*d%*d%*d"); /* skip first line */
        for (i = 0; i < n_files; i++)
            clear_bit_set(slices[i]);
        fscanf(h_file, "%d", &fid);
        while (fid != -1)
        {
            fscanf(h_file, "%d", &stmt);
            while (stmt != -1)
            {
                bit_on(slices[fid], stmt);
                SliceSet(w,
                    files[fid].stmts[stmt].froml + line_map[fid],
                    files[fid].stmts[stmt].fromc,
                    files[fid].stmts[stmt].tol + line_map[fid],
                    files[fid].stmts[stmt].toc,
                    0);
                fscanf(h_file, "%d", &stmt);
            }
            fscanf(h_file, "%d", &fid);
        }
        fclose(h_file);
    }
    SliceRedraw(w);
}

void
save_slice(int var_pid,
    int var_id,
    int file_id,
    int stmt_id,
    int is_partial,
    int n_files,
    bit_set slices[])
{
    FILE *h_file;
    history_ptr new, at;
    char text[1000];
    int fid, stmt;
    int nc;

    /*
    printf ("save %d %d %s %s fid %d stmt %d nf %d ip %d\n",
            var_pid,var_id,var_name(var_pid,var_id),
            var_pid?procs[var_pid].proc_name:"global",
            file_id,stmt_id,n_files,is_partial);
    */
    if (!is_id_valid(var_pid, var_id))
    {
        fprintf(stderr, "pid %d var %d not valid\n", var_pid, var_id);
        fflush(stderr);
    }
    h_file = fopen(history_file, "a");
    if (h_file == NULL)
    {
        fprintf(stderr, "Could not open slice history file (%s)\n", history_file);
        return;
    }
    new             = (history_ptr)malloc(sizeof(history_rec));
    new->file_index = ftell(h_file);
    new->var_pid    = var_pid;
    new->var_id     = var_id;
    new->file_id    = file_id;
    new->stmt_id    = stmt_id;
    new->is_partial = is_partial;
    new->next       = NULL;
    /*
sprintf (text,"%s(%s) %d(%sc)",var_name(var_pid,var_id),
var_pid?procs[var_pid].proc_name:"global",
files[file_id].stmts[stmt_id].froml,files[file_id].name);
            */
    sprintf(text,
        "%s(%s) %d(%sc)%s",
        var_name(var_pid, var_id),
        var_pid ? procs[var_pid].proc_name : "global",
        files[file_id].stmts[stmt_id].froml,
        files[file_id].name,
        is_partial ? " (partial)" : "");
    new->list_text = malloc(strlen(text) + 1);
    strcpy(new->list_text, text);
    if (history)
    {
        at = history;
        while (at->next)
            at = at->next;
        at->next = new;
    }
    else
        history = new;
    fprintf(h_file, "%d %d %d %d %d\n", var_pid, var_id, file_id, stmt_id, is_partial);
    for (fid = 0; fid < n_files; fid++)
    {
        nc   = fprintf(h_file, "%d ", fid);
        stmt = -1;
        while ((stmt = get_next_member(slices[fid], stmt)) >= 0)
        {
            nc += fprintf(h_file, "%d ", stmt);
            if (nc > 60)
            {
                fprintf(h_file, "\n");
                nc = 0;
            }
        }
        fprintf(h_file, "-1\n");
    }
    fprintf(h_file, "-1\n");
    fclose(h_file);
}

char **
get_slice_list(void)
{
    history_ptr at;
    int i = 1;

    list[0] = "No selection";
    at      = history;
    while (at)
    {
        if (i < LIST_MAX)
            list[i++] = at->list_text;
        at = at->next;
    }
    list[i] = NULL;
    return list;
}
