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
# define MSLICE_H_SCCS_ID "@(#)MultiSlice.h	1.3  9/29/94"

/* Resources:

 Name            	Class     			RepType     Default Value
 ----        	    -----      			-------     -------------
 background			Background			Pixel		XtDefaultBackground
 border				BorderColor			Pixel		XtDefaultForeground
 borderWidth		BorderWidth			Dimension	1
 destroyCallback	Callback			Pointer		NULL
 height				Height				Dimension	0
 mappedWhenManaged	MappedWhenManaged	Boolean		True
 sensitive			Sensitive			Boolean		True
 width				Width				Dimension	0
 x					Position			Position	0
 y					Position			Position	0

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

/* declare specific MultiSliceWidget class and instance datatypes */

typedef struct _MultiSliceClassRec*   MultiSliceWidgetClass;
typedef struct _MultiSliceRec*        MultiSliceWidget;

/* declare the class constant */

extern WidgetClass multisliceWidgetClass;

# define MAXHL 30
# define MAXLINES 5000
typedef struct {
	char	*text;
	int		length;
	int		n_highlight;
	int		start[MAXHL],stop[MAXHL];
	} Line;
typedef struct {
	int		n_files;
	int		*offset;
	int		n_lines;
	int		longest;
	Line	*line;
	} SliceSrc;
typedef struct {
	int		line;
	char	*message;
	} MultiSliceReturn;
typedef struct {
	char	*text;
	} MultiSliceFiles,*MultiSliceFilesPtr;
Widget CreateSliceBox();
#endif /* _MultiSlice_h */
