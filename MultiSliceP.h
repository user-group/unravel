#ifndef _MultiSliceP_h
#define _MultiSliceP_h
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

#include "MultiSlice.h"
/* include superclass private header file */
#include <X11/CoreP.h>
# include <X11/Xlib.h>
# define MSLICE_PH_SCCS_ID "@(#)MultiSliceP.h	1.3  9/29/94"

/* define unique representation types not found in <X11/StringDefs.h> */

#define XtRMultiSliceResource "MultiSliceResource"

typedef struct {
    int empty;
} MultiSliceClassPart;

typedef struct _MultiSliceClassRec {
    CoreClassPart   core_class;
    MultiSliceClassPart   slicetext_class;
} MultiSliceClassRec;

extern MultiSliceClassRec multisliceClassRec;

typedef struct {
    /* resources */
    XFontStruct		*font;
    XFontStruct		*alt_font;
	Pixel			foreground;
	Pixel			alt_foreground;
	Pixel			alt_background;
	int				tabstop;
	Pixel			side_fg,side_bg;
	int				side_width;
    /* private state */
	int				top;
	int				log;
	Widget			scrollbar;
	XtCallbackList	select_callback;
	GC				gc;
	GC				alt_gc;
	GC				side_gc,side_clear_gc;
	Pixmap			sidebar;
	SliceSrc		slicesrc;
	void			(*hook)();
} MultiSlicePart;

typedef struct _MultiSliceRec {
    CorePart        core;
    MultiSlicePart    slicetext;
} MultiSliceRec;

#endif /* _MultiSliceP_h */
