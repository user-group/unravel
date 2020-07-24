# include <stdio.h>
# include <sys/time.h>
# include <string.h>
# include <stdlib.h>
# include "sets.h"
# include "slice.h"

# define DEPTH 100

static  char    sccsid[] = "%W%  %G%";
static	bit_set	*slices,active;
int		lib_opt = 0;

extern	int v_opt;
	int n_opt;
	int l_opt;
	int g_opt;
	int s_opt;

extern int read_k_file(char *name);
extern int read_link_file (char *name);

int 
on_list (level,callers,called)
	int	level,callers[],called;
{
	int	i;

	for (i = 0; i < level; i++)
		if (callers[i] == called) return 1;
	return 0;
}

int 
is_recursive_call (level,callers,called)
	int	level,callers[],called;
{
	return on_list(level,callers,called);
}

void
tree_out (level,callers,calls)
	int	level,callers[];
	call_ptr	calls;
{
	char	tabs[DEPTH];
	int		i,called;
	int		list[10000],n_list;

	if (level >= DEPTH) return;
	n_list = 0;
	strcpy(tabs,"");
	for (i = 0; i < level; i++)
		strcat (tabs,"\t");
	while (calls){
		called = calls->pid;
		if (!on_list(n_list,list,called)){
			if ((procs[called].entry != -1) || lib_opt){
				printf("%s", tabs);
				printf("%s\n",procs[called].proc_name);
			}
			if (!is_recursive_call (level,callers,called)){
				callers[level] = called;
				tree_out (level+1,callers,procs[called].calls);
			}
			list[n_list++] = called;
		}
		calls = calls -> next;
	}
}

int
main(np,p)
	int		np;
	char	*p[];
{
	int		status;
	int		i,fx;
	int		proc;
	int		callers[DEPTH];

	v_opt = 0; l_opt = 0; g_opt = 0; s_opt = 0; n_opt = 0;
	if (np < 2) exit(1);
	fx = 0;
	for (i = 1; i < np; i++){
		if (p[i][0] == '-'){
			if (strcmp("-v",p[i]) == 0)v_opt = 1;
			else if (strcmp("-n",p[i]) == 0)n_opt = 1;
			else if (strcmp("-l",p[i]) == 0)lib_opt = 1;
			else if (strcmp("-g",p[i]) == 0)g_opt = 1;
			else if (strcmp("-s",p[i]) == 0)s_opt = 1;
		}
		else fx = i;
	}
	i = strlen(p[fx]);
	p[fx][i-1] = '\0';
	status = read_k_file(p[fx]);
	if(status)printf ("%sc: K file status %d\n",p[fx],status);
	status = read_link_file(p[fx]);
	if(status)printf ("%sc: LINK file status %d\n",p[fx],status);

	for (proc = 1; proc <= n_procs; proc++){
		if (!procs[proc].called_by){
			printf ("%s\n",procs[proc].proc_name);
			callers[0] = proc;
			tree_out (1,callers,procs[proc].calls);
		}
	}
}
