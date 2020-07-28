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
#ifndef _lif_h
#define _lif_h
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
#define LIF_H_SCCS_ID "	@(#)lif.h	1.9  9/29/94 "
/*
****************************************************************
*                                                              *
*             L I F    F O R M A T                             *
*                                                              *
*  id   - variable id                                          *
*  node - source program statement (or fragment)               *
*  pid  - procedure id                                         *
*  name - variable or procedure name                           *
*  level- indirection level                                    *
*  addr - address number                                       *
*  chain- chain number of pointer chain on a node              *
*  field- sequence number of field in chain                    *
*  fid  - field offset in struct                               *
*  offset number of fields in a declared structure variable    *
*  A    - is an array                                          *
*  P    - is a pointer                                         *
*  S    - static object                                        *
*  X    - is declared extern                                   *
*  G    - is a goto statement                                  *
*  B    - is a break statement                                 *
*  C    - is a continue statement                              *
*  R    - returns an expression value                          *
*                                                              *
****************************************************************
*/

#define LIF_PROC_START 1  /*  1(node,pid,name)					*/
#define LIF_PROC_END 2    /*  2(node[,S][,R])						*/
#define LIF_FORMAL_ID 3   /*  3(id,name[,A][,P])					*/
#define LIF_LOCAL_ID 4    /*  4(id,name[,S][,P][,X][,A])			*/
#define LIF_GLOBAL_ID 5   /*  5(id,name[,S][,P][,X][,A])			*/
#define LIF_FILE 6        /*  6(file_id,file_name)				*/
#define LIF_REF 7         /*  7(node,id[,level])					*/
#define LIF_DEF 8         /*  8(node,id[,level])					*/
#define LIF_GREF 9        /*  9(node,id[,level])					*/
#define LIF_GDEF 10       /* 10(node,id[,level])					*/
#define LIF_CALL_START 11 /* 11(node,pid)							*/
#define LIF_ACTUAL_SEP 12 /* 12    *** void ***                   */
#define LIF_CALL_END 13   /* 13    *** void ***                   */
#define LIF_RETURN 14     /* 14(node,1|0)							*/
#define LIF_GOTO 15       /* 15(node,G|B|C)						*/
#define LIF_SUCC 16       /* 16(from_node,to_node)				*/
#define LIF_REQUIRES 17   /* 17(required_node,node[,to node])     */
#define LIF_SOURCE_MAP 18 /* 18(node,from_line,from_col,to_l,to_c)*/
#define LIF_CHAIN 19      /* 19(chain,id)     					*/
#define LIF_GCHAIN 20     /* 20(chain,id)     					*/
#define LIF_FIELD 21      /* 21(chain,field,fid,name)             */
#define LIF_CREF 22       /* 22(node,chain)						*/
#define LIF_CDEF 23       /* 23(node,chain)						*/
#define LIF_AREF 24       /* 24(node,addr)						*/
#define LIF_ADDRESS 25    /* 25(addr,pid,id)						*/
#define LIF_STRUCT 26     /* 26(pid,id,offset)					*/

#endif /* _lif_h */
