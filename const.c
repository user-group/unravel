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

#include "ansi_parse.h"
#include <stdio.h>

/*
static char sccsid[] = "@(#)const.c	1.2  8/14/95";
*/

int n_opt = 0, c_opt = 0, p_opt = 0, i_opt = 0, a_opt = 0, k_opt = 0, o_opt = 0, x_opt = 0,
    z_opt = 0, m_opt = 0, s_opt = 0, lineno = 1, stmtno = 0, current_column = 1, global_scope = 1,
    in_local_decl = 0, in_union = 0, in_parms = 0, n_returns = 0, proc_returns[5000], id_max = 0;

FILE *outfile = NULL, *h_file = NULL;
char *progname;
char the_current_file[2000];
proc_ptr current_proc = NULL;
