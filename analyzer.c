/*
*******************************************************************
*                                                                 *
*   Unravel control panel for analyzer                            *
*                                                                 *
*   Jim Lyle 10 June 1994                                         *
*                                                                 *
*******************************************************************
*/


# include <stdio.h>
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <X11/Shell.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/Command.h>
# include <X11/Xaw/Label.h>
# include <X11/Xaw/AsciiText.h>
# include <X11/Xaw/List.h>
# include <X11/Xaw/Viewport.h>
# include <X11/Xaw/SimpleMenu.h>
# include <X11/Xaw/SmeBSB.h>
# include <X11/Xaw/SmeLine.h>
# include <X11/Xaw/MenuButton.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# define WAIT 500
# include "config.h"

//static  char    sccsid[] = "@(#)analyzer.c	1.5  8/16/95";

/*
# define XtNrunningFG "runningFG"
# define XtNrunningBG "runningBG"
	static XrmOptionDescRec opt[] = {
		{"-runningfg", "*runningFG", XrmoptionSepArg,NULL},
		{"-runningbg", "*runningBG", XrmoptionSepArg,NULL}};
	static XtResource res[] = {
		{XtNrunningFG,XtCForeground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(struct  app_resources,fg),XtRString,"red"},
		{XtNrunningBG,XtCForeground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(struct  app_resources,bg),XtRString,"yellow"}};
	top = XtAppInitialize (&ac,"Xtform",opt,XtNumber(opt),&np,p,fall,NULL,0);
	XtGetApplicationResources (top,&a_res,res,XtNumber(res),NULL,0);
	struct dirent { char	d_name[1000]; };
*/

# define XtNrunningFG "runningFG"
# define XtNrunningBG "runningBG"
Widget	help_label;
int unravel_win_id = 0;

typedef struct name_struct name_rec,*name_ptr;
struct name_struct {
	char	**use,**skip,**pick;
	struct dirent **files;
	Widget	keep,ignore;
};
typedef struct nf_struct nf_rec,*nf_ptr;

struct nf_struct {
	XtAppContext	ac;
	name_ptr		n;
	Widget			s,r,h;
	int				at;
	int				of;
	int				errs;
	int				running;
	char			current[500];
	char			*flags;
	char			*pflags;
	FILE			*history;
	Pixel			run_fg,run_bg,save_fg,save_bg;
};

void
button_help (Widget w, char *mess, XEvent *e, Boolean *ok)
{
	XtVaSetValues (help_label,
		XtNlabel, (XtArgVal) mess,
		NULL);
}

void
set_button_help (Widget w, char *mess)
{
	static char buff[] = 
		"Description of object under mouse pointer is displayed here";
	XtAddEventHandler (w,EnterWindowMask,False,button_help,mess);
	XtAddEventHandler (w,LeaveWindowMask,False,button_help,buff);
}

int 
compare(struct dirent ** a, struct dirent **b)
{
	if (strcmp((*a)->d_name,(*b)->d_name) > 0) return 1;
	return 0;
}

static int 
select_c_file(struct dirent *entry)
{
	int	len;

	if (!entry) return 0;
	len = strlen(entry->d_name);
	if (len < 2) return 0;
	if ((entry->d_name[len-2] != '.') ||
		(entry->d_name[len-1] != 'c')) return 0;
	return 1;
}

int 
myscandir (char *dir_name, struct dirent ***list, int (*select)(), int (*compare)())
{
	DIR		*dir;
	struct dirent *entry;
	typedef struct l_struct ll_node,*ll;
	struct l_struct {
		ll		next;
		struct dirent *e;
	};

	int		i,n_match = 0;
	ll		new_ll;
	struct dirent *new_de;
	ll		nodes,node_list = NULL;

	dir = opendir (dir_name);
	if (!dir){
		fprintf (stderr,"directory open failed for %s\n",
			dir_name);
		exit(1);
	}
	for (entry = readdir(dir) ; entry != NULL ; entry = readdir(dir)){
		if (select(entry)){
			new_ll = (ll) malloc(sizeof (ll_node));
			new_ll->next = node_list;
			node_list = new_ll;
			new_ll->e = (struct dirent *) malloc (
				sizeof(struct dirent)+strlen(entry->d_name)+3);
			/*
			*(new_ll->e) = *entry;
			*/
			strcpy (new_ll->e->d_name,entry->d_name);
			n_match++;
		}
	}
	*list = (struct dirent **) malloc (
		(n_match+1)*sizeof(struct dirent *));
	nodes = node_list;
	for (i = 0; i < n_match; i++){
		(*list)[i] = nodes->e;
		nodes = nodes -> next;
	}
	qsort (*list,n_match,sizeof(struct dirent *),compare);
	return n_match;
}

int 
scan_dir(f)
	struct dirent ***f;
{
	struct dirent **files;
	int		count;
	int 	alphasort();
	int		i;

	count = myscandir (".",&files,select_c_file,compare);
	files[count] = NULL;
	*f = files;
	return count;
}

void
count_lif(n,f,n_ana,n_not,n_out)
	int		n,*n_ana,*n_not,*n_out;
	char	**f;
{
	int		nk = 0,k = 0,out = 0;
	struct	stat status_c,status_lif,status_t;
	int		c,lif,t;
	char	buff[2000];

	int		s,i;

	for (i = 0; i < n; i++){
		c = stat (f[i],&status_c);
		s = strlen(f[i]);
		strcpy (buff,f[i]);
		buff[s-1] = '\0';
		strcat (buff,"LIF");
		lif = stat (buff,&status_lif);
		strcpy (buff,f[i]);
		buff[s-1] = 'T';
		t = stat (buff,&status_t);
		if ((lif == 0) && (t == 0)){
			if ((status_c.st_mtime > status_t.st_mtime) ||
				(status_c.st_mtime > status_lif.st_mtime)) out++;
			else k++;
		}
		else nk++;

	}

	*n_ana = k;
	*n_not = nk;
	*n_out = out;
}

void
count_system(n_prog,n_dup)
	int		*n_prog,*n_dup;
{
	int		n = 0,k,j;
	int		dup = 0;
	FILE	*sys;
	char	buff[2000],name[2000];
	int		ambig_state = 0;

	n = 0;
	sys = fopen ("SYSTEM","r");
	if (sys){
		while (fgets (buff,2000,sys)){
			if (buff[0] == 'M'){
				ambig_state = 0;
				k = sscanf(buff,"MAIN %s %d",name,&k);
				if (k == 2) n++;
			}
			else if (buff[0] == 'A'){
				ambig_state = 1;
			}
			else if (buff[0] == 'F'){
				ambig_state = 0;
			}
			else if (buff[0] == 'L'){
				ambig_state = 0;
			}
			else if (ambig_state) dup++;
		}
		fclose(sys);
	}
		

	*n_prog = n;
	*n_dup = dup;
}


void
notify_unravel (w)
	Widget	w;
{
	XClientMessageEvent	e;
	Status				status = 13;

	strcpy (e.data.b,"done");
	e.display = XtDisplay(w);
	e.window = unravel_win_id;
	e.type = ClientMessage;
	e.format = 8;
	e.message_type = (Atom) 1;
	if (unravel_win_id){
		status = XSendEvent (e.display,e.window,True,0L,(XEvent *)(&e));
		XFlush(e.display);
	}
}

void
do_final_analysis (nf,tt)
	nf_ptr		nf;
	XtIntervalId	*tt;
{
	char	buff[2000];

	sprintf (buff,"%s ; %s ;%s/map >> HISTORY-A 2>&1",
		"echo \" \" >>HISTORY-A","echo running map >> HISTORY-A",
		HOME);
	system(buff);
	if (nf->errs) sprintf (buff,
		"Analysis complete, %d file%s not ANSI C", nf->errs,
			nf->errs == 1?"":"s");
	else sprintf (buff,"Analysis complete");
	XtVaSetValues (nf->s,
		XtNlabel, buff,
		XtNforeground,nf->save_fg,
		XtNbackground,nf->save_bg,
		NULL);
	nf->running = 0;
	notify_unravel (nf->s);
	if(nf->r)XtVaSetValues (nf->r,
		XtNlabel,		(XtArgVal) "Analyze Selected Files",
		NULL);
	return;
}

void
do_analysis (nf,tt)
	nf_ptr		nf;
	XtIntervalId	*tt;
{
	char	buff[4000],file[1000],mess[2000];
	int		i,k,cpp_status,parse_status,status,c_status,t_status;
	int		n,result;
	int		len;
	struct stat	cpp_stat,parse_stat,c_stat,t_stat;
	XawTextPosition		insert;
	XawTextBlock		chunk;
	Widget				src;
	time_t			start,finish,elapsed;
	int				rate;

	if (nf->running == 0){
		XtVaSetValues (nf->s,
			XtNlabel, "Analysis stopped",
			NULL);
		XtVaSetValues (nf->r,
			XtNsensitive,True,
		NULL);
		return;
	}
	chunk.firstPos = 0;
	chunk.format = FMT8BIT;
	chunk.ptr = mess;
	i = nf->at;
	n = nf->of;
		strcpy (file,nf->n->use[i]);
		c_status = stat (file,&c_stat);
		len = strlen(file);
		if (len > 1) file[len-2] = '\0';
		status = unlink ("CPP.ERR");
		status = unlink ("PARSE.ERR");
		sprintf (buff,
			"PATH=%s:$PATH; %s -E %s %s.c 2>CPP.ERR | %s %s -o %s.LIF",
			HOME,
			CC,
			nf->flags,
			file,
			"parser 2>PARSE.ERR",
			nf->pflags,
			file);
		fprintf(stderr, "[%s]\n", buff);
		start = time(NULL);
		system (buff);
		/* printf ("analyze: %s\n",buff); */
		finish = time(NULL);
		elapsed = finish - start;
		if (elapsed <= 0) elapsed = 1;
		cpp_status = stat ("CPP.ERR",&cpp_stat);
		parse_status = stat ("PARSE.ERR",&parse_stat);
		if (/* cpp_stat.st_size || */ parse_stat.st_size){
			/* compile error */
			sprintf (mess,
				"%s is not ANSI C (compile errors)",
				nf->n->use[i]);
			nf->errs++;
		}
		else {
			/* no errors */
			snprintf (buff, 4000, "wc %s.c >>%s.T",file,file);
			system(buff);
			sprintf (buff,"%s/tsummary %s.c >>HISTORY-A",
				HOME,file);
			system(buff);
			sprintf (buff,
				"PATH=%s:$PATH ; export PATH ; vprep %s.c",HOME,file);
			system(buff);
			if (finish-start)rate = c_stat.st_size / (elapsed);
			else rate = c_stat.st_size;
			sprintf (mess,
			"%s (%ld chars) analysis complete %ld secs  %d chars/sec",
					nf->n->use[i],
					c_stat.st_size,elapsed,rate);
		}
		sprintf (buff,"echo \"%s\" >> HISTORY-A ; %s",
			mess,"cat *.ERR >>HISTORY-A");
		system(buff);
		i++;
		nf -> at = i;
	if ( i < n ){
		XtAppAddTimeOut (nf->ac,WAIT,do_analysis,nf);
		if (nf->errs) sprintf (buff,
			"Analyzing %s (%d of %d)  %d file%s not ANSI C",
			nf->n->use[i],i+1,n,nf->errs,nf->errs == 1?"":"s");
		else sprintf (buff,"Analyzing %s (%d of %d) ",
			nf->n->use[i],i+1,n);
		XtVaSetValues (nf->s,
			XtNlabel, buff,
			NULL);
		return;
	}
	XtVaSetValues (nf->s,
		XtNlabel, "Scanning for main programs . . .",
		NULL);
	XFlush(XtDisplay(nf->s));
	XtAppAddTimeOut (nf->ac,WAIT,do_final_analysis,nf);
	return;
}

void
run_a (w,nf,c)
	Widget		w;
	nf_ptr		nf;
	XtPointer   c;
{
	char	buff[2000],file[1000];
	int		i,k,cpp_status,parse_status,status,c_status,t_status;
	int		n;
	struct stat	cpp_stat,parse_stat,c_stat,t_stat;

	if (nf->running){
		nf->running = 0;
		XtVaSetValues (w,
			XtNlabel,		(XtArgVal) "Analyze Selected Files",
			XtNsensitive,False,
		NULL);
		return;
	}
	n = 0;
	while(nf->n->use[n])n++;
	i = 0;
	nf->at = 0;
	nf->of = n;
	nf->errs = 0;
	if (n > 0){
		XtVaSetValues (w,
			XtNlabel,		(XtArgVal) "Stop Analysis",
		NULL);
		sprintf (buff,"Analyzing %s (%d of %d)",nf->n->use[i],i+1,n);
		XtVaGetValues (nf->s,
			XtNforeground,&(nf->save_fg),
			XtNbackground,&(nf->save_bg),
			NULL);
		XtVaSetValues (nf->s,
			XtNlabel, buff,
			XtNforeground,nf->run_fg,
			XtNbackground,nf->run_bg,
			NULL);
		nf->running = 1;
		nf->r = w;
		sprintf (buff,"echo CPP Flags: %s >> HISTORY-A",nf->flags);
		system(buff);
		sprintf (buff,"echo parser Flags: %s >> HISTORY-A",nf->pflags);
		system(buff);
		XtAppAddTimeOut (nf->ac,WAIT,do_analysis,nf);
	}
}

void
push_help(w,u_data,w_data)
	Widget	w;
	XtPointer	u_data;
	XtPointer	w_data;
{
	char	buff[2000];

	sprintf (buff,
		"%s/helpu -geom +20+20 %s %s/analyzer.help &",HOME,
		"-title \"Analyzer Help\" ",HOME);
	system (buff);
}

void
done(w,u_data,w_data)
	Widget	w;
	XtPointer	u_data;
	XtPointer	w_data;
{
	notify_unravel (w);
	system ("cat HISTORY-A >> HISTORY");
	exit (0);
}

void
move_list (ix,from,to)
	char	*from[],*to[];
	int		ix;
{
	char	*moving;
	int		k;

	moving = from[ix];
	k = ix;
	while (from[k]){
		from[k] = from[k+1];
		k++;
	}
	k = 0;
	while (to[k]) k++;
	to[k] = moving;
	to[k+1] = NULL;
}

/*
plist (w,nf,list)
	Widget		w;
	nf_ptr		nf;
	XawListReturnStruct   *list;
{
	strcpy (nf->current,list->string);
}
*/

void
alist (w,names,list)
	Widget		w;
	name_ptr	names;
	XawListReturnStruct *list;
{
	int		k;

	XawListUnhighlight(w);
	move_list (list->list_index,names->use,names->skip);
	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}
/*
count_lif(n,f,n_ana,n_not,n_out)
	int		n,*n_ana,*n_not,*n_out;
	char	**f;
{
	int		nk = 0,k = 0,out = 0;
	struct	stat status_c,status_lif,status_t;
	int		c,lif,t;
	char	buff[2000];

	int		s,i;

	for (i = 0; i < n; i++){
		c = stat (f[i],&status_c);
		s = strlen(f[i]);
		strcpy (buff,f[i]);
		buff[s-1] = '\0';
		strcat (buff,"LIF");
		lif = stat (buff,&status_lif);
		strcpy (buff,f[i]);
		buff[s-1] = 'T';
		t = stat (buff,&status_t);
		if ((lif == 0) && (t == 0)){
			if ((status_c.st_mtime > status_t.st_mtime) ||
				(status_c.st_mtime > status_lif.st_mtime)) out++;
			else k++;
		}
		else nk++;

	}

	*n_ana = k;
	*n_not = nk;
	*n_out = out;
}
*/

void
line_cat (line,file)
	char	*line,*file;
{
	int		line_len,file_len;
	char	buff[2000];

	line_len = strlen(line);
	file_len = strlen(file);
	if (line_len + file_len > 60){
		sprintf (buff,"echo %s >>HISTORY-A",line);
		system (buff);
		strcpy (line,file);
		strcat (line," ");
	}
	else {
		strcat (line,file);
		strcat (line," ");
	}
}

void
clear_files (w,names,w_data)
	Widget      w;
	name_ptr    names;
	XtPointer	w_data;
{
	char	buff[2000],file[2000],line[1000];
	int		len,i,status,line_len;

	system ("echo clearing files >>HISTORY-A");
	status = unlink("SYSTEM");
	line_len = 0;
	if (!status) strcpy (line,"SYSTEM ");
	else line[0] = '\0';
	for (i = 0; names->use[i] ; i++){
		strcpy (file,names->use[i]);
		len = strlen(file);
		if (len > 2){
			file[len-1] = 'K';
			status = unlink (file);
			if (!status) line_cat(line,file);
			file[len-1] = 'Y';
			status = unlink (file);
			if (!status) line_cat(line,file);
			file[len-1] = 'T';
			status = unlink (file);
			if (!status) line_cat(line,file);
			file[len-1] = 'H';
			status = unlink (file);
			if (!status) line_cat(line,file);
			file[len-1] = '\0';
			strcat (file,"LIF");
			status = unlink (file);
			if (!status) line_cat(line,file);
			file[len-1] = '\0';
			strcat (file,"LINK");
			status = unlink (file);
			if (!status) line_cat(line,file);
		}
	}
	sprintf (buff,"echo %s >>HISTORY-A",line);
	system(buff);
	sprintf (buff,"%s ; %s ;%s/map >> HISTORY-A 2>&1",
		"echo \" \" >>HISTORY-A","echo running map >> HISTORY-A",
		HOME);
	system(buff);
	notify_unravel (w);
}

void
analyze_skipped (w,names,list)
	Widget      w;
	name_ptr    names;
	XtPointer	list;
{
	struct	stat status_c,status_lif,status_t;
	int		c,lif,t;
	int			i,len;
	int			n_skip = 0,n_use = 0;
	char		file[2000],*src;

	for (i = 0; names->files[i] ; i++){
		src = names->files[i]->d_name;
		strcpy (file,src);
		len = strlen(src);
		file[len-1] = 'T';
		t = stat (file,&status_t);
		file[len-1] = '\0';
		strcat (file,"LIF");
		lif = stat (file,&status_lif);
		c = stat (src,&status_c);
		if (c == 0)
			if ((lif == 0) && (t == 0)){
				if ((status_c.st_mtime > status_t.st_mtime) ||
					(status_c.st_mtime > status_lif.st_mtime)) 
						names->use[n_use++] = src;
				else names->skip[n_skip++] = src;
			}
			else names->use[n_use++] = src;
		else names->skip[n_skip++] = src;
	}
	names->skip[n_skip] = NULL;
	names->use[n_use] = NULL;

	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}

void
select_analyzed (w,names,w_data)
	Widget      w;
	name_ptr    names;
	XtPointer	w_data;
{
	char		**hold;

	analyze_skipped (w,names,NULL);
	hold = names->skip;
	names->skip = names->use;
	names->use = hold;

	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}

void
analyze_most (w,names,list)
	Widget      w;
	name_ptr    names;
	XtPointer	list;
{
	int			i;
	for (i = 0; names->files[i] ; i++){
		names->use[i] = names->files[i]->d_name;
	}
	names->use[i] = NULL;
	names->skip[0] = NULL;
	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}

void
analyze_some (w,names,list)
	Widget      w;
	name_ptr    names;
	XtPointer	list;
{
	int			i;
	for (i = 0; names->files[i] ; i++){
		names->skip[i] = names->files[i]->d_name;
	}
	names->skip[i] = NULL;
	names->use[0] = NULL;
	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}

void
ilist (w,names,list)
	Widget		w;
	name_ptr	names;
	XawListReturnStruct *list;
{
	int		k;

	XawListUnhighlight(w);
	move_list (list->list_index,names->skip,names->use);
	XawListChange(names->keep,names->use,0,0,True);
	XawListChange(names->ignore,names->skip,0,0,True);
}

void
adjust_lists (frame,left,right)
	Widget	frame,right,left;
{
	Dimension	frame_w,w,left_bw,right_bw,left_w,right_w;
	int			dist,default_dist;

	XtUnmanageChild (left);
	XtUnmanageChild (right);

	XtVaGetValues (frame,
		XtNdefaultDistance,&default_dist,
		XtNwidth,&frame_w,
		NULL);

	XtVaGetValues (left,
		XtNwidth,&left_w,
		XtNborderWidth,&left_bw,
		NULL);

	XtVaGetValues (right,
		XtNwidth,&right_w,
		XtNborderWidth,&right_bw,
		NULL);

	w = frame_w - 2*(left_bw + right_bw + default_dist) - default_dist;
	w = w/2;
	XtVaSetValues (right,
		XtNwidth,	(XtArgVal) (w),
		NULL);
	XtVaSetValues (left,
		XtNwidth,	(XtArgVal) (w),
		NULL);

	XtManageChild (left);
	XtManageChild (right);

}

void
rj_button (goal,left,w)
	Widget	goal,w,left;
{
	Dimension	bw,goal_w,button_w,left_w,left_bw;
	int			dd,hd;
	Position	x,left_x;

	XtUnmanageChild(w);

	XtVaGetValues (goal,
		XtNdefaultDistance,&dd,
		XtNwidth,&goal_w,
		NULL);

	XtVaGetValues (left,
		XtNborderWidth,&left_bw,
		XtNwidth,&left_w,
		XtNx,&left_x,
		NULL);

	XtVaGetValues (w,
		XtNborderWidth,&bw,
		XtNwidth,&button_w,
		XtNx,&x,
		NULL);

	hd = goal_w - (2*bw + left_x + left_w + 2*left_bw + dd + button_w);
	if (hd < 0) hd = dd;

	XtVaSetValues (w,
		XtNhorizDistance,	(XtArgVal) (hd),
		NULL);

	XtManageChild(w);
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

void
align(label1,text1,label2,text2)
	Widget	label1,text1,label2,text2;
{
	Dimension		w1,w2,bw1,bw2;
	int				dd,d;
	Widget			w,frame;

	frame = XtParent (label1);
	XtVaGetValues (frame,
		XtNdefaultDistance, &dd,
		NULL);
	XtVaGetValues (label1,
		XtNwidth, &w1,
		NULL);
	XtVaGetValues (label2,
		XtNwidth, &w2,
		NULL);
	if (w1 == w2) return;
	if (w1 > w2){
		w = text2;
		d = dd + (w1 - w2);
	}
	else {
		w = text1;
		d = dd + (w2 - w1);
	}
	XtVaSetValues (w,
		XtNhorizDistance, d,
		NULL);
	XtUnmanageChild (w);
	XtManageChild (w);
	
}

Widget 
make_list_box (parent,prefix,title)
	char	*prefix,*title;
	Widget	parent;
{
	Widget	frame,label,port,list;
	char	name[2000];

	strcpy (name,prefix);
	frame = XtVaCreateManagedWidget (strcat(name,"Frame"),
		formWidgetClass, parent,
		XtNborderWidth,	2,
		NULL);

	strcpy (name,prefix);
	label = XtVaCreateManagedWidget (strcat(name,"Label"),
		labelWidgetClass, frame,
		XtNlabel,		title,
		XtNjustify,		XtJustifyCenter,
		XtNborderWidth,	0,
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		NULL);
	strech (frame,label);

	strcpy (name,prefix);
	port = XtVaCreateManagedWidget (strcat(name,"Port"),
		viewportWidgetClass, frame,
		XtNfromVert,		label,
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainBottom,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		XtNforceBars,		True,
		XtNallowVert,		True,
		XtNheight,			200,
		NULL);

	strcpy (name,prefix);
	list = XtVaCreateManagedWidget (strcat(name,"List"),
		listWidgetClass, port,
		XtNdefaultColumns,  1,
		XtNforceColumns,True,
		NULL);
	return frame;
}

void
make_windows(top,ac)
	Widget	top;
	XtAppContext	ac;
{
	Widget			help,quit;
	Widget			selected,not_selected;
	Widget			info,status;
	Widget			frame,cpp_text,cpp_label;
	Widget			clear,select_ana;
	Widget			parse_text,parse_label;
	Widget			op_menu,opt_menu,op_button,opt_button;
	Widget			scan_all,select_all;
	Widget			select_none,select_not,run_selected;
	static char		cpp_buff[2000] = "",buff[2000];
	static char		parser_flags[2000] = "";
	Dimension		W,H;
	static struct dirent 	**files;
	static char		*file_names[3000],
					*pick_names[3000],
					*skip_names[3000] = {NULL};
	static name_rec		names;
	static nf_rec		nf;
	int				i,n_files;
	static XtResource res[] = {
		{XtNrunningFG,XtCBackground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(nf_rec,run_fg),XtRString,"red"},
		{XtNrunningBG,XtCForeground, XtRPixel, sizeof(Pixel),
			XtOffsetOf(nf_rec,run_bg),XtRString,"yellow"}};

	XtGetApplicationResources (top,&nf,res,XtNumber(res),NULL,0);

	frame = XtVaCreateManagedWidget ("frame",formWidgetClass,
		top,NULL);

	quit = XtVaCreateManagedWidget ("quit",commandWidgetClass,
		frame,
		XtNlabel,			"Exit Analyzer",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainLeft,
		NULL);
	XtAddCallback(quit,XtNcallback,done,NULL);

	opt_menu = XtVaCreatePopupShell ("optMenu",
		simpleMenuWidgetClass,top,NULL);
	
	opt_button = XtVaCreateManagedWidget ("selections",
		menuButtonWidgetClass,frame,
			XtNlabel,		(XtArgVal) "File Selection",
			XtNtop,			(XtArgVal) XawChainTop,
			XtNbottom,		(XtArgVal) XawChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNmenuName,	(XtArgVal) "optMenu",
			XtNfromHoriz,	(XtArgVal) quit,
			XtNright,		(XtArgVal) XawChainLeft,
			XtNleft,		(XtArgVal) XawChainLeft,
		NULL);
	/*
	Widget			select_none,select_not,run_selected;
	*/
	select_all = XtVaCreateManagedWidget ("selectAll",
		smeBSBObjectClass,opt_menu,
		XtNlabel,		(XtArgVal) "Select All Files",
		NULL);
	select_not = XtVaCreateManagedWidget ("selectNot",
		smeBSBObjectClass,opt_menu,
		XtNlabel,		(XtArgVal) "Select Files Not Analyzed",
		NULL);
	select_none = XtVaCreateManagedWidget ("selectNone",
		smeBSBObjectClass,opt_menu,
		XtNlabel,		(XtArgVal) "Select No Files",
		NULL);
	select_ana = XtVaCreateManagedWidget ("selectAnalyzed",
		smeBSBObjectClass,opt_menu,
		XtNlabel,		(XtArgVal) "Select Analyzed Files",
		NULL);
	XtAddCallback (select_all,XtNcallback,analyze_most,&names);
	XtAddCallback (select_none,XtNcallback,analyze_some,&names);
	XtAddCallback (select_not,XtNcallback,analyze_skipped,&names);
	XtAddCallback (select_ana,XtNcallback,select_analyzed,&names);

	/*
	op_menu = XtVaCreateManagedWidget ("opMenu",
		simpleMenuWidgetClass,top,NULL);
	
	op_button = XtVaCreateManagedWidget ("Operations",
		menuButtonWidgetClass,frame,
			XtNtop,			(XtArgVal) XawChainTop,
			XtNbottom,		(XtArgVal) XawChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNmenuName,	(XtArgVal) "opMenu",
			XtNfromHoriz,	(XtArgVal) opt_button,
			XtNright,		(XtArgVal) XawChainLeft,
			XtNleft,		(XtArgVal) XawChainLeft,
		NULL);
	scan_all = XtVaCreateManagedWidget ("scanAll",
		smeBSBObjectClass,op_menu,
		XtNlabel,		(XtArgVal) "Analyze All Files",
		NULL);
	run_selected = XtVaCreateManagedWidget ("scanAll",
		smeBSBObjectClass,op_menu,
		XtNlabel,		(XtArgVal) "Analyze Selected Files",
		NULL);
	*/
	op_button = XtVaCreateManagedWidget ("operations",
		commandWidgetClass,frame,
			XtNlabel,		(XtArgVal) "Analyze Selected Files",
			XtNtop,			(XtArgVal) XawChainTop,
			XtNbottom,		(XtArgVal) XawChainTop,
			XtNborderWidth,	(XtArgVal) 2,
			XtNfromHoriz,	(XtArgVal) opt_button,
			XtNright,		(XtArgVal) XawChainLeft,
			XtNleft,		(XtArgVal) XawChainLeft,
		NULL);

	nf.ac = ac;
	nf.n = &names;
	nf.running = 0;
	XtAddCallback (op_button,XtNcallback,run_a,&nf);
	clear = XtVaCreateManagedWidget ("clear",commandWidgetClass,
		frame,
		XtNlabel,		"Clear",
		XtNright,		(XtArgVal) XawChainLeft,
		XtNtop,			(XtArgVal) XawChainTop,
		XtNbottom,		(XtArgVal) XawChainTop,
		XtNleft,		(XtArgVal) XawChainLeft,
		XtNfromHoriz,	op_button,
		NULL);
	XtAddCallback (clear,XtNcallback,clear_files,&names);
	help = XtVaCreateManagedWidget ("help",commandWidgetClass,
		frame,
		XtNlabel,			"Help",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainRight,
		XtNright,			XawChainRight,
		XtNfromHoriz,		clear,
		NULL);
	XtAddCallback(help,XtNcallback,push_help,NULL);

	status = XtVaCreateManagedWidget ("status",labelWidgetClass,
		frame,
		XtNlabel,			"Status: waiting for selection",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		XtNfromVert,		quit,
		XtNjustify,			XtJustifyLeft,
		NULL);
	nf.s = status;
	nf.flags = cpp_buff;
	nf.pflags = parser_flags;

	cpp_label = XtVaCreateManagedWidget ("cppLabel",labelWidgetClass,
		frame,
		XtNlabel,			"CPP flags: ",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainLeft,
		XtNfromVert,		status,
		XtNjustify,			XtJustifyLeft,
		XtNborderWidth,		(XtArgVal) 0,
		NULL);

	cpp_text = XtVaCreateManagedWidget ("cppText",
		asciiTextWidgetClass,frame,
		XtNfromHoriz,		(XtArgVal) cpp_label,
		XtNfromVert,		(XtArgVal) status,
		XtNleft,			(XtArgVal) XawChainLeft,
		XtNright,			(XtArgVal) XawChainRight,
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainTop,
		XtNborderWidth,		(XtArgVal) 2,
		XtNautoFill,		(XtArgVal) False,
		XtNdisplayNonprinting, (XtArgVal) True,
		XtNeditType,		(XtArgVal) XawtextEdit,
		XtNlength,			(XtArgVal) 1000,
		/*
		XtNresize,			(XtArgVal) XawtextResizeWidth,
		XtNheight,			(XtArgVal) 200,
		XtNwidth,			(XtArgVal) 200,
		*/
		XtNstring,			(XtArgVal) cpp_buff,
		XtNuseStringInPlace,(XtArgVal) True,
		XtNscrollHorizontal,(XtArgVal) XawtextScrollAlways,
		NULL);

	parse_label = XtVaCreateManagedWidget ("parserLabel",labelWidgetClass,
		frame,
		XtNlabel,			"Parse flags: ",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainLeft,
		XtNfromVert,		cpp_text,
		XtNjustify,			XtJustifyLeft,
		XtNborderWidth,		(XtArgVal) 0,
		NULL);

	parse_text = XtVaCreateManagedWidget ("parseText",
		asciiTextWidgetClass,frame,
		XtNfromHoriz,		(XtArgVal) parse_label,
		XtNfromVert,		(XtArgVal) cpp_text,
		XtNleft,			(XtArgVal) XawChainLeft,
		XtNright,			(XtArgVal) XawChainRight,
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainTop,
		XtNborderWidth,		(XtArgVal) 2,
		XtNautoFill,		(XtArgVal) False,
		XtNdisplayNonprinting, (XtArgVal) True,
		XtNeditType,		(XtArgVal) XawtextEdit,
		XtNlength,			(XtArgVal) 1000,
		/*
		XtNresize,			(XtArgVal) XawtextResizeWidth,
		XtNheight,			(XtArgVal) 200,
		XtNwidth,			(XtArgVal) 200,
		*/
		XtNstring,			(XtArgVal) parser_flags,
		XtNuseStringInPlace,(XtArgVal) True,
		XtNscrollHorizontal,(XtArgVal) XawtextScrollAlways,
		NULL);

	selected = make_list_box (frame,"select","Selected Files");
	XtVaSetValues (selected,
		XtNfromVert,		(XtArgVal) parse_text,
		XtNleft,			(XtArgVal) XawChainLeft,
		/*
		XtNright,			(XtArgVal) XawChainRight,
		*/
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainBottom,
		NULL);

	not_selected = make_list_box (frame,"notSelect","Files Not Selected");
	XtVaSetValues (not_selected,
		XtNfromVert,		(XtArgVal) parse_text,
		/*
		XtNleft,			(XtArgVal) XawChainRight,
		*/
		XtNright,			(XtArgVal) XawChainRight,
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainBottom,
		XtNfromHoriz,		(XtArgVal) selected,
		NULL);

	XtAddCallback (XtNameToWidget(selected,"*selectList"),
		XtNcallback,alist,&names);
	n_files = scan_dir(&files);
	for (i = 0; files[i] ; i++){
		file_names[i] = files[i]->d_name;
		pick_names[i] = files[i]->d_name;
	}
	names.use = file_names;
	names.skip = skip_names;
	names.pick = pick_names;
	names.files = files;
	file_names[n_files] = NULL;
	names.ignore = XtNameToWidget (not_selected,"*notSelectList");
	names.keep = XtNameToWidget(selected,"*selectList");
	XtAddCallback (XtNameToWidget (not_selected,"*notSelectList"),
		XtNcallback,ilist,&names);

	analyze_skipped (NULL,&names,NULL);

	info = XtVaCreateManagedWidget ("info",labelWidgetClass,
		frame,
		XtNlabel,
		"Description of object under mouse pointer is displayed here",
		XtNtop,				XawChainBottom,
		XtNbottom,			XawChainBottom,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		XtNfromVert,		selected,
		XtNjustify,			XtJustifyLeft,
		NULL);

	XtVaGetValues (cpp_text,
		XtNwidth, &W,
		XtNheight,&H,
		NULL);
	H += 20;
	XtUnmanageChild(cpp_text);
	XtVaSetValues (cpp_text,
		XtNheight,H,
		NULL);
	XtManageChild(cpp_text);

	XtVaGetValues (parse_text,
		XtNwidth, &W,
		XtNheight,&H,
		NULL);
	H += 20;
	XtUnmanageChild(parse_text);
	XtVaSetValues (parse_text,
		XtNheight,H,
		NULL);
	XtManageChild(parse_text);

	XtRealizeWidget(top);

	adjust_lists (frame,selected,not_selected);
	align(cpp_label,cpp_text,parse_label,parse_text);

	rj_button (frame,clear,help);
	strech (frame,status);
	strech (frame,cpp_text);
	strech (frame,parse_text);
	strech (frame,info);
	strech (selected, XtNameToWidget (selected,"selectLabel"));
	strech (not_selected,
		XtNameToWidget (not_selected,"notSelectLabel"));

	XtInstallAccelerators (frame,quit);
	XtInstallAccelerators (frame,help);
	help_label = info;
	set_button_help (opt_button,
	   "Use this menu to select all files, no files or analyzed files");
	set_button_help (clear,
		"Click to remove analysis of selected files");
	set_button_help (op_button,
		"Click to run analysis on selected files");
	set_button_help (info,
		"Description of object under mouse pointer is displayed here");
	set_button_help (quit,"Click to exit analyzer");
	set_button_help (help,
		"Click to pop up a window describing analyzer");
	set_button_help (frame,"   . . .");
	set_button_help (parse_text,"Type in any analyzer flags");
	set_button_help (parse_label,"Flags for running the analyzer");
	set_button_help (cpp_text,"Type in any cpp flags");
	set_button_help (cpp_label,
		"Flags for running the C preprocessor (CPP)");
	set_button_help (status,"Display status as analysis runs");
	set_button_help (XtNameToWidget (selected,"*vertical"),
		"use middle mouse button to slide scroll bar");
	set_button_help (XtNameToWidget (not_selected,"*vertical"),
		"use middle mouse button to slide scroll bar");
	set_button_help (XtNameToWidget (selected,"*selectList"),
		"Click on file name to remove a file from selected list");
	set_button_help (XtNameToWidget (not_selected,"*notSelectList"),
		"Click on file name to add a file to selected list");
	set_button_help (XtNameToWidget (selected,"selectLabel"),
		"List of files selected for analysis");
	set_button_help (XtNameToWidget (not_selected,"notSelectLabel"),
		"List of files not to be analyzed");

}

char *fall[] = {
		/* don't move the first entry (iconPixmap) */
		"*iconPixmap:	analyzer.icon",
		/*"*font:	lucidasans-bold-14",*/
		"*font:         sans-serif",
		"*background: seashell3",
		"*foreground: blue",
		"*shapeStyle: oval",
		"*Label*borderWidth: 0",
		"*Command*borderWidth: 2",
		"*info*borderWidth: 3",
		"*title: Unravel Version 2.2 Analyzer Control Panel",
		"*geometry: +50+25",
		"*quit.accelerators: #override\\n \
			<KeyPress>q: set() highlight() notify()",
		"*help.accelerators:  \
			<KeyPress>h: set() highlight() notify() unset()\\n\
			<KeyPress>?: set() highlight() notify() unset()",
	NULL};

int
main (np,p)
	int		np;
	char	*p[];
{
	XtAppContext	ac;
	Widget			top;
	int				i;
	char			icon_at[2000];
	static XrmOptionDescRec opt[] = {
		{"-runningfg", "*runningFG", XrmoptionSepArg,NULL},
		{"-runningbg", "*runningBG", XrmoptionSepArg,NULL}};

	sprintf (icon_at,"*iconPixmap: %s/analyzer.icon",HOME);
	fall[0] = icon_at;

	top = XtAppInitialize (&ac,"Unravel",opt,XtNumber(opt),&np,p,
		fall,NULL,0);
	if (np == 2) unravel_win_id = atoi(p[1]);
	else unravel_win_id = 0;
	system ("echo -n ANALYZER \"  \" > HISTORY-A ; date >>HISTORY-A");
	make_windows(top,ac);
	XtAppMainLoop(ac);
}
