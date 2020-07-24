# include <stdio.h>
# include "control.h"

static char *sccs_h_id = CONTROL_SCCS_ID;
static char *sccs_id = "@(#)control.c	1.1  8/10/95";

control (code,ctrl_entry,ctrl_exit,stmt_entry,stmt_exit,else_entry,
	else_exit)
	int  code,ctrl_entry,ctrl_exit,stmt_entry,stmt_exit,else_entry,
		else_exit;
{

	if (!ctrl_file) return;
	if (code == IF_THEN_ELSE_CTRL) fprintf (ctrl_file,
		"%d(%d,%d,%d,%d,%d,%d)\n",
		code,ctrl_entry,ctrl_exit,stmt_entry,stmt_exit,
		else_entry,else_exit);
	else fprintf (ctrl_file,"%d(%d,%d,%d,%d)\n",
		code,ctrl_entry,ctrl_exit,stmt_entry,stmt_exit);
}

open_control (base)
	char	*base;
{
	char	name[2000];

	if (!base) return;
	if (strlen(base) == 0) return;
	strcpy (name,base);
	name[strlen(base) - 1] = 'O';
	/*  Not yet implemented 
	ctrl_file = fopen(name,"w");
	*/
	ctrl_file = NULL;

}
