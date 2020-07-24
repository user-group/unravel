# include <stdio.h>
# include "sets.h"
# include "slice.h"
# include "lif.h"
# include <malloc.h>
# include <string.h>

extern int v_opt,n_ptrs;
main(np,p)
	int		np;
	char	*p[];
{
	int		status;
	int		i,fx;

	if (np < 2) exit(1);
	fx = 0;
	for (i = 1; i < np; i++){
		if (p[i][0] == '-'){
			if (strcmp("-v",p[i]) == 0)v_opt = 1;
		}
		else fx = i;
	}
	i = strlen(p[fx]);
	p[fx][i-1] = '\0';
	status = read_k_file(p[fx]);
	printf ("%s: K file status %d\n",p[fx],status);
	status = read_link_file(p[fx]);
	printf ("%s: LINK file status %d\n",p[fx],status);
}
