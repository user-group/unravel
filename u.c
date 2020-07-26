# include <stdio.h>
# include <string.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <X11/Xlib.h>
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <X11/Shell.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/SimpleMenu.h>
# include <X11/Xaw/SmeBSB.h>
# include <X11/Xaw/SmeLine.h>
# include <X11/Xaw/MenuButton.h>
# include <X11/Xaw/Command.h>
# include <X11/Xaw/Label.h>
# include <X11/Xaw/Paned.h>
# include <X11/Xaw/List.h>
# include <X11/Xaw/Box.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/Viewport.h>
# include <stdlib.h>
# include <time.h>
# include "sets.h"
# include "MultiSlice.h"
# include "slice.h"
# include "config.h"

//static  char    sccsid[] = "@(#)u.c	1.12  7/26/96";

# define WIDTH 450

# define XtNrunningFG "runningFG"
# define XtNrunningBG "runningBG"
typedef enum {select_action,show,tree} proc_action;
Widget	QQQ;
Widget	top;
Widget	help_label;
Widget	progress;
typedef struct {
	char	*proc;
	char	*var;
	Widget	*state_label;
	Widget	slice_label;
	Widget	display_label;
	Widget	var_list;
	Widget	slicewin;
	Widget	history_list;
	Widget	var_popup,proc_popup;
	char	*ptext,*stext;
	char	*root_file;
	int		slice_var_index,pid,header;
	int		is_primary;
	proc_action reason;
	int		is_running;
	Pixel	active_fg,active_bg;
} proc_state_rec,*proc_state_ptr;

extern	char version[];
long		addr,start_addr;
int		current_proc;
static char	home[] = HOME;

Widget		slicewin;

static		char srcfile[1000] = " ";

int SliceTrace;
	int			slice_on_var = 0;
	int			dice_on_var = 0;
	bit_set		*slice_set,*dice_set,*alt_slice_set,
				*primary_set,*secondary_set;
char	**proc_name_list,**file_name_list;
char	***local_vars,***global_vars;
int		*proc_ids,n_proc_list,**global_ids;
int		*line_map;



bit_set	active;

void BuildSliceSrc (MultiSliceWidget w, int n, MultiSliceFilesPtr f, int nlines);
void MultiSliceHook(MultiSliceWidget w, XEvent *event, String *parm, Cardinal *n);

void SliceRedraw (MultiSliceWidget w);
void SliceClearAll(MultiSliceWidget w);
void clear_history(void);
void save_slice (int var_pid, int var_id, int file_id, int stmt_id, int is_partial,
			int n_files, bit_set slices[]);
	
void SliceSet (MultiSliceWidget w, int line_from, int col_from, int line_to, int col_to, int redraw);

void load_slice (Widget w, int index, int n_files, bit_set slices[],int line_map[]);

void MultiSliceSetHook(MultiSliceWidget w, void (*h)());
	
extern void SliceClearAll(MultiSliceWidget w);
extern void verify_input(int i);
void setup_history(char *name);
void slice_tree();

int update_progress(int old);
int read_k_file(char *name);
int read_link_file(char *name);

int 
update_progress (int old)
{
	int		n,at,f;
	char	label[1000];

	n = 0;
	for (f = 0 ; f < n_files; f++){
		at = -1;
		while ((at = get_next_member(slice_set[f],at)) >= 0)
			n++;
	}
	if (old != n){
		sprintf (label,"%5d nodes",n);
		XtVaSetValues (progress,XtNlabel,label,NULL);
	}
	return n;
}

void
button_help (w,mess,e,ok)
	Widget	w;
	char	*mess;
	XEvent	*e;
	Boolean	*ok;
{
	/* printf ("%s\n",mess); */
	XtVaSetValues (help_label,
		XtNlabel, (XtArgVal) mess,
		NULL);
}

void
set_button_help (w,mess)
	Widget	w;
	char	*mess;
{
	static char buff[] = "Describe object under mouse pointer";
	XtAddEventHandler (w,EnterWindowMask,False,button_help,mess);
	XtAddEventHandler (w,LeaveWindowMask,False,button_help,buff);
}

void
update_label(state)
	proc_state_ptr		state;
{
	char	buff[1000];
	char	*at,*name;

	if (state->pid){
		name = files[procs[state->pid].file_id].name;
		at = strrchr (name,'/');
		if (at) at++;
		else at = name;
		sprintf (buff,"File: %sc %s %s %s %s",
			at,
			"Proc:",state->proc,"Var:",
			state->slice_var_index?
				procs[state->pid].locals[state->slice_var_index].name:
			"<none>");
		XtVaSetValues (*(state->state_label),
			XtNlabel,			(XtArgVal) buff,
		NULL);
	}
	else {
		name = file_name_list[state->header];
		at = strrchr (name,'/');
		if (at) at++;
		else at = name;
		sprintf (buff,"File: %s %s %s %s %s",
			at,
			"Proc:","<global var>","Var:",
			state->slice_var_index?globals[state->slice_var_index].name:
			"<none>");
		XtVaSetValues (*(state->state_label),
			XtNlabel,			(XtArgVal) buff,
		NULL);
	}
	/*
		printf ("proc %s local %s\n",procs[state->pid].proc_name,
			procs[state->pid].locals[list->list_index].name);
		state->slice_var_index = ix;
		printf ("%s\n",globals[ix].name);
		files[procs[state->pid].file_id].name,
	XawListChange (state->var_list,local_vars[list->list_index],
	XawListChange (state->var_list,list,0,0,True);
	*/
}

void
make_line_map()
{
	int		i,sum;

	line_map = (int *) malloc (n_files*sizeof(int));
	if (!line_map) {
		fprintf (stderr,"out of memory\n");
		exit(1);
	}
	sum = 0;
	for (i = 0; i < n_files; i++){
		line_map[i] = sum;
		sum += files[i].n_lines;
	}
}

void
disp_to_file_and_line (disp_line,filex,line)
	int		disp_line,*filex,*line;
{
	int		f,lx;

	lx = disp_line;
	for (f = 0; f < n_files; f++){
		if (lx > files[f].n_lines){
			lx -= files[f].n_lines;
		}
		else {
			*filex = f;
			*line = lx;
			return;
		}
	}
	*filex = 0;
	*line = 0;
}

void
make_head_list()
{
	int		k,i,n;

	n = n_heads;
	file_name_list = (char **) malloc ((n_heads+2)*sizeof(char *));
	global_vars = (char ***) malloc ((n+2)*sizeof(char **));
	global_ids = (int **) malloc ((n+2)*sizeof(int *));
	file_name_list[0] = "No Selection";
	for (i = 0; i < n_heads; i++){
		file_name_list[i+1] = headers[i].header_file;
		global_vars[i+1] = (char **)
			malloc (sizeof(char *)*(headers[i].n+2));
		global_vars[i+1][0] = "No Selection";
		global_ids[i+1] = headers[i].ids;
		for (k = 0; k < headers[i].n; k++){
			global_vars[i+1][k+1] = headers[i].names[k];
		}
		global_vars[i+1][headers[i].n+1] = NULL;
	}
	file_name_list[n_heads+1] = NULL;
}

void
make_proc_list()
{
	int		k,i,n;

	n = 0;
	for (i = 1; i <= n_procs; i++)
		if (procs[i].entry != -1) n++;
	proc_ids = (int *) malloc((n+1)*sizeof(int));
	proc_name_list = (char **) malloc ((n+2)*sizeof(char *));
	local_vars = (char ***) malloc ((n+1)*sizeof(char **));
	n_proc_list = n;
	n = 1;
	proc_name_list[0] = "No Selection";
	for (i = 1; i <= n_procs; i++)
		if (procs[i].entry != -1){
			proc_name_list[n] = procs[i].proc_name;
			proc_ids[n] = i;
			local_vars[n] = (char **)
				malloc (sizeof(char *)*(procs[i].n_locals+2));
			local_vars[n][0] = "No Selection";
			for (k = 1; k <= procs[i].n_locals; k++)
				local_vars[n][k] = procs[i].locals[k].name;
			local_vars[n][procs[i].n_locals+1] = NULL;
			n++;
		}
	proc_name_list[n] = NULL;
}

char *get_file(np,p)
	int		np;
	char	*p[];
{
	int		status;
	int		v_opt = 0;
	int		i,fx;
	int		file,stmt,proc,var;

	if (np < 2) exit(1);
	fx = 0;
	for (i = 1; i < np; i++){
		if (p[i][0] == '-'){
			if (strcmp("-v",p[i]) == 0)v_opt = 1;
		}
		else fx = i;
	}
	i = strlen(p[fx]);
	p[fx][i-1] = '\0';
	status = read_k_file(p[fx]);
	if(status){
		fprintf (stderr,"%sc: K file status %d\n",p[fx],status);
		exit(0);
	}
	status = read_link_file(p[fx]);
	if(status){
		fprintf (stderr,"%sc: LINK file status %d\n",p[fx],status);
		exit(1);
	}
	if(v_opt)verify_input(0);
	setup_history(p[fx]);
	active = create_bit_set (n_procs+1);
	slice_set = (bit_set *) malloc (n_files*sizeof(bit_set));
	for (i = 0; i < n_files; i++)
		slice_set[i] = create_bit_set(files[i].n_stmts+1);
	alt_slice_set = (bit_set *) malloc (n_files*sizeof(bit_set));
	for (i = 0; i < n_files; i++)
		alt_slice_set[i] = create_bit_set(files[i].n_stmts+1);
	dice_set = (bit_set *) malloc (n_files*sizeof(bit_set));
	for (i = 0; i < n_files; i++)
		dice_set[i] = create_bit_set(files[i].n_stmts+1);
	primary_set = slice_set;
	secondary_set = alt_slice_set;
	return p[fx];
}

void
do_slice(x,file,stmt,proc,var,line)
	int	file,stmt,proc,var,line;
	Widget  x;
{
	int		p,f,i,status;
	int		file_proc;
	time_t			start,finish,elapsed;
	char	cmd[2000],label[1000];
	int		n = 0;
	time_t	min,sec;

	/*
	printf ("file %d stmt %d proc %d var %d\n",file,stmt,proc,var);
	*/
	
	for (i = 1; i <= n_procs; i++){
		if (procs[i].file_id == file)
			if ((stmt >= procs[i].entry) &&
				(stmt <= procs[i].exit)){
					file_proc = i;
					break;
				}
	}

	/* printf ("file %d stmt %d proc %d var %d\n",file,stmt,proc,var); */
	
	SliceClearAll(x);
	start = time(NULL);
	slice(file,file_proc,stmt,proc,var,slice_set,active);
	finish = time(NULL);
	elapsed = finish - start;
	if (elapsed <= 0) elapsed = 1;
	for (f = 0; f < n_files; f++){
		i = -1;
		while ((i = get_next_member(slice_set[f],i)) >= 0){
			n++;
			SliceSet (x,
				files[f].stmts[i].froml+line_map[f],
				files[f].stmts[i].fromc,
				files[f].stmts[i].tol+line_map[f],
				files[f].stmts[i].toc,0);
		}
	}
	min = elapsed/60;
	sec = elapsed%60;
	sprintf (label,
		"slice on %s (in %s) at line %d in %sc (%d stmts in %ld:%ld)",
		var_name(proc,var),proc?procs[proc].proc_name:"global",
		line,files[file].name,n,min,sec);
	sprintf (cmd,"echo \"%s\" >> HISTORY-S",label);
	system (cmd);
	SliceRedraw (x);
}

void
adjust_popup_list (list)
	Widget	list;
{
	Widget	port,shell,bar;
	Dimension	bar_w,list_w,list_h;
	int		list_max;

	port = XtParent(list);
	shell = XtParent(port);
	bar = XtNameToWidget (port,"vertical");
	XtUnmanageChild(port);
	XtUnmanageChild(list);

	XtVaGetValues (shell, XtNwidth,&list_w,
		XtNheight,&list_h,NULL);
	if(bar)XtVaGetValues (bar, XtNwidth,&bar_w,NULL);
	else bar_w = 16;
	XtVaGetValues (list, XtNwidth,&list_w,
		XtNlongest,&list_max,
		XtNheight,&list_h,NULL);

	XtVaSetValues (shell,
		XtNheight,list_h > 450? 430:list_h+5,
		XtNwidth, list_max+14+bar_w+5,
		NULL);
	XtManageChild(list);
	XtManageChild(port);
	XtVaGetValues (shell, XtNwidth,&list_w,
		XtNheight,&list_h,NULL);
}

void 
stop_slicing (w,x,y)
	Widget	w;
	Widget	x;
	caddr_t	y;
{
	abandon_slice = 1;
}

void 
helpB (w,x,y)
	Widget	w;
	Widget	x;
	caddr_t	y;
{
	char	cmd [2000];
	sprintf (cmd,"%s/helpu %s/u.help &",home,home);
	system (cmd);
}

void 
quitB (w,x,y)
	Widget	w;
	Widget	x;
	caddr_t	y;
{
	char	cmd [2000];
	sprintf (cmd,"cat HISTORY-S >> HISTORY");
	system (cmd);
	exit(0);
}

void 
slicecall (x,state,r)
	proc_state_ptr state;
	Widget	x;
	MultiSliceReturn *r;
{
	int		line,file,stmt;
	char	*var_name;
	char	label[1000];
	int		found;
	Pixel	save_fg,save_bg;

	if (state->is_running) return;
	state->is_running = 1;
	if (r->message) slice_set = secondary_set;
	else slice_set = primary_set;
	disp_to_file_and_line (r->line,&file,&line);
	/*
	printf ("call slice at line %d (line %d in file %s)\n",r->line,
		line,files[file].name);
		*/

	if (state->pid) var_name =
		procs[state->pid].locals[state->slice_var_index].name;
	else var_name = globals[state->slice_var_index].name;
	/*
	printf ("%s var %s\n",state->pid?"local":"global",var_name);
	*/
	stmt = line_to_stmt(file,line);
	sprintf (label,"computing slice on %s at line %d (%d of %sc)",
		var_name,r->line,line,files[file].name);
	XtVaGetValues (state->display_label,
		XtNforeground,&save_fg,
		XtNbackground,&save_bg,NULL);
	XtVaSetValues (state->display_label,
		XtNlabel,label,
		XtNforeground,state->active_fg,
		XtNbackground,state->active_bg,NULL);
	found = find_slice (state->pid,state->slice_var_index,file,stmt);
	if (found == -1){
		do_slice (x,file,stmt,state->pid,state->slice_var_index,line);
		save_slice (state->pid,state->slice_var_index,file,stmt,
			abandon_slice,n_files,slice_set);
	}
	else {
		load_slice (x,found,n_files,slice_set,line_map);
	}
	sprintf (label,"%sslice on %s at line %d (%d of %sc)",
		abandon_slice?"(partial) ":"",
		var_name,r->line,line,files[file].name);
	XtVaSetValues (state->display_label,
		XtNlabel,label,
		XtNforeground,save_fg,
		XtNbackground,save_bg,NULL);
	sprintf (label,"%s(%s) %d(%sc)%s",
		var_name,state->pid?procs[state->pid].proc_name:"global",
		line,files[file].name,
		abandon_slice?" (partial)":""
		);
	strcpy (r->message?state->stext:state->ptext,label);
	sprintf (label,"Primary: %s   Secondary: %s",
		state->ptext,state->stext);
	XtVaSetValues (state->slice_label,
		XtNlabel,label,
		NULL);
	state->is_running = 0;
	update_progress (-1);
}

void 
dodicecall (x,state,primary_set,secondary_set)
	proc_state_ptr state;
	bit_set	*primary_set,*secondary_set;
	Widget	x;
{
	int		i;
	int		f;
	char	buff[400];

	SliceClearAll(state->slicewin);
	for (f = 0; f < n_files; f++){
		i = -1;
		while ((i = get_next_member(primary_set[f],i)) >= 0){
			if (!is_bit_on (secondary_set[f],i)){
				SliceSet (state->slicewin,
				files[f].stmts[i].froml+line_map[f],
				files[f].stmts[i].fromc,
				files[f].stmts[i].tol+line_map[f],
				files[f].stmts[i].toc,0);
			}
		}
	}
	SliceRedraw (state->slicewin);
}

void 
dicecallsp (x,state,r)
	proc_state_ptr state;
	Widget	x;
	XtPointer	r;
{
	char	buff[400];
	
	sprintf (buff,"%s diced by %s",state->stext,state->ptext);
	XtVaSetValues (state->display_label,
		XtNlabel, buff,
		NULL);
	dodicecall (x,state,secondary_set,primary_set);
}

void dicecall (x,state,r)
	proc_state_ptr state;
	Widget	x;
	XtPointer	r;
{
	char	buff[400];
	
	sprintf (buff,"%s diced by %s",state->ptext,state->stext);
	XtVaSetValues (state->display_label,
		XtNlabel, buff,
		NULL);
	dodicecall (x,state,primary_set,secondary_set);
}

void 
bbcall (x,state,r)
	proc_state_ptr state;
	Widget	x;
	XtPointer	r;
{
	int		i;
	int		f;
	char	buff[400];

	SliceClearAll(state->slicewin);
	for (f = 0; f < n_files; f++){
		i = -1;
		while ((i = get_next_member(primary_set[f],i)) >= 0){
			if (is_bit_on (secondary_set[f],i)){
				SliceSet (state->slicewin,
				files[f].stmts[i].froml+line_map[f],
				files[f].stmts[i].fromc,
				files[f].stmts[i].tol+line_map[f],
				files[f].stmts[i].toc,0);
			}
		}
	}
	sprintf (buff,"Intersection of %s & %s",state->ptext,state->stext);
	XtVaSetValues (state->display_label,
		XtNlabel, buff,
		NULL);
	SliceRedraw (state->slicewin);

}

void 
unioncall (x,state,r)
	proc_state_ptr state;
	Widget	x;
	XtPointer	r;
{
	int		i;
	int		f;
	char	buff[400];

	SliceClearAll(state->slicewin);
	for (f = 0; f < n_files; f++){
		i = -1;
		while ((i = get_next_member(primary_set[f],i)) >= 0){
			SliceSet (state->slicewin,
			files[f].stmts[i].froml+line_map[f],
			files[f].stmts[i].fromc,
			files[f].stmts[i].tol+line_map[f],
			files[f].stmts[i].toc,0);
		}
		i = -1;
		while ((i = get_next_member(secondary_set[f],i)) >= 0){
			SliceSet (state->slicewin,
			files[f].stmts[i].froml+line_map[f],
			files[f].stmts[i].fromc,
			files[f].stmts[i].tol+line_map[f],
			files[f].stmts[i].toc,0);
		}
	}
	sprintf (buff,"Union of %s & %s",state->ptext,state->stext);
	XtVaSetValues (state->display_label,
		XtNlabel, buff,
		NULL);
	SliceRedraw (state->slicewin);
}

void clear_sliceB (w,state,y)
	Widget	w;
	proc_state_ptr state;
	caddr_t	y;
{
	char    buff[2000];
	sprintf (buff,"/usr/bin/rm -f %sY",state->root_file);
	clear_history();
	system (buff);
}

void 
clearB (w,state,y)
	Widget	w;
	proc_state_ptr state;
	caddr_t	y;
{
	char	buff[2000];
	if(SliceTrace)printf ("clear button pressed\n");

	sprintf (buff,"Source text for: %sc",state->root_file);
	XtVaSetValues (state->display_label,XtNlabel,buff,NULL);
	SliceClearAll(state->slicewin);
}

void
popup_selection(a,state,c)
	Widget		a;
	proc_state_ptr      state;
	XtPointer	c;
{
	Position		x,y;
	Widget		p;
	char		**list;
	int			i;

	p = XtParent(XtParent(state->history_list));
	/*
	XtTranslateCoords(top,
		(Position) 30,
		(Position) 60,
		&x,&y);
	XtVaSetValues (p,
		XtNx,	(XtArgVal) x,
		XtNy,	(XtArgVal) y,
		NULL);
		*/
	state->is_primary = (0 == strcmp (XtName(a),"Primary"));
	list = get_slice_list();
	XawListChange (state->history_list,list,0,0,True);
	adjust_popup_list (state->history_list);
	XtPopup (p,XtGrabExclusive);
}


void
proc_pop(a,p,c)
	Widget		a,p;
	XtPointer	c;
{
	Position		x,y;

	XtTranslateCoords(top,
		(Position) 30,
		(Position) 60,
		&x,&y);
	/*
	XtVaSetValues (p,
		XtNx,	(XtArgVal) x,
		XtNy,	(XtArgVal) y,
		NULL);
		*/
	XtPopup (p,XtGrabExclusive);
}

void
tree_pop (w,state,p)
	Widget	w;
	XtPointer	p;
	proc_state_ptr		state;
{
	
	state->reason = tree;
	proc_pop (w,state->proc_popup,NULL);
}

void
show_pop (w,state,p)
	Widget	w;
	XtPointer	p;
	proc_state_ptr		state;
{
	
	state->reason = show;
	proc_pop (w,state->proc_popup,NULL);
}

void
var_select(a,state,list)
	Widget		a;
	proc_state_ptr		state;
	XawListReturnStruct	*list;
{
	Widget	parent;
	char	buff[1000];
	int		ix;
	char	**addr,**base;

	parent = XtParent(a);
	while (!XtIsTransientShell(parent))parent = XtParent(parent);
	XtPopdown (parent);
	if (list->list_index == 0){
		return;
	}
	if (state->pid){
		state->slice_var_index = list->list_index;
 	}
	else {
		ix = global_ids[state->header][list->list_index - 1];
		state->slice_var_index = ix;
	}
	update_label(state);

}

void
slice_select(a,state,list)
	Widget		a;
	proc_state_ptr		state;
	XawListReturnStruct	*list;
{
	Widget	parent;
	char	label[2000];

	parent = XtParent(a);
	while (!XtIsTransientShell(parent))parent = XtParent(parent);
	XtPopdown (parent);
	if (list->list_index == 0) return;
	if (state->is_primary) slice_set = primary_set;
	else slice_set = secondary_set;
	load_slice (state->slicewin,list->list_index-1,n_files,slice_set,
		line_map);
	XtVaSetValues (state->display_label,XtNlabel,
		list->string,NULL);
	strcpy (state->is_primary?state->ptext:state->stext,list->string);
	sprintf (label,"Primary: %s   Secondary: %s",
		state->ptext,state->stext);
	XtVaSetValues (state->slice_label,XtNlabel,label,NULL);
	update_progress (-1);

}


void
head_select(a,state,list)
	Widget		a;
	proc_state_ptr		state;
	XawListReturnStruct	*list;
{
	Widget	parent;
	char	buff[1000];
	int		ix;

	parent = XtParent(a);
	while (!XtIsTransientShell(parent))parent = XtParent(parent);
	XtPopdown (parent);
	if (list->list_index == 0) return;
	XtUnmanageChild(state->var_list);
	XawListChange (state->var_list,global_vars[list->list_index],
		0,0,True);
	state->pid = 0;
	state->slice_var_index = 0;
	state->header = list->list_index;
	update_label(state);
	adjust_popup_list (state->var_list);
	XtManageChild(state->var_list);
	proc_pop (NULL,state->var_popup,NULL);
}

void
mark_call_tree (w,proc)
	Widget	w;
	int	proc;
{
	int		i,p,f;

	
	if ((proc < 1) || (proc > n_procs)) return;
	for (i = 0; i < n_files; i++)
		clear_bit_set (slice_set[i]);
    slice_tree (slice_set,proc,active);
	p = -1;
	while ((p= get_next_member(active,p)) >= 0){
		f = procs[p].file_id;
		
		i = -1;
		while ((i = get_next_member(slice_set[f],i)) >= 0){
			SliceSet (w,
				files[f].stmts[i].froml+line_map[f],
				files[f].stmts[i].fromc,
				files[f].stmts[i].tol+line_map[f],
				files[f].stmts[i].toc,0);
		}
	}
	SliceRedraw (w);
}

void
proc_select(a,state,list)
	Widget		a;
	proc_state_ptr		state;
	XawListReturnStruct	*list;
{
	Widget	parent;
	char	buff[1000];
	int		ix,i,f;

	parent = XtParent(a);
	while (!XtIsTransientShell(parent))parent = XtParent(parent);
	XtPopdown (parent);
	if (list->list_index == 0) return;
	state->proc = list->string;
	current_proc = list->list_index;
	state->pid = proc_ids[list->list_index];
	state->slice_var_index = 0;
	SliceClearAll(state->slicewin);
	f = procs[state->pid].file_id;
	i = procs[state->pid].entry;
	SliceSet (state->slicewin,
		files[f].stmts[i].froml+line_map[f],
		files[f].stmts[i].fromc,
		files[f].stmts[i].tol+line_map[f],
		files[f].stmts[i].toc,0);
	i = procs[state->pid].exit;
	SliceSet (state->slicewin,
		files[f].stmts[i].froml+line_map[f],
		files[f].stmts[i].fromc,
		files[f].stmts[i].tol+line_map[f],
		files[f].stmts[i].toc,1);
	update_label(state);
	if (state->reason != select_action){
		if (state->reason == tree)
			mark_call_tree (state->slicewin,state->pid);
		state->reason = select_action;
		return;
	}
	XtUnmanageChild(state->var_list);
	XawListChange (state->var_list,local_vars[list->list_index],
			0,0,True);
	adjust_popup_list (state->var_list);
	XtManageChild(state->var_list);
	proc_pop (NULL,state->var_popup,NULL);
	/*
	if (list->list_index == n_procs)
	else {
		XawListChange (state->var_list,procs[list->list_index].locals,
			0,0,True);
		stmt = stmt_index[procs[current_proc].proc_at];
		SliceSetTop (state->slicewin,
			(float)stmt->start_line/((float)n_lines));
	}
	adjust_popup_list (state->var_list);
	*/
}

	char	*fall[] = {
		/* don't move the first entry (iconPixmap) */
		"*iconPixmap:	slice.icon",
		"*title: Unravel Version 2.2+ Program Slicer",
		"*procpop.title: Procedures",
		"*headpop.title: Files",
		"*varpop.title: Variables",
		"*selectpop.title: Slices",
		"u.geometry: 600x450+10+10",
		"*procpop.geometry: +10+10",
		"*selectpop.geometry: +20+10",
		"*headpop.geometry: +30+10",
		"*varpop.geometry: +40+10",
		"*font:	lucidasanstypewriter-bold-12",
		"*slicewin*altFont:	lucidasanstypewriter-bold-12",
		"*slicewin*font:	lucidasanstypewriter-bold-12",
		"*slicewin*sideBackground:	LightBlue1",
		"*slicewin*sideForeground:	Blue1",
		"*slicewin*altBackground:	blue",
		"*slicewin*altForeground:	yellow",
		"*background: seashell3",
		"*foreground: blue",
		"*shapeStyle: oval",
		"*Label*borderWidth: 1",
		"*criterion.borderWidth: 3",
		"*info.borderWidth: 3",
		"*progress.borderWidth: 0",
		"*Command*borderWidth: 2",
		"*MenuButton*borderWidth: 2",
		"*procpop.width: 500",
		"*varpop.width: 500",
		"*quit.accelerators: #override\\n \
			<KeyPress>q: set() highlight() notify()",
		"*Help.accelerators:  \
			<KeyPress>h: set() highlight() notify() unset()\\n\
			<KeyPress>?: set() highlight() notify() unset()",
		NULL};
	XtAppContext	ac;
void 
continue_events(proc,change)
	int		proc,change;
{
	XEvent	e;
	static int pass = 0;
	static int n    = 0;

	if (pass%100 == 0){
		n = update_progress (n);
	}
	pass++;
	if (XtAppPending(ac)){
		XtAppNextEvent(ac,&e);
		XtDispatchEvent(&e);
	}
}
void active_hook();

int
main(argc, argv)
    int argc;
    char **argv;
{
	Widget		create_widgets();
	int			fd,openok;
	char		icon_res[2000];
	char		*suffix,file_name_base[2000];
	MultiSliceFiles  f[100];
	int					i;
	int			t_lines = 0;
	char		*root;
	static XrmOptionDescRec opt[] = {
		{"-runningfg", "*runningFG", XrmoptionSepArg,NULL},
		{"-runningbg", "*runningBG", XrmoptionSepArg,NULL}};

	printf ("unravel version %s\n","2.2+");
	sprintf (icon_res,"*iconPixmap: %s/slice.icon",HOME);
	fall[0] = icon_res;
	top = XtAppInitialize (&ac,"Unravel",opt,XtNumber(opt),&argc,argv,
		fall,NULL,0);
	root = get_file (argc,argv);
	slicewin = create_widgets(top,root);

	/*
	printf ("\nn files %d\n",n_files);
	*/
	for (i = 0; i < n_files; i++){
		f[i].text = files[i].text;
		t_lines += files[i].n_lines;
		/*
		printf ("file %d lines %d total %d\n",i,files[i].n_lines,
			t_lines);
		*/
	}
	if (t_lines == 0) {
		fprintf (stderr,"Could not find source code, no lines found\n");
		exit(1);
	}
	BuildSliceSrc (slicewin,n_files,f,t_lines);
	make_line_map();

	slice_pass_hook = continue_events;
	MultiSliceSetHook(slicewin,active_hook);
	XtAppMainLoop(ac);
}

void
strech (goal,w)
	Widget	goal,w;
{
	Dimension		width,bw;
	Position		x;
	int				dist;

	XtUnmanageChild(w);
	XtVaGetValues (goal,
		XtNdefaultDistance, &dist,
		XtNwidth,&width,NULL);
	XtVaGetValues (w,
		XtNborderWidth,&bw,
		XtNx,&x,
		NULL);
	XtVaSetValues (w,
		XtNwidth,	(XtArgVal) (width-2*bw - dist - x),
		NULL);
	XtManageChild(w);
}

Widget 
create_widgets (top,file_name)
	Widget	top;
	char	*file_name;
{
	static Widget	frame,quit,file_button,file_menu;
	static Widget	load,line,content;
	static Widget	select_button,select_menu;
	static Widget	proc,var,primary,secondary;
	static Widget	op_button,op_menu;
	static Widget	dicesp,dice,bb,merge;
	static Widget	proc_state,selection_state;
	static Widget	proc_popup,proc_port,proc_list;
	static Widget	head_popup,head_port,head_list;
	static Widget	var_popup,var_port,var_list;
	static Widget	select_popup,select_port,select_list;
	static Widget	help;
	static Widget	interrupt,show_tree,show_proc;
	static Widget	display_label;
	static Widget	slicewin,clear,clear_slice,sliceframe;
	static proc_state_rec	ps;
	Dimension		www,ww,w,bw;
	Position		cx,hx,x,y;
	int				at,dist;
	char			buff[1000];
	static char			*plist[] = {"start","fetch","redactor",
					"look_up_value_in_long_name_list",
					"alpha","beta","OMEGA",
					NULL};

	static char		pbuff[500] = "<none>",sbuff[500] = "<none>";
	static XtResource res[] = {
		{XtNrunningFG,XtCBackground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(proc_state_rec,active_fg),XtRString,"red"},
		{XtNrunningBG,XtCForeground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(proc_state_rec,active_bg),XtRString,"yellow"}};

	XtGetApplicationResources (top,&ps,res,XtNumber(res),NULL,0);
	ps.proc = NULL;
	ps.var = NULL;
	ps.state_label = &selection_state;
	ps.root_file = file_name;
	ps.ptext = pbuff;
	ps.stext = sbuff;
	/*
	file_menu = XtVaCreatePopupShell ("fileMenu",simpleMenuWidgetClass,
		top,
		NULL);
		*/
	select_menu = XtVaCreatePopupShell ("selectMenu",
		simpleMenuWidgetClass, top,
		NULL);
	/*
	set_button_help (file_menu,"Quit to exit unravel");
	*/
	set_button_help (select_menu,
		"Menu for selecting criteria variables or previous slices");
	op_menu = XtVaCreatePopupShell ("opMenu",
		simpleMenuWidgetClass, top,
		NULL);
	set_button_help (op_menu,
		"Menu for combining primary and secondary slices");
	select_popup = XtVaCreatePopupShell ("selectpop",
		transientShellWidgetClass,top,
		NULL);
	select_port = XtVaCreateManagedWidget ("selectport",
		viewportWidgetClass,select_popup,
			XtNforceBars,	(XtArgVal) True,
			XtNallowVert,	(XtArgVal) True,
		NULL);
	select_list = XtVaCreateManagedWidget ("selectlist",
		listWidgetClass,select_port,
			XtNdefaultColumns, (XtArgVal) 1,
		NULL);
	set_button_help (select_list,"list of already computed slices");
	ps.history_list = select_list;
	head_popup = XtVaCreatePopupShell ("headpop",
		transientShellWidgetClass,top,
		NULL);
	head_port = XtVaCreateManagedWidget ("headport",
		viewportWidgetClass,head_popup,
			XtNforceBars,	(XtArgVal) True,
			XtNallowVert,	(XtArgVal) True,
		NULL);
	proc_popup = XtVaCreatePopupShell ("procpop",
		transientShellWidgetClass,top,
		NULL);
	proc_port = XtVaCreateManagedWidget ("procport",
		viewportWidgetClass,proc_popup,
			XtNforceBars,	(XtArgVal) True,
			XtNallowVert,	(XtArgVal) True,
		NULL);
	var_popup = XtVaCreatePopupShell ("varpop",
		transientShellWidgetClass,top,
		NULL);
	ps.var_popup = var_popup;
	var_port = XtVaCreateManagedWidget ("varport",
		viewportWidgetClass,var_popup,
			XtNforceBars,	(XtArgVal) True,
			XtNallowVert,	(XtArgVal) True,
		NULL);
	current_proc = n_procs;
	make_proc_list();
	make_head_list();
	head_list = XtVaCreateManagedWidget ("headlist",
		listWidgetClass,head_port,
			XtNlist,		(XtArgVal) file_name_list,
			XtNdefaultColumns, (XtArgVal) 1,
		NULL);
	adjust_popup_list (head_list);
	set_button_help (head_list,"File containing global variable");
	proc_list = XtVaCreateManagedWidget ("proclist",
		listWidgetClass,proc_port,
			XtNlist,		(XtArgVal) proc_name_list,
			XtNdefaultColumns, (XtArgVal) 1,
		NULL);
	adjust_popup_list (proc_list);
	set_button_help (proc_list,"Select a proc to see list of local variables");
	var_list = XtVaCreateManagedWidget ("varlist",
		listWidgetClass,var_port,
			/*
			XtNlist,		(XtArgVal) global_names,
			*/
			XtNdefaultColumns, (XtArgVal) 1,
		NULL);
	set_button_help (var_list,"Select a variable for a slicing criterion");
	ps.var_list = var_list;
	XtAddCallback (head_list,XtNcallback,head_select,&ps);
	XtAddCallback (proc_list,XtNcallback,proc_select,&ps);
	XtAddCallback (var_list,XtNcallback,var_select,&ps);
	XtAddCallback (select_list,XtNcallback,slice_select,&ps);
	frame = XtVaCreateManagedWidget ("frame",formWidgetClass,top,
		NULL);
	set_button_help (frame,". . .");
	/*
	file_button = XtVaCreateManagedWidget ("File",
		menuButtonWidgetClass,frame,
			XtNtop,			(XtArgVal) XtChainTop,
			XtNbottom,		(XtArgVal) XtChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNmenuName,	(XtArgVal) "fileMenu",
			XtNright,		(XtArgVal) XtChainLeft,
			XtNleft,		(XtArgVal) XtChainLeft,
			NULL);
	set_button_help (file_button,"Quit button is in this menu");
	load = XtVaCreateManagedWidget ("load",smeBSBObjectClass,file_menu,
			XtNlabel,		(XtArgVal) "load",
			XtNsensitive,	(XtArgVal) False,
		NULL);
	line = XtVaCreateManagedWidget ("line",smeLineObjectClass,file_menu,
			XtNlineWidth,		(XtArgVal) 4,
		NULL);
	quit = XtVaCreateManagedWidget ("quit",smeBSBObjectClass,file_menu,
			XtNlabel,		(XtArgVal) "Exit",
		NULL);
	*/
	quit = XtVaCreateManagedWidget ("quit",commandWidgetClass,frame,
			XtNlabel,		(XtArgVal) "Exit",
			XtNtop,			(XtArgVal) XtChainTop,
			XtNbottom,		(XtArgVal) XtChainTop,
			XtNright,		(XtArgVal) XtChainLeft,
			XtNleft,		(XtArgVal) XtChainLeft,
		NULL);
	XtAddCallback (quit,XtNcallback,quitB,NULL);
	set_button_help (quit,"Click quit to exit slicer");

	select_button = XtVaCreateManagedWidget ("Select",
		menuButtonWidgetClass,frame,
			XtNtop,			(XtArgVal) XtChainTop,
			XtNbottom,		(XtArgVal) XtChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNmenuName,	(XtArgVal) "selectMenu",
			XtNfromHoriz,	(XtArgVal) quit,
			XtNright,		(XtArgVal) XtChainLeft,
			XtNleft,		(XtArgVal) XtChainLeft,
			NULL);
	set_button_help (select_button,
		"Menu for selecting criteria variables or previous slices");
	proc = XtVaCreateManagedWidget ("Procedure",
		smeBSBObjectClass,select_menu,
			XtNlabel,		(XtArgVal) "Local Variable",
		NULL);
	XtAddCallback (proc,XtNcallback,proc_pop,proc_popup);
	var = XtVaCreateManagedWidget ("Variable",
		smeBSBObjectClass,select_menu,
			XtNlabel,		(XtArgVal) "Global Variable",
		NULL);
	XtAddCallback (var,XtNcallback,proc_pop,head_popup);
	show_proc = XtVaCreateManagedWidget ("Markproc",
		smeBSBObjectClass,select_menu,
		XtNlabel,	"Mark proc",
		NULL);
	show_tree = XtVaCreateManagedWidget ("ShowCallTree",
		smeBSBObjectClass,select_menu,
		XtNlabel,	"Show call tree",
		NULL);
	primary = XtVaCreateManagedWidget ("Primary",
		smeBSBObjectClass,select_menu,
		NULL);
	secondary = XtVaCreateManagedWidget ("Secondary",
		smeBSBObjectClass,select_menu,
		NULL);
	ps.proc_popup = proc_popup;
	XtAddCallback (primary,XtNcallback,popup_selection,&ps);
	XtAddCallback (secondary,XtNcallback,popup_selection,&ps);
	XtAddCallback (show_proc,XtNcallback,show_pop,&ps);
	XtAddCallback (show_tree,XtNcallback,tree_pop,&ps);

	op_button = XtVaCreateManagedWidget ("Operation",
		menuButtonWidgetClass,frame,
			XtNtop,			(XtArgVal) XtChainTop,
			XtNbottom,		(XtArgVal) XtChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNmenuName,	(XtArgVal) "opMenu",
			XtNfromHoriz,	(XtArgVal) select_button,
			XtNright,		(XtArgVal) XtChainLeft,
			XtNleft,		(XtArgVal) XtChainLeft,
			NULL);
	set_button_help (op_button,"Popup menu of operations on slices");
	dice = XtVaCreateManagedWidget ("Dice",
		smeBSBObjectClass,op_menu,
		NULL);
	dicesp = XtVaCreateManagedWidget ("Dicesp",
		smeBSBObjectClass,op_menu,
			XtNlabel,		(XtArgVal) "Dice S-P",
		NULL);
	bb = XtVaCreateManagedWidget ("Backbone",
		smeBSBObjectClass,op_menu,
			XtNlabel,		(XtArgVal) "Intersection",
		NULL);
	merge = XtVaCreateManagedWidget ("Union",
		smeBSBObjectClass,op_menu,
		NULL);
    clear = XtCreateManagedWidget("Clear", smeBSBObjectClass,
			   op_menu, NULL, 0);
    clear_slice = XtVaCreateManagedWidget("ClearSlice",
		smeBSBObjectClass, op_menu,
			XtNlabel,		(XtArgVal) "Clear Slice History",
			   NULL);

	XtVaGetValues (op_button,
		XtNwidth,&w,
		XtNborderWidth,&bw,
		XtNx,&x,
		NULL);
	interrupt = XtVaCreateManagedWidget ("interrupt",
		commandWidgetClass,frame,
		XtNlabel,			(XtArgVal) "Interrupt",
		XtNfromHoriz,		(XtArgVal) op_button,
		XtNright,			(XtArgVal) XtChainLeft,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		NULL);
	set_button_help (interrupt,
		"Abandon computing slice, show partial result");
	XtAddCallback (interrupt,XtNcallback,stop_slicing,NULL);
	progress = XtVaCreateManagedWidget ("progress",
		labelWidgetClass,frame,
		XtNlabel,			(XtArgVal) "      nodes",
		XtNfromHoriz,		(XtArgVal) interrupt,
		XtNright,			(XtArgVal) XtChainLeft,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		NULL);
	set_button_help (progress,"Give feedback on progress by display of \
	# of statements in slice");

	at = WIDTH;
	help = XtVaCreateManagedWidget ("Help",commandWidgetClass,frame,
		/*
		XtNhorizDistance,	(XtArgVal) at,
		*/
		XtNfromHoriz,		(XtArgVal) progress,
		XtNright,			(XtArgVal) XtChainRight,
		XtNleft,			(XtArgVal) XtChainRight,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		NULL);
	set_button_help (help,"Pop up display of general help");
	XtAddCallback (help,XtNcallback,helpB,NULL);
	sprintf (buff,"%s %s",
		"File: <none> Proc: <none>","Var: <none>");
	XtVaGetValues (frame,
		XtNwidth,&w,
		NULL);
	selection_state = XtVaCreateWidget ("state",
		labelWidgetClass,frame,
		XtNfromVert,		(XtArgVal) help,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNright,			(XtArgVal) XtChainRight,
		XtNlabel,			(XtArgVal) buff,
		XtNwidth,			(XtArgVal) w,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		XtNjustify,			(XtArgVal) XtJustifyLeft,
		NULL);

	set_button_help (selection_state,"Show variable selected for slicing criterion");
	XtManageChild(selection_state);

	sprintf (buff,"%s %s",
		"Primary: <none>","Secondary: <none>");
	proc_state = XtVaCreateManagedWidget ("pstate",
		labelWidgetClass,frame,
		XtNfromVert,		(XtArgVal) selection_state,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNright,			(XtArgVal) XtChainRight,
		XtNlabel,			(XtArgVal) buff,
		XtNwidth,			(XtArgVal) w,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		XtNjustify,			(XtArgVal) XtJustifyLeft,
		NULL);
	set_button_help (proc_state,
		"Show the criteria of the primary and secondary slices");

	ps.slice_label = proc_state;

	sprintf (buff,"Source text for: %sc",ps.root_file);
	display_label = XtVaCreateManagedWidget ("criterion",
		labelWidgetClass,frame,
		XtNfromVert,		(XtArgVal) proc_state,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNright,			(XtArgVal) XtChainRight,
		XtNlabel,			(XtArgVal) buff,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainTop,
		NULL);
	ps.display_label = display_label;
	set_button_help (display_label,"Describe current display");

	QQQ = slicewin = CreateSliceBox("slicebox",frame);
	sprintf (buff,"Describe object under mouse pointer");
	content = XtVaCreateManagedWidget ("info",
		labelWidgetClass,frame,
		XtNfromVert,		(XtArgVal) XtParent(slicewin),
		XtNright,			(XtArgVal) XtChainRight,
		XtNleft,			(XtArgVal) XtChainLeft,
		XtNtop,				(XtArgVal) XtChainBottom,
		XtNbottom,			(XtArgVal) XtChainBottom,
		XtNjustify,			(XtArgVal) XtJustifyCenter,
		XtNlabel,			(XtArgVal) buff,
		NULL);
	help_label = content;
	XtAddEventHandler (content,EnterWindowMask,False,button_help,
		"Describe object under mouse pointer");
	ps.slicewin = slicewin;
	set_button_help (slicewin,"Select slice location via mouse");
	XtAddCallback (clear,XtNcallback,clearB,&ps);
	XtAddCallback (clear_slice,XtNcallback,clear_sliceB,&ps);
	XtAddCallback (slicewin,XtNcallback,slicecall,&ps);
	XtAddCallback (bb,XtNcallback,bbcall,&ps);
	XtAddCallback (merge,XtNcallback,unioncall,&ps);
	XtAddCallback (dice,XtNcallback,dicecall,&ps);
	XtAddCallback (dicesp,XtNcallback,dicecallsp,&ps);
	sliceframe = XtParent(slicewin);
	set_button_help (XtNameToWidget(sliceframe,"bar"),
		"Scroll bar to navigate through source text");
	set_button_help (XtNameToWidget(sliceframe,"up"),
		"Click left mouse button to back up source text by one line");
	set_button_help (XtNameToWidget(sliceframe,"dn"),
		"Click left mouse button to advance source text by one line");
	XtUnmanageChild(sliceframe);
	XtVaSetValues(sliceframe,
		XtNfromVert,		(XtArgVal) display_label,
		XtNtop,				(XtArgVal) XtChainTop,
		XtNbottom,			(XtArgVal) XtChainBottom,
		NULL);
	XtManageChild(sliceframe);

	XtInstallAccelerators (frame,help);
	XtInstallAccelerators (frame,quit);
	XtRealizeWidget (top);

	XtVaGetValues (frame,
		XtNwidth,&w,
		XtNdefaultDistance, &dist,
		NULL);
	XtVaGetValues (help,
		XtNwidth,&www,
		XtNborderWidth,&bw,
		XtNx,&hx,
		NULL);
	XtVaGetValues (progress,
		XtNwidth,&ww,
		XtNx,&cx,
		NULL);
	ww = w - (cx + ww + 4*bw + dist + www);
	XtVaGetValues (selection_state,
		XtNborderWidth,&bw,
		NULL);
	XtUnmanageChild(help);
	XtUnmanageChild(selection_state);
	XtUnmanageChild(proc_state);
	XtVaSetValues (selection_state,
		XtNwidth,			(XtArgVal) (w-2*(dist + bw)),
		NULL);
	XtVaSetValues (proc_state,
		XtNwidth,			(XtArgVal) (w-2*(dist + bw)),
		NULL);
	XtVaSetValues (help,
		XtNhorizDistance, ww,
		NULL);
	XtManageChild(selection_state);
	XtManageChild(proc_state);
	XtManageChild(help);
	strech (frame,content);
	strech (frame,display_label);
	XtTranslateCoords(top,
		(Position) 30,
		(Position) 60,
		&x,&y);
	/*
	XtVaSetValues (proc_popup,
		XtNx,	(XtArgVal) x,
		XtNy,	(XtArgVal) y,
		NULL);
		*/
	ps.slicewin = slicewin;
	ps.is_primary = 1;
	return slicewin;
}
