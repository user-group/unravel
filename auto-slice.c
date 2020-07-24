# include <stdio.h>
# include <sys/time.h>
# include "sets.h"
# include "slice.h"
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>


static  char    sccsid[] = "@(#)auto-slice.c	1.4  9/23/94";
static	bit_set	*slices,active;

extern	int		v_opt;
	int			n_opt;
	int			l_opt,g_opt,s_opt;

void 
time_out(sig, code, scp, addr)
	int sig, code;
	struct sigcontext *scp;
	char *addr;
{
	abandon_slice = 1;
}


int 
find_main()
{
	int		p;
	for (p = 1; p <= n_procs; p++){
		if (strcmp(procs[p].proc_name,"main") == 0)
			return p;
	}
	return 0;
}

float	f_stmts = 0.0;
int		hours = 0,mins = 0;
unsigned int max_time;

int
main(np,p)
	int		np;
	char	*p[];
{
	int		status;
	int		i,fx;
	int		file,stmt,proc,var;
	char	message[1000],cmd[2000];
	int		line_number;
	int		is_err;
	int		t_locals = 0;
	int		t_stmts = 0;

	signal (SIGALRM,time_out);
	v_opt = 0; l_opt = 0; g_opt = 0; s_opt = 0; n_opt = 0;
	if (np < 2) exit(1);
	fx = 0;
	for (i = 1; i < np; i++){
		if (p[i][0] == '-'){
			if (strcmp("-v",p[i]) == 0)v_opt = 1;
			else if (strcmp("-n",p[i]) == 0)n_opt = 1;
			else if (strcmp("-l",p[i]) == 0)l_opt = 1;
			else if (strcmp("-g",p[i]) == 0)g_opt = 1;
			else if (strcmp("-s",p[i]) == 0)s_opt = 1;
			else if (strcmp("-h",p[i]) == 0){
				i++;
				if (i < np) hours = atoi(p[i]);
			}
			else if (strcmp("-m",p[i]) == 0){
				i++;
				if (i < np) mins = atoi(p[i]);
			}
		}
		else fx = i;
	}
	max_time = 60*(60*hours + mins);
	printf ("Max time is %d\n",max_time);
	i = strlen(p[fx]);
	p[fx][i-1] = '\0';
	status = read_k_file(p[fx]);
	if(status)printf ("%sc: K file status %d\n",p[fx],status);
	status = read_link_file(p[fx]);
	if(status)printf ("%sc: LINK file status %d\n",p[fx],status);
	slices = (bit_set *) malloc (n_files*sizeof(bit_set));
	for (i = 0; i < n_files; i++)
		slices[i] = create_bit_set (1+files[i].n_stmts);
	active = create_bit_set (1+n_procs);

	for (proc = 1; proc <= n_procs; proc++)
		t_locals += procs[proc].n_locals;
	for (file = 0; file < n_files; file++)
		t_stmts += files[file].n_stmts;
	f_stmts = t_stmts;
	sprintf (message,"%sc: %d globals  %d locals  %d stmts",
		p[fx],n_globals,t_locals,t_stmts);
	printf ("%s\n",message);
	proc = find_main();
	if (!proc){
		fprintf (stderr,"no main procedure in %sc\n",p[fx]);
		exit(1);
	}
	file = procs[proc].file_id;
	stmt = procs[proc].exit;
	system ("date >AUTO");
	sprintf (cmd,"echo \"%s\" >>AUTO",message);
	system (cmd);
	if (g_opt)for (var = 1; var <= n_globals; var++){
		verify_criterion(file,stmt,0,var);
		do_slice (file,stmt,0,var);
	}
	if (l_opt)for (proc = 1; proc <= n_procs; proc++){
		for (var = 1; var <= procs[proc].n_locals; var++){
			file = procs[proc].file_id;
			stmt = procs[proc].exit;
			verify_criterion(file,stmt,proc,var);
			do_slice (file,stmt,proc,var);
		}
	}
	system ("date >>AUTO");
}

void
verify_criterion(file,stmt,proc,var)
	int	file,stmt,proc,var;
{
	if ((file < 0) || (file >= n_files)){
		fprintf (stderr,"file %d is out of range [0..%d]",file,
			n_files-1);
		exit(1);
	}
	if ((proc < 0) || (proc > n_procs)){
		fprintf (stderr,"proc %d is out of range [1..%d]",proc,
			n_procs);
		exit(1);
	}
	if (proc){
		if ((var <= 0) || (var > procs[proc].n_locals)){
			fprintf (stderr,"local var %d is out of range [1..%d]",var,
				procs[proc].n_locals);
			exit(1);
		}
	}
	else {
		if ((var <= 0) || (var > n_globals)){
			fprintf (stderr,"global var %d is out of range [1..%d]",var,
				n_globals);
			exit(1);
		}
	}
	if ((stmt < 0) || (stmt > files[file].n_stmts)){
		fprintf (stderr,"stmt %d is out of range [1..%d]",stmt,
			files[file].n_stmts);
		exit(1);
	}
}

void
do_slice(file,stmt,proc,var)
	int	file,stmt,proc,var;
{
	int		f,i,at,status;
	int		stmt_proc;
	int		size = 0;
	time_t	start,finish;
	int		elapsed;
	char		cmd[1000];
	float	pcent;

	clear_active();
	for (i = 1; i <= n_procs; i++){
		if (procs[i].file_id == file)
			if ((stmt >= procs[i].entry) &&
				(stmt <= procs[i].exit)){
					stmt_proc = i;
					break;
				}
	}
	if ((stmt_proc < 1) || (stmt_proc > n_procs)){
		fprintf (stderr,"stmt %d not found for file %s\n",
		stmt,files[file].name);
		return;
	}
	set_criteria (file,stmt_proc,stmt,proc,var);
	/*
	printf ("%s%s@%d %s'%s:\n",files[file].name,
		procs[stmt_proc].proc_name,stmt,
		proc?procs[proc].proc_name:"global",
		proc? procs[proc].locals[var].name:
		globals[var].name);
		*/
	sprintf (cmd,
	"echo -n \"slice on %s in %s at line %d in %sc\" >>AUTO",
		var_name (proc,var),proc?procs[proc].proc_name:"global",
		files[file].stmts[stmt].froml,
		files[file].name);
	system(cmd);
	if (max_time) alarm(max_time);
	start = time(NULL);
	slice (file,stmt_proc,stmt,proc,var,slices,active);
	finish = time(NULL);
	elapsed = (int)(finish - start);
	if (!abandon_slice && max_time) alarm(0);
	if (elapsed <= 0) elapsed = 1;
	for (f = 0; f < n_files; f++){
		at = -1;
		while ((at = get_next_member(slices[f],at)) >= 0){
			size++;
		}
	}
	pcent = (100.00*size)/f_stmts;
	sprintf (cmd,
		"echo \"  %d (%.1f)   %d (%d:%d:%d)%c\" >>AUTO",
		size, pcent,elapsed,elapsed/3600,(elapsed/60)%60,elapsed%60,
		abandon_slice?'A':' ');
	system(cmd);
	printf ("slice on %s in %s at line %d in %sc %d (%.1f)   %d (%d:%d:%d)%c\n",
		var_name (proc,var),proc?procs[proc].proc_name:"global",
		files[file].stmts[stmt].froml,
		files[file].name,size,pcent,
		elapsed,elapsed/3600,(elapsed/60)%60,elapsed%60,
		abandon_slice?'A':' ');
	if(s_opt)print_slices1 (n_opt,slices,active);
	if(v_opt)print_all_active (slices,active);
}
