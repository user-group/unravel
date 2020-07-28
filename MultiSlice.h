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
#ifndef _MultiSlice_h
#define _MultiSlice_h
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

/****************************************************************
 *
 * MultiSlice widget header file
 *
 ****************************************************************/
#define MSLICE_H_SCCS_ID "@(#)MultiSlice.h	1.3  9/29/94"

/* Resources:

 Name               Class     			RepType     Default Value
 ----        	    -----      			-------     -------------
 background	    Background			        Pixel		XtDefaultBackground
 border	    BorderColor			Pixel		XtDefaultForeground
 borderWidth	    BorderWidth			Dimension	1
 destroyCallback    Callback			        Pointer	NULL
 height	    Height				Dimension	0
 mappedWhenManaged  MappedWhenManaged	                Boolean	True
 sensitive	    Sensitive		         	Boolean	True
 width		    Width				Dimension	0
 x		    Position			        Position	0
 y		    Position			        Position	0

*/

/* define any special resource names here that are not in <X11/StringDefs.h> */

#define XtNtabStop "tabStop"
#define XtNaltForeground "altForeground"
#define XtNaltBackground "altBackground"
#define XtNsideForeground "sideForeground"
#define XtNsideBackground "sideBackground"
#define XtNsideWidth "sideWidth"

#define XtCAltFont XtCFont
#define XtCTabStop "TabStop"

#include <X11/IntrinsicP.h>

/* declare specific MultiSliceWidget class and instance datatypes */

typedef struct _MultiSliceClassRec *MultiSliceWidgetClass;
typedef struct _MultiSliceRec *MultiSliceWidget;

/* declare the class constant */

extern WidgetClass multisliceWidgetClass;

#define MAXHL 30
#define MAXLINES 5000

typedef struct
{
    char *text;
    int length;
    int n_highlight;
    int start[MAXHL], stop[MAXHL];
} Line;

typedef struct
{
    int n_files;
    int *offset;
    int n_lines;
    int longest;
    Line *line;
} SliceSrc;

typedef struct
{
    int line;
    char *message;
} MultiSliceReturn;

typedef struct
{
    char *text;
} MultiSliceFiles, *MultiSliceFilesPtr;

Widget CreateSliceBox();
#endif /* _MultiSlice_h */
