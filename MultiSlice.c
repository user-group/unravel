#include <stdio.h>

#include "MultiSliceP.h"
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Scrollbar.h>
#include <stdlib.h>

static char sccsid[]    = "@(#)MultiSlice.c	1.5  8/14/95";
static char *sccs_h_id  = MSLICE_H_SCCS_ID;
static char *sccs_ph_id = MSLICE_PH_SCCS_ID;

void SliceSetTop(MultiSliceWidget w, float pc);

static XtResource resources[] = {
#define offset(field) XtOffset(MultiSliceWidget, slicetext.field)
    /* {name, class, type, size, offset, default_type, default_addr}, */
    {XtNfont,
        XtCFont,
        XtRFontStruct,
        sizeof(XFontStruct *),
        offset(font),
        XtRString,
        XtDefaultFont},
    {XtNaltBackground,
        XtCForeground,
        XtRPixel,
        sizeof(Pixel),
        offset(alt_background),
        XtRString,
        XtDefaultForeground},
    {XtNaltForeground,
        XtCBackground,
        XtRPixel,
        sizeof(Pixel),
        offset(alt_foreground),
        XtRString,
        XtDefaultBackground},
    {XtNforeground,
        XtCForeground,
        XtRPixel,
        sizeof(Pixel),
        offset(foreground),
        XtRString,
        XtDefaultForeground},
    {XtNsideForeground,
        XtCBackground,
        XtRPixel,
        sizeof(Pixel),
        offset(side_fg),
        XtRString,
        XtDefaultBackground},
    {XtNsideBackground,
        XtCForeground,
        XtRPixel,
        sizeof(Pixel),
        offset(side_bg),
        XtRString,
        XtDefaultForeground},
    {XtNsideWidth, XtCWidth, XtRInt, sizeof(int), offset(side_width), XtRString, "15"},
    {XtNtabStop, XtCTabStop, XtRInt, sizeof(int), offset(tabstop), XtRString, "4"},
    {XtNcallback,
        XtCCallback,
        XtRCallback,
        sizeof(XtCallbackList),
        offset(select_callback),
        XtRCallback,
        NULL},
#undef offset
};

#define Max_line_length 1000
static void doexpose();
int DEBUG = 0;

static void SliceResize();

void dojump(w, x, y) Widget w;
XtPointer x, y;
{
    float *p;
    int at_line;

    p = (float *)y;
    if (DEBUG)
        printf("Do jump %6.2f\n", *p);
    SliceSetTop(x, *p);
    /*
    at_line = (*p)*Lines_in_file + 1;
    if (DEBUG)printf (" at %d\n",at_line);
    */
}

static void fakeexpose(w) MultiSliceWidget w;
{
    XAnyEvent e;

    e.display = XtDisplay(w);
    e.window  = XtWindow(w);
    doexpose(w, &e, NULL);
}

void doscroll(x, w, y) MultiSliceWidget w;
XtPointer x, y;
{
    int i;
    int page = 1;

    i = (int)y;

    if (DEBUG)
        printf("Do scroll %d\n", i);
    page = 0.5 + (w->core.height) / (float)(w->slicetext.font->ascent + w->slicetext.font->descent);
    page--;
    if (DEBUG)
        printf("page %d\n", page);
    if (i < 0)
        w->slicetext.top -= page;
    else
        w->slicetext.top += page;
    if (w->slicetext.top < 1)
        w->slicetext.top = 1;
    else if (w->slicetext.top + page > w->slicetext.slicesrc.n_lines)
        w->slicetext.top = w->slicetext.slicesrc.n_lines - page;
    if (DEBUG)
        printf("Set top to %d\n", w->slicetext.top);
    SliceResize(w);
    fakeexpose(w);
}

static void SliceSetThumb(w, sb) MultiSliceWidget w;
Widget sb;
{ w->slicetext.scrollbar = sb; }

static void reset_top_line(w, delta) MultiSliceWidget w;
int delta;
{
    int page, top;

    top  = w->slicetext.top + delta;
    page = 0.5 + (w->core.height) / (float)(w->slicetext.font->ascent + w->slicetext.font->descent);
    page--;
    if (top < 1)
        top = 1;
    else if (top + page > w->slicetext.slicesrc.n_lines)
        top = w->slicetext.top;
    if (top == w->slicetext.top)
        return;
    w->slicetext.top = top;
    SliceResize(w);
    fakeexpose(w);
}

static void up_a_line(x, w, y) MultiSliceWidget w;
XtPointer x, y;
{ reset_top_line(w, -1); }

static void down_a_line(x, w, y) MultiSliceWidget w;
XtPointer x, y;
{ reset_top_line(w, 1); }

#include "down.xbm"
#include "up.xbm"
Widget CreateSliceBox(name, parent) char *name;
Widget parent;
{
    Widget bar;
    Widget frame;
    Widget slicewin;
    Widget up, dn;
    Arg args[9];
    int na;
    int box_height;
    int sb_left = 0;
    Pixmap up_bitmap, dn_bitmap;
    Display *d;

    d         = XtDisplay(parent);
    up_bitmap = XCreateBitmapFromData(
        XtDisplay(parent), RootWindow(d, DefaultScreen(d)), up_bits, up_width, up_height);
    dn_bitmap = XCreateBitmapFromData(
        XtDisplay(parent), RootWindow(d, DefaultScreen(d)), down_bits, down_width, down_height);
    frame = XtCreateManagedWidget(name, formWidgetClass, parent, NULL, 0);

    if (sb_left) {
        na = 0;
        XtSetArg(args[na], XtNlength, 600);
        na++;
        XtSetArg(args[na], XtNtop, XtChainTop);
        na++;
        XtSetArg(args[na], XtNbottom, XtChainBottom);
        na++;
        XtSetArg(args[na], XtNleft, XtChainLeft);
        na++;
        XtSetArg(args[na], XtNright, XtChainLeft);
        na++;

        bar = XtCreateWidget("bar", scrollbarWidgetClass, frame, args, na);

        XtManageChild(bar);

        na = 0;
        XtSetArg(args[na], XtNresizable, True);
        na++;
        XtSetArg(args[na], XtNtop, XtChainTop);
        na++;
        XtSetArg(args[na], XtNbottom, XtChainBottom);
        na++;
        XtSetArg(args[na], XtNleft, XtChainLeft);
        na++;
        XtSetArg(args[na], XtNright, XtChainRight);
        na++;
        slicewin = XtCreateManagedWidget("slicewin", multisliceWidgetClass, frame, args, na);
    } else {
        na = 0;
        XtSetArg(args[na], XtNresizable, True);
        na++;
        XtSetArg(args[na], XtNtop, XtChainTop);
        na++;
        XtSetArg(args[na], XtNbottom, XtChainBottom);
        na++;
        XtSetArg(args[na], XtNleft, XtChainLeft);
        na++;
        XtSetArg(args[na], XtNright, XtChainRight);
        na++;
        slicewin = XtCreateManagedWidget("slicewin", multisliceWidgetClass, frame, args, na);

        up = XtVaCreateManagedWidget("up",
            commandWidgetClass,
            frame,
            XtNborderWidth,
            0,
            XtNbitmap,
            up_bitmap,
            XtNshapeStyle,
            XmuShapeRectangle,
            XtNfromHoriz,
            slicewin,
            XtNtop,
            XtChainTop,
            XtNbottom,
            XtChainTop,
            XtNleft,
            XtChainRight,
            XtNright,
            XtChainRight,
            NULL);
        na = 0;
        XtSetArg(args[na], XtNlength, 600 - 36);
        na++;
        XtSetArg(args[na], XtNtop, XtChainTop);
        na++;
        XtSetArg(args[na], XtNbottom, XtChainBottom);
        na++;
        XtSetArg(args[na], XtNleft, XtChainRight);
        na++;
        XtSetArg(args[na], XtNright, XtChainRight);
        na++;
        XtSetArg(args[na], XtNfromVert, up);
        na++;
        XtSetArg(args[na], XtNfromHoriz, slicewin);
        na++;

        bar = XtCreateWidget("bar", scrollbarWidgetClass, frame, args, na);

        XtManageChild(bar);
        dn = XtVaCreateManagedWidget("dn",
            commandWidgetClass,
            frame,
            XtNborderWidth,
            0,
            XtNbitmap,
            dn_bitmap,
            XtNshapeStyle,
            XmuShapeRectangle,
            XtNfromHoriz,
            slicewin,
            XtNfromVert,
            bar,
            XtNtop,
            XtChainBottom,
            XtNbottom,
            XtChainBottom,
            XtNleft,
            XtChainRight,
            XtNright,
            XtChainRight,
            NULL);

        XtAddCallback(up, XtNcallback, up_a_line, slicewin);
        XtAddCallback(dn, XtNcallback, down_a_line, slicewin);
    }

    XtAddCallback(bar, XtNscrollProc, doscroll, slicewin);
    XtAddCallback(bar, XtNjumpProc, dojump, slicewin);

    SliceSetThumb(slicewin, bar);
    return slicewin;
}

static void doinit(r, nw, a, n) MultiSliceWidget r, nw;
ArgList a;
Cardinal *n;
{
    XGCValues values;
    Window root = RootWindowOfScreen(XtScreen(nw));

    if (DEBUG)
        printf("Init WxH (%dx%d)\n", nw->core.width, nw->core.height);
    /*
    printf ("    FG %d  ",nw->core.border_pixel);
    */
    if (DEBUG)
        printf("    FG %lu  ", nw->slicetext.foreground);
    if (DEBUG)
        printf("    BG %lu\n", nw->core.background_pixel);
    if (DEBUG)
        printf("Alt FG %lu  ", nw->slicetext.alt_foreground);
    if (DEBUG)
        printf("Alt BG %lu\n", nw->slicetext.alt_background);
    if (DEBUG)
        fflush(stdout);
    if (nw->core.height == 0)
        nw->core.height = 600;
    if (nw->core.width == 0)
        nw->core.width = 800;
    values.foreground    = nw->slicetext.foreground;
    values.background    = nw->core.background_pixel;
    values.font          = nw->slicetext.font->fid;
    nw->slicetext.gc     = XtGetGC((Widget)nw, GCForeground | GCBackground | GCFont, &values);
    values.foreground    = nw->slicetext.alt_foreground;
    values.background    = nw->slicetext.alt_background;
    values.font          = nw->slicetext.font->fid;
    nw->slicetext.alt_gc = XtGetGC((Widget)nw, GCForeground | GCBackground | GCFont, &values);
    nw->slicetext.slicesrc.n_lines = 0;
    nw->slicetext.top              = 1;
    nw->slicetext.scrollbar        = NULL;
    nw->slicetext.log              = 0;
    nw->slicetext.sidebar          = XCreatePixmap(XtDisplay(nw),
        root,
        nw->slicetext.side_width,
        DisplayHeight(XtDisplay(nw), DefaultScreen(XtDisplay(nw))),
        DefaultDepthOfScreen(XtScreen(nw)));
    values.foreground              = nw->slicetext.side_fg;
    values.background              = nw->slicetext.side_bg;
    nw->slicetext.side_gc          = XtGetGC((Widget)nw, GCForeground | GCBackground, &values);
    values.foreground              = nw->slicetext.side_bg;
    values.background              = nw->slicetext.side_fg;
    nw->slicetext.side_clear_gc    = XtGetGC((Widget)nw, GCForeground | GCBackground, &values);
    XFillRectangle(XtDisplay(nw),
        nw->slicetext.sidebar,
        nw->slicetext.side_clear_gc,
        0,
        0,
        nw->slicetext.side_width,
        DisplayHeight(XtDisplay(nw), DefaultScreen(XtDisplay(nw))));
    nw->slicetext.hook = NULL;
    if (DEBUG)
        printf("Tabstop %d\n", nw->slicetext.tabstop);
}

static void buildline(buff, line, n, tabstop, linemap) char *buff, *line;
int *linemap;
int n, tabstop;
{
    int in_at, out_at, add_tabs, n_spaces;
    int i;

    if (DEBUG)
        printf("buildline:\n");
    if (DEBUG)
        for (i = 0; i < n; i++)
            printf("%c", line[i]);
    if (DEBUG)
        printf("[%d,%lu]\n", n, strlen(line));
    out_at = 0;
    for (in_at = 0; in_at < n; in_at++) {
        linemap[in_at] = out_at;
        if (line[in_at] == '\t') {
            n_spaces = tabstop - (out_at % tabstop);
            for (add_tabs = 0; add_tabs < n_spaces; add_tabs++)
                buff[out_at++] = ' ';
        } else
            buff[out_at++] = line[in_at];
    }
    buff[out_at] = '\0';
    linemap[n]   = out_at;
    if (DEBUG)
        printf("%s[%d,%lu]\n", buff, out_at, strlen(buff));
    for (i = 0; i <= n; i++)
        if (DEBUG)
            printf("%3d", linemap[i]);
    if (DEBUG)
        printf("\n");
}

/*
                 drawline (e->display,e->window,
                        w, line_buff, lineoffset,base,at
                 );
*/
static void drawline(d, win, w, buff, offset, base, at, linemap) Display *d;
Window win;
MultiSliceWidget w;
char *buff;
int offset, base, at, *linemap;
{
    int highlight[Max_line_length];
    int i, chunk, from, to, pos, last;
    GC gc, gcx, gc_to_use;
    XFontStruct font, fontx;

    for (i = 0; i <= strlen(buff); i++)
        highlight[i] = 0;
    if (DEBUG)
        printf("chunks(%d) ", w->slicetext.slicesrc.line[at].n_highlight);
    for (chunk = 0; chunk < w->slicetext.slicesrc.line[at].n_highlight; chunk++) {
        if (DEBUG)
            printf("[%d,%d] ",
                w->slicetext.slicesrc.line[at].start[chunk],
                w->slicetext.slicesrc.line[at].stop[chunk]);
        from = w->slicetext.slicesrc.line[at].start[chunk]
                   ? w->slicetext.slicesrc.line[at].start[chunk]
                   : 1;
        to = w->slicetext.slicesrc.line[at].stop[chunk] ? w->slicetext.slicesrc.line[at].stop[chunk]
                                                        : w->slicetext.slicesrc.line[at].length;
        for (i = from; i <= to; i++)
            highlight[i] = 1;
    }
    pos  = 1;
    last = w->slicetext.slicesrc.line[at].length;
    while (pos <= last) {
        to = pos + 1;
        while ((to <= last) && (highlight[pos] == highlight[to]))
            to++;
        gc_to_use = highlight[pos] ? w->slicetext.alt_gc : w->slicetext.gc;
        if (DEBUG)
            printf("gc (%p,%p,%p)\n", gc_to_use, gc, gcx);
        if (DEBUG)
            printf("pos %d\n", pos);
        fflush(stdout);
        if (DEBUG)
            printf("line %d (%d,%d) -> (%d,%d)\n", at, pos, to, linemap[pos - 1], linemap[to - 1]);
        XDrawImageString(d,
            win,
            gc_to_use,
            offset,
            base,
            &buff[linemap[pos - 1]],
            linemap[to - 1] - linemap[pos - 1]);
        /*
        &buff[pos - 1],to-pos);
        */
        offset += XTextWidth(highlight[pos] ? w->slicetext.font : w->slicetext.font,
            &buff[linemap[pos - 1]],
            linemap[to - 1] - linemap[pos - 1]);
        /*
        &buff[pos - 1],to-pos);
        */
        pos = to;
    }
    for (i = 0; i < w->slicetext.slicesrc.line[at].length; i++)
        if (DEBUG)
            printf("%c", w->slicetext.slicesrc.line[at].text[i]);
    if (DEBUG)
        printf("\n");
    for (i = 0; i < w->slicetext.slicesrc.line[at].length; i++)
        if (DEBUG)
            printf("%1d", highlight[i]);
    if (DEBUG)
        printf("\n");
}

int adjust_line(w, line) int line;
MultiSliceWidget w;
{
    int *off, i;

    off = w->slicetext.slicesrc.offset;
    for (i = 1; i < w->slicetext.slicesrc.n_files; i++) {
        if (off[i] >= line)
            return line - off[i - 1];
    }
    return line - off[w->slicetext.slicesrc.n_files - 1];
}

static void doexpose(w, e, r) MultiSliceWidget w;
XAnyEvent *e;
Region r;
{
    int base, ascent, descent, h, line, linemap[Max_line_length];
    GC gc_to_use;
    int LastLine, Nvisible, FirstLine;
    int lineoffset;
    char line_no[10], format[20], line_buff[Max_line_length];

    base    = w->slicetext.font->ascent;
    descent = w->slicetext.font->descent;
    ascent  = base;
    h       = ascent + descent;
    /*
    if (turn)gc_to_use = w->slicetext.gc;
    else gc_to_use = w->slicetext.alt_gc;
    */

    Nvisible = 0.5 + w->core.height / ((float)h);
    LastLine = w->slicetext.top + Nvisible;
    if (LastLine > w->slicetext.slicesrc.n_lines) {
        LastLine  = w->slicetext.slicesrc.n_lines;
        FirstLine = w->slicetext.slicesrc.n_lines - Nvisible + 1;
    } else
        FirstLine = w->slicetext.top;
    if (FirstLine < 1)
        FirstLine = 1;
    if (FirstLine != w->slicetext.top)
        w->slicetext.top = FirstLine;

    if (DEBUG)
        printf("Expose %d lines %d - %d\n", Nvisible, FirstLine, LastLine);
    sprintf(format, " %%%dd ", w->slicetext.log);
    lineoffset = (w->slicetext.log + 3) * w->slicetext.font->max_bounds.width;
    for (line = FirstLine; line <= LastLine; line++) {
        /*
if (DEBUG)printf ("Expose %d\n",turn);
if (DEBUG)fflush(stdout);
        if (DEBUG)printf ("Line %d\n",line); fflush(stdout);
        */
        sprintf(line_no, format, adjust_line(w, line));
        if (w->slicetext.slicesrc.line[line].n_highlight)
            gc_to_use = w->slicetext.alt_gc;
        else
            gc_to_use = w->slicetext.gc;
        XClearArea(e->display, e->window, 0, base - ascent, 0, h, False);
        XDrawImageString(e->display,
            e->window,
            gc_to_use,
            w->slicetext.font->max_bounds.width,
            base,
            line_no,
            w->slicetext.log + 2);
        /*
        static void buildline (buff,line,n,tabstop)
                         XDrawImageString (e->display,e->window,gc_to_use,
                                lineoffset,base,w->slicetext.slicesrc.line[line].text,
                                w->slicetext.slicesrc.line[line].length
                                );
        */
        buildline(line_buff,
            w->slicetext.slicesrc.line[line].text,
            w->slicetext.slicesrc.line[line].length,
            w->slicetext.tabstop,
            linemap);
        drawline(e->display, e->window, w, line_buff, lineoffset, base, line, linemap);
        /* *********************************
        XDrawImageString (e->display,e->window,gc_to_use,
               lineoffset,base,line_buff,
               strlen(line_buff)
               );
               *********************************** */
        base += h;
    }
    XClearArea(e->display, e->window, 0, base - ascent, 0, 0, False);
    XCopyArea(e->display,
        w->slicetext.sidebar,
        e->window,
        w->slicetext.side_gc,
        0,
        0,
        w->slicetext.side_width,
        w->core.height,
        w->core.width - w->slicetext.side_width,
        0);
}

void side_set(w, line_from) int line_from;
MultiSliceWidget w;
{
    int ix, at, h;

    h  = (w->core.height) / (w->slicetext.slicesrc.n_lines) - 1;
    h  = h < 1 ? 1 : h;
    at = ((line_from - 1) * w->core.height) / (w->slicetext.slicesrc.n_lines);
    for (ix = 0; ix < h; ix++, at++)
        XDrawLine(XtDisplay(w), w->slicetext.sidebar, w->slicetext.side_gc, 0, at, 100, at);
}

static void SliceResize(w) MultiSliceWidget w;
{

    float top, shown;
    int win, line;

    if (!w->slicetext.slicesrc.n_lines)
        return;
    win =
        (0.5) + ((float)w->core.height) / (w->slicetext.font->ascent + w->slicetext.font->descent);
    shown = ((float)win) / ((float)w->slicetext.slicesrc.n_lines);
    top   = (w->slicetext.top - 1) / ((float)w->slicetext.slicesrc.n_lines);
    if (DEBUG)
        printf("h %d a %d d %d   ",
            w->core.height,
            w->slicetext.font->ascent,
            w->slicetext.font->descent);
    if (DEBUG)
        printf("win %d  file %d  shown %f\n", win, w->slicetext.slicesrc.n_lines, shown);
    if (w->slicetext.scrollbar == NULL)
        return;
    XawScrollbarSetThumb(w->slicetext.scrollbar, top, shown);
    /*
    w->slicetext.top = 1;
            w->slicetext.slicesrc.line[line].n_highlight = 0;
    */
    XFillRectangle(XtDisplay(w),
        w->slicetext.sidebar,
        w->slicetext.side_clear_gc,
        0,
        0,
        w->slicetext.side_width,
        DisplayHeight(XtDisplay(w), DefaultScreen(XtDisplay(w))));
    for (line = 1; line <= w->slicetext.slicesrc.n_lines; line++)
        if (w->slicetext.slicesrc.line[line].n_highlight)
            side_set(w, line);
}

void SliceSetTop(w, pc) float pc;
MultiSliceWidget w;
{
    int line;
    int old_top;

    old_top = w->slicetext.top;
    if (DEBUG)
        printf("Setting Top line %f\n", pc);
    w->slicetext.top = (int)(w->slicetext.slicesrc.n_lines * pc);
    if ((pc < 0.0) || (pc > 1.0))
        w->slicetext.top = 1;
    if (w->slicetext.top != old_top) {
        XawScrollbarSetThumb(w->slicetext.scrollbar, pc, -1.0);
        fakeexpose(w);
    }
}

void SliceRedraw(MultiSliceWidget w) { fakeexpose(w); }

void SliceSet(w, line_from, col_from, line_to, col_to, redraw) int line_from, col_from, line_to,
    col_to, redraw;
MultiSliceWidget w;
{
    int line, ix, at, h;

    ix = w->slicetext.slicesrc.line[line_from].n_highlight;
    if (ix + 5 > MAXHL)
        return;
    ix = w->slicetext.slicesrc.line[line_to].n_highlight;
    if (ix + 5 > MAXHL)
        return;
    if (DEBUG)
        printf("Setting line %d\n", line);
    if (DEBUG)
        printf("Set (%d,%d)--(%d,%d) <%d>",
            line_from,
            col_from,
            line_to,
            col_to,
            w->slicetext.slicesrc.line[line_from].length);
    if (line_from != line_to) {
        for (line = line_from; line <= line_to; line++)
            if ((line <= w->slicetext.slicesrc.n_lines) && (line > 0)) {
                ix = w->slicetext.slicesrc.line[line].n_highlight;
                if (ix + 5 < MAXHL)
                    w->slicetext.slicesrc.line[line].n_highlight++;
                w->slicetext.slicesrc.line[line].start[ix] = 0;
                w->slicetext.slicesrc.line[line].stop[ix]  = 0;
            }
        line = line_from;
        ix   = w->slicetext.slicesrc.line[line].n_highlight - 1;
        w->slicetext.slicesrc.line[line].start[ix] = col_from;
        line                                       = line_to;
        ix = w->slicetext.slicesrc.line[line].n_highlight - 1;
        w->slicetext.slicesrc.line[line].stop[ix] = col_to;
    } else {
        line = line_from;
        ix   = w->slicetext.slicesrc.line[line].n_highlight;
        if (ix < MAXHL)
            w->slicetext.slicesrc.line[line].n_highlight++;
        w->slicetext.slicesrc.line[line].start[ix] = col_from;
        w->slicetext.slicesrc.line[line].stop[ix]  = col_to;
        if (DEBUG)
            printf(" Same line");
    }
    if (DEBUG)
        printf("\n");
    side_set(w, line_from);
    if (redraw)
        fakeexpose(w);
}

void SliceClearAll(MultiSliceWidget w) {
    int line;

    if (DEBUG)
        printf("Clear %d lines\n", w->slicetext.slicesrc.n_lines);

    for (line = 1; line <= w->slicetext.slicesrc.n_lines; line++) {
        if (DEBUG)
            printf("Clear Line %d\n", line);
        fflush(stdout);
        w->slicetext.slicesrc.line[line].n_highlight = 0;
    }
    XFillRectangle(XtDisplay(w),
        w->slicetext.sidebar,
        w->slicetext.side_clear_gc,
        0,
        0,
        w->slicetext.side_width,
        DisplayHeight(XtDisplay(w), DefaultScreen(XtDisplay(w))));
    fakeexpose(w);
}

void MultiSliceSetHook(MultiSliceWidget w, void (*h)())
// void (*h)();
// MultiSliceWidget	w;
{
    w->slicetext.hook = h;
}

void find_line_and_file(w, line, fid, line_in_file) int line, *fid, *line_in_file;
MultiSliceWidget w;
{
    int *off, i;

    off = w->slicetext.slicesrc.offset;
    if (line > off[w->slicetext.slicesrc.n_files])
        line = off[w->slicetext.slicesrc.n_files];
    for (i = 1; i < w->slicetext.slicesrc.n_files; i++) {
        if (off[i] >= line) {
            *fid          = i - 1;
            *line_in_file = line - off[i - 1];
            return;
        }
    }
    *fid          = w->slicetext.slicesrc.n_files - 1;
    *line_in_file = line - off[w->slicetext.slicesrc.n_files - 1];
    return;
}

// static
void MultiSliceHook(MultiSliceWidget w, XEvent *event, String *parm, Cardinal *n) {
    static int line = 0;
    int y;
    int fid, f_line, nf;

    if (DEBUG)
        printf("event %d buttonrelease (%d) kdn/up(%d %d)\n",
            event->type,
            ButtonRelease,
            KeyPress,
            KeyRelease);
    if (event->type == ButtonRelease)
        y = event->xbutton.y;
    else if ((event->type == KeyRelease) || (event->type == KeyPress))
        y = event->xkey.y;
    else
        return;
    line = w->slicetext.top + y / (w->slicetext.font->ascent + w->slicetext.font->descent);
    if (DEBUG)
        printf("y (%d) line (%d)\n", y, line);
    find_line_and_file(w, line, &fid, &f_line);
    printf("Click on line %d of display (%d of fid %d)\n", line, f_line, fid);
    if (w->slicetext.hook) {
        (*w->slicetext.hook)(fid, f_line);
    }
}

static void MultiSliceSelect(w, event, parm, n) MultiSliceWidget w;
XEvent *event;
String *parm;
Cardinal *n;
{
    static int line = 0;
    static MultiSliceReturn r;
    int y;

    if (event->type == ButtonRelease)
        y = event->xbutton.y;
    else if ((event->type == KeyRelease) || (event->type == KeyPress))
        y = event->xkey.y;
    else
        y = 1;
    line = w->slicetext.top + y / (w->slicetext.font->ascent + w->slicetext.font->descent);
    if (DEBUG)
        printf("y (%d) line (%d)\n", y, line);
    r.line = line;
    if (*n == 0)
        r.message = NULL;
    else
        r.message = parm[0];
    if (event->xbutton.x >= (w->core.width - w->slicetext.side_width)) {
        SliceSetTop(w, ((float)y) / ((float)(w->core.height ? w->core.height : 1)));
    } else
        XtCallCallbacks((Widget)w, XtNcallback, (XtPointer)&r);
}

static void MultiSliceAction(w, event, parm, n) MultiSliceWidget w;
XEvent *event;
String *parm;
Cardinal *n;
{
    static int line = 0;
    int y;

    if (DEBUG)
        printf("event %d buttonrelease (%d) kdn/up(%d %d)\n",
            event->type,
            ButtonRelease,
            KeyPress,
            KeyRelease);
    if (event->type == ButtonRelease)
        y = event->xbutton.y;
    else if ((event->type == KeyRelease) || (event->type == KeyPress))
        y = event->xkey.y;
    else
        return;
    line = w->slicetext.top + y / (w->slicetext.font->ascent + w->slicetext.font->descent);
    if (DEBUG)
        printf("y (%d) line (%d)\n", y, line);
    SliceSet(w, line, 0, line, 0, True);
}

static XtActionsRec actions[] = {
    {"slicetexthook", MultiSliceHook},
    {"setslicetext", MultiSliceAction},
    {"selectslicetext", MultiSliceSelect},
};

static char translations[] = "<Btn3Up>:     setslicetext()  \n\
 <Btn2Up>:     selectslicetext(\"dice\")  \n\
 None<Btn1Up>:     selectslicetext()  \n\
 Shift<Btn1Up>:     slicetexthook()  \n\
";

MultiSliceClassRec multisliceClassRec = {{/* core fields */
                                             /* superclass       */ (WidgetClass)&widgetClassRec,
                                             /* class_name       */ "MultiSlice",
                                             /* widget_size      */ sizeof(MultiSliceRec),
                                             /* class_initialize     */ NULL,
                                             /* class_part_initialize    */ NULL,
                                             /* class_inited     */ FALSE,
                                             /* initialize       */ doinit,
                                             /* initialize_hook      */ NULL,
                                             /* realize          */ XtInheritRealize,
                                             /* actions          */ actions,
                                             /* num_actions      */ XtNumber(actions),
                                             /* resources        */ resources,
                                             /* num_resources        */ XtNumber(resources),
                                             /* xrm_class        */ NULLQUARK,
                                             /* compress_motion      */ TRUE,
                                             /* compress_exposure    */ TRUE,
                                             /* compress_enterleave  */ TRUE,
                                             /* visible_interest     */ FALSE,
                                             /* destroy          */ NULL,
                                             /* resize           */ SliceResize,
                                             /* expose           */ doexpose,
                                             /* set_values       */ NULL,
                                             /* set_values_hook      */ NULL,
                                             /* set_values_almost    */ XtInheritSetValuesAlmost,
                                             /* get_values_hook      */ NULL,
                                             /* accept_focus     */ NULL,
                                             /* version          */ XtVersion,
                                             /* callback_private     */ NULL,
                                             /* tm_table         */ translations,
                                             /* query_geometry       */ XtInheritQueryGeometry,
                                             /* display_accelerator  */ XtInheritDisplayAccelerator,
                                             /* extension        */ NULL},
    {/* slicetext fields */
        /* empty            */ 0}};

WidgetClass multisliceWidgetClass = (WidgetClass)&multisliceClassRec;

void BuildSliceSrc(MultiSliceWidget w, int n, MultiSliceFilesPtr f, int nlines) {
    char *string;
    int at_line = 1;
    char *old, *at;
    int longest_line = 0, lines;
    SliceSrc *src;
    int i;

    if (DEBUG)
        printf("Build %d lines\n", nlines);
    src          = &w->slicetext.slicesrc;
    src->n_lines = 0;
    src->line    = (Line *)malloc((nlines + 2) * sizeof(Line));
    if (src->line == NULL) {
        fprintf(stderr, "Not enough memory for %d lines\n", nlines);
        exit(1);
    }
    src->n_files = n;
    src->offset  = (int *)malloc((n + 1) * sizeof(int));
    if (src->offset == NULL) {
        fprintf(stderr, "Not enough memory for %d lines\n", nlines);
        exit(1);
    }
    src->n_files = n;
    for (i = 0; i < n; i++) {
        at             = f[i].text;
        src->offset[i] = at_line - 1;
        if (!at)
            return;
        while (*at) {
            src->line[at_line].text        = at;
            src->line[at_line].n_highlight = 0;
            old                            = at;
            while ((*at != '\0') && (*at != '\n'))
                at++;
            src->line[at_line].length = at - old;
            if (src->line[at_line].length > longest_line)
                longest_line = src->line[at_line].length;
            src->n_lines++;
            at_line++;
            if (*at)
                at++;
        }
    }
    src->offset[n]   = at_line - 1;
    src->longest     = longest_line;
    lines            = src->n_lines;
    w->slicetext.log = 0;
    while (lines > 0) {
        lines = lines / 10;
        w->slicetext.log++;
    }
    SliceResize(w);
    if (DEBUG)
        printf(" %d lines, longest %d\n", src->n_lines, longest_line);
    if (longest_line >= Max_line_length) {
        fprintf(stderr, "Longest line %d is too long (limit %d)\n", longest_line, Max_line_length);
        exit(1);
    }
}

void PrintSliceSrc(w) MultiSliceWidget w;
{
    int i, l;
    char *at;
    SliceSrc *src;

    src = &w->slicetext.slicesrc;
    if (DEBUG)
        printf("%d lines, longest is %d\n", src->n_lines, src->longest);
    for (i = 1; i <= src->n_lines; i++) {
        at = src->line[i].text;
        for (l = 0; l < src->line[i].length; l++)
            if (DEBUG)
                printf("%c", *at++);
        if (DEBUG)
            printf("\n");
    }
}
