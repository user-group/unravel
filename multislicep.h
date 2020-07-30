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

#ifndef _multislicep_h
#define _multislicep_h
/*
*****************************************************************
*                                                               *
*  Unravel Project: Develop a program slicer for ANSI C         *
*                                                               *
*  Developed at N.I.S.T. Computer Systems Lab by:               *
*                                                               *
*      D. Wallace     Project Leader                            *
*                                                               *
*      J. Lyle, J. Graham, K. Gallagher, D. Binkley & J. Poole  *
*                                                               *
*****************************************************************
*/

#include "multislice.h"
/* include superclass private header file */
#include <X11/CoreP.h>
#include <X11/Xlib.h>
#define MSLICE_PH_SCCS_ID "@(#)MultiSliceP.h	1.3  9/29/94"

/* define unique representation types not found in <X11/StringDefs.h> */

#define XtRMultiSliceResource "MultiSliceResource"

typedef struct
{
    int empty;
} MultiSliceClassPart;

typedef struct _MultiSliceClassRec
{
    CoreClassPart core_class;
    MultiSliceClassPart slicetext_class;
} MultiSliceClassRec;

extern MultiSliceClassRec multisliceClassRec;

typedef struct
{
    /* resources */
    XFontStruct *font;
    XFontStruct *alt_font;
    Pixel foreground;
    Pixel alt_foreground;
    Pixel alt_background;
    int tabstop;
    Pixel side_fg, side_bg;
    int side_width;
    /* private state */
    int top;
    int log;
    Widget scrollbar;
    XtCallbackList select_callback;
    GC gc;
    GC alt_gc;
    GC side_gc, side_clear_gc;
    Pixmap sidebar;
    SliceSrc slicesrc;
    void (*hook)();
} MultiSlicePart;

typedef struct _MultiSliceRec
{
    CorePart core;
    MultiSlicePart slicetext;
} MultiSliceRec;

#endif /* _MultiSliceP_h */
