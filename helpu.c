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

/*
static char sccsid[] = "@(#)helpu.c	1.1  10/14/94";
*/

void
quitB(Widget w, Widget x, caddr_t y)
{
    exit(0);
}

int
main(int argc, char **argv)
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
    static char *fall[] = {"*iconPixmap:		helpu.icon", /* don't move */
        "*Command*shapeStyle:	oval",
        "*Command*borderWidth: 4",
        "*Command*font:        lucidasans-bolditalic-14",
        "*font:		lucidasanstypewriter-bold-14",
        "*background:		PeachPuff2",
        "*foreground:		blue2",
        "*Command.label:	Close Help",
        "*geometry: 		+250+150",
        "*quit.accelerators:   <KeyPress>q: set() highlight() notify()",
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
    return 0;
}
