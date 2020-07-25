# include <stdio.h>
# include <stdlib.h>

	static  char    sccsid[] = "@(#)visit-post.c	1.2  8/16/95";
	/*
		force proc entry and exit to be first and last
	*/
	int		first,last,nodes[1000000],i,entry_at,exit_at;

int main(np,p)
	int		np;
	char	*p[];
{
	int		k,n,entry_seen;
	char	buff[2000];

	if (np != 3){
		fprintf (stderr,"%s: usage visit-post first-node last-node\n",
			p[0]);
		exit(1);
	}

	first = atoi(p[1]);
	last = atoi(p[2]);
	i = 0;
	while (gets(buff)){
		k = sscanf(buff,"%d",&nodes[i]);
		if (k == 1) i++;
	}
	n = i;
	entry_seen = 0;
	for (i = 0; i < n; i++){
		if (first == nodes[i]){
			entry_seen = 1;
		}
		if (entry_seen){
			if (nodes[i] != last)printf ("%d\n",nodes[i]);
		}
	}
	entry_seen = 0;
	for (i = 0; i < n; i++){
		if (first == nodes[i]){
			break;
		}
		if (nodes[i] != last)printf ("%d\n",nodes[i]);
	}
	printf ("%d\n",last);
	return 0;
}
