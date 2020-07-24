# include <stdio.h>
# include "ansi_parse.h"
# include "lif.h"
# include <string.h>
# include "control.h"
	static  char    sccsid[] = "@(#)parser.c	1.9  8/16/95";

	int	yydebug;
print_options(program_name)
	char	*program_name;
{
	printf ("Usage: %s options [input_source_file]\n",program_name);
	printf ("Options:\n");
	printf ("-h       Print this list\n");
	printf ("-o file  Send output to \"file\"\n");
	printf ("-f proc  \"proc\" is mem alloc fun (like \"malloc\")\n");
	printf ("-a       no default mem alloc functions\n");
	printf ("-d       Parser (yacc) debugging\n");
	printf ("-k       Token debugging\n");
	printf ("-c       Decl debugging\n");
	printf ("-x       xpr debugging\n");
	printf ("-m       memory usage summary\n");
	printf ("-s       print symbol table\n");
	printf ("-z       LIF details\n");
}

int main (np,parm)
	int	np;
	char	*parm[];
{
	int	i;
	char	cmd[2000],buff[2000];
	int		status;
	FILE	*open_ok;
	FILE	*t_file = NULL;
	int		file_open = 0;
	char	*dot_at;
	int		out_file_parm;

	progname = parm[0];
	outfile = stdout;
	ctrl_file = NULL;
	for (i=1; i < np; i++){
		if ((parm[i][0] == '-') && (strlen(parm[i]) == 2))
			switch (parm[i][1]){
		case 'h': print_options(progname);
			return(0);
			break;
		case 'd': yydebug = 1;
			printf ("yydebug on\n");
			break;
		case 'n': n_opt = 1;
			break;
		case 'c': c_opt = 1;
			break;
		case 's': s_opt = 1;
			break;
		case 'k': k_opt = 1;
			break;
		case 'p': p_opt = 1;
			break;
		case 'i': i_opt = 1;
			break;
		case 'f': if (i == np - 1) fprintf (stderr,
				"%s: no proc name after -f\n",progname);
			  else add_malloc(parm[++i]);
			break;
		case 'a': a_opt = 1;
			break;
		case 'm': m_opt = 1;
			break;
		case 'x': x_opt = 1;
			break;
		case 'z': z_opt = 1;
			break;
		case 'o': if (i == np - 1) fprintf (stderr,
				"%s: no file after -o\n",progname);
			  else {o_opt = 1;
			  outfile = fopen(parm[++i],"w");
			  strcpy (buff,parm[i]);
			  out_file_parm = i;
			  dot_at = strrchr (buff,'.');
			  if(dot_at)strcpy (dot_at,".H");
			  else strcat (buff,".H");
			  h_file = fopen(buff,"w");
			  open_control(buff);
			  }
			break;
		default: fprintf(stderr,"%s: Unknown option: %s\n",
			progname,parm[i]);
		}
		else { /* not an option since no dash ("-"),
				  assume input file name				*/
			if (file_open){  /* only one input file allowed */
				fprintf(stderr,
					"%s: Cannot open %s since %s is already open.  ",
					progname,parm[i],parm[file_open]);
				fprintf (stderr,"Only one input file allowed\n");
				return (1);
			}
			else {
				open_ok = freopen(parm[i],"r",stdin);
				file_open = i;
				if (!open_ok){
					fprintf (stderr,"%s: Cannot open %s as input\n",
						progname,parm[i]);
					return(2);
				}
			}
		}
	}
	/* if no H file requested, send to /dev/null */
	if (h_file == NULL) h_file = fopen("/dev/null","w");
	if (!a_opt){
		add_malloc("malloc");
	}
	if(x_opt)print_malloc();
	if (!outfile){
		fprintf (stderr,"Parser could not open LIF file\n");
		fprintf (stderr,"Check directory for write permission or\n");
		fprintf (stderr,
			"for existing LIF file without write permission\n");
		exit(1);
	}
	status = yyparse();
	if (o_opt && (!status)){
	  buff[strlen(buff) - 1] = 'T';
	  t_file = fopen(buff,"w");
	  if(t_file){
		output_proc_list(t_file);
		fprintf (t_file,"%d %d %d\n", current_global(),
			current_chain(),current_addr());
		fclose(t_file);
	  }
	  /*	this error is a nuance
	  else {
		fprintf (stderr,"%s: failed to create .T file\n",progname);
		return 4;
	  }
	  */
	}
	if (status) close_scope();
	if(s_opt) print_st();
	if(m_opt) alloc_summary();
	if (o_opt && status){
		unlink(parm[out_file_parm]);
	 	buff[strlen(buff) - 1] = 'H';
		unlink(buff);
	}
	return (status);
}
