#ifndef _control_h
#define _control_h
# define CONTROL_SCCS_ID "	@(#)control.h	1.1  8/10/95 "
# include <stdio.h>
/*
***********************************************************************
*                                                                     *
*   codes for defining control structure                              *
*                                                                     *
*   General form:                                                     *
*      STMT (CTRL-FROM,CTRL-TO,STMT-FROM,STMT-TO) Comments            *
*                                                                     *
*      STMT identifies type of control statement                      *
*      CTRL- identifies controling nodes                              *
*      STMT- identifies statements controled                          *
*                                                                     *
*      An IF_THEN_ELSE has two sets of controled statements           *
*                                                                     *
***********************************************************************
*/

#define IF_CTRL				1
#define IF_THEN_ELSE_CTRL	2
#define DO_CTRL				3
#define WHILE_CTRL			4
#define FOR_CTRL			5
#define SWITCH_CTRL			6

FILE	*ctrl_file;

void control (int code, int ctrl_entry, int ctrl_exit, int stmt_entry, int stmt_exit, int else_entry,
	int else_exit);


#endif /* _control_h */
