# include "ansi_parse.h"
# include "lif.h"
# include <malloc.h>
# include <string.h>
	static  char    sccsid[] = "@(#)sym_tab.c	1.4  4/26/95";
static int scope_level = 1;
static symbol_table_entries	symbol_table[MAX_NEST];
static	max_local_id = 1;
static	max_global_id = 1;
static	in_typedef = 0;
# define MAX_ST 10000
static symbol_table_entries	closed_symbol_tables[MAX_ST];
static int	n_st = 0;

declared(var)
	char	*var;
{
	if (scope_level > 1) return;
	if(var)fprintf(h_file,"\t%s\n",var);
}

int	is_var_ptr_mod(t)
	token_ptr	t;
{
	type_ptr	mod;

	if (!t) return False;
	mod = t->desc.type_desc;
	while (mod){
		if (mod -> is == IS_PTR_TO) return True;
		mod = mod -> next;
	}
	return False;
}

int	is_type_ptr (t)
	type_ste_ptr	t;
{
	if (!t) return False;
	if (is_var_ptr_mod (t->token)) return True;
	if (t->detail  != STE_TYPEDEF) return False;

	if ( is_type_ptr (t -> type_entry)) return True;
	return False;
}

int	is_var_ptr(var)
	var_ste_ptr	var;
{
	/* is var declared pointer					*/
	if (is_var_ptr_mod(var->token)) return True;
	if (var->detail != STE_TYPEDEF) return False;
	/* var is a user defined type (might be pointer type) */

	if ( is_type_ptr (var -> type_entry)) return True;
	return False;
}

var_ste_ptr	get_var_members (var)
	var_ste_ptr	var;
{
	type_ste_ptr	t;

	if (!var) return NULL;
	if (var->detail == STE_ANON_STRUCT) return var->anon_entry;
	if (var->detail == STE_TAGGED_STRUCT)
		return var->tag_entry->vars;
	if (var->detail != STE_TYPEDEF) return NULL;
	t = var->type_entry;
	while (t){
		if (t->detail == STE_ANON_STRUCT) return t->anon_entry;
		if (t->detail == STE_TAGGED_STRUCT)
			return t->tag_entry->vars;
		if (t->detail != STE_TYPEDEF) return NULL;
		t = t->type_entry;
	}
	return NULL;

}


/*
struct hh {			 STE for variable 
	token_ptr	 token;
	token_ptr	 type_decl;
	int			 detail;
	type_ste_ptr type_entry;
	tag_ste_ptr	 tag_entry;
	var_ste_ptr	 anon_entry;
	int			 id;
	var_ste_ptr	 next;
};
*/

is_decl_array_or_pointer(t,is_array,is_pointer)
	type_ste_ptr	t;
	int			*is_array,*is_pointer;
{
	type_ptr    details;

	if (!t) return;
	details = t->token->desc.type_desc;
	while (details) {
		if (details->is == IS_ARRAY_OF) *is_array = 1;
		if (details->is == IS_PTR_TO) *is_pointer = 1;
		details = details -> next;
	}
	if ((!(*is_pointer && *is_array)) &&
		(t->type_decl->desc.decl->decl_flags & FLAG_TYPE_USER_NAME)){
		/* look at user defined type unless both array and pointer on */
		if (t->detail == STE_TYPEDEF)
			is_decl_array_or_pointer (t->type_entry,is_array,is_pointer);
		}
}

int is_var_array(v)
	var_ste_ptr  v;
{
	int		is_array,is_pointer;
	type_ptr	details;

	if(!v) return 0;
	details = v->token->desc.type_desc;
	while (details) {
		if (details->is == IS_ARRAY_OF) return 1;
		details = details -> next;
	}
	is_pointer = 0;
	is_array = 0;
	is_decl_array_or_pointer (v->type_entry,&is_array,&is_pointer);
	return is_array;

}

/*
*********************************************************************
*                                                                   *
* do_ids is called when a scope level is closed to output a list    *
* of all identifiers in LIF format                                  *
*                                                                   *
*********************************************************************
*/
do_ids()
{
	var_ste_ptr	v;
	int		class;
	int		is_array,is_pointer;
	type_ptr	details;

	class = scope_level == 1 ? LIF_GLOBAL_ID : LIF_LOCAL_ID;
	v = symbol_table[scope_level].var;
	while (v){
		is_array = 0;
		is_pointer = 0;
		details = v->token->desc.type_desc;
		while (details) {
			if (details->is == IS_ARRAY_OF) is_array = 1;
			if (details->is == IS_PTR_TO) is_pointer = 1;
			details = details -> next;
		}
		if ((!(is_pointer && is_array)) &&
			(v->type_decl->desc.decl->decl_flags &
				FLAG_TYPE_USER_NAME)){
	/* look at user defined type unless both array and pointer on */
			if (v->detail == STE_TYPEDEF)
				is_decl_array_or_pointer
					(v->type_entry,&is_array,&is_pointer);
		}
		fprintf(outfile,"%d(%d,%s",
			class,v->id,v->token->text);
		if (FLAG_SC_STATIC & v->type_decl->desc.decl->decl_flags)
			fprintf(outfile,",S");
		if (is_pointer) fprintf(outfile,",P");
		if (FLAG_SC_EXTERN & v->type_decl->desc.decl->decl_flags)
			fprintf(outfile,",X");
		if (is_array) fprintf(outfile,",A");
		fprintf(outfile,")");
		if (z_opt){
			fprintf (outfile,"    %s id",
				class == LIF_GLOBAL_ID ? "Global":"local");
			if (is_pointer) fprintf(outfile," pointer");
			if (is_array) fprintf(outfile," array");
		}
		fprintf(outfile,"\n");
		v = v -> next;
	}
}

/*
*********************************************************************
*                                                                   *
* do_formals is called when a fun decl is done to output a list     *
* of all formal parms in LIF format                                 *
*                                                                   *
*********************************************************************
*/
do_formals()
{
	var_ste_ptr	v;
	type_ptr	details;
	int			is_array = 0, is_pointer = 0;

	v = symbol_table[scope_level].var;
	while (v){
		is_array = 0;
		is_pointer = 0;
		details = v->token->desc.type_desc;
		while (details) {
			if (details->is == IS_ARRAY_OF) is_array = 1;
			if (details->is == IS_PTR_TO) is_pointer = 1;
			details = details -> next;
		}
		if ((!(is_pointer && is_array)) &&
			(v->type_decl->desc.decl->decl_flags &
				FLAG_TYPE_USER_NAME)){
	/* look at user defined type unless both array and pointer on */
			if (v->detail == STE_TYPEDEF)
				is_decl_array_or_pointer
					(v->type_entry,&is_array,&is_pointer);
		}
		fprintf(outfile,"%d(%d,%s",
			LIF_FORMAL_ID,v->id,v->token->text);
		if (is_pointer) fprintf(outfile,",P");
		if (is_array) fprintf(outfile,",A");
		fprintf(outfile,")");
		if (z_opt){
			fprintf (outfile,"    Formal parm");
			if (is_pointer) fprintf(outfile," pointer");
			if (is_array) fprintf(outfile," array");
		}
		fprintf(outfile,"\n");
		v = v -> next;
	}
}

/*
*********************************************************************
*                                                                   *
* reset id counter for a new procedure                              *
*                                                                   *
*********************************************************************
*/
start_local_decls()
{
	max_local_id = 1;
}

/*
*********************************************************************
*                                                                   *
* allocate an id from locals or globals                             *
*                                                                   *
*********************************************************************
*/
int current_global()
{
	return max_global_id - 1;
}
int current_local()
{
	return max_local_id - 1;
}
int alloc_local()
{
	return max_local_id++;
}
int alloc_global()
{
	return max_global_id++;
}
int current_id()
{
	if (scope_level == 1) return max_global_id - 1;
	return max_local_id - 1;
}

int	alloc_id()
{
	int		id;

	if (scope_level == 1) id = max_global_id++;
	else id = max_local_id++;

	return id;
}

/*
*********************************************************************
*                                                                   *
* modify the token (some typename e.g. int) with a flag to          *
* indicate the type being used                                      *
*                                                                   *
*********************************************************************
*/
make_decl (token,flag)
	token_ptr	token;
	unsigned int flag;
{
	token->desc.decl = alloc_decl();
	token->desc.decl->decl_flags = flag;
	token->desc.decl->vars = NULL;
	token->desc.decl->tag = NULL;
	token->desc.decl->members = NULL;
	token->desc.decl->member_ste = NULL;
	token->desc.decl->offset = 0;
}

/*
*********************************************************************
*                                                                   *
* modify an identifier (token) to indicate pointers, arrays,        *
* or style of function declaration                                  *
*                                                                   *
*********************************************************************
*/
modify_type (token,flag,style,formals)
	token_ptr	token,formals;
	unsigned int flag;
	int			style;
{
	type_ptr	new,old;

	new = alloc_type();
	new -> is = flag;
	new -> next = NULL;
	new -> style = style;
	new -> formals = formals;
	if (old = token->desc.type_desc){
		while (old->next) old = old->next;
		old->next = new;
	}
	else token->desc.type_desc = new;
}

type_ptr make_abstract_type (flag,style,formals)
	token_ptr	formals;
	unsigned int flag;
	int			style;
{
	type_ptr	new,old;

	new = alloc_type();
	new -> is = flag;
	new -> next = NULL;
	new -> style = style;
	new -> formals = formals;
	return new;
}

merge_abstract_type (old,new)
	type_ptr    new,old;
{
	if (old){
		while (old->next) old = old->next;
		old->next = new;
	}
}

/*
*********************************************************************
*                                                                   *
* stack a new scope level for the symbol table                      *
*                                                                   *
*********************************************************************
*/
open_scope()
{
	scope_level++;
	if (c_opt) printf ("Open scope level %d\n",scope_level);
	symbol_table[scope_level].var = NULL;
	symbol_table[scope_level].type = NULL;
	symbol_table[scope_level].tag = NULL;
}

/*
*********************************************************************
*                                                                   *
*  close a scope level and if c_opt print the variables, tags and   *
*  types.                                                           *
*                                                                   *
*********************************************************************
*/
close_scope()
{
	type_ste_ptr	types;
	var_ste_ptr		vars;
	tag_ste_ptr		tags;

	if (c_opt) printf ("Close scope level %d ==> st[%d]\n",
		scope_level,n_st);
	if (scope_level == 2) do_formals ();
	else do_ids();
	closed_symbol_tables[n_st].var = symbol_table[scope_level].var;
	closed_symbol_tables[n_st].type = symbol_table[scope_level].type;
	closed_symbol_tables[n_st++].tag = symbol_table[scope_level].tag;
	if (c_opt){
		vars = symbol_table[scope_level].var;
		printf ("\nS C O P E    L E V E L    %d    => st[%d]\n",
			scope_level,n_st-1);
		printf ("Declared variables:\n");
		while (vars){
			printf ("%s\n",vars->token->text);
			vars = vars -> next;
		}
		types = symbol_table[scope_level].type;
		printf ("Declared types:\n");
		while (types){
			printf ("%s\n",types->token->text);
			types = types -> next;
		}
		tags = symbol_table[scope_level].tag;
		printf ("Declared tags:\n");
		while (tags){
			printf ("%s\n",tags->tag->desc.decl->tag->text);
			tags = tags -> next;
		}
	}
	scope_level--;
}

/*
*********************************************************************
*                                                                   *
* look for a tag,  return the ste and if tag is found in            *
* the local scope, set local_flag to true.                          *
*                                                                   *
*********************************************************************
*/
tag_ste_ptr find_tag (tag,local_flag)
	char	*tag;
	int		*local_flag;
{
	int				level;
	tag_ste_ptr		vars;

	level = scope_level;

	/*printf ("look for tag %s\n",tag);*/
	*local_flag = True;
	while (level >= 0){
	/*	printf ("searching level %d\n",level); */
		vars = symbol_table[level].tag;
		while (vars){
			if (strcmp(vars->tag->desc.decl->tag->text,tag) == 0)
				return vars;
			vars = vars -> next;
		}
		*local_flag = False;
		level--;
	}
	/*printf ("tag %s not found\n",tag);*/
	return NULL;

}

/*
*********************************************************************
*                                                                   *
* search symbol table for a type name                               *
*                                                                   *
*********************************************************************
*/
type_ste_ptr find_type (type_name)
	char	*type_name;
{
	int				level;
	type_ste_ptr	t;

	if (!type_name) return NULL;
	level = scope_level;

	/*
	printf ("look for type_name %s\n",type_name);
	*/
	while (level >= 0){
	/*	printf ("searching level %d\n",level); */
		t = symbol_table[level].type;
		while (t){
			if (strcmp(t->token->text,type_name) == 0)
				return t;
			t = t -> next;
		}
		level--;
	}
	return NULL;

}

/*
*********************************************************************
*                                                                   *
*                                                                   *
*                                                                   *
*********************************************************************
*/
static tag_decl (type_desc,var_list,tag_entry_ptr,
					detail_ptr,with_members)
	token_ptr		type_desc,var_list;
	tag_ste_ptr		*tag_entry_ptr;
	int				*detail_ptr,with_members;
{
	tag_ste_ptr		new_tag,tag_entry = NULL;
	int				no_tag = 0;
	int				is_local_tag;

	if (type_desc->desc.decl->decl_flags & FLAG_TYPE_US){
		/*printf ("Do struct/union\n");*/
		/* if this decl has a tag, see if it exists already */
		if (type_desc->desc.decl->tag) tag_entry = find_tag(
			type_desc->desc.decl->tag->text,&is_local_tag);
		else /* this structure has no tag => anon struct */
			{tag_entry = NULL; no_tag = 1;}
		if(c_opt)printf ("entry %d  no_tag (%d): ",tag_entry,no_tag);
		if (!tag_entry && !no_tag){ /* tag not found anywhere */
			tag_entry = new_tag = alloc_tag_ste();
			new_tag -> next = symbol_table[scope_level].tag;
			new_tag ->tag = type_desc;
			new_tag ->vars= type_desc->desc.decl->member_ste;
			symbol_table[scope_level].tag = new_tag;
			if(c_opt)
				printf ("declare new %s tag (%d) at scope level %d ",
					is_local_tag?"local":"non-local",tag_entry,
					scope_level);
		}
		else if (is_local_tag){ /* tag exists locally */
		/* if the new decl has members and the existing
			decl does not, then move the member list from
			the new decl to the tagged_ste */
			if (tag_entry){
				if (!tag_entry->vars) tag_entry->vars =
					type_desc->desc.decl->member_ste;
			}
		}
		else if (no_tag){ /* anon struct */
		}
		else { /* tag found non-local */
			/* declare tag as local if either:
				(1) tag has members or
				(2) tag does not have variables
			*/
			if ((type_desc->desc.decl->members) ||
					!var_list || with_members){
				tag_entry = new_tag = alloc_tag_ste();
				new_tag -> next = symbol_table[scope_level].tag;
				new_tag ->tag = type_desc;
				new_tag ->vars= type_desc->desc.decl->member_ste;
				symbol_table[scope_level].tag = new_tag;
				if(c_opt)printf (
					"declare new masking tag (%d) at scope level %d ",
					tag_entry,scope_level);
			}
		}
		if(c_opt)if (type_desc->desc.decl->tag)
			printf ("tag %s ",type_desc->desc.decl->tag->text);
		if(c_opt)if (type_desc->desc.decl->decl_flags & FLAG_TYPE_UNION)
			printf ("union ");
			else printf ("struct ");
		if(c_opt)printf ("declared %s members ",
		type_desc->desc.decl->members ?"with":"without");
		if(c_opt)printf ("and %s variables\n",
			var_list?"with":"without");
	}
	*tag_entry_ptr = tag_entry;
	*detail_ptr = no_tag;
	if(c_opt)printf ("tag decl returns tag_entry %d and no tag %d\n",
		tag_entry,no_tag);
}


/*
*********************************************************************
*                                                                   *
*  struct_decl is called to reduce the member list of a struct/union*
*                                                                   *
*  type_desc is a list of declarations (type tokens)                *
*  each declaration has a type and a list of variables              *
*                                                                   *
*  struct_decl returns a symbol table for the members of the struct *
*                                                                   *
*********************************************************************
struct_or_union_specifier
	: struct_or_union identifier
		{
			$1->desc.decl->tag = $2;
			decl ($1,NULL,True);
		}
		'{' struct_declaration_list '}'
		{
			$1->desc.decl->member_ste = struct_decl($5);
			$1->desc.decl->members = $5;
		}
	| struct_or_union '{' struct_declaration_list '}'
		{
			$1->desc.decl->members = $3;
			$1->desc.decl->member_ste = struct_decl($3);
		}
*/
var_ste_ptr	struct_decl (type_desc,off)
	token_ptr	type_desc;
	int			*off;
{
	token_ptr	d,vars;
	var_ste_ptr	st = NULL,new_var;
	int			detail,no_tag;
	int			off_set = 0;
	var_ste_ptr	anon,last;
	tag_ste_ptr tag_entry;
	type_ste_ptr type_entry = NULL;

	if(c_opt)printf ("struct decl: ");
	d = type_desc;
	while (d){
		vars = d->desc.decl->vars;
		if(c_opt)printf ("(%08o) ",d->desc.decl->decl_flags);
		detail = STE_NONE;
		anon = NULL;
		tag_entry = NULL;
		if (d->desc.decl->decl_flags & FLAG_TYPE_US){
		/*
			if (d->desc.decl->tag){
				tag_entry = find_tag(d->desc.decl->tag->text,
					&is_local_tag);
				if (tag_entry) detail = STE_TAGGED_STRUCT;
				else detail = STE_NONE;
			}
			else {
				detail = STE_ANON_STRUCT;
				anon = d->desc.decl->member_ste;
			}
		*/
			tag_decl (d,vars,&tag_entry,&no_tag,
					d->desc.decl->members!=NULL);
			if (no_tag){
				detail = STE_ANON_STRUCT;
				anon = d->desc.decl->member_ste;
			}
			else {
				if (tag_entry) detail = STE_TAGGED_STRUCT;
				else detail = STE_NONE;
			}
		}
		else if (d->desc.decl->decl_flags & FLAG_TYPE_USER_NAME){
			type_entry = find_type (d->text);
			if (type_entry) detail = STE_TYPEDEF;
		}
		else {
			detail = STE_NONE;
			anon = NULL;
		}
		last = st;
		if(last) while (last->next) last = last -> next;
		while(vars){
			if(c_opt)printf (" %s <detail %d> ",vars->text,detail);
			new_var = alloc_var_ste();
			new_var -> detail = detail;
			new_var -> next = NULL;
			new_var -> type_entry = type_entry;
			new_var -> tag_entry = tag_entry;
			new_var -> anon_entry = anon;
			new_var -> token = vars;
			new_var -> type_decl = d;
			new_var -> id = ++off_set;
			new_var -> addr = 0;
			if (last) last-> next = new_var;
			else st = new_var;
			last = new_var;
			if (!is_var_ptr(new_var)){
				if(c_opt)
					printf ("call insert from struct_decl add(%s)\n",
					vars->text);
				insert_struct_members (vars->text,
					get_var_members (new_var),&st,&off_set);
				if(last) while (last->next) last = last -> next;
			}
			vars = vars->next;
		}

		d = d -> next;
	}
	if(c_opt)printf ("\n");
	if(c_opt){ var_ste_ptr	s;
		printf ("Declare a struct: (members)\n");
		s = st;
		while (s){
			printf ("\t%s \n",s->token->text); fflush(stdout);
			s = s -> next;
		}
	}
	*off = off_set;
	return st;
}

insert_ptr_var(scope,var)
	int	scope;
	var_ste_ptr	var;
{
	var -> next = symbol_table[scope].var;
	symbol_table[scope].var = var;
	declared(var->token->text);
}

/*
*********************************************************************
*                                                                   *
*  used to declare an undeclared old style parm (declared as int)   *
*                                                                   *
*********************************************************************
*/

insert_var_decl (var)
	token_ptr	var;
{
	var_ste_ptr		new_var;

	new_var = alloc_var_ste();
	new_var -> token = var;
	new_var -> type_decl = create_token((char *)NULL);
	new_var -> next = symbol_table[scope_level].var;
	new_var -> detail = 0;
	new_var -> type_entry = NULL;
	new_var -> tag_entry = NULL;
	new_var -> anon_entry = NULL;
	new_var -> id = alloc_id();
	new_var -> addr = 0;
	symbol_table[scope_level].var = new_var;
	make_decl(new_var->type_decl,FLAG_TYPE_INT);
	/* used to declare an undeclared old style parm
		(declared as int)							*/
}

insert_struct_members (base,members,st,off_set)
	char	*base;
	var_ste_ptr	members,*st;
	int		*off_set;

{
	char		name[3000];
	var_ste_ptr	new_var,last;
	
	last = *st;
	if (last){
		while (last->next) last = last-> next;
	}
	while (members){
		sprintf (name,"%s.%s",base,members->token->text);
		if(c_opt)printf ("INS struct mem new: %s\n",name);
		new_var = alloc_var_ste();
		new_var -> token = create_token(name);
		new_var -> token -> desc = members -> token -> desc;
		new_var -> type_decl = members -> type_decl;
		new_var -> next = NULL;
		new_var -> detail = members -> detail;
		new_var -> type_entry = members -> type_entry;
		new_var -> tag_entry = members -> tag_entry;
		new_var -> anon_entry = members -> anon_entry;
		new_var -> id = ++(*off_set);
		new_var -> addr = 0;
		if (last){
			last->next = new_var;
		}
		else {
			*st = new_var;
		}
		last = new_var;
		members = members->next;
	}
}

insert_members (base,members)
	char	*base;
	var_ste_ptr	members;
{
	char		name[3000];
	var_ste_ptr	new_var,last;
	
	last = symbol_table[scope_level].var;
	if (last){
		while (last->next) last = last-> next;
	}
	while (members){
		sprintf (name,"%s.%s",base,members->token->text);
		/* printf ("new: %s\n",name); */
		new_var = alloc_var_ste();
		new_var -> token = create_token(name);
		new_var -> token -> desc = members -> token -> desc;
		new_var -> type_decl = members -> type_decl;
		new_var -> next = NULL;
		new_var -> detail = members -> detail;
		new_var -> type_entry = members -> type_entry;
		new_var -> tag_entry = members -> tag_entry;
		new_var -> anon_entry = members -> anon_entry;
		new_var -> id = alloc_id();
		new_var -> addr = 0;
		/*
		symbol_table[scope_level].var = new_var;
		*/
		declared(name);
		if (last){
			last->next = new_var;
		}
		else {
			symbol_table[scope_level].var = new_var;
		}
		last = new_var;
		/*
		if (!is_var_ptr(new_var)){
			insert_members (name,
				get_var_members (new_var));
		}
		*/
		members = members->next;
	}
}

/*
*********************************************************************
*                                                                   *
*   Declare the variables on var_list as type type_desc in the      *
*   current symbol table level.                                     *
*                                                                   *
*   with_members is used to declare a structure tag with members    *
*   from inside a structure (i.e. nested structures)                *
*                                                                   *
*********************************************************************
*/
decl (type_desc,var_list,with_members)
	token_ptr	type_desc,var_list;
	int			with_members;
{
	token_ptr		var,enum_const;
	type_ste_ptr	new_type,type_entry;
	var_ste_ptr		new_var;
	tag_ste_ptr		tag_entry = NULL;
	int				no_tag = 0,pid;
	type_ptr		type_mod;

	if(c_opt)printf ("Do Decl at line %d\n",type_desc?type_desc->
		at.line_start:lineno);
	var = var_list;
	if (type_desc){
		if (type_desc->desc.decl->decl_flags & FLAG_TYPE_USER_NAME){
			type_entry = find_type (type_desc->text);
			if(c_opt)printf ("at line %d using user defined type: ",
				lineno);
			if(c_opt)printf ("%s",type_desc->text);
			if(c_opt)if (type_entry)printf (
					" %s\n",type_entry->token->text);
				else printf (" not found\n");
		}
		else type_entry = NULL;
		if (type_desc->desc.decl->decl_flags & FLAG_TYPE_US){
			tag_decl (type_desc,var_list,&tag_entry,&no_tag,
				with_members);
		}
		if (type_desc->desc.decl->decl_flags & FLAG_TYPE_ENUM){
			enum_const = type_desc->desc.decl->members;
			while (enum_const){
				if(c_opt)printf ("enum %s\n",enum_const->text);

				new_var = alloc_var_ste();
				new_var -> token = enum_const;
				new_var -> type_decl = type_desc;
				new_var -> next = symbol_table[scope_level].var;
				new_var -> detail = 0;
				new_var -> type_entry = NULL;
				new_var -> tag_entry = NULL;
				new_var -> anon_entry = NULL;
				new_var -> id = alloc_id();
				new_var -> addr = 0;
				symbol_table[scope_level].var = new_var;
				enum_const = enum_const->next;
			}
		}
		if (type_desc->desc.decl->decl_flags & FLAG_SC_TYPEDEF){
			/* var_list is list of type names */
			while(var){
				if(c_opt)printf ("User defined type: %s  at level %d\n",
					var->text,scope_level);
				new_type = alloc_type_ste();
				new_type -> next = symbol_table[scope_level].type;
				new_type -> token = var;
				new_type -> type_decl = type_desc;
				new_type -> anon_entry = NULL;
				if (type_entry) new_type -> detail = STE_TYPEDEF;
				else if (tag_entry)new_type->detail = STE_TAGGED_STRUCT;
				else if (no_tag) {
					new_type -> detail = STE_ANON_STRUCT;
					new_type -> anon_entry =
						type_desc->desc.decl->member_ste;
				}
				else new_type->detail = STE_NONE;
				new_type -> tag_entry = tag_entry;
				new_type -> type_entry = type_entry;
				symbol_table[scope_level].type = new_type;
				var = var -> next;
			}
		}
	else {
		 /* variable declaration */
		 if(c_opt)printf ("Do vars %s (%d)\n",
			tag_entry?"tag found":"tag not found", no_tag);
		while(var){
			if(c_opt)printf ("variable declared: %s  at level %d\n",
				var->text,scope_level);
			type_mod = var->desc.type_desc;
			if (type_mod && (type_mod->is == IS_FUNC_RET)){
				if(c_opt)printf ("%s Is a function\n",var->text);
			}
			else {
			new_var = alloc_var_ste();
			new_var -> next = symbol_table[scope_level].var;
			new_var -> detail = 0;
			if (type_entry) new_var -> detail = STE_TYPEDEF;
			new_var -> type_entry = type_entry;
			if (tag_entry){
				new_var -> tag_entry = tag_entry;
				new_var -> detail = STE_TAGGED_STRUCT;
			}
			else new_var -> tag_entry = NULL;
			if (no_tag){
				new_var->anon_entry = type_desc->desc.decl->member_ste;
				new_var -> detail = STE_ANON_STRUCT;
			}
			else new_var -> anon_entry = NULL;
			new_var -> token = var;
			new_var -> type_decl = type_desc;
			new_var -> id = alloc_id();
			new_var -> addr = 0;
			symbol_table[scope_level].var = new_var;
			declared(var->text);
			/*
			printf ("Is %s ",var->text);
			if ( get_var_members (new_var)) printf ("struct ");
			if ( is_var_ptr(new_var)){
				printf (" yes*\n");
			}
			else {printf ("not a pointer\n"); }
			*/
			if (!is_var_ptr(new_var)){
				insert_members (var->text,
					get_var_members (new_var));
				if ((current_id() - new_var->id) > 0){
					if (scope_level > 1) pid = current_proc->proc_id;
					else pid = 0;
					/*
					if (scope_level > 1) printf (
						"cur proc is %d %s ",current_proc->proc_id,
						current_proc->proc->text);
					else printf ("global");
					printf (" struct %s\n",var->text);
					*/
					fprintf (outfile,"%d(%d,%d,%d)",LIF_STRUCT,pid,
						new_var->id,current_id() - new_var->id);
					if(z_opt) fprintf (outfile,
						"     %s is structure with %d members",
						var->text,current_id() - new_var->id);
					fprintf (outfile,"\n");
				}
			}
			}
			var = var -> next;
		}
	}
	}
}

/*
*********************************************************************
*                                                                   *
* called by scanner to look for possible TYPENAME tokens            *
*                                                                   *
*********************************************************************
*/
int is_type_name (t,local_flag)
	char	*t;
	int		*local_flag;
{
	int				level;
	type_ste_ptr	types;
	var_ste_ptr		vars;

	level = scope_level;

	*local_flag = True;
	while (level >= 0){
		types = symbol_table[level].type;
		while (types){
			if (strcmp(types->token->text,t) == 0) return True;
			types = types -> next;
		}
		vars = symbol_table[level].var;
		while (vars){
			if (strcmp(vars->token->text,t) == 0) return False;
			vars = vars -> next;
		}
		*local_flag = False;
		level--;
	}
	return False;

}

/*
*********************************************************************
*                                                                   *
*  find the STE entry for an identifier                             *
*  tabel is NULL for searching the full symbol table (normal case)  *
*                                                                   *
*  A table is passed when looking for structure members. Where the  *
*  table is a symbol table for the structure.                       *
*                                                                   *
*********************************************************************
*/
var_ste_ptr look_up_id (table,name,scope)
	char	*name;
	int		*scope;
	var_ste_ptr	table;
{
	int				level;
	var_ste_ptr		vars;

	level = scope_level;

	if (table){
			*scope = 0;
			vars = table;
			while (vars){
				if (strcmp(vars->token->text,name) == 0) return vars;
				vars = vars -> next;
			}
	}
	else {
		while (level >= 0){
			*scope = level;
			vars = symbol_table[level].var;
			while (vars){
				if (strcmp(vars->token->text,name) == 0) return vars;
				vars = vars -> next;
			}
			level--;
		}
	}
	return NULL;
}

/*
*********************************************************************
*                                                                   *
*  scanner/parser communication about typedef statements            *
*                                                                   *
*********************************************************************
*/
start_typedef()
{
	in_typedef = True;
}

end_typedef()
{
	in_typedef = False;
}

int top_scope() { return scope_level == 1;}

/*
*********************************************************************
*                                                                   *
* Print the members of a structure                                  *
*                                                                   *
*********************************************************************
*/
static print_memb(prefix,v)
	var_ste_ptr		v;
	char			*prefix;
{
	var_ste_ptr		vars = v;
	char			buff[2000];
	static int		n;
	static var_ste_ptr	history[1000];
	int				i;

	for (i = 0; i < n; i++){
		if (history[i] == v){
			printf ("recursive\n");
			return;
		}
	}
	history[n++] = v;
	if (!vars) printf ("no members\n");
	while (vars){
		printf ("%s.%s " ,prefix,vars->token->text);
		vars = vars->next;
	}
	vars = v;
	while (vars){
		if (vars->detail == STE_ANON_STRUCT) {
			sprintf (buff ,"%s.%s",prefix,vars->token->text);
			printf ("\n\t\tanon members: ");
			print_memb(buff,vars->anon_entry);
		}
		else if (vars->detail == STE_TAGGED_STRUCT) {
			sprintf (buff ,"%s.%s",prefix,vars->token->text);
			printf ("\n\t\ttagged members: ");
			print_memb(buff,vars->tag_entry->vars);
		}
		vars = vars->next;
	}
	n--;
}

/*
*********************************************************************
*                                                                   *
*   See if variable is really a struct and see to printing members  *
*                                                                   *
*********************************************************************
*/
static print_types (t)
	type_ste_ptr	t;
{
	if (t->detail == STE_TYPEDEF){
		printf ("%s ",t->type_entry->token->text);
		if (t->type_entry->detail) print_types (t->type_entry);
	}
	else if (t->detail == STE_TAGGED_STRUCT){
		print_memb(t->token->text,t->tag_entry->vars);
	}
	else if (t->detail == STE_ANON_STRUCT){
		print_memb(t->token->text,t->anon_entry);
	}
}

/*
*********************************************************************
*                                                                   *
*  Print the symbol tables (even closed) (-s option)                *
*                                                                   *
*********************************************************************
*/
print_st()
{
	type_ste_ptr	types;
	var_ste_ptr		vars;
	tag_ste_ptr		tags;
	token_ptr		token,t;
	type_ptr		details;
	int		i;
	static	char	*ty[] = {"scalar","fun ret","array of","ptr to"};

	list_procs();
	printf ("\n%10sS Y M B O L    T A B L E\n\n","");
	for (i = 0; i < n_st; i++){
		vars = closed_symbol_tables[i].var;
		types = closed_symbol_tables[i].type;
		tags = closed_symbol_tables[i].tag;
		if(vars||tags||types)printf ("\n\tSymbol table %d\n",i);
		if (vars)printf ("Declared variables:\n");
		while (vars){
			printf ("\t%-15s (%03d.%02d..%02d)  ",vars->token->text,
				vars->token->at.line_start,
				vars->token->at.col_start,
				vars->token->at.col_end);
			printf ("%03d ",vars->id);
			printf ("F(%07o) <%d>  ",
				vars->type_decl->desc.decl->decl_flags,
				vars->detail);
			/*
			if (vars->type_decl->desc.decl->decl_flags &
				FLAG_TYPE_USER_NAME){
				printf ("Defined type ");
			} else if (vars->type_decl->desc.decl->decl_flags &
				FLAG_TYPE_US){
				printf ("struct/union ");
			}
			*/
			details = vars->token->desc.type_desc;
			while (details){
				printf ("%s ",ty[details->is]);
				details = details->next;
			}
			print_flags (vars->type_decl->desc.decl->decl_flags);
			if (vars->tag_entry) printf ("{%s} ",
				vars->tag_entry->tag->desc.decl->tag->text);
			if (vars->type_entry) printf ("[%s] ",
				vars->type_entry->token->text);
			printf ("\n");
			if (vars->detail == STE_ANON_STRUCT){
				printf ("%20s","members: ");
				print_memb(vars->token->text,vars->anon_entry);
				printf ("\n");
			}
			else if (vars->detail == STE_TAGGED_STRUCT){
				printf ("%20s","members: ");
				print_memb(vars->token->text,vars->tag_entry->vars);
				printf ("\n");
			}
			vars = vars -> next;
		}
		if(types)printf ("Declared types:\n");
		while (types){
			printf ("\t%-15s (%d.%d..%d) <%d>  ",types->token->text,
				types->token->at.line_start,
				types->token->at.col_start,
				types->token->at.col_end,
				types->detail);
			details = types->token->desc.type_desc;
			while (details){
				printf ("%s ",ty[details->is]);
				details = details->next;
			}
			if (types->detail) print_types (types);
			printf ("\n");
			types = types -> next;
		}
		if(tags)printf ("Declared tags:\n");
		while (tags){
			token = tags->tag->desc.decl->tag;
			printf ("\t%-15s (%d.%d..%d)",token->text,
				token->at.line_start,
				token->at.col_start,
				token->at.col_end);
			/*
			if (tags->tag->desc.decl->members){
				printf (" with members");
				t = tags->tag->desc.decl->members;
				while (t){
					if (t->desc.decl) print_decl (t,
						t->desc.decl->vars);
					t = t -> next;
				}
			}
			*/
			printf ("%20s","members: ");
			print_memb("",tags->vars);
			printf ("\n");
			tags = tags -> next;
		}
	}
	printf ("\n");
}

int is_ptr_to_type_ptr (te)
	type_ste_ptr	te;
{
	int		n_ptr = 0;
	type_ptr    tp = NULL;

	if (!te) return 0;
	if (te->token){
		tp = te->token->desc.type_desc;
		while (tp){
			if (tp->is == IS_PTR_TO) n_ptr++;
			tp = tp->next;
		}
	}
	if (!te->type_decl) return n_ptr;
	if (te->detail == STE_TYPEDEF){
			n_ptr += is_ptr_to_type_ptr (te->type_entry);
	}
	return n_ptr;
}

int is_ptr_to_ptr(t,nx)
	token_ptr	t;
	int			*nx;
{
	int		n_ptr = 0;
	type_ptr	tp = NULL;
	type_ste_ptr type_entry;

	if (!t) return 0;
	tp = t ->desc.type_desc;
	while (tp){
		if (tp->is == IS_PTR_TO) n_ptr++;
		tp = tp -> next;
	}
	*nx = n_ptr;
/*	if (n_ptr > 1) return n_ptr; */
	if (t->desc.decl->decl_flags & FLAG_TYPE_USER_NAME){
		type_entry = find_type (t->text);
		if (type_entry){
			n_ptr += is_ptr_to_type_ptr (type_entry);
		}
	}
	return n_ptr;
}

static token_ptr get_base_type(t)
	token_ptr	t;
{
	type_ste_ptr type_entry;

	type_entry = find_type (t->text);
	if (type_entry){
		while (type_entry->detail == STE_TYPEDEF){
			type_entry = type_entry -> type_entry;
		}
		return type_entry->type_decl;
	}
	return NULL;
}

token_ptr fake_var_decl(t)
	token_ptr	t;
{
	decl_ptr	ty;
	token_ptr	var_token;
	token_ptr	new_token,type_token;
	char		buff[1000],name[1000],*at;
	static int	ix = 0;
	int			save_scope;
	int			nx,np;

	if(c_opt)printf ("make malloc at %d on line %d\n",stmtno,lineno);
	var_token = alloc_token();
	at = strrchr (the_current_file,'/');
	if (at) at++;
	else at = the_current_file;
	strcpy (name,at);
	at = strchr (name,'.');
	if (at) *at = '\0';
	sprintf (buff,"@%s#%d[%d]",name,lineno,ix++);
	var_token->text = malloc(strlen(buff) + 1);
	var_token->text = strcpy(var_token->text,buff);
	var_token->next = NULL;
	if ((!t) || ((np = is_ptr_to_ptr(t,&nx)) > 1)){
		type_token = alloc_token();
		make_decl(type_token,FLAG_SC_STATIC|FLAG_TYPE_CHAR);
		modify_type (var_token,IS_PTR_TO,NO_STYLE,NULL);
	}
	else {
		if(nx)type_token = t;
		else {
			type_token = get_base_type(t);
			if (!type_token) type_token = t;
			else {
				new_token = alloc_token();
				new_token -> text = type_token->text;
				new_token -> at = t -> at;
				new_token -> desc = type_token ->desc;
				ty = new_token -> desc.decl = alloc_decl();
				ty -> decl_flags = (type_token->desc.decl->decl_flags)
					& (~FLAG_SC_TYPEDEF);
				ty -> vars = type_token->desc.decl->vars;
				ty -> tag = type_token->desc.decl->tag;
				ty -> members = type_token->desc.decl->members;
				ty -> member_ste = type_token->desc.decl->member_ste;
				ty -> offset = type_token->desc.decl->offset;
				type_token = new_token;
			}
		}
	}
	var_token->desc.any = NULL;
	decl(type_token,var_token,0);
	save_scope = scope_level;
	scope_level = 1;
	scope_level = save_scope;
	return var_token;
}
