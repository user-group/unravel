# include <stdio.h>
# include <stdlib.h>

static  char    sccsid[] = "@(#)visit-filter.c	1.1  10/6/94";

int main(np,p)
	int		np;
	char	*p[];
{
	int		low,high,from,to;

	if (np != 3) exit(1);
	low = atoi(p[1]);
	high = atoi(p[2]);
	while (EOF != scanf("%d%d",&from,&to)){
		if ((from >= low) &&
			(from <= high) &&
			(to >= low) &&
			(to <= high)) printf("%d %d\n",from,to);
	}
	return 0;
}

