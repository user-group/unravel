#include "config.h"
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static char sccsid[] = "@(#)helpu.c	1.1  10/14/94";

void quitB(w, x, y) Widget w;
Widget x;
caddr_t y;
{ exit(0); }

int main(argc, argv) int argc;
char **argv;
{
    Widget toplevel, panes, quit;
    Widget editwin, bbox;
    XtAppContext ac;
    char res[2000];

    static char buff[2000];
    static Arg wargs[] = {
        {XtNtype, (XtArgVal)XawAsciiFile},
        {XtNstring, (XtArgVal)buff},
        {XtNlength, (XtArgVal)4999},
        {XtNeditType, (XtArgVal)XawtextEdit},
        {XtNscrollVertical, (XtArgVal)XawtextScrollWhenNeeded},
        {XtNscrollHorizontal, (XtArgVal)XawtextScrollWhenNeeded},
        {XtNheight, (XtArgVal)400},
        {XtNwidth, (XtArgVal)700},
        {XtNeditType, (XtArgVal)XawtextRead},
    };
    static char *fall[] = {
        "*iconPixmap:				helpu.icon", /* don't move */
        "*Command*shapeStyle:		oval",
        "*Command*borderWidth:		4",
        /*"*Command*font:				lucidasans-bolditalic-14",*/
        "*Command*font:                                sans-serif",
        "*font:						lucidasanstypewriter-bold-14",
        "*background:				PeachPuff2",
        "*foreground:				blue2",
        "*Command.label:			Close Help",
        "*geometry: 				+250+150",
        "*quit.accelerators: <KeyPress>q: set() highlight() notify()",
        NULL};

    sprintf(res, "*iconPixmap:  %s/helpu.icon", HOME);
    fall[0]  = res;
    toplevel = XtAppInitialize(&ac, "Unravel", NULL, 0, &argc, argv, fall, NULL, 0);
    panes    = XtCreateManagedWidget("frame", panedWidgetClass, toplevel, NULL, 0);

    bbox = XtCreateManagedWidget("buttons", boxWidgetClass, panes, NULL, 0);
    quit = XtCreateManagedWidget("quit", commandWidgetClass, bbox, NULL, 0);
    XtAddCallback(quit, XtNcallback, quitB, NULL);
    sprintf(buff, "%s", argv[1]);
    editwin = XtCreateManagedWidget("editor", asciiTextWidgetClass, panes, wargs, XtNumber(wargs));

    XtInstallAccelerators(panes, quit);
    XtRealizeWidget(toplevel);

    XtAppMainLoop(ac);
}
