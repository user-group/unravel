# include <stdio.h>
# include <string.h>
# include <malloc.h>
# include <stdlib.h>

# include "lif.h"
	static  char    sccsid[] = "@(#)tsummary.c	1.3  8/16/95";
int		v_opt = 0;

typedef struct local_struct local_rec,*local_ptr;
typedef struct addr_struct addr_rec,*addr_ptr;
typedef struct chain_struct chain_rec,*chain_ptr;
typedef struct field_struct field_rec,*field_ptr;
typedef struct global_struct global_rec,*global_ptr;
typedef struct file_struct file_rec,*file_ptr;
typedef struct proc_struct proc_rec,*proc_ptr;
typedef struct proc_list_struct proc_list_rec,*proc_list;
typedef struct h_struct h_rec,*h_ptr;
typedef struct var_struct var_rec,*var_ptr;

struct h_struct {
	char	*name;
	var_ptr vars;
	int		n_v;
	h_ptr	next;
};

struct var_struct {
	int		id;
	var_ptr	next;
};

struct local_struct {
	int		local,global;
	char	*text;
	char	*flags;
	local_ptr next;
};

struct addr_struct {
	int		addr,pid,id,new_id;
	int		new_addr,new_pid;
};

struct chain_struct {
	int		old,new,id,new_id;
	int		pid;
	int		done;
	field_ptr field;
};

struct field_struct {
	int			chain,fid;
	int			offset;
	char		*flags;
	field_ptr	next;
};

struct global_struct {
	int		new;
	char	*text;
	char	*flags;
	int		is_static;
};

struct proc_list_struct {
	proc_list	next;
	proc_ptr	proc;
};

struct file_struct {
	int			n_procs,n_stmts;
	int			n_globals,n_addrs,n_chains;
	int			n_lines,n_chars;
	char		*name;
	proc_ptr	procs;
	global_ptr	globals;
	chain_ptr	chains;
	addr_ptr	addrs;
};

struct proc_struct {
	int			proc_id,
				new_id,
				exit,
				n_locals,
				entry;
	char		*name;
	char		s_or_x;
	local_ptr	localx;
};

int		n_globals,n_procs,n_addrs,n_chains;

void
get_t_files (sys)
	char	 *sys;
{
	int		i,j,k,len;
	char	buff[2000],format[2000];
	FILE	*t_file;
	int		t_globals = 0;
	int		n_lib_proc = 0, n_def_proc = 0;
	proc_ptr	procs;
	int			n_procs,n_stmts;
	int			n_globals,n_addrs,n_chains;
	int			n_lines,n_chars;
	int			max_len;

		strcpy (buff,sys);
		len = strlen(buff);
		buff[len-1] = 'T';
		t_file = fopen (buff,"r");
		if (!t_file){
			fprintf (stderr,"tsummary: couldn't open %s\n",buff);
			exit(1);
		}
		else {
			fscanf (t_file,"%d %d",&n_procs,&n_stmts);
			procs = (proc_ptr)
				calloc (n_procs,sizeof(proc_rec));
			for (j = 0; j < n_procs; j++){
				fscanf (t_file,"%d %d %d %d %c %s",
					&procs[j].proc_id,
					&procs[j].entry,
					&procs[j].exit,
					&procs[j].n_locals,
					&procs[j].s_or_x,
					buff);
				if (procs[j].entry == -1) n_lib_proc++;
				else n_def_proc++;
				procs[j].name = malloc (strlen(buff) + 1);
				strcpy (procs[j].name,buff);
				procs[j].localx = NULL;
			}
			fscanf (t_file,"%d%d%d %d %*d %d",&n_globals,
				&n_chains,&n_addrs,
				&n_lines,&n_chars);
			fclose(t_file);
		}
	printf ("\n%s summary\n",sys);
	printf ("%4d %s",n_lines,"Source program lines\n");
	printf ("%4d %s",n_stmts,
		"Source program statements (flow graph nodes)\n");
	printf ("%4d %s",n_globals,"Global variables\n");
	printf ("%4d %s",n_chains,"Pointers to structure chains\n");
	printf ("%4d %s",n_addrs,
		"Addresses (variables referenced by pointer)\n");
	max_len = 0;
	for (j = 0; j < n_procs; j++){
		len = strlen (procs[j].name);
		if (len > max_len) max_len = len;
	}
	sprintf (format,"%%-%d.%ds   ",max_len,max_len);
	printf ("%4d %s",n_def_proc,"Procedures defined\n");
	printf ("%4d %s",n_lib_proc,"External procedures called\n");
	if (n_lib_proc != n_procs){
		printf ("\nProcedures defined:\n");
		k = 0;
		for (j = 0; j < n_procs; j++){
			if (procs[j].entry != -1)
				k+=printf (format,procs[j].name);
			if (k + max_len > 65) {
				printf ("\n");
				k = 0;
			}
		}
		if (k != 0)printf ("\n");
	}
	if (n_lib_proc){
		printf ("\nExternal procedures called:\n");
		k = 0;
		for (j = 0; j < n_procs; j++){
			if (procs[j].entry == -1)
				k+=printf (format,procs[j].name);
			if (k + max_len > 65) {
				printf ("\n");
				k = 0;
			}
		}
		if (k != 0)printf ("\n");
	}
}

int
main(np,p)
	int		np;
	char	*p[];
{
	int			i,fx;


	if (np != 2) {
		fprintf (stderr,"%s: wrong number of parms\n",p[0]);
		fprintf (stderr,"%s usage: %s file_name.c\n",
			p[0],p[0]);
		exit (1);
	}
	fx = 1;
	get_t_files (p[1]);
	exit (0);
}
