#include "ansi_parse.h"
#include <stdio.h>
//static char sccsid[] = "@(#)const.c	1.2  8/14/95";

int n_opt = 0, c_opt = 0, p_opt = 0, i_opt = 0, a_opt = 0, k_opt = 0, o_opt = 0, x_opt = 0,
    z_opt = 0, m_opt = 0, s_opt = 0, lineno = 1, stmtno = 0, current_column = 1, global_scope = 1,
    in_local_decl = 0, in_union = 0, in_parms = 0, n_returns = 0, proc_returns[5000], id_max = 0;

FILE *outfile = NULL, *h_file = NULL;
char *progname;
char the_current_file[2000];
proc_ptr current_proc = NULL;
