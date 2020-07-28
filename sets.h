#ifndef _sets_h
#define _sets_h
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

#define SETS_SCCS_ID "	@(#)sets.h	1.3  8/16/95 "
typedef struct bit_set_struct bit_set_node, *bit_set;
typedef struct set_struct set_node, *set_ptr;
typedef struct pair_struct pair_node, *pair_ptr;
typedef struct var_struct var_node, *var_ptr;
typedef struct id_set_struct id_set_node, *id_set_ptr;

struct bit_set_struct
{
    int max;
    unsigned int *bits;
};

struct id_set_struct
{
    id_set_ptr next;
    int id, pid;
};

struct set_struct
{
    int id;
    set_ptr next;
};

struct pair_struct
{
    int id, level;
    pair_ptr next;
};

struct var_struct
{
    int id;
    int code; /* see lif.h */
    int level;
    var_ptr next;
};

#define new_set (set_ptr) malloc(sizeof(set_node))
#define new_pair (pair_ptr) malloc(sizeof(pair_node))
#define new_var (var_ptr) malloc(sizeof(var_node))
#define make_set(x, a)                                                                             \
    x       = new_set;                                                                             \
    x->id   = a;                                                                                   \
    x->next = NULL
#define make_pair(x, a, b)                                                                         \
    x        = new_pair;                                                                           \
    x->id    = a;                                                                                  \
    x->level = b;                                                                                  \
    x->next  = NULL
#define make_var(x, a, b, c)                                                                       \
    x        = new_var;                                                                            \
    x->id    = a;                                                                                  \
    x->level = b;                                                                                  \
    x->code  = c;                                                                                  \
    x->next  = NULL

bit_set create_bit_set();
int is_bit_on(), is_in_id_set();
int get_next_member();
var_ptr add_to_var_set();
set_ptr add_to_set();
int cunion_bit_set();
int is_subset();
void clear_bit_set(bit_set set);

#endif /* _sets_h */
