/*
*******************************************************************
*                                                                 *
*   Unravel sysgen map                                            *
*                                                                 *
*   Jim Lyle  25 Jan 1994                                         *
*                                                                 *
*******************************************************************
*/

#include <dirent.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
static char sccsid[] = "@(#)map.c	1.6  8/11/95";

int v_opt = 0;

typedef struct file_struct file_rec, *file_ptr;
typedef struct proc_struct proc_rec, *proc_ptr;
typedef struct main_struct main_rec, *main_ptr;
typedef struct proc_list_struct proc_list_rec, *proc_list;
typedef struct file_list_struct file_list_rec, *file_list;

struct file_list_struct {
    file_list next;
    file_ptr file;
};

struct proc_list_struct {
    proc_list next;
    proc_ptr proc;
};

struct file_struct {
    int n_procs;
    char *name;
    proc_list procs;       /* procs defined (with body) */
    proc_list not_defined; /* called only				 */
    file_ptr next;
};

struct proc_struct {
    int proc_id, entry;
    char *name;
    char s_or_x; /* static or extern			 */
    file_ptr in_file;
    proc_ptr same_name;
    proc_ptr next;
};

struct main_struct {
    file_list files;
    file_ptr main_file;
    proc_list done;
    proc_list ambigous_procs;
    proc_list lib_procs;
    main_ptr next;
};

proc_list proc_entry(proc) proc_ptr proc;
{
    proc_list new;

    new       = (proc_list)malloc(sizeof(proc_list_rec));
    new->next = NULL;
    new->proc = proc;
    return new;
}

file_list file_entry(file) file_ptr file;
{
    file_list new;

    new       = (file_list)malloc(sizeof(file_list_rec));
    new->next = NULL;
    new->file = file;
    return new;
}

file_ptr find_file(list, file) file_ptr list, file;
{
    file_ptr at;

    at = list;
    while (at) {
        if (strcmp(at->name, file->name) == 0)
            return at;
        at = at->next;
    }
    return NULL;
}

file_list find_file_list(list, file) file_ptr file;
file_list list;
{
    file_list at;

    at = list;
    while (at) {
        if (strcmp(at->file->name, file->name) == 0)
            return at;
        at = at->next;
    }
    return NULL;
}

proc_ptr find_proc_list(list, proc) proc_ptr proc;
proc_list list;
{
    proc_list at;

    at = list;
    while (at) {
        if (strcmp(at->proc->name, proc->name) == 0)
            return at->proc;
        at = at->next;
    }
    return NULL;
}

proc_ptr find_proc(list, proc) proc_ptr list, proc;
{
    proc_ptr at;

    at = list;
    while (at) {
        if (strcmp(at->name, proc->name) == 0)
            return at;
        at = at->next;
    }
    return NULL;
}

void match_procs_to_files(mp, fp, pp) main_ptr mp;
file_ptr fp;
proc_ptr pp;
{
    main_ptr m;
    file_ptr f;
    file_list add_f;
    proc_ptr p, found;
    proc_list not_list;
    proc_list new_proc_entry, p_list;
    file_list files, new_file_entry;

    m = mp;
    while (m) {
        if (v_opt)
            printf("\nmatch main %s\n", m->main_file->name);
        m->done  = NULL;
        m->files = file_entry(m->main_file);
        files    = m->files;
        while (files) {
            f = files->file;
            if (v_opt)
                printf("File: %s\n", f->name);
            not_list = f->not_defined;
            p_list   = f->procs;
            while (p_list) {
                if (v_opt)
                    printf("\tAdd proc %s\n", p_list->proc->name);
                found = find_proc_list(m->done, p_list->proc);
                if (found) {
                    if (v_opt)
                        printf("\t\tfound %s in %s\n", found->name, f->name);
                    found = find_proc(pp, found);
                    if (f != m->main_file)
                        if (found->same_name) {
                            if (v_opt)
                                printf("\t\tambig %s in %s\n", found->name, f->name);
                            if (!find_proc_list(m->ambigous_procs, found)) {
                                new_proc_entry       = proc_entry(/*p*/ found);
                                new_proc_entry->next = m->ambigous_procs;
                                m->ambigous_procs    = new_proc_entry;
                            }
                        }
                } else {
                    found = find_proc(pp, p_list->proc);
                    if ((f != m->main_file) && (found->same_name)) {
                        if (v_opt)
                            printf("\t\tambig %s in %s\n", found->name, f->name);
                        if (!find_proc_list(m->ambigous_procs, found)) {
                            new_proc_entry       = proc_entry(/*p*/ found);
                            new_proc_entry->next = m->ambigous_procs;
                            m->ambigous_procs    = new_proc_entry;
                        }
                    } else {
                        new_proc_entry       = proc_entry(p_list->proc);
                        new_proc_entry->next = m->done;
                        m->done              = new_proc_entry;
                    }
                }
                p_list = p_list->next;
            }
            while (not_list) {
                p     = not_list->proc;
                found = find_proc_list(m->done, p);
                if (!found) {
                    new_proc_entry       = proc_entry(p);
                    new_proc_entry->next = m->done;
                    m->done              = new_proc_entry;
                    found                = find_proc(pp, p);
                    if (found) {
                        if (found->same_name) {
                            new_proc_entry       = proc_entry(/*p*/ found);
                            new_proc_entry->next = m->ambigous_procs;
                            m->ambigous_procs    = new_proc_entry;
                        } else if (!find_file_list(m->files, found->in_file)) {
                            new_file_entry = file_entry(found->in_file);
                            add_f          = files;
                            while (add_f->next)
                                add_f = add_f->next;
                            add_f->next = new_file_entry;
                            /*
                            p_list = found->in_file->procs;
                            while (p_list){
                                    new_proc_entry =
                                            proc_entry (p_list->proc);
                                    new_proc_entry -> next = m -> done;
                                    m -> done = new_proc_entry;
                                    p_list = p_list -> next;
                            }
                            */
                        }
                    } else {
                        new_proc_entry       = proc_entry(p);
                        new_proc_entry->next = m->lib_procs;
                        m->lib_procs         = new_proc_entry;
                    }
                }
                not_list = not_list->next;
            }
            files = files->next;
        }
        m = m->next;
    }
}

void print_match_results(mp) main_ptr mp;
{
    file_list need_files;
    proc_list p;
    proc_ptr ps, same;
    FILE *sys;
    int n;
    char buff[2000];
    int col;

    sys = stdout;
    while (mp) {
        col        = fprintf(sys, "Main program file %s requires", mp->main_file->name);
        need_files = mp->files;
        n          = 0;
        while (need_files) {
            n++;
            need_files = need_files->next;
        }
        col += fprintf(sys, " %d files: ", n);
        need_files = mp->files;
        while (need_files) {
            if (col > 60) {
                fprintf(sys, "\n");
                col = 0;
            }
            col += fprintf(sys, "%s ", need_files->file->name);
            need_files = need_files->next;
        }
        p = mp->ambigous_procs;
        if (p)
            fprintf(sys, "\nThe following procedures are defined in more than one file:\n");
        else
            fprintf(sys, "\n");
        while (p) {
            fprintf(sys, "\t%s ", p->proc->name);
            same = p->proc->same_name;
            col  = fprintf(sys, "%s ", p->proc->in_file->name);
            while (same) {
                col += fprintf(sys, "%s ", same->in_file->name);
                same = same->same_name;
                if (same && col + strlen(same->in_file->name) > 60) {
                    fprintf(sys, "...");
                    same = NULL;
                }
            }
            fprintf(sys, "\n");
            p = p->next;
        }
        mp = mp->next;
    }
    fclose(sys);
}

void output_match_results(dir_name, mp) main_ptr mp;
char *dir_name;
{
    file_list need_files;
    proc_list p;
    proc_ptr ps, same;
    FILE *sys;
    int n;
    char buff[2000];
    int col;

    sprintf(buff, "%s/SYSTEM", dir_name);
    sys = fopen(buff, "w");
    if (!sys) {
        fprintf(stderr, "Could not open 'SYSTEM' file\n");
        fprintf(stderr, "check current directory write permissions,\n");
        fprintf(stderr, "\t\tls -ld .\n");
        fprintf(stderr, "current directory not being writable or ");
        fprintf(stderr, "existing SYSTEM file with\n");
        fprintf(stderr, "no write permission ");
        fprintf(stderr, "is most likely cause of write failure\n");
        exit(1);
    }
    while (mp) {
        fprintf(sys, "MAIN %s", mp->main_file->name);
        need_files = mp->files;
        n          = 0;
        while (need_files) {
            n++;
            need_files = need_files->next;
        }
        fprintf(sys, " %d\n", n);
        fprintf(sys, "FILES\n");
        need_files = mp->files;
        while (need_files) {
            fprintf(sys, "\t%s\n", need_files->file->name);
            need_files = need_files->next;
        }
        fprintf(sys, "AMBIG\n");
        p = mp->ambigous_procs;
        while (p) {
            fprintf(sys, "\t%s ", p->proc->name);
            same = p->proc->same_name;
            col  = fprintf(sys, "%s ", p->proc->in_file->name);
            while (same) {
                col += fprintf(sys, "%s ", same->in_file->name);
                same = same->same_name;
                if (same && col + strlen(same->in_file->name) > 60) {
                    fprintf(sys, "...");
                    same = NULL;
                }
            }
            fprintf(sys, "\n");
            p = p->next;
        }
        fprintf(sys, "LIB\n");
        p = mp->lib_procs;
        while (p) {
            fprintf(sys, "\t%s\n", p->proc->name);
            p = p->next;
        }
        mp = mp->next;
    }
    fclose(sys);
}

/*
print_match_results (mp)
        main_ptr	mp;
{
        file_list	need_files;
        proc_list	p;

        while (mp){
                printf ("\n\nmatching for main in %s\n",mp->main_file->name);
                printf ("AMBIG\n");
                p = mp->ambigous_procs;
                while(p){
                        printf ("\t%s\n",p->proc->name);
                        p = p -> next;
                }
                printf ("LIB\n");
                p = mp->lib_procs;
                while(p){
                        printf ("\t%s\n",p->proc->name);
                        p = p -> next;
                }
                printf ("FILES\n");
                need_files = mp->files;
                while(need_files){
                        printf ("\t%s\n",need_files->file->name);
                        need_files = need_files -> next;
                }
                mp = mp -> next;
        }
}
*/

int file_selector(entry) struct dirent *entry;
{
    int ix;

    ix = strlen(entry->d_name) - 2;

    if (ix > 0)
        if ((entry->d_name[ix] == '.') && (entry->d_name[ix + 1] == 'T')) {
            return 1;
        }
    return 0;
}

int compare(a, b) struct dirent **a, **b;
{
    if (strcmp((*a)->d_name, (*b)->d_name) > 0)
        return 1;
    return 0;
}

int myscandir(dir_name, list, select, compare) char *dir_name;
struct dirent ***list;
int (*select)();
int (*compare)();
{
    DIR *dir;
    struct dirent *entry;
    typedef struct l_struct ll_node, *ll;
    struct l_struct {
        ll next;
        struct dirent *e;
    };

    int i, n_match = 0;
    ll new_ll;
    struct dirent *new_de;
    ll nodes, node_list = NULL;

    dir = opendir(dir_name);
    if (!dir) {
        fprintf(stderr, "directory open failed for %s\n", dir_name);
        exit(1);
    }
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (select(entry)) {
            new_ll       = (ll)malloc(sizeof(ll_node));
            new_ll->next = node_list;
            node_list    = new_ll;
            new_ll->e = (struct dirent *)malloc(sizeof(struct dirent) + strlen(entry->d_name) + 3);
            strcpy(new_ll->e->d_name, entry->d_name);
            n_match++;
        }
    }
    *list = (struct dirent **)malloc((n_match + 1) * sizeof(struct dirent *));
    nodes = node_list;
    for (i = 0; i < n_match; i++) {
        (*list)[i] = nodes->e;
        nodes      = nodes->next;
    }
    qsort(*list, n_match, sizeof(struct dirent *), compare);
    return n_match;
}

int scan_dir(dir, f) struct dirent ***f;
char *dir;
{
    struct dirent **files;
    int count;
    int alphasort();
    int i;

    count = myscandir(dir, &files, file_selector, compare);
    if (count == -1) {
        fprintf(
            stderr, "map: could not open %s\n", strcmp(dir, ".") == 0 ? "current directory" : dir);
        exit(1);
    }
    files[count] = NULL;
    *f           = files;
    return count;
}

int main(np, p) int np;
char *p[];
{
    /* struct direct 	**files; */
    struct dirent **files;
    int n_files, i, j, length;
    char *dir_name;
    FILE *t_file;
    char buff[3000];
    proc_ptr same;

    file_ptr fp = NULL, new_fp;
    proc_ptr pp = NULL, new_pp;
    main_ptr mp = NULL, new_main;
    proc_list new_proc_list;
    proc_ptr new_proc;

    int n_procs = 0, n_mains = 0;
    int fx;

    if (np > 3) {
        fprintf(stderr, "map: too many parameters (%d)\n", np - 1);
        fprintf(stderr, "%s usage: %s [-v] [directory_name]\n", "map", "map");
        exit(1);
    }
    files = NULL;
    fx    = 0;
    for (i = 1; i < np; i++) {
        if (p[i][0] == '-') {
            if (strcmp("-v", p[i]) == 0)
                v_opt = 1;
            else {
                fprintf(stderr, "%s: option (%s) is not valid\n", "map", p[i]);
                fprintf(stderr, "%s usage: %s [-v] [directory_name]\n", "map", "map");
                exit(1);
            }
        } else if (fx == 0)
            fx = i;
        else {
            fprintf(stderr, "map: only one directory name allowed\n");
            fprintf(stderr, "%s usage: %s [-v] [directory_name]\n", "map", "map");
            exit(1);
        }
    }
    if (fx != 0)
        dir_name = p[fx];
    else
        dir_name = ".";
    n_files = scan_dir(dir_name, &files);
    for (i = 0; i < n_files; i++) {
        sprintf(buff, "%s/%s", dir_name, files[i]->d_name);
        t_file = fopen(buff, "r");
        if (t_file == NULL) {
            fprintf(stderr, "map: could not open file %s\n", files[i]->d_name);
            return 1;
        }

        fscanf(t_file, "%d %*d", &n_procs);
        new_fp                   = (file_ptr)malloc(sizeof(file_rec));
        new_fp->next             = fp;
        new_fp->name             = files[i]->d_name;
        length                   = strlen(new_fp->name);
        new_fp->name[length - 1] = 'c';
        new_fp->n_procs          = n_procs;
        new_fp->procs            = NULL;
        fp                       = new_fp;
        if (v_opt)
            printf("File %s (%d procs)\n", files[i]->d_name, n_procs);
        for (j = 0; j < n_procs; j++) {
            new_pp              = (proc_ptr)malloc(sizeof(proc_rec));
            new_pp->in_file     = new_fp;
            new_pp->same_name   = NULL;
            new_proc_list       = (proc_list)malloc(sizeof(proc_list_rec));
            new_proc_list->proc = new_pp;
            fscanf(t_file,
                "%d %d %*d %*d %c %s",
                &new_pp->proc_id,
                &new_pp->entry,
                &new_pp->s_or_x,
                buff);
            new_pp->name = strcpy(malloc(1 + strlen(buff)), buff);

            if (v_opt)
                printf("\tproc %s %s ", buff, new_pp->s_or_x == 'S' ? "static" : "extern");
            if (strcmp("main", buff) == 0) {
                new_main                 = (main_ptr)malloc(sizeof(main_rec));
                new_main->next           = mp;
                mp                       = new_main;
                new_main->main_file      = new_fp;
                new_main->files          = NULL;
                new_main->ambigous_procs = NULL;
                new_main->lib_procs      = NULL;
            }

            if (new_pp->entry == -1) { /* not defined */
                new_proc_list->next = new_fp->not_defined;
                new_fp->not_defined = new_proc_list;
                if (v_opt)
                    printf("called, but not defined");
            } else if (new_pp->s_or_x != 'S') {
                new_proc_list->next = new_fp->procs;
                new_fp->procs       = new_proc_list;
                same                = find_proc(pp, new_pp);
                if (same) {
                    if (v_opt)
                        printf("overloaded name");
                    while (same->same_name)
                        same = same->same_name;
                    same->same_name = new_pp;
                } else {
                    new_pp->next = pp;
                    pp           = new_pp;
                }
            }
            if (v_opt)
                printf("\n");
        }
        fclose(t_file);
    }
    if (v_opt) {
        new_fp = fp;
        printf("\nFiles:\n");
        while (new_fp) {
            new_proc_list = new_fp->procs;
            printf("File %s (%d procs)\n", new_fp->name, new_fp->n_procs);
            if (new_proc_list)
                printf("\tDefined procs:\n");
            while (new_proc_list) {
                new_proc = new_proc_list->proc;
                printf("\t%20.20s %c %c\n",
                    new_proc->name,
                    new_proc->entry == -1 ? 'C' : 'D',
                    new_proc->s_or_x);
                new_proc_list = new_proc_list->next;
            }
            new_proc_list = new_fp->not_defined;
            if (new_proc_list)
                printf("\tCalled but not defined procs:\n");
            while (new_proc_list) {
                new_proc = new_proc_list->proc;
                printf("\t%20.20s %c %c\n",
                    new_proc->name,
                    new_proc->entry == -1 ? 'C' : 'D',
                    new_proc->s_or_x);
                new_proc_list = new_proc_list->next;
            }
            new_fp = new_fp->next;
        }
        new_pp = pp;
        printf("\nProcs:\n");
        while (new_pp) {
            new_proc = new_pp;
            printf("\t%s:", new_proc->name);
            printf(" (%s)", new_proc->in_file->name);
            if (new_pp->same_name) {
                new_proc = new_pp->same_name;
                while (new_proc) {
                    printf(" %s", new_proc->in_file->name);
                    new_proc = new_proc->same_name;
                }
            }
            printf("\n");
            new_pp = new_pp->next;
        }
        new_main = mp;
        printf("\nMain procs:\n");
        while (new_main) {
            new_main = new_main->next;
        }
    }
    match_procs_to_files(mp, fp, pp);
    print_match_results(mp);
    output_match_results(dir_name, mp);
    return 0;
}
