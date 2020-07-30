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

/*
*******************************************************************
*                                                                 *
*   Unravel control panel                                         *
*                                                                 *
*   Jim Lyle  9 June 1994                                         *
*                                                                 *
*******************************************************************
*/

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define WAIT 500
#include "config.h"
#define XtNlinkOnly "linkOnly"

/*
static char sccsid[] = "@(#)unravel.c	1.8  7/26/96";
*/

static int link_only = 0;

typedef struct
{
    Widget top, src, ana, not, out, prog, dup, link;
    Widget text;
    char *dir_buff;
} label_rec, *label_ptr;

Widget help_label;

void
button_help(Widget w, char *mess, XEvent *e, Boolean *ok)
{
    XtVaSetValues(help_label, XtNlabel, (XtArgVal)mess, NULL);
}

void
set_button_help(Widget w, char *mess)
{
    static char buff[] = "Description of object under mouse pointer is displayed here";
    XtAddEventHandler(w, EnterWindowMask, False, button_help, mess);
    XtAddEventHandler(w, LeaveWindowMask, False, button_help, buff);
}

int
compare(struct dirent **a, struct dirent **b)
{
    if (strcmp((*a)->d_name, (*b)->d_name) > 0)
        return 1;
    return 0;
}

int
myscandir(char *dir_name, struct dirent ***list, int (*select)(), int (*compare)())
{
    DIR *dir;
    struct dirent *entry;
    typedef struct l_struct ll_node, *ll;
    struct l_struct
    {
        ll next;
        struct dirent *e;
    };

    int i, n_match = 0;
    ll new_ll;
    ll nodes, node_list = NULL;

    dir = opendir(dir_name);
    if (!dir)
    {
        fprintf(stderr, "directory open failed for %s\n", dir_name);
        exit(1);
    }
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir))
    {
        if (select(entry))
        {
            new_ll       = (ll)malloc(sizeof(ll_node));
            new_ll->next = node_list;
            node_list    = new_ll;
            new_ll->e = (struct dirent *)malloc(sizeof(struct dirent) + strlen(entry->d_name) + 3);
            strcpy(new_ll->e->d_name, entry->d_name);
            n_match++;
        }
    }
    *list = (struct dirent **)malloc((n_match + 1) * sizeof(struct dirent *));
    nodes = node_list;
    for (i = 0; i < n_match; i++)
    {
        (*list)[i] = nodes->e;
        nodes      = nodes->next;
    }
    qsort(*list, n_match, sizeof(struct dirent *), compare);
    return n_match;
}

int
file_selector(struct dirent *entry)
{
    int ix;

    ix = strlen(entry->d_name) - 2;

    if (ix > 0)
        if ((entry->d_name[ix] == '.') && (entry->d_name[ix + 1] == 'c'))
        {
            return 1;
        }
    return 0;
}

int
scan_dir(struct dirent ***f)
{
    struct dirent **files;
    int count;

    count        = myscandir(".", &files, file_selector, compare);
    files[count] = NULL;
    *f           = files;
    return count;
}

void
count_lif(int n, char **f, int *n_ana, int *n_not, int *n_out)
{
    int nk = 0, k = 0, out = 0;
    struct stat status_c, status_lif, status_t;
    int c, lif, t;
    char buff[2000];
    int s, i;

    for (i = 0; i < n; i++)
    {
        c = stat(f[i], &status_c);
        s = strlen(f[i]);
        strcpy(buff, f[i]);
        buff[s - 1] = '\0';
        strcat(buff, "LIF");
        lif = stat(buff, &status_lif);
        strcpy(buff, f[i]);
        buff[s - 1] = 'T';
        t           = stat(buff, &status_t);
        if ((lif == 0) && (t == 0))
        {
            if ((status_c.st_mtime > status_t.st_mtime) ||
                (status_c.st_mtime > status_lif.st_mtime))
                out++;
            else
                k++;
        }
        else
            nk++;
    }

    *n_ana = k;
    *n_not = nk;
    *n_out = out;
}

void
count_system(int *n_prog, int *n_dup, int *n_link)
{
    int n   = 0, k, s, k_ok, link_ok;
    int dup = 0;
    FILE *sys;
    char buff[2000], name[2000];
    int ambig_state = 0;
    struct stat status_k, status_link;

    n       = 0;
    *n_link = 0;
    sys     = fopen("SYSTEM", "r");
    if (sys)
    {
        while (fgets(buff, 2000, sys))
        {
            if (buff[0] == 'M')
            {
                ambig_state = 0;
                k           = sscanf(buff, "MAIN %s %d", name, &k);
                if (k == 2)
                {
                    n++;
                    strcpy(buff, name);
                    s           = strlen(name);
                    buff[s - 1] = '\0';
                    strcat(buff, "LINK");
                    link_ok     = stat(buff, &status_link);
                    buff[s - 1] = '\0';
                    strcat(buff, "K");
                    k_ok = stat(buff, &status_k);
                    if ((k_ok == 0) && (link_ok == 0))
                        (*n_link)++;
                }
            }
            else if (buff[0] == 'A')
            {
                ambig_state = 1;
            }
            else if (buff[0] == 'F')
            {
                ambig_state = 0;
            }
            else if (buff[0] == 'L')
            {
                ambig_state = 0;
            }
            else if (ambig_state)
                dup++;
        }
        fclose(sys);
    }

    *n_prog = n;
    *n_dup  = dup;
}

void
update(Widget w, int n, char *mess)
{
    char buff[2000];

    sprintf(buff, mess, n);
    XtVaSetValues(w, XtNlabel, buff, NULL);
}

void
update_src_info(label_ptr label)
{
    char **file_names = NULL;
    struct dirent **files;
    int n_files, i, n_analyzed, n_prog;
    int n_not, n_out, n_dup, n_link;
    Widget src, ana, not, out, prog, dup, link;

    src        = label->src;
    ana        = label->ana;
    not        = label->not ;
    out        = label->out;
    prog       = label->prog;
    dup        = label->dup;
    link       = label->link;
    n_files    = scan_dir(&files);
    file_names = (char **)malloc((n_files + 1) * sizeof(char *));
    for (i = 0; files[i]; i++)
    {
        file_names[i] = files[i]->d_name;
    }
    file_names[n_files] = NULL;
    count_lif(n_files, file_names, &n_analyzed, &n_not, &n_out);
    count_system(&n_prog, &n_dup, &n_link);

    update(ana,
        n_analyzed,
        n_analyzed == 1 ? "%3d file analyzed & up to date" : "%3d files analyzed & up to date");
    update(src, n_files, n_files == 1 ? "%3d source file" : "%3d source files");
    update(
        not, n_not, n_not == 1 ? "%3d source file not analyzed" : "%3d source files not analyzed");
    update(out,
        n_out,
        n_out == 1 ? "%3d file analyzed & out of date" : "%3d files analyzed & out of date");
    update(link,
        n_link,
        n_link == 1 ? "%3d main program file linked" : "%3d main program files linked");
    update(prog,
        n_prog,
        n_prog == 1 ? "%3d main program file analyzed" : "%3d main program files analyzed");
    update(dup,
        n_dup,
        n_dup == 1 ? "%3d duplicate procedure found" : "%3d duplicate procedures found");
}

void
ana_done(Widget w, label_ptr labels, XEvent *e, Boolean *ok)
{
    update_src_info(labels);
}

void
push_analysis(Widget w, label_ptr labels, XtPointer w_data)
{
    char buff[2000];
    static int set = 0;

    if (!set)
    {
        set = 1;
        XtAddEventHandler(w, NoEventMask, True, ana_done, labels);
    }
    sprintf(buff, "PATH=%s:$PATH ; analyzer %ld &", HOME, XtWindow(w));
    fprintf(stderr, "executing [%s]\n", buff);
    system(buff);
    update_src_info(labels);
}

void
push_review(Widget w, int hist, XtPointer w_data)
{
    char buff[2000];
    static char *title[]     = {"Result of Last Analysis",
        "Last Slices Requested",
        "Log of Current Unravel Session",
        "Unravel Log of this Directory"},
                *file_name[] = {"HISTORY-A", "HISTORY-S", "HISTORY", "HISTORY.LOG"};
    sprintf(buff,
        "%s/helpu %s -geom +10+10 -title \"%s\" %s &",
        HOME,
        "-xrm \"*Command.label: History Review Done\"",
        title[hist],
        file_name[hist]);
    system(buff);
}

void
push_slice(Widget w, XtPointer u_data, XtPointer w_data)
{
    char buff[2000];
    sprintf(buff, "%s/select %s >HISTORY-S 2>&1 &", HOME, link_only ? "-noslice" : "");
    system(buff);
}

void
push_dir(Widget w, label_ptr labels, XtPointer w_data)
{
    char buff[2000];
    int status;
    XawTextPosition from, to;
    XawTextBlock block;

    status = chdir(labels->dir_buff);
    if (status)
    {
        strcpy(buff, (char *)getcwd(NULL, 1000));
        block.firstPos = 0;
        block.length   = strlen(buff);
        block.ptr      = buff;
        block.format   = FMT8BIT;

        from = 0;
        to   = strlen(labels->dir_buff);
        /*
        XawTextReplace (labels->text,from,to,&block);
        XawTextSetInsertionPoint (labels->text,block.length);
        */
    }
    else
    {
        update_src_info(labels);
        sprintf(buff, "echo -n \"Unravel change directory: %s \" >> HISTORY", labels->dir_buff);
        system(buff);
        system("date >> HISTORY");
    }
}

void
push_help(Widget w, XtPointer u_data, XtPointer w_data)
{
    char buff[2000];

    sprintf(buff,
        "%s/helpu -geom +10+10 %s/unravel.help %s &",
        HOME,
        HOME,
        "-title \"Unravel Main Help\" ");
    system(buff);
}

void
done(Widget w, XtPointer u_data, XtPointer w_data)
{
    system("/bin/cat HISTORY >> HISTORY.LOG; /bin/rm -f HISTORY *ERR");
    exit(0);
}

void
strech(Widget goal, Widget w)
{
    Dimension width, bw;
    Position x;
    int dist;

    XtUnmanageChild(w);
    XtVaGetValues(goal, XtNdefaultDistance, &dist, XtNwidth, &width, NULL);
    XtVaGetValues(w, XtNborderWidth, &bw, XtNx, &x, NULL);
    XtVaSetValues(w, XtNwidth, (XtArgVal)(width - 2 * bw - dist - x), NULL);
    XtManageChild(w);
}

void
make_windows(Widget top)
{
    Widget analysis, review, slice, help, quit;
    Widget info, dir;
    Widget src, ana, not, out, prog, dup;
    Widget frame, link;
    Widget review_menu, review_ana, review_slice, review_all, review_this;
    static label_rec labels;
    static char dir_buff[2000] = "";
    char buff[2000];

    labels.top      = top;
    labels.dir_buff = dir_buff;
    frame           = XtVaCreateManagedWidget("frame", formWidgetClass, top, NULL);

    quit = XtVaCreateManagedWidget("quit",
        commandWidgetClass,
        frame,
        /*
        XtNlabel,			"Exit Unravel",
        */
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainLeft,
        NULL);
    XtAddCallback(quit, XtNcallback, done, NULL);

    analysis = XtVaCreateManagedWidget("analysis",
        commandWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainLeft,
        XtNfromHoriz,
        quit,
        NULL);
    XtAddCallback(analysis, XtNcallback, push_analysis, &labels);

    review_menu = XtVaCreatePopupShell("reviewMenu", simpleMenuWidgetClass, top, NULL);

    review       = XtVaCreateManagedWidget("review",
        menuButtonWidgetClass,
        frame,
        XtNlabel,
        "Review History",
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainLeft,
        XtNfromHoriz,
        analysis,
        XtNmenuName,
        (XtArgVal) "reviewMenu",
        NULL);
    review_ana   = XtVaCreateManagedWidget("reviewAnalysis",
        smeBSBObjectClass,
        review_menu,
        XtNlabel,
        (XtArgVal) "Last Analysis",
        NULL);
    review_slice = XtVaCreateManagedWidget(
        "reviewSlice", smeBSBObjectClass, review_menu, XtNlabel, (XtArgVal) "Last Slice", NULL);
    review_this = XtVaCreateManagedWidget(
        "review", smeBSBObjectClass, review_menu, XtNlabel, (XtArgVal) "This Session", NULL);
    review_all = XtVaCreateManagedWidget(
        "reviewAll", smeBSBObjectClass, review_menu, XtNlabel, (XtArgVal) "All History", NULL);
    XtAddCallback(review_ana, XtNcallback, push_review, 0);
    XtAddCallback(review_slice, XtNcallback, push_review, 1);
    XtAddCallback(review_this, XtNcallback, push_review, 2);
    XtAddCallback(review_all, XtNcallback, push_review, 3);

    slice = XtVaCreateManagedWidget("slice",
        commandWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainLeft,
        XtNfromHoriz,
        review,
        NULL);
    XtAddCallback(slice, XtNcallback, push_slice, NULL);

    help = XtVaCreateManagedWidget("help",
        commandWidgetClass,
        frame,
        XtNlabel,
        "Help",
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainRight,
        XtNright,
        XawChainRight,
        XtNfromHoriz,
        slice,
        NULL);
    XtAddCallback(help, XtNcallback, push_help, NULL);

    strcpy(buff, "Current directory: ");

    /*
    strcat(buff, "/home/alan/Documents/unravel");
    */
    strcat(buff, getcwd(NULL, 1000));
    dir = XtVaCreateManagedWidget("dir",
        labelWidgetClass,
        frame,
        XtNlabel,
        buff,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        quit,
        XtNjustify,
        XtJustifyLeft,
        NULL);
    /*
    dir_label = XtVaCreateManagedWidget ("dirLabel",
            commandWidgetClass,
            frame,
            XtNlabel,			buff,
            XtNtop,				XawChainTop,
            XtNbottom,			XawChainTop,
            XtNleft,			XawChainLeft,
            XtNright,			XawChainLeft,
            XtNfromVert,		quit,
            XtNjustify,			XtJustifyLeft,
            NULL);

    XtAddCallback(dir_label,XtNcallback,push_dir,&labels);
    strcpy (dir_buff,getcwd(NULL,1000));
    dir = XtVaCreateManagedWidget ("dir",
            asciiTextWidgetClass,
            frame,
            XtNfromHoriz,		(XtArgVal) dir_label,
            XtNfromVert,		(XtArgVal) quit,
            XtNleft,			(XtArgVal) XawChainLeft,
            XtNright,			(XtArgVal) XawChainRight,
            XtNtop,				(XtArgVal) XawChainTop,
            XtNbottom,			(XtArgVal) XawChainTop,
            XtNborderWidth,		(XtArgVal) 0,
            XtNautoFill,		(XtArgVal) False,
            XtNdisplayNonprinting, (XtArgVal) True,
            XtNeditType,		(XtArgVal) XawtextEdit,
            XtNlength,			(XtArgVal) 1000,
            XtNstring,			(XtArgVal) dir_buff,
            XtNuseStringInPlace,(XtArgVal) True,
            NULL);
    XawTextSetInsertionPoint (dir,strlen(dir_buff));
    */
    labels.text = dir;

    src = XtVaCreateManagedWidget("src",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        dir,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    ana = XtVaCreateManagedWidget("ana",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        src,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    not = XtVaCreateManagedWidget("not",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        ana,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    out = XtVaCreateManagedWidget("out",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        not,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    prog = XtVaCreateManagedWidget("prog",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        out,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    link = XtVaCreateManagedWidget("link",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        prog,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    dup = XtVaCreateManagedWidget("dup",
        labelWidgetClass,
        frame,
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        link,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    info = XtVaCreateManagedWidget("info",
        labelWidgetClass,
        frame,
        XtNlabel,
        "Description of object under mouse pointer is displayed here",
        XtNtop,
        XawChainTop,
        XtNbottom,
        XawChainTop,
        XtNleft,
        XawChainLeft,
        XtNright,
        XawChainRight,
        XtNfromVert,
        dup,
        XtNjustify,
        XtJustifyLeft,
        NULL);

    labels.src  = src;
    labels.ana  = ana;
    labels.not  = not ;
    labels.out  = out;
    labels.prog = prog;
    labels.dup  = dup;
    labels.link = link;
    update_src_info(&labels);

    XtRealizeWidget(top);
    strech(frame, ana);
    strech(frame, dir);
    strech(frame, info);
    strech(frame, not );
    strech(frame, out);
    strech(frame, prog);
    strech(frame, link);
    strech(frame, dup);
    strech(frame, src);

    help_label = info;
    set_button_help(quit, "Click to exit when finished");
    set_button_help(help, "Click to pop up a window describing unravel");
    set_button_help(ana, "Display how many source files have been analyzed");
    set_button_help(not, "Display how many source files have not been analyzed");
    set_button_help(out, "Display how many source files have been modified since being analyzed");
    set_button_help(slice, "Click to setup and run the program slicer");
    set_button_help(review, "Click to pop up menu of history displays");
    set_button_help(analysis, "Click to setup and run the analyzer");
    set_button_help(link, "Display # of main programs linked");
    set_button_help(prog, "Display # of files analyzed \
		containing a main procedure");
    set_button_help(dup, "Display how many procedures are unresolved");
    set_button_help(src, "Display how many source files are in the directory");
    set_button_help(dir, "Display the current directory name");
    /*
    set_button_help (dir,
            "Display the current directory name or edit new name");

    set_button_help (dir_label,"Click to change directory");
    */
    set_button_help(frame, "   . . .");
    XtInstallAccelerators(frame, quit);
    XtInstallAccelerators(frame, help);
    /*
    XtInstallAccelerators (dir,dir_label);
    */
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
    "*MenuButton*borderWidth: 2",
    "*info*borderWidth: 3",
    "*quit.label: Exit Unravel",
    "*analysis.label: Run Analyzer",
    "*slice.label: Run Slicer",
    "*title: Unravel Version 2.3 Main Control Panel",
    "*geometry: +10+10",
    "*quit.accelerators: #override\\n \
			<KeyPress>q: set() highlight() notify()",
    "*help.accelerators:  \
			<KeyPress>h: set() highlight() notify() unset()\\n\
			<KeyPress>?: set() highlight() notify() unset()",
    /*
    "*dirLabel.accelerators: #override\\n\
            <KeyPress>Return: set() highlight() notify() unset()\\n\
            <LeaveNotify>: set() highlight() notify() unset()",
    */
    NULL};

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

*/
static XtResource res[] = {{XtNlinkOnly, XtCValue, XtRInt, sizeof(int), 0, XtRString, "0"}};

static XrmOptionDescRec opt[] = {{"-noslice", "*linkOnly", XrmoptionNoArg, "1"}};

int
main(int np, char **p)
{
    XtAppContext ac;
    Widget top;
    int i;
    char buff[2000];
    char icon_at[2000], cmd[2000];
    int sys_ok;
    char *cwd;

    sprintf(icon_at, "*iconPixmap: %s/unravel.icon", HOME);
    fall[0] = icon_at;

    top = XtAppInitialize(&ac, "Unravel", opt, XtNumber(opt), &np, p, fall, NULL, 0);
    fprintf(stderr, "top: %ld\n", ((long int)top));
    XtGetApplicationResources(top, &link_only, res, XtNumber(res), NULL, 0);
    fprintf(stderr, "back from xtgetappres()\n");
    if (np == 2)
    {
        fprintf(stderr, "np==2\n");
        /* printf ("changing to %s\n",p[1]); */
        errno = 0;
        i     = chdir(p[1]);
        fprintf(stderr, "chdir returned %d %d\n", i, errno);
        if (i != 0)
        {
            fprintf(stderr, "%s could not change directory to %s\n", p[0], p[1]);
            perror("");
            exit(1);
        }
    }

    sys_ok = system("echo \" \" >HISTORY");
    if (sys_ok != 0)
    {
        fprintf(stderr, "%s could not write in file directory\n", p[0]);
        fprintf(stderr, "continue? [y|n] ");
        scanf("%c", cmd);
        if (cmd[0] != 'y')
            exit(1);
    }


    cwd = getcwd(buff, 2000);
    fprintf(stderr, "creating cmd...[%s]\n", cwd);

    sprintf(cmd, "echo -n UNRAVEL \" %s \" >> HISTORY ; date >>HISTORY", cwd);
    fprintf(stderr, "cmd: [%s]\n", cmd);
    system(cmd);
    system("echo \"No slices computed this session\" >HISTORY-S");
    system("echo \"No analysis done this session\" >HISTORY-A");
    make_windows(top);
    XtAppMainLoop(ac);
    return 0;
}
