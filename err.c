# include <stdio.h>
# include "ansi_parse.h"
static  char    sccsid[] = "@(#)err.c	1.1  1/27/94";



void
warning (s,t)
	char	*s, *t;
{
	/*
	extern	int	lineno;
	extern	char	*progname;
	extern	char	the_current_file[];
	*/

	fprintf (stderr,"%s: %s",progname,s);
	if (t) fprintf (stderr," %s",t);
	fprintf (stderr," near line %d in file %s\n",lineno,
		the_current_file);
}

void
yyerror (s)
	char	*s;
{
	warning (s, (char *)0);
}

