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

#include "ansi_parse.h"
#include "lif.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// static char sccsid[] = "@(#)xpr.c	1.4  4/26/95";

#define MAX_MALLOC 100
static int max_addr   = 0;
addr_tab_ptr addr_tab = NULL;
int n_malloc          = 0;
char *malloc_funs[MAX_MALLOC];

void print_trees(tree_ptr t);
int get_chain(tree_ptr t);
void assemble_dot(char *buff, tree_ptr t, chain_ptr c);
void source_map(int n, token_ptr from, token_ptr to);
void assemble_actuals(tree_ptr t, int node_id, int *from, int *upto, int *to);
void connect_nodes(int from, int to);

int current_addr(void) 
{ 
    return max_addr; 
}

int is_malloc(char * name) 
{
    int i;
    for (i = 0; i < n_malloc; i++)
        if (strcmp(name, malloc_funs[i]) == 0)
            return 1;
    return 0;
}

void print_malloc(void) 
{
    int i;
    for (i = 0; i < n_malloc; i++)
        printf("\t%s\n", malloc_funs[i]);
}

void add_malloc(char *name) 
{ 
    malloc_funs[n_malloc++] = name; 
}

addr_tab_ptr addr_of(token_ptr t) 
{
    var_ste_ptr var;
    int scope;
    addr_tab_ptr new;

    if (!t)
        return NULL;
    var = look_up_id((var_ste_ptr)NULL, t->text, &scope);
    if (!var)
        return 0;
    if (var->addr == NULL) {
        new          = alloc_addr_tab();
        new->addr    = ++max_addr;
        new->next    = addr_tab;
        addr_tab     = new;
        new->proc_id = scope > 1 ? current_proc_id() : 0;
        new->var_id  = var->id;
        var->addr    = new;
        fprintf(outfile, "%d(%d,%d,%d)", LIF_ADDRESS, new->addr, new->proc_id, new->var_id);
        if (z_opt) {
            fprintf(outfile,
                "    address %d is var %d (%s) of proc %d",
                new->addr,
                new->var_id,
                t->text,
                new->proc_id);
        }
        fprintf(outfile, "\n");
    }
    return var->addr;
}

void make_malloc(int node_id, tree_ptr t)
{
    token_ptr var_token;
    addr_tab_ptr addr;

    var_token = fake_var_decl(t ? t->token : NULL);
    addr      = addr_of(var_token);
    if (addr) {
        fprintf(outfile, "%d(%d,%d)", LIF_AREF, node_id, addr->addr);
        if (z_opt) {
            fprintf(outfile, "    address %d refed at node %d", addr->addr, node_id);
        }
        fprintf(outfile, "\n");
    }
}

tree_ptr make_leaf(int code, token_ptr token) 
{
    tree_ptr new;

    new          = alloc_tree();
    new->left    = NULL;
    new->right   = NULL;
    new->op_code = code;
    new->token   = token;
    return new;
}

tree_ptr make_tree(int code, tree_ptr left, tree_ptr right) 
{
    tree_ptr new;

    new          = alloc_tree();
    new->left    = left;
    new->right   = right;
    new->op_code = code;
    new->token   = NULL;
    return new;
}

void print_tree(tree_ptr t) 
{
    if (!x_opt)
        return;
    printf("\nprint expression tree for line %d\n", lineno);
    print_trees(t);
    printf("\n");
}

/*static*/
void print_trees(tree_ptr t)
{
    static char *ops[] = {"!?",
        "-",
        "+",
        "->",
        "*",
        ".",
        "&",
        "f()",
        "--",
        "++",
        "CAST",
        "[]",
        "=",
        "+=",
        "ID",
        "K",
        "{",
        "}"};
    if (t) {
        switch (t->op_code) {
        case UN_OP:
        case ADDR_OP:
        case DEREF_OP:
        case PRE_OP:
            printf("(%s", ops[t->op_code]);
            print_trees(t->left);
            printf(")");
            break;
        case POST_OP:
            printf("(");
            print_trees(t->left);
            printf("%s", ops[t->op_code]);
            printf(")");
            break;
        case CALL_OP:
            print_trees(t->left);
            printf("(");
            print_trees(t->right);
            printf(")");
            break;
        case COMMA_OP:
            print_trees(t->left);
            printf(",");
            print_trees(t->right);
            break;
        case CAST_OP:
        case BIN_OP:
        case POINTER_OP:
        case DOT_OP:
        case ASGN_OP:
        case RELASGN_OP:
            printf("(");
            print_trees(t->left);
            printf("%s", ops[t->op_code]);
            print_trees(t->right);
            printf(")");
            break;
        case ARRAY_OP:
            print_trees(t->left);
            printf("[");
            print_trees(t->right);
            printf("]");
            break;
        case ID_OP:
            printf("%s", t->token->text);
            break;
        case CON_OP:
            printf("K");
            break;
        case LEFTP_OP:
        case RIGHTP_OP:
            print_trees(t->left);
            print_trees(t->right);
            break;
        default:
            print_trees(t->left);
            print_trees(t->right);
            break;
        }
    }
}
#define REF 0
#define DEF 1

void cref_def(int is_def, int node, int chain) 
{
    int code;

    if (is_def)
        code = LIF_CDEF;
    else
        code = LIF_CREF;

    fprintf(outfile, "%d(%d,%d)", code, node, chain);
    if (z_opt)
        fprintf(outfile, "   %s to chain (%d) at stmt %d ", is_def ? "def" : "ref", chain, node);
    fprintf(outfile, "\n");
}

void ref_def_id(int is_def, int node, int id, int scope)
{
    int code, level;

    if (is_def) {
        if (scope > 1)
            code = LIF_DEF;
        else
            code = LIF_GDEF;
    } else {
        if (scope > 1)
            code = LIF_REF;
        else
            code = LIF_GREF;
    }
    level = 0;

    fprintf(outfile, "%d(%d,%d", code, node, id);
    if (level)
        fprintf(outfile, ",%d", level);
    fprintf(outfile, ")");
    if (z_opt)
        fprintf(outfile,
            "   %s %s to (%d) at stmt %d ",
            scope > 1 ? "local" : "global",
            is_def ? "def" : "ref",
            id,
            node);
    fprintf(outfile, "\n");
}

void ref_def_var(int is_def, int node, token_ptr t, int used_as_array) 
{
    int scope, code, id, level;
    var_ste_ptr var;
    addr_tab_ptr addr;

    /*
    printf ("ref def is(%d) node (%d) t (%d)\n",is_def,node,t);
    */
    if (!t)
        return; /* empty */
    var = look_up_id((var_ste_ptr)NULL, t->text, &scope);
    if (is_var_array(var) && (!used_as_array)) {
        /* printf ("var %s is an array %s\n",t->text,
        used_as_array?"with subscript":"address"); */
        addr = addr_of(t);
        if (addr) {
            fprintf(outfile, "%d(%d,%d)", LIF_AREF, node, addr->addr);
            if (z_opt) {
                fprintf(outfile, "    array address %d refed at node %d", addr->addr, node);
            }
            fprintf(outfile, "\n");
        }
    } else {
        if (var) {
            id = var->id;
            if (is_def) {
                if (scope > 1)
                    code = LIF_DEF;
                else
                    code = LIF_GDEF;
            } else {
                if (scope > 1)
                    code = LIF_REF;
                else
                    code = LIF_GREF;
            }
            level = t->deref_level;
        } else
            return; /* oops */

        fprintf(outfile, "%d(%d,%d", code, node, id);
        if (level)
            fprintf(outfile, ",%d", level);
        fprintf(outfile, ")");
        if (z_opt)
            fprintf(outfile,
                "   %s %s to %s (%d) at stmt %d on line %d",
                scope > 1 ? "local" : "global",
                is_def ? "def" : "ref",
                t->text,
                id,
                node,
                t->at.line_start);
        fprintf(outfile, "\n");
    }
}

token_ptr leftmost(tree_ptr t)
{
    token_ptr left, right;

    if (t) {
        right = leftmost(t->right);
        if (t->op_code == COMMA_OP)
            left = right;
        else
            left = leftmost(t->left);
        if (!t->token) {
            if (!left)
                return right;
            if (!right)
                return left;
            if (left->at.line_start * 1000 + left->at.col_start <
                right->at.line_start * 1000 + right->at.col_start)
                return left;
            else
                return right;
        } else {
            if (!left && !right)
                return t->token;
            else if (left) {
                if (left->at.line_start * 1000 + left->at.col_start <
                    t->token->at.line_start * 1000 + t->token->at.col_start)
                    return left;
                else
                    return t->token;
            } else {
                return t->token;
            }
        }
    }
    return NULL;
}

static token_ptr rightmost(tree_ptr t)
{
    token_ptr left, right;

    if (t) {
        right = rightmost(t->right);
        if (t->op_code == COMMA_OP)
            left = right;
        else
            left = rightmost(t->left);
        if (!t->token) {
            if (!left)
                return right;
            if (!right)
                return left;
            if (left->at.line_end * 1000 + left->at.col_end <
                right->at.line_end * 1000 + right->at.col_end)
                return right;
            else
                return left;
        } else {
            if (!left && !right)
                return t->token;
            else if (left) {
                if (left->at.line_end * 1000 + left->at.col_end >
                    t->token->at.line_end * 1000 + t->token->at.col_end)
                    return left;
                else
                    return t->token;
            } else {
                return t->token;
            }
        }
    }
    return NULL;
}

#define ID_LVALUE 1
#define PTR_LVALUE 2
static int get_lvalue(tree_ptr t, token_ptr *tk, int *chain_id, int *array_seen)
{
    int lvalue, scope;
    var_ste_ptr var;
    char name[5000];
    token_ptr new, left, right;
    chain_rec c;

    if (t) {
        /*
        printf ("get lvalue op(%d)\n",t->op_code);
        */
        switch (t->op_code) {
        case ID_OP:
            t->token->deref_level = 0;
            *tk                   = t->token;
            /*
            if (*tk) printf ("var (%s)\n",t->token->text);
            */
            return ID_LVALUE;
        case DEREF_OP:
            lvalue = get_lvalue(t->left, tk, chain_id, array_seen);
            if (lvalue == ID_LVALUE)
                (*tk)->deref_level++;
            return lvalue;
        case COMMA_OP:
            return 0;
        case UN_OP:
        case LEFTP_OP:
        case RIGHTP_OP:
            return get_lvalue(t->left, tk, chain_id, array_seen);
        case DOT_OP:
            name[0]     = '\0';
            c.chain_id  = -1;
            c.no_output = 0;
            assemble_dot(name, t, &c);
            if (c.chain_id != -1) {
                var = c.fields;
                if (x_opt)
                    printf("\n\nField search\n");
                if (x_opt)
                    while (var) {
                        printf("\t%s\n", var->token->text);
                        var = var->next;
                    }
                var = look_up_id(c.fields, name, &scope);
                if (!var)
                    printf("%s not member at %d\n", name, lineno);
                if (!var)
                    return 0;
                ++c.current_seq;
                fprintf(outfile,
                    "%d(%d,%d,%d,%s)",
                    LIF_FIELD,
                    c.chain_id,
                    c.current_seq,
                    var->id,
                    name);
                if (z_opt)
                    fprintf(outfile,
                        "    field %d (%s) of chain %d",
                        c.current_seq,
                        var->token->text,
                        c.chain_id);
                fprintf(outfile, "\n");
                *chain_id = c.chain_id;
                if (*chain_id)
                    return PTR_LVALUE;
                return 0;
            }
            /*
            printf ("DOT found: (%s)\n",name);
            */
            new                = alloc_token();
            new->text          = find_string(name);
            left               = leftmost(t);
            right              = rightmost(t);
            new->at.line_start = left->at.line_start;
            new->at.col_start  = left->at.col_start;
            new->at.line_end   = right->at.line_end;
            new->at.col_end    = right->at.col_end;
            *tk                = new;
            return ID_LVALUE;
        case POINTER_OP:
            *chain_id = get_chain(t);
            if (*chain_id)
                return PTR_LVALUE;
            return 0;
        case ARRAY_OP:
            *array_seen += 1;
            return get_lvalue(t->left, tk, chain_id, array_seen);
        case BIN_OP:
        case PRE_OP:
        case POST_OP:
        case RELASGN_OP:
        case ASGN_OP:
        case CALL_OP:
        case ADDR_OP:
        case CAST_OP:
        case CON_OP:
        default:
            break;
        }
    }
    return 0;
}

static void scan_tree(tree_ptr t, int node_id, int *from_node, int *upto_node, int *to_node)
{
    static tree_ptr recent_typename = NULL;
    int new_from, new_to, new_upto;
    int lvalue, new_node;
    token_ptr tk;
    int chain_id, proc_id;
    int is_array = 0;
    addr_tab_ptr addr;

    if (t) {
        /*
        printf ("scan %d op %d left %d right %d\n",t,
                t->op_code,t->left,t->right);
        */
        switch (t->op_code) {
        case CON_OP:
        case ID_OP:
            return;
        case COMMA_OP:
            new_node = get_stmt_no();
            /*
            printf ("split %d from %d\n",new_node,node_id);
            printf ("node %d  from %d upto %d to %d\n",
                            node_id,*from_node,*upto_node,*to_node);
            */
            source_map(new_node, leftmost(t->left), rightmost(t->left));
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, new_node, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, new_node, chain_id);
            if (t->left) {
                new_from = 0;
                new_to   = 0;
                new_upto = 0;
                scan_tree(t->left, new_node, &new_from, &new_upto, &new_to);
                /*
                printf ("new %d  from %d upto %d to %d  ",
                        new_node,new_from,new_upto,new_to);
                */
                if (!new_from)
                    new_from = new_node;
                if (!new_upto)
                    new_upto = new_node;
                else
                    connect_nodes(new_upto, new_node);
                if (!new_to)
                    new_to = new_node;
                if (*from_node)
                    connect_nodes(new_to, *from_node);
                *from_node = new_from;
                if (!(*upto_node))
                    *upto_node = new_to;
            }
            /*
            printf ("node %d  from %d upto %d to %d\n",
                            node_id,*from_node,*upto_node,*to_node);
                            */
            lvalue = get_lvalue(t->right, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case LEFTP_OP:
        case RIGHTP_OP:
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            return;
        case ARG_OP:
        case BIN_OP:
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            lvalue = get_lvalue(t->right, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case ASGN_OP:
            is_array = 0;
            lvalue   = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(DEF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(DEF, node_id, chain_id);
            if (is_array) {
                if (lvalue == ID_LVALUE)
                    ref_def_var(REF, node_id, tk, is_array);
                else if (lvalue == PTR_LVALUE)
                    cref_def(REF, node_id, chain_id);
            }
            lvalue = get_lvalue(t->right, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case UN_OP:
            /*
            lvalue = get_lvalue (t->left,&tk,&chain_id,&is_array);
            printf ("un_op: lvalue %d\n",lvalue);
            if (lvalue == ID_LVALUE) ref_def_var (REF,node_id,tk,is_array);
            else if (lvalue == PTR_LVALUE)
                    cref_def(REF,node_id,chain_id);
            */
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case PRE_OP:
            new_node = get_stmt_no();
            if (*upto_node) {
                connect_nodes(*upto_node, new_node);
            } else {
                *from_node = new_node;
            }
            *upto_node = new_node;
            source_map(new_node, leftmost(t), rightmost(t->left));
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE) {
                ref_def_var(REF, new_node, tk, is_array);
                ref_def_var(DEF, new_node, tk, is_array);
                ref_def_var(REF, node_id, tk, is_array);
            } else if (lvalue == PTR_LVALUE) {
                cref_def(REF, node_id, chain_id);
                cref_def(REF, new_node, chain_id);
                cref_def(DEF, new_node, chain_id);
            }
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            return;
        case POST_OP:
            new_node = get_stmt_no();
            /*
            printf ("split (post) %d from %d\n",new_node,node_id);
            */
            if (*to_node == 0)
                connect_nodes(node_id, new_node);
            else
                connect_nodes(*to_node, new_node);
            *to_node = new_node;
            source_map(new_node, leftmost(t->left), rightmost(t));
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE) {
                ref_def_var(REF, new_node, tk, is_array);
                ref_def_var(DEF, new_node, tk, is_array);
                ref_def_var(REF, node_id, tk, is_array);
            } else if (lvalue == PTR_LVALUE) {
                cref_def(REF, new_node, chain_id);
                cref_def(DEF, new_node, chain_id);
                cref_def(REF, node_id, chain_id);
            }
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            return;
        case RELASGN_OP:
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE) {
                ref_def_var(DEF, node_id, tk, is_array);
                ref_def_var(REF, node_id, tk, is_array);
            } else if (lvalue == PTR_LVALUE) {
                cref_def(REF, node_id, chain_id);
                cref_def(DEF, node_id, chain_id);
            }
            lvalue = get_lvalue(t->right, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case ADDR_OP:
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            if ((lvalue == ID_LVALUE) && tk) {
                addr = addr_of(tk);
                if (addr) {
                    fprintf(outfile, "%d(%d,%d)", LIF_AREF, node_id, addr->addr);
                    if (z_opt) {
                        fprintf(outfile, "    address %d refed at node %d", addr->addr, node_id);
                    }
                    fprintf(outfile, "\n");
                }
            }
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            return;
        case TYPENAME_OP:
            recent_typename = t;
            return;
        case CAST_OP:
        case ARRAY_OP:
            lvalue = get_lvalue(t->right, &tk, &chain_id, &is_array);
            if (lvalue == ID_LVALUE)
                ref_def_var(REF, node_id, tk, is_array);
            else if (lvalue == PTR_LVALUE)
                cref_def(REF, node_id, chain_id);
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            recent_typename = NULL;
            return;
        case CALL_OP:
            lvalue = get_lvalue(t->left, &tk, &chain_id, &is_array);
            /*
            printf ("call lvalue %d\n",lvalue);
            */
            if (lvalue == ID_LVALUE) {
                proc_id = call_to(tk);
                /*
                printf ("call %s proc id %d\n",tk->text,proc_id);
                */
                if (!proc_id)
                    return;
                if (is_malloc(tk->text)) {
                    make_malloc(node_id, recent_typename);
                }
                fprintf(outfile, "%d(%d,%d)", LIF_CALL_START, node_id, proc_id);
                if (z_opt) {
                    fprintf(outfile, "     call to %s at %d", tk->text, node_id);
                }
                fprintf(outfile, "\n");
                assemble_actuals(t->right, node_id, from_node, upto_node, to_node);
                fprintf(outfile, "%d", LIF_CALL_END);
                if (z_opt)
                    fprintf(outfile, "     end call to %s at %d", tk->text, node_id);
                fprintf(outfile, "\n");
            }
            return;
        case DEREF_OP:
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            return;
        case DOT_OP:
        case POINTER_OP:
        default:
            if (t->left)
                scan_tree(t->left, node_id, from_node, upto_node, to_node);
            if (t->right)
                scan_tree(t->right, node_id, from_node, upto_node, to_node);
            break;
        }
    }
    return;
}

void make_arg(tree_ptr t, int node_id, int *from, int *upto, int* to)
{
    token_ptr tk;
    int lvalue;
    int chain_id;
    int is_array = 0;

    lvalue = get_lvalue(t, &tk, &chain_id, &is_array);
    if (lvalue == ID_LVALUE)
        ref_def_var(REF, node_id, tk, is_array);
    else if (lvalue == PTR_LVALUE)
        cref_def(REF, node_id, chain_id);
    scan_tree(t, node_id, from, upto, to);
}

void assemble_actuals(tree_ptr t, int node_id, int *from, int *upto, int *to) 
{
    if (!t)
        return;
    if (t->op_code == ARG_OP) {
        assemble_actuals(t->left, node_id, from, upto, to);
        fprintf(outfile, "%d%s\n", LIF_ACTUAL_SEP, z_opt ? "     Actual seperator" : "");
        make_arg(t->right, node_id, from, upto, to);
    } else
        make_arg(t, node_id, from, upto, to);
}

int xpr_gen(tree_ptr t, int *from, int *to) 
{
    token_ptr tk;
    int lvalue;
    int node_id, upto;
    int chain_id;
    int is_array;

    *from = *to = 0;
    node_id     = get_stmt_no();
    print_tree(t);
    upto   = 0;
    lvalue = get_lvalue(t, &tk, &chain_id, &is_array);
    if (lvalue == ID_LVALUE)
        ref_def_var(REF, node_id, tk, is_array);
    else if (lvalue == PTR_LVALUE)
        cref_def(REF, node_id, chain_id);
    scan_tree(t, node_id, from, &upto, to);
    /*
    q = leftmost(t);
    r = rightmost(t);
    printf ("leftmost token: (%d,%d) rightmost  (%d,%d)",
            q->at.line_start,q->at.col_start,
            r->at.line_end,r->at.col_end);
    source_map (node_id,leftmost(t),rightmost(t));
            */
    if (upto)
        connect_nodes(upto, node_id);
    if (*from == 0)
        *from = node_id;
    if (*to == 0)
        *to = node_id;
    return node_id; /* return the main stmt node */
}

