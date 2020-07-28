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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//static char sccsid[] = "@(#)summary.c	1.3  4/26/95";

typedef struct p_struct {
    int proc_id;
    int n_locals;
    int n_global_refs;
    int n_global_defs;
    int n_global_irefs;
    int n_global_idefs;
    int n_irefs, n_idefs;
    int n_refs, n_defs;
    int n_crefs, n_cdefs;
    int n_arefs;
    int has_return_value;
    int n_calls;
    char *name;
} proc_rec, *proc_ptr;

int main(int np, char **p)
{
    proc_rec procs[10000];
    char buff[2000], name[2000];
    int last_global_id = 0;
    int last_node      = 0;
    int n_procs        = 0;
    int last_addr      = 0;
    FILE *ff_file      = NULL;
    int max_local = 0, max_formal = 0;
    int id, node, code;
    int level;

    if (np == 2) {
        ff_file = fopen(p[1], "r");
        if (ff_file == NULL) {
            fprintf(stderr, "summary: could not open %s\n", p[1]);
            exit(1);
        }
    }
    if (ff_file == NULL) {
        fprintf(stderr, "summary: no LIF file given\n");
        exit(1);
    }
    while (NULL != fgets(buff, 2000, ff_file)) {
        sscanf(buff, "%d", &code);
        switch (code) {
        case LIF_PROC_START:
            sscanf(buff, "%*d(%*d,%d,%[^)]", &id, name);
            procs[n_procs].name = malloc(strlen(name) + 1);
            strcpy(procs[n_procs].name, name);
            procs[n_procs].proc_id          = id;
            procs[n_procs].n_locals         = 0;
            procs[n_procs].n_global_refs    = 0;
            procs[n_procs].n_global_defs    = 0;
            procs[n_procs].has_return_value = 0;
            procs[n_procs].n_calls          = 0;
            procs[n_procs].n_arefs          = 0;
            procs[n_procs].n_refs           = 0;
            procs[n_procs].n_defs           = 0;
            procs[n_procs].n_irefs          = 0;
            procs[n_procs].n_idefs          = 0;
            procs[n_procs].n_global_irefs   = 0;
            procs[n_procs].n_global_idefs   = 0;
            break;
        case LIF_PROC_END:
            sscanf(buff, "%*d(%d", &node);
            if (node > last_node)
                last_node = node;
            n_procs++;
            max_local  = 0;
            max_formal = 0;
            break;
        case LIF_FORMAL_ID:
            sscanf(buff, "%*d(%d", &id);
            if (id > max_formal)
                max_formal = id;
            break;
        case LIF_LOCAL_ID:
            sscanf(buff, "%*d(%d", &id);
            if (id > procs[n_procs].n_locals)
                procs[n_procs].n_locals = id;
            break;
        case LIF_GLOBAL_ID:
            sscanf(buff, "%*d(%d", &id);
            if (id > last_global_id)
                last_global_id = id;
            break;
        case LIF_AREF:
            procs[n_procs].n_arefs++;
            break;
        case LIF_ADDRESS:
            sscanf(buff, "%*d(%d", &id);
            sscanf(buff, "%*d(%d", &id);
            if (id > last_addr)
                last_addr = id;
            break;
        case LIF_REF:
            level = sscanf(buff, "%*d(%*d,%*d,%d", &id);
            if (level)
                procs[n_procs].n_irefs++;
            else
                procs[n_procs].n_refs++;
            break;
        case LIF_DEF:
            level = sscanf(buff, "%*d(%*d,%*d,%d", &id);
            if (level)
                procs[n_procs].n_idefs++;
            else
                procs[n_procs].n_defs++;
            break;
        case LIF_GREF:
            level = sscanf(buff, "%*d(%*d,%*d,%d", &id);
            if (level)
                procs[n_procs].n_global_irefs++;
            else
                procs[n_procs].n_global_refs++;
            break;
        case LIF_GDEF:
            level = sscanf(buff, "%*d(%*d,%*d,%d", &id);
            if (level)
                procs[n_procs].n_global_idefs++;
            else
                procs[n_procs].n_global_defs++;
            break;
        case LIF_CREF:
            procs[n_procs].n_crefs++;
            break;
        case LIF_CDEF:
            procs[n_procs].n_cdefs++;
            break;
        case LIF_CALL_START:
            procs[n_procs].n_calls++;
            break;
        case LIF_ACTUAL_SEP:
            break;
        case LIF_CALL_END:
            break;
        case LIF_RETURN:
            sscanf(buff, "%*d(%*d,%d", &id);
            procs[n_procs].has_return_value |= id;
            break;
        case LIF_GOTO:
            break;
        case LIF_SUCC:
            break;
        case LIF_REQUIRES:
            break;
        case LIF_SOURCE_MAP:
            break;
        default:
            break;
        }
    }
    printf("last_global %d last_node %d last_addr %d n_procs %d\n",
        last_global_id,
        last_node,
        last_addr,
        n_procs);
    printf("%10s %10s %10s %10s %10s %10s\n",
        "proc_id",
        "n_locals",
        "global_refs",
        "local_refs",
        "chain_refs",
        "has_return");
    for (id = 0; id < n_procs; id++) {
        printf("%10d %10d %10d %10d %10d\n",
            procs[id].proc_id,
            procs[id].n_locals,
            procs[id].n_global_refs,
            procs[id].n_refs,
            procs[id].n_crefs);
    }
    printf("\n");
    printf(" %3s %4s %4s %4s %4s %4s %4s %4s %4s %4s\n",
        "PID",
        "iref",
        "idef",
        "gdef",
        "def",
        "gir",
        "gid",
        "nc",
        "cd",
        "ar");
    for (id = 0; id < n_procs; id++) {
        printf("%3d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d =%s= %s\n",
            procs[id].proc_id,
            procs[id].n_irefs,
            procs[id].n_idefs,
            procs[id].n_global_defs,
            procs[id].n_defs,
            procs[id].n_global_irefs,
            procs[id].n_global_idefs,
            procs[id].n_calls,
            procs[id].n_cdefs,
            procs[id].n_arefs,
            procs[id].has_return_value,
            (procs[id].n_idefs + procs[id].n_global_defs + procs[id].n_global_idefs +
                procs[id].n_cdefs)
                ? "??"
                : "OK",
            procs[id].name);
    }
    return 0;
}

