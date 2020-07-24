# include <stdio.h>
# include "sets.h"
# include "slice.h"
# include "lif.h"
# include <malloc.h>
# include <string.h>

static  char    sccsid[] = "@(#)pss.c	1.4  4/26/95";
typedef struct ptr_struct ptr_rec,*ptr_ptr;
struct ptr_struct {
	int		id; /* index in ids array */
	int		proc_id;
	set_ptr	addrs;
};

int		v_opt = 0;
int		n_vars,n_ptrs;
id_ptr	*ids;
ptr_ptr	ptrs;
bit_set	ptrs_in,ptrs_out,ptrs_refed;
set_ptr	*proc_ret_ptrs;

set_ptr ptr_points_to(base_ptr)
	int		base_ptr;
{
    if (base_ptr) return ptrs[base_ptr].addrs;
    else return NULL;
}

# define LINE 70
print_pvs()
{
	int		i,idx;
	int		a_ix,a_id,a_pid;
	set_ptr	to;

	printf ("\n");
	for (i = 1; i <= n_procs; i++){
		if (procs[i].returns_xpr){
			printf ("%d %s returns: ",i,procs[i].proc_name);
			to = proc_ret_ptrs[i];
			if (!to) printf ("<NULL>");
			while (to){
				a_ix = to->id;
				a_id = addrs[a_ix].id;
				a_pid = addrs[a_ix].pid;
				printf (" %s",a_pid?procs[a_pid].locals[a_id].name:
					globals[a_id].name);
				to = to -> next;
			}
			printf ("\n");
		}
	}
	printf ("\n");
	for (i = 1; i <= n_ptrs; i++){
		idx = ptrs[i].id;
		printf ("%s (%s) -> ",ids[idx]->name,
			ptrs[i].proc_id?procs[ptrs[i].proc_id].proc_name:
			"global");
		to = ptrs[i].addrs;
		if (!to) printf ("<NULL>");
		while (to){
			a_ix = to->id;
			a_id = addrs[a_ix].id;
			a_pid = addrs[a_ix].pid;
			printf (" %s",a_pid?procs[a_pid].locals[a_id].name:
				globals[a_id].name);
			to = to -> next;
		}
		printf ("\n");
	}
}

print_ptr_map()
{
	int		ptr,lx,i,id,z;
	char	buff[1000];

	lx = printf ("Pointers to vars: ");
	for (i = 1; i <= n_ptrs; i++){
		id = ptrs[i].id;
		if(id){
			sprintf (buff," %s(%d)->(%d)",ids[id]->name,i,id);
			z = strlen(buff);
			if (z+lx > LINE){
				printf ("\n");
				lx = 0;
			}
			lx += printf ("%s",buff);
		}
	}
	if (lx) printf ("\n");
	lx = printf ("Variables to pointers: ");
	for (i = 1; i <= n_vars; i++){
		if (ids[i] && ids[i]->ptr_id){
			ptr = ids[i]->ptr_id;
			id = ptrs[ptr].id;
			if(id){
				sprintf (buff," %s(%d)->(%d)",ids[id]->name,id,ptr);
				z = strlen(buff);
				if (z+lx > LINE){
					printf ("\n");
					lx = 0;
				}
				lx += printf ("%s",buff);
			}
		}
	}
	if (lx) printf ("\n");
}

void
count_vars(void)
{
	int		ptr_id,i,proc,var;

	n_vars = n_globals;
	n_ptrs = 0;

	for (proc = 1; proc <= n_procs; proc++){
		n_vars += procs[proc].n_locals;
	}
	/*
	printf ("%d IDs (%d globals, %d locals)\n",n_vars,n_globals,
		n_vars-n_globals);
		*/
	ids = (id_ptr *) malloc ((1+n_vars)*sizeof (id_ptr));
	n_ptrs = 0;
	for (var = 1; var <= n_globals; var++){
		ids[var] = &globals[var];
		if (ids[var]->is_pointer){
			n_ptrs++;
			ids[var]->ptr_id = n_ptrs;
		}
		else ids[var]->ptr_id = 0;
	}
	var = n_globals;
	for (proc = 1; proc <= n_procs; proc++){
		for (i = 1; i <= procs[proc].n_locals; i++){
			var++;
			ids[var] = &procs[proc].locals[i];
			if (ids[var]->is_pointer){
				n_ptrs++;
				ids[var]->ptr_id = n_ptrs;
			}
			else ids[var]->ptr_id = 0;
		}
	}
	/*
	printf ("%d pointers\n",n_ptrs);
	*/
	ptrs = (ptr_ptr) malloc ((n_ptrs+1)*sizeof (ptr_rec));
	for (i = 1; i <= n_ptrs; i++){
		ptrs[i].id = 0;
		ptrs[i].proc_id = 0;
		ptrs[i].addrs = NULL;
	}
	for (i = 1; i <= n_vars; i++){
		if(ids[i] && ids[i]->ptr_id)
			if (ptr_id = ids[i]->ptr_id){ /* assignment (= [sic]) */
				ptrs[ptr_id].id = i;
		}
	}
	proc_ret_ptrs = (set_ptr *) malloc ((n_procs+1)*sizeof (set_ptr));
	for (proc = 1; proc <= n_procs; proc++){
		proc_ret_ptrs[proc] = NULL;
		for (i = 1; i <= procs[proc].n_locals; i++){
			if (ptr_id = procs[proc].locals[i].ptr_id){
				ptrs[ptr_id].proc_id = proc;
			}
		}
	}
	if(v_opt)print_ptr_map();
}

char *ptr_to_name(ptr)
{
	int		var_ix;

	var_ix = ptrs[ptr].id;
	return ids[var_ix]->name;
}

char *addr_to_name(addr)
	int		addr;
{
	int		pid,id;

	id = addrs[addr].id;
	pid = addrs[addr].pid;
	if (pid) return procs[pid].locals[id].name;
	else return globals[id].name;
}

char *addr_to_scope(addr)
	int		addr;
{
	int		pid;

	pid = addrs[addr].pid;
	if (pid) return procs[pid].proc_name;
	else return "global";
}

void
print_ptr_set (ptr,s)
	set_ptr	s;
	int	ptr;
{
	printf ("#### Addr set %d (%s): ",ptr,ptr_to_name(ptr));
	while (s){
		printf (" %d(%s)",s->id,s->id?addr_to_name(s->id):"???");
		s = s -> next;
	}
	printf ("\n");
	fflush(stdout);
}

int add_aref_to_var (r,id)
	int		id;
	var_ptr	r;
{
	set_ptr	old;

	/*
	printf ("\nadd aref ***%d*** (%s of %s) to ptr %d (%s)\n",
		r->id,addr_to_name(r->id),addr_to_scope(r->id),id,
		ptr_to_name(id));
	*/
	old = ptrs[id].addrs;
	/* print_ptr_set (id,ptrs[id].addrs); */
	ptrs[id].addrs = add_to_set (ptrs[id].addrs,r->id);
	if (old == ptrs[id].addrs) return 0;
	else return 1;
}

int call_assign (dix,pid)
	int		pid,dix;
{
	set_ptr	old;
	set_ptr	addrs;

	old = ptrs[dix].addrs;
	if (dix){
	addrs = proc_ret_ptrs[pid];
		while (addrs){
			ptrs[dix].addrs = add_to_set (ptrs[dix].addrs,addrs->id);
			addrs = addrs->next;
		}
	}
	if (old == ptrs[dix].addrs) return 0;
	else return 1;
}

int ret_assign (pid,rix)
	int		pid,rix;
{
	set_ptr	old;
	set_ptr	addrs;

	old = proc_ret_ptrs[pid];
	if (rix){
	addrs = ptrs[rix].addrs;
		while (addrs){
			proc_ret_ptrs[pid] =
				add_to_set (proc_ret_ptrs[pid],addrs->id);
			addrs = addrs->next;
		}
	}
	if (old == proc_ret_ptrs[pid]) return 0;
	else return 1;
}

int pointer_assign (dix,rix)
	int		dix,rix;
{
	set_ptr	old;
	set_ptr	addrs;

	old = ptrs[dix].addrs;
	addrs = ptrs[rix].addrs;
	while (addrs){
		/*
		print_ptr_set (dix,ptrs[dix].addrs);
		printf ("\nassign: add ***%d*** (%s of %s) to ptr %d (%s)\n",
			addrs->id,addr_to_name(addrs->id),
			addr_to_scope(addrs->id),dix,ptr_to_name(dix));
		*/
		ptrs[dix].addrs = add_to_set (ptrs[dix].addrs,addrs->id);
		addrs = addrs->next;
	}
	if (old == ptrs[dix].addrs) return 0;
	else return 1;
}

void
resolve_chain (in,out,f)
	bit_set	in,out;
	field_ptr f;
{
	int		new_id,a_id,ptr,id,pid,new_ptr;
	set_ptr addr;

	clear_bit_set (out);
	/*
	printf ("resolve field %d %s at %d\n",f->fid,f->name,f->offset);
	*/
	ptr = -1;
	while ((ptr = get_next_member (in,ptr)) >= 0){
		/*
		printf ("\tptr %d %s",ptr,ptr?ids[ptrs[ptr].id]->name:"???");
		printf ("\n");
		*/
		if (ptr){
			addr = ptrs[ptr].addrs;
			while (addr){
				a_id = addr->id;
				/*
				printf ("\t\taddr %d",addr->id);
				printf (" (pid %d, id %d) %s",addrs[a_id].pid,
					addrs[a_id].id,addrs[a_id].pid?
					procs[addrs[a_id].pid].locals[addrs[a_id].id].name:
					globals[addrs[a_id].id].name);
				*/
				if (offset_check(addrs[a_id].pid,addrs[a_id].id,
					f->offset)){
					new_id = addrs[a_id].id + f->offset;
					if ( new_id <= (addrs[a_id].pid?
							procs[addrs[a_id].pid].n_locals:
							n_globals)){
					/*
					fflush(stdout);
					printf (" (new %d) %s",
						new_id,addrs[a_id].pid?
						procs[addrs[a_id].pid].locals[new_id].name:
						globals[new_id].name);
					*/
					new_ptr = addrs[a_id].pid?
						procs[addrs[a_id].pid].locals[new_id].ptr_id:
						globals[new_id].ptr_id;
					if(new_ptr)bit_on (out,new_ptr);
					/*
					printf (" new ptr %d",new_ptr);
					printf ("\n");
					*/
					}
				}
				addr = addr -> next;
			}
		}
	}
}

get_cdefs(chain)
	int		chain;
{
	int		ptr,id,pid;
	bit_set	in,out,swap;
	field_ptr f;

	id = chains[chain].id;
	pid = chains[chain].pid;
	if (pid)ptr = procs[pid].locals[id].ptr_id;
	else ptr = globals[id].ptr_id;
	/*
	printf ("chain (%d) on ptr %d\n",chain,ptr);
	*/
	out = ptrs_in;
	in = ptrs_out;
	clear_bit_set(out);
	bit_on (out,ptr);
	f = chains[chain].fields;
	while (f){
		swap = in;
		in = out;
		out = swap;
		resolve_chain (in,out,f);

		f = f -> next;
	}
	ptr = -1;
	/*
	printf ("Resolves to:");
	while ((ptr = get_next_member (out,ptr)) >= 0){
		printf (" %d",ptr);
	}
	printf("\n");
	*/
	if (out != ptrs_out){
		swap = ptrs_in;
		ptrs_in = ptrs_out;
		ptrs_out = swap;
	}
}

get_crefs(chain)
	int		chain;
{
	bit_set	swap;

	swap = ptrs_out;
	ptrs_out = ptrs_refed;
	get_cdefs(chain);
	ptrs_refed = ptrs_out;
	ptrs_out = swap;

}

int capture_assigns (pid,fid,sid)
	int		pid,fid,sid;
{
	var_ptr	r,d;
	int		a_pid,a_id,var_defed,id;
	int		count = 0,ptr;

	/* printf ("%5d %5d %5d\n",pid,fid,sid); */
	r = files[fid].stmts[sid].refs;
	while (r){
		if (r -> code == LIF_AREF){
			a_pid = addrs[r->id].pid;
			a_id  = addrs[r->id].id;
			if (procs[pid].returns_xpr &&
				files[fid].stmts[sid].is_return){
				proc_ret_ptrs[pid] = add_to_set (proc_ret_ptrs[pid],
					r->id);
			}
			d = files[fid].stmts[sid].defs;
			while (d){
				var_defed = d->id;
				switch (d->code){
				case LIF_DEF:
					if (procs[pid].locals[var_defed].is_pointer){
						id = procs[pid].locals[var_defed].ptr_id;
						/*
						printf ("assign to %d addr of %d: ",
							d->id,r->id);
						if (id){
							printf (" %s = &",
								procs[pid].locals[var_defed].name);
							if (a_pid)printf ("%s",
								procs[a_pid].locals[a_id].name);
							else printf ("%s",globals[a_id].name);
						}
						printf ("\n");
							*/
						count += add_aref_to_var (r,id);
					}
					break;
				case LIF_GDEF:
					if (globals[var_defed].is_pointer){
						id = globals[var_defed].ptr_id;
						/*
						printf ("assign to %d addr of %d: ",
							d->id,r->id);
						if (id){
							printf (" %s = &",
								globals[var_defed].name);
							if (a_pid)printf ("%s",
								procs[a_pid].locals[a_id].name);
							else printf ("%s",globals[a_id].name);
						}
						printf ("\n");
						*/
						count += add_aref_to_var (r,id);
					}
					break;
				case LIF_CDEF:
					/*
					printf ("cdef %d (%d,%d)\n",var_defed,
						chains[var_defed].id,
						chains[var_defed].pid);
					*/
					get_cdefs(var_defed);
					ptr = -1;
					/*
					printf ("update ptrs (%d):",id);
					*/
					while ((ptr = get_next_member (ptrs_out,ptr)) >= 0){
					/*
						printf (" %d",ptr);
					*/
						count += add_aref_to_var (r,ptr);
					}
					/*
					printf ("\n");
					*/
					break;
				default:
					break;
				}
				d = d -> next;
			}
		}
		r = r -> next;
	}
	return count;
}

int prop_call (pid,fid,sid)
	int		pid,fid,sid;
{
	var_ptr	r,d;
	int		rp,var_refed,rix,var_defed,id;
	int		count = 0,ptr;
	int		called_pid;
	call_ptr c;
	set_ptr	ret_addrs,old;

	/* printf ("%5d %5d %5d\n",pid,fid,sid); */
	d = files[fid].stmts[sid].defs;
	if (!d && !files[fid].stmts[sid].is_return) return 0;
	if (!files[fid].stmts[sid].calls) return 0;
	c = files[fid].stmts[sid].calls;
	while (c){
		called_pid = c->pid;
		if (proc_ret_ptrs[called_pid]){
			if (procs[pid].returns_xpr &&
				files[fid].stmts[sid].is_return){
					old = proc_ret_ptrs[pid];
					ret_addrs = proc_ret_ptrs[called_pid];
					while (ret_addrs){
						proc_ret_ptrs[pid] = add_to_set (
							proc_ret_ptrs[pid],ret_addrs->id);
						ret_addrs = ret_addrs->next;
					}
					count += (old != proc_ret_ptrs[pid]);
			}
			d = files[fid].stmts[sid].defs;
			while (d && (d->level == 0)){
				var_defed = d->id;
				id = 0;
				switch (d->code){
				case LIF_DEF:
					if (procs[pid].locals[var_defed].is_pointer){
						id = procs[pid].locals[var_defed].ptr_id;
					}
					break;
				case LIF_GDEF:
					if (globals[var_defed].is_pointer){
						id = globals[var_defed].ptr_id;
					}
					break;
				case LIF_CDEF:
					get_cdefs(var_defed);
					id = 0;
					break;
				default:
					break;
				}
				/* count += pointer_assign (id,rp); */
				if (id){
					count += call_assign (id,called_pid);
				}
				else {
					ptr = -1;
					while ((ptr = get_next_member (ptrs_out,ptr)) >= 0){
						/*
						if(rp)count += pointer_assign (ptr,rp);
						*/
					}
				}
				d = d -> next;
			}
		}
		c = c -> next;
	}
	return count;
}

int prop_stmt (pid,fid,sid)
	int		pid,fid,sid;
{
	var_ptr	r,d;
	int		rp,var_refed,rix,var_defed,id;
	int		count = 0,ptr;

	/* printf ("%5d %5d %5d\n",pid,fid,sid); */
	r = files[fid].stmts[sid].refs;
	while (r){
		/*
		printf ("rcode %s\n",
			r->code == LIF_AREF?"AREF":(r->code == LIF_CREF?
				"chain ref":(r->code == LIF_REF?"local ref":
				"global ref")));
		*/
		if (r -> code != LIF_AREF){
			if (r -> code != LIF_CREF){
				var_refed = r->id;
				if (r->code == LIF_REF){
					rp = procs[pid].locals[var_refed].ptr_id;
				}
				else {
					rp = globals[var_refed].ptr_id;
				}
				if ((rp == 0) && (r -> code != LIF_CREF))
					{r = r->next; continue;}
				rix = ptrs[rp].id;
				/*
				printf ("ptr id %d addr %d, idx %d var_refed %d %s\n",
					rp,ptrs[rp].addrs,
					rix,var_refed,r->code==LIF_REF?"local":"global");
				*/
			}
			else {
				get_crefs (r->id);
				rp = 0;
			}
			if (procs[pid].returns_xpr &&
				files[fid].stmts[sid].is_return){
					count += ret_assign (pid,rp);
			}
			d = files[fid].stmts[sid].defs;
			while (d && (d->level == 0)){
				var_defed = d->id;
				/*
				printf ("def code %d\n",d->code);
				*/
				switch (d->code){
				case LIF_DEF:
					if (procs[pid].locals[var_defed].is_pointer){
						id = procs[pid].locals[var_defed].ptr_id;
						/*
						printf ("assign to ptr %d contents of ptr %d: ",
							id,rp);
						if (id){
							printf (" %s = ",
								procs[pid].locals[var_defed].name);
							printf ("%s", ids[rix]->name);
						}
						printf ("\n");
						*/
						if(rp)count += pointer_assign (id,rp);
						else {
							rp = -1;
							/*
							printf ("update ptrs (%d):",rp);
							*/
							while ((rp = get_next_member
									(ptrs_refed,rp)) >= 0){
								count += pointer_assign (id,rp);
							}
						}
					}
					break;
				case LIF_GDEF:
					if (globals[var_defed].is_pointer){
						id = globals[var_defed].ptr_id;
						/*
						printf ("assign to %d contents of %d: ",
							id,rp);
						if (id){
							printf (" %s = ",
								globals[var_defed].name);
							printf ("%s", ids[rix]->name);
						}
						printf ("\n");
							*/
						if(rp)count += pointer_assign (id,rp);
						else {
							rp = -1;
							/*
							printf ("update ptrs (%d):",rp);
							*/
							while ((rp = get_next_member
									(ptrs_refed,rp)) >= 0){
								count += pointer_assign (id,rp);
							}
						}
					}
					break;
				case LIF_CDEF:
					/*
					printf ("cdef %d (%d,%d)\n",var_defed,
						chains[var_defed].id,
						chains[var_defed].pid);
					*/
					get_cdefs(var_defed);
					ptr = -1;
					/*
					printf ("update ptrs (%d):",rp);
					*/
					while ((ptr = get_next_member (ptrs_out,ptr)) >= 0){
						/*
						printf (" %d",ptr);
						WHAT ABOUT RP == 0 ??????
						*/
						if(rp)count += pointer_assign (ptr,rp);
						else {
							rp = -1;
							while ((rp = get_next_member
									(ptrs_refed,rp)) >= 0){
								count += pointer_assign (ptr,rp);
							}
							rp = 0;
						}
					}
					/*
					printf ("\n");
					*/
					break;
				default:
					break;
				}
				d = d -> next;
			}
		}
		r = r -> next;
	}
	return count;
}

int do_a_call(pid,fid,call)
	int		pid,fid;
	call_ptr call;
{
	int		change = 0,called_pid,fp;
	int		rix,a_id,a_pid,fpx;
	id_ptr	formals;
	actual_ptr act;
	var_ptr	a_list;

	called_pid = call->pid;
	if (is_lib_proc(called_pid)) return 0;
	formals = procs[called_pid].locals;
	/*
	printf ("call %s (%d formals)",procs[called_pid].proc_name,
		procs[called_pid].n_formals);
		*/
	for (fp = 1; fp <= procs[called_pid].n_formals; fp++){
		/*
		printf ("  %s (%d)",formals[fp].name,fp);
		*/
		if (formals[fp].is_pointer){
			/*
			printf("*");
			*/
			fpx = formals[fp].ptr_id;
			act = call->actuals;
			while (act && act->actual_number != fp)
				act = act->next;
			if (act){
				a_list = act->actuals;
				while(a_list){
					/*
					printf ("actual code %d\n",a_list->code);
					*/
					if (a_list->code == LIF_AREF){
						change += add_aref_to_var (a_list,fpx);
						a_pid = addrs[a_list->id].pid;
						a_id = addrs[a_list->id].id;
						/*
						printf ("==>&%s",a_pid?
							procs[a_pid].locals[a_id].name:
							globals[a_id].name);
						*/
					}
					else {
						if (a_list->code == LIF_REF){
							rix = procs[pid].locals[a_list->id].ptr_id;
						}
						else if (a_list->code == LIF_GREF){
							rix = globals[a_list->id].ptr_id;
						}
						else rix = 0;
						if(rix){
							change += pointer_assign (fpx,rix);
						}
						else if(a_list->code == LIF_CREF){
							get_cdefs (a_list->id);
							rix = -1;
							/*
							printf ("update call ptrs (%d):",rix);
							*/
							while ((rix =
								get_next_member (ptrs_out,rix)) >= 0){
								change += pointer_assign (fpx,rix);
							}
							/*
							printf ("\n");
							*/
						}
					}
					a_list = a_list->next;
				}
			}
		}
	}
	/*
	printf ("\n");
	*/
	return change;
}

int direct_assigns()
{
	int		pid,fid,sid;
	int		change = 0;

	/*
	printf ("get direct assigns\n");
	*/
	for (pid = 1; pid <= n_procs; pid++){
		if (procs[pid].entry != -1){
			fid = procs[pid].file_id;
			for (sid = procs[pid].entry; sid <= procs[pid].exit; sid++){
				change += capture_assigns(pid,fid,sid);
			}
		}
	}
	return change;
}

int propagate()
{
	int		pid,fid,sid;
	int		change = 0;

	/*
	printf ("propagate\n");
	*/
	for (pid = 1; pid <= n_procs; pid++){
		if (procs[pid].entry != -1){
			/*
			printf ("prop %s (%d)\n",procs[pid].proc_name,pid);
			*/
			fid = procs[pid].file_id;
			for (sid = procs[pid].entry; sid <= procs[pid].exit; sid++){
				change += prop_call(pid,fid,sid);
				change += prop_stmt(pid,fid,sid);
			}
		}
	}
	return change;
}

int do_calls()
{
	int		pid,fid,sid;
	int		change = 0;
	call_ptr call_list;

	/*
	printf ("do_calls\n");
	*/
	for (pid = 1; pid <= n_procs; pid++){
		if (procs[pid].entry != -1){
			fid = procs[pid].file_id;
			call_list = procs[pid].calls;
			while(call_list){
				change += do_a_call(pid,fid,call_list);
				call_list = call_list->next;
			}
		}
	}
	return change;
}

flow()
{
	int		change;

	ptrs_in = create_bit_set (n_ptrs+1);
	ptrs_out = create_bit_set (n_ptrs+1);
	ptrs_refed = create_bit_set (n_ptrs+1);
	change = 1;
	while (change){
		change = 0;
		change += direct_assigns();
		change += do_calls();
		change += propagate();
		if(v_opt)printf ("%d changes\n",change);
	}
	if(v_opt)print_pvs();
}
