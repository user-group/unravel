# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "slice.h"
# include "lif.h"

static  char    sccsid[] = "@(#)slice.c	1.7  8/16/95";
static char *sccs_h_id = SLICE_SCCS_ID;
extern int v_opt;
static bit_set dont_descend;
void	(*slice_hook)() = NULL;
void	(*slice_pass_hook)() = NULL;
void  *QQQ;

int stmt_to_proc (file,stmt)
	int	file,stmt;
{
	int		i;

	for (i = 1; i <= n_procs; i++){
		if (procs[i].file_id == file)
			if ((stmt >= procs[i].entry) &&
				(stmt <= procs[i].exit)) return i;
	}
	return 0; /* no proc found */
}

int is_id_valid (pid,id)
	int		pid,id;
{
	if ((pid < 0) || (pid > n_procs)){
		fprintf (stderr,"proc %d out of range %d\n",
			pid,n_procs); fflush(stderr);
		return 0;
	}
	if (pid){
		if ((id < 0) || (id > procs[pid].n_locals)){
			fprintf (stderr,"local id %d out of range %d\n",
				id,procs[pid].n_locals);
			fflush (stderr);
			return 0;
		}
	}
	else {
		if ((id < 0) || (id > n_globals)){
			fprintf (stderr,"global id %d out of range %d\n",
				id,n_globals);
			fflush (stderr);
			return 0;
		}
	}
	return 1;
	
}

void
set_criteria (file,stmt_proc,stmt,var_proc,var)
	int		file,stmt,stmt_proc,var_proc,var;
{
	int		var_ix,var_max,var_n;

	if (var_proc){ /* get all members of a structure */
		var_n = procs[var_proc].n_locals;
		var_max = procs[var_proc].locals[var].offset + var;
	}
	else {
		var_n = n_globals;
		var_max = globals[var].offset + var;
	}
	for (; var <= var_max; var++){
		is_id_valid (var_proc,var);
		if (var_proc){
			if (stmt_proc == var_proc)
				bit_on (files[file].stmts[stmt].active_local,var);
			else {
				add_to_id_set (&files[file].stmts[stmt].active_other,
					var_proc,var);
			}
		}
		else bit_on (files[file].stmts[stmt].active_global,var);
	}
}

void
clear_active_proc(proc)
	int		proc;
{
	int	file,stmt;

	file = procs[proc].file_id;
	for (stmt = procs[proc].entry; stmt <= procs[proc].exit; stmt++){
		clear_bit_set (files[file].stmts[stmt].active_global);
		clear_bit_set (files[file].stmts[stmt].active_local);
		clear_id_set (files[file].stmts[stmt].active_other);
		files[file].stmts[stmt].active_other = NULL;
	}
}

void
clear_active(void)
{
	int	file,stmt;

	for (file = 0; file < n_files; file++)
		for (stmt = 1; stmt <= files[file].n_stmts; stmt++){
			clear_bit_set (files[file].stmts[stmt].active_global);
			clear_bit_set (files[file].stmts[stmt].active_local);
			clear_id_set (files[file].stmts[stmt].active_other);
			files[file].stmts[stmt].active_other = NULL;
		}
}

int 
lib_defs (stmt,id,is_local)
	int		id;
	stmt_ptr stmt;
{
	call_ptr	calls;
	var_ptr		var;
	actual_ptr	act;
	int			cx,ax;

	calls = stmt->calls;
	while (calls){
		cx = calls->pid;
		if(v_opt)printf ("call to %s\n",procs[cx].proc_name);
		act = calls->actuals;
		while (act){
			var = act -> actuals;
			while (var){
				if (var -> code == LIF_AREF){
					ax = var->id;
					if ((addrs[ax].id == id) &&
						((is_local && addrs[ax].pid) ||
						(!is_local && !addrs[ax].pid))) return 1;
				}
				var = var -> next;
			}
			act = act -> next;
		}
		calls = calls->next_this_stmt;
	}
	return 0;
}

int 
is_var_defed (stmt,id,is_local)
	int		id,is_local;
	stmt_ptr stmt;
{
	var_ptr	defs;
	int		result;

	defs = stmt->defs;
	while (defs){
		if (defs->level == 0)switch (defs->code){
		case LIF_DEF:
			if (!is_local) break;
			if (id == defs->id) return 1;
			break;
		case LIF_GDEF:
			if (is_local) break;
			if (id == defs->id) return 1;
			break;
		default:;
		}
		defs = defs->next;
	}
	result = lib_defs (stmt,id,is_local);
	if(v_opt)printf ("is %d defed %d\n",result);
	return result;
/*
	if (is_local) return is_bit_on (stmt->active_local,id);
	return is_bit_on (stmt->active_global,id);
*/
}

print_addr(addr_set)
	set_ptr	addr_set;
{
	int		addr,id,pid;

	if (addr_set){
		addr = addr_set -> id;
		id = addrs[addr].id;
		pid = addrs[addr].pid;
	}
}

static int base_id,base_pid,base_ptr,base_level,base_done = 1;;
static var_ptr	base;
static set_ptr	base_addrs[200];
static int		base_off[200];

int 
offset_check(pid,id,off)
    int pid,id,off;
{
    id_ptr  ids;
    int     n,is_new;
    static id_set_ptr   trouble = NULL;
 
    if (pid){
        ids = procs[pid].locals;
        n = procs[pid].n_locals;
    }
    else {
       n = n_globals;
        ids = globals;
    }
    if (id > n){
        if(add_to_id_set (&trouble,pid,id))
            fprintf (stderr,"id %d out of range (%d)\n",
                id,n);
           return 0;
    }
    if ((off > ids[id].offset)||(off+id > n)){
        if(add_to_id_set(&trouble,pid,id))
            fprintf (stderr,"offset on %s is too large %d (%d)\n"
,
            var_name(pid,id),off,ids[id].offset);
            return 0;
    }
    return 1;
}
 

void
print_field_resolve(ptr,f)
	int		ptr;
	field_ptr f;
{
	set_ptr	addr;
	int		id,pid,p;

	if (!f) return;
	addr = ptr_points_to(ptr);
	while(addr){
		print_addr(addr); printf(" ");
		printf ("field %d %s +%d: ",f->fid,f->name,f->offset);
		id = addrs[addr->id].id;
		pid = addrs[addr->id].pid;
		id += f->offset;
		p = pid?procs[pid].locals[id].ptr_id:globals[id].ptr_id;
		printf ("[[%s]]",pid?procs[pid].locals[id].name:
			globals[id].name);
		printf ("\n");
		print_field_resolve(p,f->next);
		addr = addr -> next;
	}
}

void
chain_resolve_set (def,proc)
	int		proc;
	int		def;
{
	int		chain_id,head_id,head_pid,ptr;
	int		id,pid;
	set_ptr	addr;
	id_ptr	head;
	field_ptr f;

	if(v_opt) printf ("proc %s\n",procs[proc].proc_name);
	chain_id = def;
	head_id = chains[chain_id].id;
	head_pid = chains[chain_id].pid;
	f = chains[chain_id].fields;
	if(v_opt)printf ("Set chain %d on %s->%s\n",def,
		var_name(head_pid,head_id),f?f->name:"<No field>");
	head = head_pid?procs[head_pid].locals:globals;
	ptr = head[head_id].ptr_id;
	addr = ptr_points_to(ptr);
	base_level = 0;
	base_addrs[base_level] = addr;
	while (f){
		base_off[base_level] = f->offset;
		base_level++;
		if (addr){
			id = addrs[addr->id].id;
			pid = addrs[addr->id].pid;
			if (offset_check(pid,id,f->offset)){
				id += f->offset;
				ptr = pid?procs[pid].locals[id].ptr_id:globals[id].ptr_id;
				addr = ptr_points_to(ptr);
				base_addrs[base_level] = addr;
			}
			else {
				base_addrs[base_level] = NULL;
			}
		}
		else base_addrs[base_level] = NULL;
		f = f -> next;
	}
	if (v_opt){
		printf ("base level %d\n",base_level);
		printf ("base off %d %d %d %d\n",base_off[0],base_off[1],
			base_off[2],base_off[3]);
		printf ("base addrs %d %d %d %d\n",base_addrs[0],base_addrs[1],
			base_addrs[2],base_addrs[3]);
	}
}

int 
chain_resolve_get (id,pid)
	int		*id,*pid;
{
	int		level,addr;
	int		idx,pidx,ptr;
	set_ptr	objs;

	/*
	level = base_level - 1;
	if (level < 0) return 0;
	printf ("get level %d\n",level); fflush(stdout);
	objs = base_addrs[level];
	while ((!objs) && (level >= 0)){
		if(v_opt)printf ("level %d\n",level);
		if (base_addrs[level]){
			base_addrs[level] = base_addrs[level]->next;
			objs = base_addrs[level];
			if (objs)while ((level >= 0) && (level < (base_level - 1))){
				if(v_opt)printf ("\tlevel %d\n",level);
				if (base_addrs[level]){
					addr = base_addrs[level]->id;
					idx = addrs[addr].id;
					pidx = addrs[addr].pid;
					if (offset_check(pidx,idx,base_off[level])){
						idx += base_off[level];
						ptr = pidx?procs[pidx].locals[idx].ptr_id:
							globals[idx].ptr_id;
						base_addrs[level] = base_addrs[level]->next;
						level++;
						if (ptr) base_addrs[level] = ptr_points_to(ptr);
						else {
							level--;
						}
					}
					else {
							base_addrs[level] = base_addrs[level]->next;
					}
				}
				else {
					level--;
				}
			}
			else {
				level--;
			}
		}
		else level--;
		if (level == (base_level - 1))objs = base_addrs[level];
		else objs = NULL;
	}
*/
	level = base_level - 1;
	if (level < 0) return 0;
	objs = base_addrs[level];
	if (!objs) level--;
	while ((level >= 0) && (level < (base_level - 1))){
		if(v_opt)printf ("level %d\n",level);
		if (base_addrs[level]){
			base_addrs[level] = base_addrs[level]->next;
			objs = base_addrs[level];
			if(v_opt)printf ("\tlevel %d\n",level);
			if (base_addrs[level]){
				addr = base_addrs[level]->id;
				idx = addrs[addr].id;
				pidx = addrs[addr].pid;
				if (offset_check(pidx,idx,base_off[level])){
					idx += base_off[level];
					ptr = pidx?procs[pidx].locals[idx].ptr_id:
						globals[idx].ptr_id;
					level++;
					if (ptr) base_addrs[level] = ptr_points_to(ptr);
					else {
						level--;
					}
				}
				else {
						base_addrs[level] = base_addrs[level]->next;
				}
			}
			else {
				level--;
			}
		}
		else level--;
	}
	if (base_addrs[level] && (level == (base_level - 1))){
		addr = base_addrs[level]->id;
		if (offset_check (addrs[addr].pid,addrs[addr].id,
			base_off[level])){
			*id = addrs[addr].id + base_off[level];
			*pid = addrs[addr].pid;
			base_addrs[level] = base_addrs[level]->next;
			return 1;
		}
	}
	return 0;
}

void
var_resolve_set (def,proc)
	int		proc;
	var_ptr	def;
{
	int		i,id,pid,ptr,addr;

	base = def;
	base_done = 0;
	base_level = def->level;
	base_id = def->id;
	if ((def->code == LIF_GDEF) ||
		(def->code == LIF_GREF)) base_pid = 0;
	else base_pid = proc;
	base_ptr = base_pid?procs[proc].locals[base_id].ptr_id:
		globals[base_id].ptr_id;
	base_addrs[0] = ptr_points_to(base_ptr);
	for (i = 1; i < base_level; i++){
		if (base_addrs[i-1] != NULL){
			addr = base_addrs[i-1]->id;
			pid = addrs[addr].pid;
			id = addrs[addr].id;
			ptr = pid?procs[pid].locals[id].ptr_id:
				globals[id].ptr_id;
			if (ptr) base_addrs[i] = ptr_points_to(ptr);
			else base_addrs[i] = NULL;
		}
		else base_addrs[i] = NULL;
	}
}
print_base_state()
{
	int		i,top,at;
	int		ptr,addr;
	set_ptr objs;

	printf ("resolve %s at level %d\n",base_pid?
		procs[base_pid].locals[base_id].name:
		globals[base_id].name,base_level);
	for (i = 0; i < base_level; i++){
		if (base_addrs[i]){
			printf ("level %d is:",i);
			objs = base_addrs[i];
			while (objs){
				printf (" %d",objs->id);
				objs = objs->next;
			}
			printf ("\n");
		}
	}
}

/********************************************************************/
/*                                                                  */
/* var_resolve_get does a depth first traversal of the pointer state*/
/*                                                                  */
/********************************************************************/
int var_resolve_get (id,pid)
	int		*id,*pid;
{
	int		i,top,at;
	int		ptr,addr;
	set_ptr objs;

	top = base_level - 1;
	objs = base_addrs[top];
	while ((!objs) && (top >= 0)){
	/*	top--; */
		if (base_addrs[top]){
			base_addrs[top] = base_addrs[top]->next;
			objs = base_addrs[top];
			if (objs)while (((top < (base_level - 1))&&(top >= 0))){
				if (base_addrs[top]){
					int	id,pid; /* D E C L */
					addr = base_addrs[top]->id;
					top++;
					pid = addrs[addr].pid;
					id = addrs[addr].id;
					ptr = pid?procs[pid].locals[id].ptr_id:
						globals[id].ptr_id;
					if (ptr) base_addrs[top] = ptr_points_to(ptr);
					else {
						top--;
						base_addrs[top] = base_addrs[top]->next;
					}
				}
				else {
					top--;
				}
			}
		}
		else top--;

		if (top == (base_level - 1)) objs = base_addrs[top];
		else objs = NULL;
	}
	if (objs){
		base_addrs[top] = base_addrs[top]->next;
		addr = objs->id;
		if ((addr > 0) && (addr <= n_addrs)){
			*id = addrs[addr].id;
			*pid = addrs[addr].pid;
			return 1;
		}
	}
	return 0;
}

chain_resolve_path (f,n,proc)
	int		f,n,proc;
{
	int		i,id,pid,addr;

	if (base_level < 2) return;
	for (i = 0; i < (base_level - 1); i++)
		if (base_addrs[i]){
		addr = base_addrs[i]->id;
		if (offset_check(addrs[addr].pid,
			addrs[addr].id,
			base_off[i])){
			id = addrs[addr].id + base_off[i];
			pid = addrs[addr].pid;
			if ((pid == 0) || (pid == proc)){
				bit_on (pid?files[f].stmts[n].active_local:
					files[f].stmts[n].active_global,id);
			}
			else add_to_id_set (&files[f].stmts[n].active_other,pid,id);
		}

	}
}


var_resolve_path (f,n,proc)
	int		f,n,proc;
{
	int		i,id,pid,addr;

	if (base_level < 2) return;
	for (i = 0; i < (base_level - 1); i++)
		if (base_addrs[i]){
		addr = base_addrs[i]->id;
		id = addrs[addr].id;
		pid = addrs[addr].pid;
		if ((pid == 0) || (pid == proc)){
			bit_on (pid?files[f].stmts[n].active_local:
				files[f].stmts[n].active_global,id);
		}
		else add_to_id_set (&files[f].stmts[n].active_other,pid,id);

	}
}

int is_other_active (set,pid,id)
	id_set_ptr	set;
	int		id,pid;
{
	while (set){
		if ((set->id == id) && (set->pid == pid))
			return 1;
		set = set -> next;
	}
	return 0;
}

int active_defed (f,n,succ,proc)
	int	f,n,succ,proc;
{
	int match = 0;

	if (!files[f].stmts[n].defs) return 0;
	if (files[f].stmts[n].defs->level == 0){
		switch (files[f].stmts[n].defs->code){
		case LIF_DEF:
			if(is_bit_on (files[f].stmts[succ].active_local,
					files[f].stmts[n].defs->id)){
				return 1;
			}
			break;
		case LIF_GDEF:
			if (is_bit_on (files[f].stmts[succ].active_global,
					files[f].stmts[n].defs->id)){
				return 1;
			}
			break;
		case LIF_CDEF:
			{ int id,pid;
			if(v_opt)
				printf ("\ndef to chain %d\n",
					files[f].stmts[n].defs->id);
			chain_resolve_set (files[f].stmts[n].defs->id,proc);
			while (chain_resolve_get(&id,&pid)){
				if(v_opt)printf ("chain resolves to: %s\n",
					pid?procs[pid].locals[id].name:
					globals[id].name);
				if (!pid || (pid == proc)){
					if (is_bit_on(pid?files[f].stmts[succ].active_local:
						files[f].stmts[succ].active_global,id)){
				
						match = 1;
						chain_resolve_path (f,n,proc);
					}
				}
				else {
				if (is_other_active (files[f].stmts[succ].active_other,
					pid,id)){
							match += add_to_id_set (
								&files[f].stmts[n].active_other,pid,id);
								chain_resolve_path (f,n,proc);
				}
				}
			}
			
			if (match){
				id = chains[files[f].stmts[n].defs->id].id;
				pid = chains[files[f].stmts[n].defs->id].pid;
				if (pid == 0)
					bit_on (files[f].stmts[n].active_global,id);
				else if (pid == proc)
					bit_on (files[f].stmts[n].active_local,id);
				}
				else {
					if (is_other_active (files[f].stmts[succ].active_other,
						pid,id)){
						match += add_to_id_set (
							&files[f].stmts[n].active_other,pid,id);
						return match;
				}
			}
			}
			break;
		default:;
		}
	}
	else {
		int		id,pid;

		if(v_opt)print_base_state();
		var_resolve_set (files[f].stmts[n].defs,proc);
		while (var_resolve_get(&id,&pid)){
			/*
			printf ("resolves to: %s\n",pid?
				procs[proc].locals[id].name: globals[id].name);
			*/
			if ((pid == 0) || (pid == proc)){
				if (is_bit_on (pid?files[f].stmts[succ].active_local:
						files[f].stmts[succ].active_global,id)){

					var_resolve_path (f,n,proc);
					match = 1;
				}
			}
			else {
				if (is_other_active (files[f].stmts[succ].active_other,
					pid,id)){
					match += add_to_id_set (
						&files[f].stmts[n].active_other,pid,id);
					return match;
				}
			}
		}
		if (match)
			if (files[f].stmts[n].defs->code == LIF_GDEF)
				bit_on (files[f].stmts[n].active_global,
					files[f].stmts[n].defs->id);
			else if (files[f].stmts[n].defs->code == LIF_DEF)
				bit_on (files[f].stmts[n].active_local,
					files[f].stmts[n].defs->id);
	}
	return match;
}

add_to_active (f,n,pid,id,proc_id)
	int	f,n,pid,id,proc_id;
{
	bit_set	active;

	is_id_valid (pid,id);
	if(v_opt)printf ("add %s to active %d (L %d) of %s\n",
		var_name(pid,id),n,files[f].stmts[n].froml,
		procs[proc_id].proc_name);
	if (proc_id == pid) active =
		files[f].stmts[n].active_local;
	else if (pid){
		add_to_id_set (&files[f].stmts[n].active_other,pid,id);
		return;
	}
	else active = files[f].stmts[n].active_global;
	bit_on(active,id);
}

de_ref_chain (f,pid,refs,n)
	var_ptr	refs;
	int		pid,f,n;
{
	int		id,proc_id;
	stmt_ptr stmt;
	int		chain_id;

	stmt = &files[f].stmts[n];
	chain_resolve_set (refs->id,pid);
	chain_id = refs->id;
	add_to_active (f,n,pid,chains[chain_id].id,
		chains[chain_id].pid);
	while (chain_resolve_get (&id,&proc_id)){
		if(v_opt)printf ("chain resolve to %d %d %s \n",id,proc_id,
			var_name(proc_id,id));
		if(v_opt)fflush(stdout);
		chain_resolve_path (f,n,pid);
		add_to_active (f,n,proc_id,id,pid);
	}
}

de_ref_ref (f,pid,refs,n)
	var_ptr	refs;
	int		pid,f,n;
{
	int		id,proc_id;
	stmt_ptr stmt;

	stmt = &files[f].stmts[n];
	if(v_opt)printf ("de ref %s %s in %s at %d\n",
		refs->code == LIF_REF?"local":(refs->code == LIF_GREF?
		"global":"???"),
		refs->code == LIF_REF?procs[pid].locals[refs->id].name:
		(refs->code == LIF_GREF?
		globals[refs->id].name:"???"),
		procs[pid].proc_name,stmt->froml);
	if(v_opt)fflush(stdout);
	var_resolve_set (refs,pid);
	while (var_resolve_get (&id,&proc_id)){
		if(v_opt)printf ("resolve to %d %d %s \n",id,proc_id,
			var_name(proc_id,id));
		fflush(stdout);
		var_resolve_path (f,n,pid);
		add_to_active (f,n,proc_id,id,pid);
	}
}

int add_stmt_to_slice(pid,f,stmt,slice,succ)
	int		pid,f,stmt;
	stmt_ptr succ;
	bit_set slice;
{
	int		already_in_slice = 0;
	int		change = 0;
	bit_set active;
	id_set_ptr others;
	int		at;
	var_ptr	refs;
	set_ptr	control;

	if(v_opt)printf ("add stmt %d to slice  ",stmt);
	if (is_bit_on (slice,stmt)) already_in_slice = 1;
	else bit_on (slice,stmt);
	if (already_in_slice) if(v_opt)printf ("already in ");
	if(v_opt)printf ("\n");
	if (!already_in_slice){
		refs = files[f].stmts[stmt].refs;
		while (refs){
			switch (refs->code){
			case LIF_REF:
				bit_on (files[f].stmts[stmt].active_local,refs->id);
				if (refs->level != 0)
					de_ref_ref (f,pid,refs,stmt);
				break;
			case LIF_GREF:
				bit_on(files[f].stmts[stmt].active_global,refs->id);
				if (refs->level != 0)
					de_ref_ref (f,pid,refs,stmt);
				break;
			case LIF_CREF:
				de_ref_chain (f,pid,refs,stmt);
				break;
			default:;
			}
			refs = refs->next;
		}
		control = files[f].stmts[stmt].requires;
		while (control){
			if (!is_bit_on(slice,control->id)){
				add_stmt_to_slice(pid,f,control->id,slice,(stmt_ptr)NULL);
				if(v_opt)printf ("add %d required by %d\n",
					control->id,stmt);
			}
			control = control -> next;
		}
	}
	if (succ){
		at = -1;
		while ((at = get_next_member(succ->active_local,at)) >= 0){
			if(!is_var_defed(&files[f].stmts[stmt],at,1)){
				if (!is_bit_on(files[f].stmts[stmt].active_local,at)){
					bit_on (files[f].stmts[stmt].active_local,at);
					change += 1;
				}
			}
		}
		at = -1;
		while ((at = get_next_member(succ->active_global,at)) >= 0){
			if(!is_var_defed(&files[f].stmts[stmt],at,0)){
				if (!is_bit_on(files[f].stmts[stmt].active_global,at)){
					bit_on (files[f].stmts[stmt].active_global,at);
					change += 1;
				}
			}
		}
		others = succ->active_other;
		while(others){
			change += add_to_id_set (&files[f].stmts[stmt].active_other,
				others->pid,others->id);
			others = others -> next;
		}
	}

	return change + 1 - already_in_slice;
}

int pass_criteria (f,stmt,succ)
	int		f,stmt,succ;
{
	int		change,c;
	id_set_ptr active;

	change = 0;
	if (!bit_set_equal (files[f].stmts[stmt].active_global,
						files[f].stmts[succ].active_global)){
		change = cunion_bit_set (files[f].stmts[stmt].active_global,
			files[f].stmts[succ].active_global);
	}
	if (!bit_set_equal (files[f].stmts[stmt].active_local,
						files[f].stmts[succ].active_local)){
		c = cunion_bit_set (files[f].stmts[stmt].active_local,
			files[f].stmts[succ].active_local);
		change += c;
	}
	active = files[f].stmts[succ].active_other;
	while (active){
		change += add_to_id_set (&files[f].stmts[stmt].active_other,
			active->pid,active->id);
		active = active -> next;
	}
	return change;
}

# define LINE 60
print_active_stmt (f,proc,stmt,mark)
	int		f,stmt,mark;
{
	stmt_ptr s;
	int		k,var,n;
	char	buff[100];
	id_set_ptr active;

		s = &files[f].stmts[stmt];
		var = -1;
		n = printf ("%3d%c:",stmt,mark?'*':' ');
		/*
		n = printf ("%3d%c:",stmt,is_bit_on(slice,stmt)?'*':' ');
		*/
		while ((var = get_next_member(s->active_global,var)) >= 0){
			sprintf (buff," %s",globals[var].name);
			k = strlen(buff);
			if (k + n > LINE) {
				printf ("\n");
				n = printf ("    %s",buff);
			}
			else n += printf ("%s",buff);
		}
		var = -1;
		n += printf (" & ");
		while ((var = get_next_member(s->active_local,var)) >= 0){
			sprintf (buff," %s",procs[proc].locals[var].name);
			k = strlen(buff);
			if (k + n > LINE) {
				printf ("\n");
				n = printf ("    %s",buff);
			}
			else n += printf ("%s",buff);
		}
		n += printf (" &+ ");
		active = s->active_other;
		while (active){
			sprintf (buff," %s",var_name(active->pid,active->id));
			/*
			sprintf (buff," %s",procs[active->pid].locals[
				active->id].name);
				*/
			k = strlen(buff);
			if (k + n > LINE) {
				printf ("\n");
				n = printf ("    %s",buff);
			}
			else n += printf ("%s",buff);
			active = active -> next;
		}
		printf ("\n");
}

void active_hook (fid,line)
	int		fid,line;
{
	int		stmt,proc;

	stmt = line_to_stmt(fid,line);
	proc = stmt_to_proc (fid,stmt);
	if (proc){
		printf ("\nProcedure %s may define: ",procs[proc].proc_name);
		print_proc_defs (0,proc);
		printf ("\nline %d in %s ",line,procs[proc].proc_name);
		print_active_stmt (fid,proc,stmt,0);
	}
	else {
		printf ("can't find proc for file %d line %d\n",fid,line);
	}
}

print_active (proc,slice)
	int		proc;
	bit_set slice;
{
	int		stmt;
	char	buff[100];
	int		k,var,n,f;
	stmt_ptr s;
	id_set_ptr active;

	printf ("active sets for proc %s\n",procs[proc].proc_name);
	f = procs[proc].file_id;
	for (stmt = procs[proc].entry; stmt <= procs[proc].exit; stmt++){
		print_active_stmt (f,proc,stmt,is_bit_on(slice,stmt));
	}
}

print_all_active (slices,active)
	bit_set		slices[],active;
{
	int			file,proc;

	proc = -1;
	while ((proc = get_next_member(active,proc)) >= 0){
		file = procs[proc].file_id;
		print_active (proc,slices[file]);
	}
}

add_ptr_to_active (f,pid,ptr_id,stmt)
	int	f,pid,stmt,ptr_id;
{
	int		ptr,var_id,proc_id;
	set_ptr	a;
	bit_set	active;

	a = ptr_points_to(ptr_id);
	while (a){
		var_id = addrs[a->id].id;
		proc_id = addrs[a->id].pid;
		if (proc_id){
			if (proc_id == pid)
				active = files[f].stmts[stmt].active_local;
			/* else add to other active */
			else active = NULL;
		}
		else active = files[f].stmts[stmt].active_global;
		if (active) bit_on (active,var_id);
		ptr = proc_id?procs[proc_id].locals[var_id].ptr_id:
			globals[var_id].ptr_id;
		if(ptr)add_ptr_to_active (f,pid,ptr,stmt);
		a = a -> next;
	}
}

add_an_actual (from_pid,call,act)
	call_ptr	call;
	int			from_pid;
	actual_ptr	act;
{
	int		pid,stmt,f;
	var_ptr		refs;

	stmt = call->stmt;
	f = procs[from_pid].file_id;
	pid = from_pid;
	refs = act->actuals;
	while(refs){
		switch (refs->code){
		case LIF_AREF:
			if (addrs[refs->id].pid)
				bit_on (files[f].stmts[stmt].active_local,
					addrs[refs->id].id);
			else
				bit_on (files[f].stmts[stmt].active_global,
					addrs[refs->id].id);
			break;
		case LIF_REF:
			bit_on (files[f].stmts[stmt].active_local,refs->id);
			if(v_opt)printf ("add actual %d %s\n",refs->id,
				procs[from_pid].locals[refs->id].name);
			if (procs[from_pid].locals[refs->id].is_pointer)
				add_ptr_to_active (f,pid,
					procs[from_pid].locals[refs->id].ptr_id,stmt);
			break;
		case LIF_GREF:
			bit_on(files[f].stmts[stmt].active_global,refs->id);
			if (refs->level != 0)
				de_ref_ref (f,pid,refs,stmt);
			break;
		case LIF_CREF:
			de_ref_chain (f,pid,refs,stmt);
			break;
		default:;
		}
		refs = refs->next;
	}
}

add_actuals (from_pid,call)
	call_ptr	call;
	int			from_pid;
{
	int		pid,stmt,f;
	actual_ptr	act;
	var_ptr		refs;

	stmt = call->stmt;
	f = procs[from_pid].file_id;
	pid = from_pid;
	act = call->actuals;
	while (act){
		refs = act->actuals;
		while(refs){
			switch (refs->code){
			case LIF_AREF:
				if (addrs[refs->id].pid)
					bit_on (files[f].stmts[stmt].active_local,
						addrs[refs->id].id);
				else
					bit_on (files[f].stmts[stmt].active_global,
						addrs[refs->id].id);
				break;
			case LIF_REF:
				bit_on (files[f].stmts[stmt].active_local,refs->id);
				if (refs->level != 0)
					de_ref_ref (f,pid,refs,stmt);
				break;
			case LIF_GREF:
				bit_on(files[f].stmts[stmt].active_global,refs->id);
				if (refs->level != 0)
					de_ref_ref (f,pid,refs,stmt);
				break;
			case LIF_CREF:
				de_ref_chain (f,pid,refs,stmt);
				break;
			default:;
			}
			refs = refs->next;
		}
		act = act -> next;
	}
}

actual_ptr	find_actual (call,n)
	int		n;
	call_ptr	call;
{
	actual_ptr	act;

	act = call->actuals;
	while (act){
		if (act->actual_number == n) return act;
		act = act -> next;
	}
	return NULL;
}

export_called_active (call,called_proc,calling_proc,calling_stmt)
	int		calling_proc,called_proc;
	int		calling_stmt;
	call_ptr	call;
{
	int		f,fs,start;
	int		at;
	actual_ptr act;
	id_set_ptr	others;
	int		change;

	start = procs[called_proc].entry;
	f = procs[calling_proc].file_id;
	fs = procs[called_proc].file_id;
	union_bit_set (files[f].stmts[calling_stmt].active_global,
		files[fs].stmts[start].active_global);
	at = -1;
	while ((at=get_next_member(files[fs].stmts[start].active_local,at))
			>= 0){
		if(v_opt)printf ("in call to %s need id %d (%s)\n",
			procs[called_proc].proc_name,at,
			procs[called_proc].locals[at].name);
		act = find_actual (call,at);
		if (act) add_an_actual (calling_proc,call,act);
	}
	others = files[fs].stmts[start].active_other;
	while (others){
		if (others->pid == calling_proc){
				bit_on (files[f].stmts[calling_stmt].active_local,
						others->id);
		}
		else {
			change = add_to_id_set (
				&files[f].stmts[calling_stmt].active_other,
				others->pid,others->id);
		}
		others = others->next;
	}
}

slice_across_call (final,slice_sets,call,proc,active)
	int		proc;
	bit_set	final[],slice_sets[];
	bit_set	active;
	call_ptr	call;
{
	int		pid;
	int		f,fs;
	set_ptr	r;
	int		i,at;
	id_set_ptr others;
	int		change;

	if(v_opt)printf ("slice across\n");
	pid = call->pid;
	if (is_bit_on (dont_descend,pid)) return;
	f = procs[pid].file_id;
	fs = procs[proc].file_id;
	clear_active_proc(pid);
	for (i = procs[pid].entry; i <= procs[pid].exit; i++)
		bit_off (slice_sets[f],i);
	/* if (!is_bit_on (active,pid)) */{
		add_stmt_to_slice (pid,f,procs[pid].entry,slice_sets[f],(stmt_ptr)NULL);
		add_stmt_to_slice (pid,f,procs[pid].exit,slice_sets[f],(stmt_ptr)NULL);
		r = procs[pid].return_stmts;
		while (r){
			add_stmt_to_slice (pid,f,r->id,slice_sets[f],(stmt_ptr)NULL);
			r = r -> next;
		}
	}
	bit_on (active,pid);
	/* for (i = procs[pid].entry; i <= procs[pid].exit; i++) */
	union_bit_set (files[f].stmts[procs[pid].exit].active_global,
		files[fs].stmts[call->stmt].active_global);
	others = files[fs].stmts[call->stmt].active_other;
	while (others){
		if (others->pid == pid)
			bit_on(files[f].stmts[procs[pid].exit].active_local,
				others->id);
		else add_to_id_set (
			&files[f].stmts[procs[pid].exit].active_other,
			others->pid,others->id);
		others = others->next;
	}
	at = -1;
	while ((at = get_next_member(
			files[fs].stmts[call->stmt].active_local,at)) >= 0){
		change = add_to_id_set (
			&files[f].stmts[procs[pid].exit].active_other,
			proc,at);
	}
	slice_proc(final,slice_sets,pid,active);
	export_called_active (call,pid,proc,call->stmt);
}

int globals_defed(call,succ,pid)
	call_ptr	call;
	int			pid,succ;
{
	int		stmt,f;
	int		at;
	id_set_ptr others;

	if (!procs[call->pid].global_defs && !procs[call->pid].other_defs)
		return 0;
	stmt = call->stmt;
	f = procs[pid].file_id;
	/*
	at = -1;
	while ((at = get_next_member(
			files[f].stmts[succ].active_global,at)) >= 0){
		printf ("active %s\n",var_name (0,at));
	}
	*/
	at = -1;
	while ((at = get_next_member(procs[call->pid].global_defs,at)) >=
			0){
		if(v_opt)printf ("%s\n",var_name (0,at));
		if (is_bit_on (files[f].stmts[succ].active_global,at))
			return 1;
	}
	others = procs[call->pid].other_defs;
	while (others){
		if (others->pid == pid){
			if (is_bit_on (
				files[f].stmts[succ].active_local,others->id)){
					if(v_opt)
						printf ("var %s defed in call to %s from %s\n",
							var_name(pid,others->id),
							procs[call->pid].proc_name,
							procs[pid].proc_name);
					return 1;
			}
		}
		else if (is_in_id_set (files[f].stmts[succ].active_other,
			others->pid,others->id)) return 1;
		others = others -> next;
	}
	return 0;
}

int active_actuals(call,succ,pid)
	call_ptr	call;
	int			pid,succ;
{
	int		stmt,f;
	actual_ptr	act;
	var_ptr		refs;
	int			var_id,proc_id;
	bit_set		active;

	if(v_opt)printf ("any actuals ptr to active?\n");
	stmt = call->stmt;
	f = procs[pid].file_id;
	act = call->actuals;
	while (act){
		refs = act->actuals;
		while(refs){
			switch (refs->code){
			case LIF_AREF:
				proc_id = addrs[refs->id].pid;
				var_id  = addrs[refs->id].id;
				if(v_opt)printf ("actual aref to %s at %d from %d  ",
					var_name(proc_id,var_id),stmt,succ);
				active = proc_id?files[f].stmts[succ].active_local:
					files[f].stmts[succ].active_global;
				if (is_bit_on(active,var_id)) if(v_opt)printf ("active\n");
				else if(v_opt)printf ("not\n");
				if (is_bit_on(active,var_id)) return 1;
				break;
			case LIF_REF:
				break;
			case LIF_GREF:
				break;
			case LIF_CREF:
				break;
			default: ;
			}
			refs = refs -> next;
		}
		act = act -> next;
	}
	return 0;
}
int lib_def_active(calling_proc,f,stmt,succ,call)
	call_ptr	call;
	int			calling_proc,f,stmt,succ;
{
	int		called_proc;
	actual_ptr	act;
	int status = 0;
	var_ptr		var;
	int			id,pid;
	bit_set		active;

	called_proc = call ->pid;
	act = call->actuals;
	while (act){
		var = act->actuals;
		while (var){
			switch (var->code){
			case LIF_AREF:
				id = addrs[var->id].id;
				pid = addrs[var->id].pid;
				if (pid == calling_proc)
					active = files[f].stmts[succ].active_local;
				else if (!pid)
					active = files[f].stmts[succ].active_global;
				else active = NULL;
				if (active && is_bit_on (active,id)){
					add_to_active (f,stmt,pid,id,calling_proc);
					status = 1;
				}
				break;
			case LIF_CREF:
				break;
			case LIF_REF:
				break;
			case LIF_GREF:
				break;
			default: ;
			}
			var = var -> next;
		}
		act = act -> next;
	}
	return status;
}

add_req_set(f,pid,s,slice)
	int	f,s,pid;
	bit_set	slice;
{
	set_ptr	r;

	r = files[f].stmts[s].requires;
	while (r){
		add_stmt_to_slice (pid,f,r->id,slice,(stmt_ptr)NULL);
		r = r -> next;
	}
}
	

int check_calls (fs,slices,proc,stmt,succ,active)
	int		proc,stmt;
	set_ptr	succ;
	bit_set	fs[],slices[];
	bit_set	active;
{
	call_ptr	calls;
	int			status;
	int			f;

	status = 0;
	f = procs[proc].file_id;
	calls = files[f].stmts[stmt].calls;
	if (!calls) return status;
	if(v_opt)printf ("\ncheck calls at %d (succ %d)\n",stmt,succ->id);
	while (calls){
		if (procs[calls->pid].entry == -1)
			if(v_opt)printf ("Library ");
		if(v_opt)printf ("call %s at %d\n",procs[calls->pid].proc_name,
			calls->stmt);
		if (procs[calls->pid].entry == -1){
			if (lib_def_active(proc,f,stmt,succ->id,calls)){
				bit_on (slices[f],stmt);
				add_req_set(f,proc,stmt,slices[f]);
			}
			if (is_bit_on (slices[f],stmt))
				add_actuals (proc,calls);
		}
		else {
			if (active_actuals(calls,succ->id,proc)){
				bit_on (slices[f],stmt);
			}
			else if (globals_defed(calls,succ->id,proc)){
				bit_on (slices[f],stmt);
			}
			if (is_bit_on (slices[f],stmt)){
				add_req_set(f,proc,stmt,slices[f]);
				slice_across_call (fs,slices,calls,proc,active);
				/*
				add_actuals (proc,calls);
				*/
			}
		}
		calls = calls->next_this_stmt;
	}
	return status;
}


int slice_pass (fs,slices,proc,up,active)
	bit_set	slices[],fs[];
	int		proc,up;
	bit_set	active;
{
	bit_set	slice;
	int		from,to,delta,file,stmt;
	int		change = 0,inc;
	set_ptr	succ;

	file = procs[proc].file_id;
	slice = slices[file];
	if (up){
		from = procs[proc].entry;
		to = procs[proc].exit + 1;
		delta = 1;
	}
	else {
		to = procs[proc].entry - 1;
		from = procs[proc].exit;
		delta = -1;
	}
	stmt = from;
	while ((stmt >= procs[proc].entry) &&
			(stmt <= procs[proc].exit)){
		if (slice_hook)	(*slice_hook)(proc,stmt);
		succ = files[file].stmts[stmt].succ;
		while (succ){
			if (active_defed (file,stmt,succ->id,proc)){
				inc = add_stmt_to_slice(proc,file,stmt,slice,
					&files[file].stmts[succ->id]);
				change += inc;
				change += check_calls (fs,slices,proc,stmt,succ,active);
			}
			else {
				inc = pass_criteria (file,stmt,succ->id);
				change += inc;
				change += check_calls (fs,slices,proc,stmt,succ,active);
			}
			succ = succ -> next;
		}
		stmt = files[file].stmts[stmt].visit_next;
	}
	if(v_opt)print_active (proc,slice);
	if (slice_pass_hook)	(*slice_pass_hook)(proc,change);
	return change;
}

slice_tree (slices,proc,active)
	bit_set		active,slices[];
	int			proc;
{
	int			p,caller_pid,caller_file;
	int			change,n = 0;
	call_ptr	caller;

	clear_bit_set (active);
	bit_on (active,proc);
	caller_pid = proc;
	caller_file = procs[caller_pid].file_id;
	add_stmt_to_slice (caller_pid,caller_file,
		procs[caller_pid].entry,
		slices[caller_file],(stmt_ptr)NULL);
	add_stmt_to_slice (caller_pid,caller_file,
		procs[caller_pid].exit,
		slices[caller_file],(stmt_ptr)NULL);
	change = 1;
	while (change){
		change = 0;
		p = -1;
		while ((p = get_next_member(active,p)) >= 0){
			caller = procs[p].called_by;
			while (caller){
				caller_pid = caller->pid;
				caller_file = procs[caller_pid].file_id;
				if (!is_bit_on (slices[caller_file],caller->stmt)) {
					change = 1;
					add_stmt_to_slice (caller_pid,caller_file,
						caller->stmt, slices[caller_file],(stmt_ptr)NULL);
					bit_on (active,caller->pid);
					add_stmt_to_slice (caller_pid,caller_file,
						procs[caller_pid].entry,
						slices[caller_file],(stmt_ptr)NULL);
					add_stmt_to_slice (caller_pid,caller_file,
						procs[caller_pid].exit,
						slices[caller_file],(stmt_ptr)NULL);
					/* add caller to slice; */
				}
				caller = caller -> next;
			}
		}
	}
}

add_call_to_slice (slice_sets,proc,call)
	bit_set	slice_sets[];
	int		proc;
	call_ptr	call;
{
	set_ptr		return_at;

	if (procs[call->pid].entry == -1){
	/* undefined function: should look for library info */
		/* add actuals to active set */
	}
	else { /* defined proc */
		/*
		if (procs[proc].returns_xpr)
		*/
			if(v_opt)printf ("returns %d ",procs[call->pid].returns_xpr);
		return_at = procs[call->pid].return_stmts;
		if(return_at)if(v_opt)printf ("(");
		while (return_at){
			if(v_opt)printf ("%d",return_at->id);
			return_at = return_at -> next;
			if(return_at)if(v_opt)printf (",");
			else if(v_opt)printf (") ");
		}
	}
}

slice_up (final_slice_sets,slice_sets,proc,active,dont_slice)
	int		proc;
	bit_set	final_slice_sets[],slice_sets[];
	bit_set	active,dont_slice;
{
	call_ptr	calls;

	/* printf ("\nSLICE UP %s\n",procs[proc].proc_name);*/
	clear_bit_set (dont_descend);
	slice_proc(final_slice_sets,slice_sets,proc,active);
	if (procs[proc].entry == -1) return;
	if (is_bit_on(dont_slice,proc))return;
	calls = procs[proc].called_by;
	bit_on (dont_slice,proc);
	while (calls){
		export_called_active (calls,
			proc,calls->pid,calls->stmt);
		slice_up (final_slice_sets,slice_sets,
			calls->pid,active,dont_slice);
		calls = calls -> next;
	}
}

int call_pass (slice_sets,proc) /* d e a d    c o d e */
	bit_set	slice_sets[];
	int		proc;
{
	int		status = 0;
	call_ptr	calls;
	int		file;

	calls = procs[proc].calls;
	file = procs[proc].file_id;
	while (calls){
		if (is_bit_on (slice_sets[file],calls->stmt)){
			add_call_to_slice (slice_sets,proc,calls);
			if(v_opt)printf ("in slice ");
		}
		if (procs[calls->pid].entry == -1)
			if(v_opt)printf ("Library ");
		if(v_opt)printf ("call %s at %d\n",procs[calls->pid].proc_name,
			calls->stmt);
		calls = calls->next;
	}
	return status;
}

slice_proc (final_slice_sets,slice_sets,proc,active)
	int		proc;
	bit_set	final_slice_sets[],slice_sets[];
	bit_set	active;
{
	int		i,status;
	int		pid = proc;

	if(v_opt)printf ("enter slice proc on %s\n",procs[proc].proc_name); 
	if(is_bit_on (dont_descend,proc)) return; /* don't recurse */
	bit_on (dont_descend,proc);
	if(v_opt)printf ("start slice proc on %s\n",procs[proc].proc_name);
	status = 1;
    while (status){
		if (abandon_slice) return;
/*		printf ("pass start %s\n",procs[proc].proc_name); */
        status = slice_pass (final_slice_sets,slice_sets,proc,0,active);
/*		printf ("pass end   %s %d\n",procs[proc].proc_name,status); */
    }
	i = procs[proc].file_id;
	union_bit_set (final_slice_sets[i],slice_sets[i]);
	/*CheckW(QQQ);*/
	if(v_opt)printf ("end slice proc on %s\n",procs[proc].proc_name);
	bit_off (dont_descend,proc);
}

/*
*********************************************************************
*                                                                   *
*  main control for computing a slice                               *
*  criterion --                                                     *
*     file      file id of file containing stmt                     *
*     proc      proc containing criterion statement                 *
*     stmt      criterion statement (node)                          *
*     var_proc  proc id of var (if local) or 0 (if var global)      *
*     var       criterion var                                       *
*                                                                   *
*  results --                                                       *
*     slice_sets bit sets (one per file) marking stmts in slice     *
*     active     bit set marking procs in the slice                 *
*                                                                   *
*********************************************************************
*/

void
slice (file,proc,stmt,var_proc,var,final_slice_sets,active)
	int file,proc,stmt,var_proc,var;
	bit_set	final_slice_sets[],active;
{
	int		i,status,call_status;
	static	bit_set	*slice_sets,dont_slice = NULL;
	static	int	need = 1;

	abandon_slice = 0;
	if (!dont_slice){
		dont_slice = create_bit_set (n_procs+1);
		dont_descend = create_bit_set (n_procs+1);
	}
	clear_bit_set (dont_slice);
	if (need){
		need = 0;
		slice_sets = (bit_set *) malloc (n_files*sizeof(bit_set));
		if (!slice_sets){
			fprintf (stderr,"Out of memory in slice\n");
			exit(1);
		}
		for (i = 0; i < n_files; i++)
			slice_sets[i] = create_bit_set (1+files[i].n_stmts);
		/*
		printf ("creating slice sets for %d files\n",n_files);
		for (i = 0; i < n_files; i++){
			printf ("%d  %d  %d\n",i,final_slice_sets[i]->max,
				slice_sets[i]->max);
		}
		*/
	}
	clear_active();
    for (i = 0; i < n_files; i++)
        clear_bit_set (final_slice_sets[i]);
    for (i = 0; i < n_files; i++)
        clear_bit_set (slice_sets[i]);
    clear_bit_set (active);
    slice_tree (final_slice_sets,proc,active);
    set_criteria (file,proc,stmt,var_proc,var);
	/*
	slice_proc(final_slice_sets,slice_sets,proc,active);
	*/
	slice_up (final_slice_sets,slice_sets,proc,active,dont_slice);

}

# define LINE_LENGTH 67
void
print_slices (print_nodes,slices,active)
	bit_set	slices[],active;
	int		print_nodes;
{
	int			file,proc;
	int			stmt,count;
	int			line;

	proc = -1;
	while ((proc = get_next_member(active,proc)) >= 0){
		if (procs[proc].entry > 0){
			file = procs[proc].file_id;
			printf ("\n%s'%s\n",files[file].name,procs[proc].proc_name);
			count = 0;
			stmt = -1;
			while ((stmt = get_next_member(slices[file],stmt)) >= 0){
				if (print_nodes)count += printf ("%4d ",stmt);
				else {
					line = files[file].stmts[stmt].froml;
					count += printf ("%4d ",line);
				}
				if (count > LINE_LENGTH) {
					printf ("\n");
					count = 0;
				}
			}
			if (count) printf ("\n");
		}
	}
}

void
print_slices1 (print_nodes,slices,active)
	bit_set	slices[],active;
	int		print_nodes;
{
	int			file,proc;
	int			stmt,count;
	int			line,from,to;
	int			max = 0,f;
	static 		bit_set		map = NULL;

	if (!print_nodes && !map){
		for (f = 0; f < n_files; f++)
			if (max < files[f].n_lines) max = files[f].n_lines;
		map = create_bit_set (max+1);
	}
	for (file = 0; file < n_files; file++){
		count = printf ("    %sc: ",files[file].name);
		stmt = -1;
		if(!print_nodes)clear_bit_set (map);
		while ((stmt = get_next_member(slices[file],stmt)) >= 0){
			if (print_nodes)count += printf ("%4d ",stmt);
			else {
				from = files[file].stmts[stmt].froml;
				to = files[file].stmts[stmt].tol;
				bit_on (map,from);
				for (line = from+1; line <= to; line++)
					bit_on (map,line);
			}
			if (count > LINE_LENGTH) {
				printf ("\n");
				count = 0;
			}
		}
		if (!print_nodes){
			line = -1;
			while ((line = get_next_member(map,line)) >= 0){
				count += printf ("%4d ",line);
				if (count > LINE_LENGTH) {
					printf ("\n");
					count = 0;
				}
			}
		}
		if (count) printf ("\n");
	}
}
