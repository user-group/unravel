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
# include <sys/stat.h>
# include <sys/time.h>
# include <malloc.h>
# include <errno.h>
# include "config.h"
# define WAIT 500

	static  char    sccsid[] = "@(#)select.c	1.6  8/16/95";

int	no_slice = 0;
int sys_nerr;
extern __const char *__const sys_errlist[];

char	*slicer = "u";
Widget	help_label;
typedef struct id_struct id_rec,*id_ptr;
struct id_struct {
	char	*file_name;
	Widget	status,list;
	XtAppContext	ac;
	char	**progs;
};

int	rescan_system(main_file_name,progs)
	char	*main_file_name;
	char	*progs[];
{
	int		n = 0,k,j;
	FILE	*sys;
	char	buff[2000],name[2000];

	sys = fopen ("SYSTEM","r");
	if (!sys) return 0;
	while (fgets (buff,2000,sys)){
		if (buff[0] == 'M'){
			k = sscanf(buff,"MAIN %s %d",name,&j);
			if (k == 2) {
				if (strcmp (name,main_file_name) == 0){
					progs[n] = malloc (strlen(name)+1);
					strcpy(progs[n],name);
					fgets (buff,2000,sys);
					n = 0;
					while (n < j){
						fscanf (sys,"%s",name);
						progs[n] = malloc (strlen(name)+1);
						strcpy(progs[n],name);
						n++;
					}
					return n;
				}
			}
		}
	}
	fclose(sys);
	return 0;
}

int clear_y(file)
	char	*file;
{
	char	file_name[2000];
	int		end;

	end = strlen(file) - 1;
	strcpy (file_name,file);
	file_name[end] = 'Y';
	unlink (file_name);
	return 1;
}

cannot_link (main,file,fmt)
	char	*main,*file,*fmt;
{
	printf ("Link error (link failed)\n  ");
	printf (fmt,main,file);
	printf ("\n");
	exit(1);
}

int need_to_link (main_file_name)
	char	*main_file_name;
{
/*
********************************************************************
*                                                                  *
*  return 0  if link file ok (no need to link)                     *
*  return 1  if need to link                                       *
*                                                                  *
********************************************************************
*/

	struct stat	c_stat,k_stat,link_stat,lif_stat,t_stat,h_stat;
	struct	stat	src_stat;
	char	*progs[1000];
	char	file_name[2000];
	int		i,n,end;

	end = strlen(main_file_name);
	if (end < 3) cannot_link (main_file_name,"","No base name: %s%s");
	end--;

	if (stat(main_file_name,&c_stat))
		cannot_link (main_file_name,errno < sys_nerr?
			sys_errlist[errno]:"","Could not access %s: %s");
	strcpy (file_name,main_file_name);
	file_name[end] = 'K';
	if (stat(file_name,&k_stat)) return clear_y(main_file_name);
	file_name[end] = '\0';
	strcat (file_name,"LINK");
	if (stat(file_name,&link_stat)) return clear_y(main_file_name);
	if ((k_stat.st_mtime < c_stat.st_mtime) ||
		(link_stat.st_mtime < c_stat.st_mtime))
			return clear_y(main_file_name);
	n = rescan_system (main_file_name,progs);
	for (i = 0; i < n; i++){
		strcpy (file_name,progs[i]);
		end = strlen(file_name) - 1;
		if (stat(file_name,&src_stat))
			cannot_link (file_name,errno < sys_nerr?
				sys_errlist[errno]:"","Could not access %s: %s");
		file_name[end] = 'T';
		if (stat(file_name,&t_stat))
			cannot_link (file_name,errno < sys_nerr?
				sys_errlist[errno]:"","Could not access %s: %s");
		file_name[end] = 'H';
		if (stat(file_name,&h_stat))
			cannot_link (file_name,errno < sys_nerr?
				sys_errlist[errno]:"","Could not access %s: %s");
		file_name[end] = '\0';
		strcat (file_name,"LIF");
		if (stat(file_name,&lif_stat))
			cannot_link (file_name,errno < sys_nerr?
				sys_errlist[errno]:"","Could not access %s: %s");
		if ( (h_stat.st_mtime < src_stat.st_mtime) ||
			(t_stat.st_mtime < src_stat.st_mtime) ||
			(lif_stat.st_mtime < src_stat.st_mtime) )
			cannot_link (main_file_name,progs[i],
				"%s: %s newer than LIF, H or T (run analyzer again?)");
		if ( (h_stat.st_mtime > k_stat.st_mtime) ||
			(t_stat.st_mtime > k_stat.st_mtime) ||
			(lif_stat.st_mtime > k_stat.st_mtime) ){
				return clear_y(main_file_name);
				}
		if ( (h_stat.st_mtime > link_stat.st_mtime) ||
			(t_stat.st_mtime > link_stat.st_mtime) ||
			(lif_stat.st_mtime > link_stat.st_mtime) ){
				return clear_y(main_file_name);
				}
	}
	printf ("%s already linked\n",main_file_name);
	return 0;

}

only_one(file_name)
	char	*file_name;
{
	char	buff[2000];

	printf ("Linking %s\n",file_name);
	if(need_to_link (file_name)){
		sprintf (buff,"%s/slink  %s >>HISTORY-S 2>&1 ",HOME,file_name);
		 /* fclose(stdout); */ fflush(stdout);
		system (buff);
	}
	else /* fclose(stdout); */ fflush(stdout);
	if (!no_slice){
		sprintf (buff,"%s/%s  %s >>HISTORY-S 2>&1 &",HOME,slicer
			,file_name);
		system (buff);
	}
	/*
	sprintf (buff,"%s/%s  %s >>HISTORY-S 2>>HISTORY-S &",
		HOME,slicer,file_name);
	system (buff);
	*/
	exit(0);
}

int cmp(a,b)
	char	**a,**b;
{
	return strcmp (*a,*b);
}

char	**scan_system()
{
	int		n = 0,k,j;
	int		dup = 0;
	FILE	*sys;
	char	buff[2000],name[2000];
	static  char	*progs[1000];
	int		ambig_state = 0;

	sys = fopen ("SYSTEM","r");
	if (sys){
		while (fgets (buff,2000,sys)){
			if (buff[0] == 'M'){
				ambig_state = 0;
				k = sscanf(buff,"MAIN %s %d",name,&j);
				if (k == 2) {
					progs[n] = malloc (strlen(name)+1);
					strcpy(progs[n],name);
					n++;
				}
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
	if (n == 1) only_one (progs[0]);
	qsort (progs,n,sizeof (char *),cmp);
	progs[n] = NULL;
	return progs;
		
/*************
SYNOPSIS
     qsort(base, nel, width, compar)
     char *base;
     int (*compar)();

DESCRIPTION
     qsort() is an implementation of the quicker-sort  algorithm.
     It sorts a table of data in place.

     base points to the element at the base of the table.  nel is
     the  number of elements in the table.  width is the size, in
     bytes, of each element in the table.  compar is the name  of
     the  comparison function, which is called with two arguments
     that point to the elements being compared.  As the  function
     must  return an integer less than, equal to, or greater than
     zero, so must the first argument to be  considered  be  less
     than, equal to, or greater than the second.
*************/
}

button_help (w,mess,e,ok)
	Widget	w;
	char	*mess;
	XEvent	*e;
	Boolean	*ok;
{
	XtVaSetValues (help_label,
		XtNlabel, (XtArgVal) mess,
		NULL);
}

set_button_help (w,mess)
	Widget	w;
	char	*mess;
{
	static char buff[] =
		"Description of object under mouse pointer is displayed here";
	XtAddEventHandler (w,EnterWindowMask,False,button_help,mess);
	XtAddEventHandler (w,LeaveWindowMask,False,button_help,buff);
}

do_slice (id,tt)
	id_ptr			id;
	XtIntervalId	*tt;
{
	char	buff[2000];
	int		n,at,shifting;

	if (!no_slice){
		sprintf (buff,"%s/%s  %s >>HISTORY-S 2>&1 &",
			HOME,slicer,id->file_name);
		system (buff);
		exit(0);
	}

	at = 0;
	shifting = 0;
	n = 0;
	while (id->progs[at]){
		n++;
		if (shifting){
			id->progs[at] = id->progs[at+1];
		}
		else {
			if (strcmp(id->file_name,id->progs[at]) == 0){
				shifting = 1;
				id->progs[at] = id->progs[at+1];
				n--;
			}
		}
		at++;
	}

	if(n) XawListChange (id->list,id->progs,0,0,False);
	else exit(0);

}

do_link (id,tt)
	id_ptr			id;
	XtIntervalId	*tt;
{
	char	buff[2000];

	if (need_to_link (id->file_name)){
		sprintf (buff,"%s/slink  %s >>HISTORY-S 2>&1 ",HOME,id->file_name);
		/*
		fclose(stdout);
		*/
		fflush(stdout);
		system (buff);
		XtVaSetValues (id->status,
			XtNlabel, "Link finished.  Select another program or exit",
		NULL);
		printf ("F I N I S H E D\n"); fflush(stdout);
	}
	else {/* fclose(stdout); */ fflush(stdout);
		XtVaSetValues (id->status,
			XtNlabel, "Already linked.  Select another program or exit",
		NULL);
	}
	/*
	XtAppAddTimeOut (id->ac,WAIT,do_slice,id);
	*/
	do_slice(id,NULL);
	return;
}

strech_to (frame,w,goal)
	Widget	goal,w,frame;
{
	Dimension		width,width_goal,width_w,bw_w,bw;
	Position		x;
	int				dist;

	XtVaGetValues (frame,
		XtNwidth,&width,
		XtNdefaultDistance, &dist,
		NULL);
	XtVaGetValues (goal,
		XtNwidth,	&width_goal,
		XtNx,		&x,
		XtNborderWidth,&bw,
		NULL);
	XtVaGetValues (w,
		XtNwidth,	&width_w,
		XtNborderWidth,&bw_w,
		NULL);

	width_w = width_w + width - (x + 2*bw + width_goal + dist);

	XtUnmanageChild(w);
	XtVaSetValues (w,
		XtNwidth,	(XtArgVal) width_w,
		NULL);
	XtManageChild(w);
}

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

call_slice (w,id,list)
	Widget	w;
	id_ptr					id;
	XawListReturnStruct   *list;
{

	XtVaSetValues (id->status,
		XtNlabel, "Linking . . .",
		NULL);
	id->file_name = list->string;
	/*
	XtAppAddTimeOut (id->ac,WAIT,do_link,id);
	*/
	do_link(id,NULL);
}

push_quit(w,u_data,w_data)
	Widget	w;
	XtPointer	u_data;
	XtPointer	w_data;
{

	exit(0);
}

push_help(w,u_data,w_data)
	Widget	w;
	XtPointer	u_data;
	XtPointer	w_data;
{
	char	buff[2000];

	sprintf (buff,
		"%s/helpu -geom +20+20 %s %s/select.help &",HOME,
		"-title \"Analyzer Help\" ",HOME);
	system (buff);
}

v_wrap (p,lw)
	Widget	p,lw;
{
	Dimension	hp,hl;

	XtVaGetValues (p,
		XtNheight, &hp,
	NULL);

	XtVaGetValues (lw,
		XtNheight, &hl,
	NULL);


	XtUnmanageChild (p);
	XtUnmanageChild (lw);
	XtVaSetValues (p,
		XtNheight, hl,
	NULL);
	XtManageChild(p);
	XtManageChild(lw);
}

Widget make_list_box (parent,help,status,prefix,title,id)
	char	*prefix,*title;
	Widget	parent,help,status;
	id_ptr	id;
{
	Widget	frame,label,port,list;
	char	name[2000];
	char	**progs;

	strcpy (name,prefix);
	frame = XtVaCreateManagedWidget (strcat(name,"Frame"),
		formWidgetClass, parent,
		XtNborderWidth,	2,
		XtNfromVert,		(XtArgVal) help,
		XtNleft,			(XtArgVal) XawChainLeft,
		XtNright,			(XtArgVal) XawChainRight,
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainBottom,
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
	set_button_help (label, "List of programs ready to be sliced");

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
		XtNheight,			300,
		NULL);


	strcpy (name,prefix);
	id->progs = progs = scan_system();
	list = XtVaCreateManagedWidget (strcat(name,"List"),
		listWidgetClass, port,
		XtNlist,			progs,
		NULL);
	id->list = list;
	if (progs[0] == NULL){
		XtVaSetValues (status,
		XtNlabel,	"Status: No main programs have been analyzed",
		NULL);
	}
	set_button_help (list, "Click on program to link for slicing");
	set_button_help (label, "List of programs available for linking/slicing");
	return frame;
}

make_windows(top,ac)
	Widget	top;
	XtAppContext	ac;
{
	static Widget status,info,selected,frame;
	static Widget	help,quit;
	static id_rec		id;

	id.ac = ac;

	frame = XtVaCreateManagedWidget ("frame",formWidgetClass,
		top,NULL);

	quit = XtVaCreateManagedWidget ("quit",commandWidgetClass,
		frame,
		XtNlabel,			"Exit",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainLeft,
		NULL);
	XtAddCallback(quit,XtNcallback,push_quit,NULL);
	status = XtVaCreateManagedWidget ("status",labelWidgetClass,
		frame,
		XtNlabel,			"Status: waiting for selection",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		XtNfromHoriz,		quit,
		XtNjustify,			XtJustifyLeft,
		NULL);
	help = XtVaCreateManagedWidget ("help",commandWidgetClass,
		frame,
		XtNlabel,			"Help",
		XtNtop,				XawChainTop,
		XtNbottom,			XawChainTop,
		XtNleft,			XawChainRight,
		XtNright,			XawChainRight,
		XtNfromHoriz,		status,
		NULL);
	XtAddCallback(help,XtNcallback,push_help,NULL);
	selected = make_list_box (frame,help,status,"select",
		"Main Program Files",&id);
	id.status = status;
	XtAddCallback(XtNameToWidget (selected,"*selectList"),
		XtNcallback,call_slice,&id);

	/*
	XtVaSetValues (selected,
		XtNfromVert,		(XtArgVal) help,
		XtNleft,			(XtArgVal) XawChainLeft,
		XtNright,			(XtArgVal) XawChainRight,
		XtNtop,				(XtArgVal) XawChainTop,
		XtNbottom,			(XtArgVal) XawChainBottom,
		NULL);
	*/

	info = XtVaCreateManagedWidget ("info",labelWidgetClass,
		frame,
		XtNlabel,
		"Description of object under mouse pointer is displayed here",
		XtNtop,				XawChainBottom,
		XtNbottom,			XawChainBottom,
		XtNleft,			XawChainLeft,
		XtNright,			XawChainRight,
		XtNjustify,			XtJustifyLeft,
		XtNfromVert,		selected,
		NULL);
	help_label = info;
	
	XtRealizeWidget(top);
	/*
	v_wrap (
		XtNameToWidget (selected,"*selectPort"),
		XtNameToWidget (selected,"*selectList"));
		*/

	strech (selected, XtNameToWidget (selected,"*selectLabel"));
	strech (frame,selected);
	strech (frame,info);
	strech_to (frame,status,help);
	/*
	strech (frame,status);
	*/
	set_button_help (status, "Display current status");
	set_button_help (quit, "Click to quit without running slicer");
	set_button_help (help,
		"Click to pop up a window describing selection");
	set_button_help (info,
		"Description of object under mouse pointer is displayed here");
	XtInstallAccelerators (frame,quit);
	XtInstallAccelerators (frame,help);
}

char *fall[] = {
		/* don't move the first entry (iconPixmap) */
		"*iconPixmap:	unravel.icon",
		"*font:	lucidasans-bold-14",
		"*background: seashell3",
		"*foreground: blue",
		"*shapeStyle: oval",
		"*Label*borderWidth: 0",
		"*Command*borderWidth: 2",
		"*info*borderWidth: 3",
		"*title: Unravel Version 2.2 Program Selection Panel",
		"*geometry: +10+10",
		"*quit.accelerators: #override\\n \
			<KeyPress>q: set() highlight() notify()",
		"*help.accelerators:  \
			<KeyPress>h: set() highlight() notify() unset()\\n\
			<KeyPress>?: set() highlight() notify() unset()",
	NULL};

main (np,p)
	int		np;
	char	*p[1000];
{
	XtAppContext	ac;
	Widget			top;
	int				i;
	char			icon_at[2000];
	time_t			clock;

	sprintf (icon_at,"*iconPixmap: %s/unravel.icon",HOME);
	fall[0] = icon_at;

	top = XtAppInitialize (&ac,"Unravel",NULL,0,&np,p,
		fall,NULL,0);
	fprintf(stderr, "made top()\n");
	//clock = time(NULL);
	//fprintf (stderr, "Link & Slice  %s\n",ctime(&clock));
	no_slice = 0;
	if (np == 2) no_slice = 1;
	else if (np == 3) slicer = p[2];
	
	printf ("np (%d) no_slice(%d) slicer (%s)\n",np,no_slice,slicer);
	fflush(stdout);
	
	make_windows(top,ac);
	XtAppMainLoop(ac);
}
