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
#include "stmt.h"
#include "ansi_parse.h"
#include "control.h"
#include "lif.h"
#include <string.h>

/*
static char sccsid[] = "@(#)stmt.c	1.9  8/11/95";
*/
list_ptr returns      = NULL;
static int max_proc   = 0;
static proc_ptr procs = NULL;
static int pid        = 0;

void connect_nodes(int from, int to);

label_ptr labels = NULL;

label_ptr
find_label(char *label)
{
    label_ptr at;

    at = labels;
    while (at)
    {
        if (strcmp(label, at->label) == 0)
            return at;
        at = at->next;
    }
    return NULL;
}

void
label_def(token_ptr label_token, int node)
{
    label_ptr new;
    list_ptr list;

    new = find_label(label_token->text);
    if (new)
    {
        new->node = node;
        list      = new->gotos;
        while (list)
        {
            connect_nodes(list->node, node);
            list = list->next;
        }
        new->gotos = NULL;
    }
    else
    {
        new        = alloc_label();
        new->next  = labels;
        labels     = new;
        new->label = label_token->text;
        new->node  = node;
        new->gotos = NULL;
    }
}

void
label_ref(token_ptr label_token, int goto_node)
{
    label_ptr new;
    list_ptr list;

    new = find_label(label_token->text);
    if (new)
    {
        if (new->node)
            connect_nodes(goto_node, new->node);
        else
        {
            list       = alloc_list();
            list->node = goto_node;
            list->next = new->gotos;
            new->gotos = list;
        }
    }
    else
    {
        new              = alloc_label();
        new->next        = labels;
        labels           = new;
        new->label       = label_token->text;
        new->node        = 0;
        new->gotos       = alloc_list();
        new->gotos->node = goto_node;
        new->gotos->next = NULL;
    }
}

int
current_proc_id(void)
{
    return pid;
}

void
output_proc_list(FILE *t_file)
{
    int n_procs = 0;
    proc_ptr p;

    p = procs;
    while (p)
    {
        n_procs++;
        p = p->next;
    }
    fprintf(t_file, "%d %d\n", n_procs, stmtno);
    p = procs;
    while (p)
    {
        fprintf(t_file,
            "%3d %4d %4d %4d %c %s\n",
            p->proc_id,
            p->entry_node,
            p->exit_node,
            p->n_locals,
            p->static_or_extern,
            p->proc->text);
        p = p->next;
    }
}

void
list_procs(void)
{
    proc_ptr p;

    printf("\nProcedures\n");
    p = procs;
    while (p)
    {
        printf("%2d %4d %s\n", p->proc_id, p->entry_node, p->proc->text);
        p = p->next;
    }
}

proc_ptr
find_proc(token_ptr t)
{
    proc_ptr p;

    if (!t)
        return NULL;
    p = procs;
    while (p)
    {
        if (0 == strcmp(t->text, p->proc->text))
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
void
add_proc(token_ptr t, int node, int pid)
{
    proc_ptr p, q;

    if (!t)
        return;
    p = procs;
    while (p)
    {
        if (0 == strcmp(t->text, p->proc->text))
        {
            if ((p->entry_node == -1) && (node != -1))
            {
                p->entry_node = node;
                current_proc  = p;
            }
            return;
        }
        p = p->next;
    }
    p                   = alloc_proc();
    p->next             = NULL;
    p->proc             = t;
    p->proc_id          = pid;
    p->entry_node       = node;
    p->exit_node        = 0;
    p->n_locals         = 0;
    p->static_or_extern = 'X';
    p->has_return_xpr   = 0;
    q                   = procs;
    if (q)
    {
        while (q->next)
            q = q->next;
        q->next = p;
    }
    else
        procs = p;
    if (node != -1)
        current_proc = p;
}

int
call_to(token_ptr t)
{
    proc_ptr p;

    if (!t)
        return 0;
    p = find_proc(t);
    if (!p)
    {
        add_proc(t, -1, ++max_proc);
        return max_proc;
    }
    return p->proc_id;
}

int
get_stmt_no(void)
{
    return ++stmtno;
}

void
connect_nodes(int from, int to)
{
    fprintf(outfile, "%d(%d,%d)", LIF_SUCC, from, to);
    if (z_opt)
        fprintf(outfile, "   connect from %d to %d", from, to);
    fprintf(outfile, "\n");
}

void
source_map(int n, token_ptr from, token_ptr to)
{
    /* printf ("source map %d %d \n",from,to); */
    if (from && to)
    {
        /* printf ("tokens: (%s) (%s)\n",from->text,to->text); */
        fprintf(outfile,
            "%d(%d,%d,%d,%d,%d)",
            LIF_SOURCE_MAP,
            n,
            from->at.line_start,
            from->at.col_start,
            to->at.line_end,
            to->at.col_end);
        if (z_opt)
        {
            fprintf(outfile, "    source for node %d", n);
            if (from->at.line_start == to->at.line_start)
                fprintf(outfile,
                    " line %d cols %d-%d",
                    from->at.line_start,
                    from->at.col_start,
                    to->at.col_end);
            else
                fprintf(outfile,
                    " line %d col %d to line %d col %d",
                    from->at.line_start,
                    from->at.col_start,
                    to->at.line_end,
                    to->at.col_end);
        }
        fprintf(outfile, "\n");
    }
}

stmt_ptr
make_stmt(int from, int to, int entry_node, int exit_node)
{
    stmt_ptr new;

    new            = alloc_stmt();
    new->from      = from;
    new->to        = to;
    new->entry     = entry_node;
    new->exit      = exit_node;
    new->breaks    = NULL;
    new->cases     = NULL;
    new->continues = NULL;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;
    ;
    return new;
}
static void
merge_bc(stmt_ptr from, stmt_ptr to)
{
    list_ptr nodes;

    if (from->breaks)
    {
        nodes = from->breaks;
        while (nodes->next)
            nodes = nodes->next;
        nodes->next = to->breaks;
    }
    else
        from->breaks = to->breaks;

    if (from->continues)
    {
        nodes = from->continues;
        while (nodes->next)
            nodes = nodes->next;
        nodes->next = to->continues;
    }
    else
        from->continues = to->continues;

    if (from->cases)
    {
        nodes = from->cases;
        while (nodes->next)
            nodes = nodes->next;
        nodes->next = to->cases;
    }
    else
        from->cases = to->cases;
}

stmt_ptr
join_stmt(stmt_ptr from, stmt_ptr to)
{
    list_ptr c;

    if (!from)
        return to;
    if (!to)
        return from;
    if (from->is_jump && to->cases && from->cases)
    {
        c = from->cases;
        while (c)
        {
            c->is_marked = True;
            c            = c->next;
        }
        /* from->cases = NULL; */
    }
    c = from->cases;
    while (c)
    {
        if (!c->is_marked)
            gen_require(c->node, to->entry, to->entry);
        c = c->next;
    }
    if ((!from->is_jump) && to->cases && from->cases)
    {
        c = from->cases;
        while (c)
        {
            c->is_marked = True;
            c            = c->next;
        }
    }
    if (!from->is_jump)
        connect_nodes(from->exit, to->entry);
    from->exit = to->exit;
    /*
    from->to = to->to;
    */
    from->is_jump    = to->is_jump;
    from->last->next = to;
    from->last       = to;
    to->next         = NULL;

    merge_bc(from, to);

    return from;
}

void
gen_require(int node_required, int from, int to)
{
    if ((node_required == from) && (node_required == to))
        return;

    fprintf(outfile, "%d(%d,%d", LIF_REQUIRES, node_required, from);
    if (from != to)
        fprintf(outfile, ",%d", to);
    fprintf(outfile, ")");
    if (z_opt)
        fprintf(outfile, "     nodes %d-%d require node %d", from, to, node_required);
    fprintf(outfile, "\n");
}

void
require_stmt(int node_required, stmt_ptr stmt)
{
    stmt_ptr at;
    int start, end,
    try
        ;

    if (!stmt)
        return;
    at    = stmt;
    start = at->from;
    end   = at->to;
    at    = at->next;
    while (at)
    {
        try
            = at->from;
        if (end + 1 == try)
        {
        }
        else
        {
            gen_require(node_required, start, end);
            start = at->from;
        }
        end = at->to;
        at  = at->next;
    }
    gen_require(node_required, start, end);
}

stmt_ptr
stmt_label(token_ptr label_token, stmt_ptr stmt)
{
    stmt_ptr new;
    int node;

    node = get_stmt_no();
    source_map(node, label_token, label_token);
    new = make_stmt(node, node, node, node);
    label_def(label_token, node);
    connect_nodes(node, stmt->entry);
    stmt->to    = node;
    stmt->entry = node;
    return stmt;
}

stmt_ptr
goto_stmt(token_ptr continue_token, token_ptr label_token, token_ptr semi_token)
{
    stmt_ptr new;
    int node;

    node = get_stmt_no();
    source_map(node, continue_token, semi_token);
    new          = make_stmt(node, node, node, node);
    new->is_jump = True;
    fprintf(outfile, "%d(%d,%s)", LIF_GOTO, node, "G");
    if (z_opt)
        fprintf(outfile, "    go to %s", label_token->text);
    fprintf(outfile, "\n");
    label_ref(label_token, node);

    return new;
}

stmt_ptr
continue_stmt(token_ptr continue_token, token_ptr semi_token)
{
    stmt_ptr new;
    int node;
    list_ptr list;

    node = get_stmt_no();
    source_map(node, continue_token, semi_token);
    new            = make_stmt(node, node, node, node);
    list           = alloc_list();
    list->node     = node;
    list->next     = NULL;
    new->continues = list;
    new->is_jump   = True;
    fprintf(outfile, "%d(%d,%s)%s", LIF_GOTO, node, "C", z_opt ? "     continue\n" : "\n");

    return new;
}

stmt_ptr
break_stmt(token_ptr break_token, token_ptr semi_token)
{
    stmt_ptr new;
    int node;
    list_ptr list;

    node = get_stmt_no();
    source_map(node, break_token, semi_token);
    new          = make_stmt(node, node, node, node);
    list         = alloc_list();
    list->node   = node;
    list->next   = NULL;
    new->breaks  = list;
    new->is_jump = True;
    fprintf(outfile, "%d(%d,%s)%s", LIF_GOTO, node, "B", z_opt ? "     break\n" : "\n");

    return new;
}

stmt_ptr
brace_stmt(token_ptr left_token, stmt_ptr stmt, token_ptr right_token)
{
    int left, right;

    left  = get_stmt_no();
    right = get_stmt_no();
    source_map(left, left_token, left_token);
    source_map(right, right_token, right_token);
    if (!stmt)
    {
        connect_nodes(left, right);
        stmt = make_stmt(left, right, left, right);
    }
    else
    {
        connect_nodes(left, stmt->entry);
        if (!stmt->is_jump)
            connect_nodes(stmt->exit, right);
        stmt->entry = left;
        stmt->exit  = right;
        require_stmt(left, stmt);
        stmt->to   = right;
        stmt->from = right;
    }
    gen_require(right, left, left);
    return stmt;
}

void
break_continue(stmt_ptr stmt, int break_node, int continue_node)
{
    list_ptr nodes;

    if (stmt)
    {
        nodes = stmt->breaks;
        while (nodes && break_node)
        {
            connect_nodes(nodes->node, break_node);
            nodes = nodes->next;
        }
        nodes = stmt->continues;
        while (nodes && continue_node)
        {
            connect_nodes(nodes->node, continue_node);
            nodes = nodes->next;
        }
    }
}

void
break_cases(stmt_ptr stmt, int xpr_to_case, int break_to_end)
{
    list_ptr nodes;

    if (stmt)
    {
        nodes = stmt->breaks;
        while (nodes && break_to_end)
        {
            connect_nodes(nodes->node, break_to_end);
            nodes = nodes->next;
        }
        nodes = stmt->cases;
        while (nodes && xpr_to_case)
        {
            connect_nodes(xpr_to_case, nodes->node);
            nodes = nodes->next;
        }
    }
}

/* $$ = if_stmt ($1,$3,$4,$5,$6,$7) */
stmt_ptr
if_stmt(token_ptr if_token,
    tree_ptr xpr_tree,
    token_ptr rp_token,
    stmt_ptr then_stmt,
    token_ptr else_token,
    stmt_ptr else_stmt)
{
    stmt_ptr new;
    int if_part, else_part, join_part;
    int xfrom, xto;
    /*
    *****************************************************************
    *                                                               *
    *   IF ( XPR ) then_stmt join_part                              *
    *            |   ^     |  ^                                     *
    *            +---+     +--+                                     *
    *            |            |                                     *
    *            +------------+                                     *
    *                                                               *
    *****************************************************************
    */

    if_part = xpr_gen(xpr_tree, &xfrom, &xto);
    if (else_token)
        else_part = get_stmt_no();
    join_part = get_stmt_no();
    gen_require(if_part, join_part, join_part);
    require_stmt(join_part, then_stmt);

    if (!then_stmt->is_jump)
        connect_nodes(then_stmt->exit, join_part);
    connect_nodes(xto, then_stmt->entry);
    if (else_token)
    {
        connect_nodes(else_part, else_stmt->entry);
        if (!else_stmt->is_jump)
            connect_nodes(else_stmt->exit, join_part);
        connect_nodes(xto, else_part);
        source_map(else_part, else_token, else_token);
        gen_require(join_part, else_part, else_part);
        require_stmt(join_part, else_stmt);
        require_stmt(else_part, else_stmt);
    }
    else
        connect_nodes(xto, join_part);

    source_map(if_part, if_token, rp_token);
    source_map(join_part, rp_token, rp_token);
    new = alloc_stmt();
    /* new -> from = then_stmt->from; */
    new->from      = if_part;
    new->to        = join_part;
    new->entry     = xfrom;
    new->exit      = join_part;
    new->breaks    = then_stmt->breaks;
    new->cases     = then_stmt->cases;
    new->continues = then_stmt->continues;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;

    if (else_stmt)
        merge_bc(new, else_stmt);
    /*
    printf ("IF %d-%d joins at %d  then [%d-%d]\n",
            xfrom,xto,join_part,then_stmt->entry,then_stmt->exit);
    */
    return new;
}

/*
*****************************************************************
*                                                               *
*                                                               *
*   WHILE ( XPR ) STMT                                          *
*             | ^   ^                                           *
*             +-+   |                                           *
*             |     |                                           *
*             +-----+                                           *
*                                                               *
*****************************************************************
*/
stmt_ptr
while_stmt(token_ptr while_token, tree_ptr xpr_tree, token_ptr rp_token, stmt_ptr stmt)
{
    int while_node, rp_node;
    int xfrom, xto;
    stmt_ptr new;

    while_node = xpr_gen(xpr_tree, &xfrom, &xto);
    rp_node    = get_stmt_no();
    source_map(while_node, while_token, rp_token);
    source_map(rp_node, rp_token, rp_token);
    connect_nodes(xto, stmt->entry);
    connect_nodes(xto, rp_node);
    connect_nodes(stmt->exit, xfrom);
    break_continue(stmt, rp_node, xfrom);

    gen_require(while_node, rp_node, rp_node);
    require_stmt(rp_node, stmt);

    /*
    printf ("WHILE [%d-%d] join %d  [%d-%d]\n",xfrom,xto,rp_node,
            stmt->entry,stmt->exit);
            */
    control(WHILE_CTRL, xfrom, rp_node, stmt->entry, stmt->exit, 0, 0);

    new       = alloc_stmt();
    new->from = while_node;
    new->to   = rp_node;

    new->entry = xfrom;
    new->exit  = rp_node;

    new->breaks    = NULL;
    new->cases     = NULL;
    new->continues = NULL;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;

    return new;
}

/*
*****************************************************************
*                                                               *
*    +----------------+                                         *
*    v                |                                         *
*   DO stmt WHILE ( EXPR )                                      *
*    |  ^ |  ^        |  ^                                      *
*    +--+ +--+        +--+                                      *
*                                                               *
*****************************************************************
*/
stmt_ptr
do_stmt(
    token_ptr do_token, stmt_ptr stmt, token_ptr while_token, tree_ptr xpr_tree, token_ptr rp_token)
{
    int do_node, while_node, rp_node;
    int xfrom, xto;
    stmt_ptr new;

    do_node    = get_stmt_no();
    while_node = xpr_gen(xpr_tree, &xfrom, &xto);
    rp_node    = get_stmt_no();
    break_continue(stmt, rp_node, xfrom);
    source_map(do_node, do_token, do_token);
    source_map(rp_node, rp_token, rp_token);
    source_map(while_node, while_token, rp_token);

    connect_nodes(do_node, stmt->entry);
    connect_nodes(stmt->exit, xfrom);
    connect_nodes(xto, do_node);
    connect_nodes(xto, rp_node);

    gen_require(while_node, do_node, do_node);
    gen_require(rp_node, while_node, while_node);
    require_stmt(do_node, stmt);
    new        = alloc_stmt();
    new->from  = do_node;
    new->to    = rp_node;
    new->entry = do_node;
    new->exit  = rp_node;

    new->breaks    = NULL;
    new->cases     = NULL;
    new->continues = NULL;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;

    return new;
}

/*
        | FOR '(' oexpr ';' oexpr ';' oexpr ')' statement
                {$$ = for_stmt ($1,$2,$3,$5,$7,$8,$9);}
*/

stmt_ptr
for_stmt(token_ptr for_token,
    token_ptr lp_token,
    tree_ptr init_tree,
    token_ptr semi1_token,
    tree_ptr cond_tree,
    token_ptr semi2_token,
    tree_ptr delta_tree,
    token_ptr rp_token,
    stmt_ptr stmt)

{
    int init_node, for_node, exit_node;
    int init_from, init_to, delta_node, delta_from, delta_to, cond_node, cond_from, cond_to;
    stmt_ptr new;

    for_node = get_stmt_no();
    source_map(for_node, for_token, lp_token);
    if (init_tree)
    {
        init_node = xpr_gen(init_tree, &init_from, &init_to);
        source_map(init_node, lp_token, semi1_token);
        gen_require(for_node, init_from, init_to);
    }
    if (cond_tree)
    {
        cond_node = xpr_gen(cond_tree, &cond_from, &cond_to);
        source_map(cond_node, semi1_token, semi2_token);
        gen_require(for_node, cond_from, cond_to);
    }
    if (delta_tree)
    {
        delta_node = xpr_gen(delta_tree, &delta_from, &delta_to);
        source_map(delta_node, semi2_token, rp_token);
        gen_require(for_node, delta_from, delta_to);
    }

    exit_node = get_stmt_no();
    source_map(exit_node, rp_token, rp_token);
    gen_require(exit_node, for_node, for_node);

    break_continue(
        stmt, exit_node, delta_tree ? delta_from : (cond_tree ? cond_from : stmt->entry));

    require_stmt(for_node, stmt);
    if (cond_tree)
    {
        require_stmt(cond_node, stmt);
        if (delta_tree)
            gen_require(cond_node, delta_from, delta_to);
    }

    if (init_tree)
    {
        connect_nodes(for_node, init_from);
        if (cond_tree)
            connect_nodes(init_to, cond_from);
        else
            connect_nodes(init_to, stmt->entry);
    }
    else if (cond_tree)
        connect_nodes(for_node, cond_from);
    else
        connect_nodes(for_node, stmt->entry);

    if (cond_tree)
    {
        connect_nodes(cond_to, stmt->entry);
        connect_nodes(cond_to, exit_node);
    }

    if (delta_tree)
    {
        connect_nodes(stmt->exit, delta_from);
        if (cond_tree)
            connect_nodes(delta_to, cond_from);
        else
            connect_nodes(delta_to, stmt->entry);
    }
    else if (cond_tree)
        connect_nodes(stmt->exit, cond_from);
    else
        connect_nodes(stmt->exit, stmt->entry);

    new        = alloc_stmt();
    new->from  = stmt->from;
    new->to    = exit_node;
    new->entry = for_node;
    new->exit  = exit_node;

    new->breaks    = NULL;
    new->cases     = NULL;
    new->continues = NULL;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;

    return new;
}

/*
        | SWITCH '(' expr ')' statement
                {$$ = switch_stmt ($1,$3,$4,$5);}
*/
stmt_ptr
switch_stmt(token_ptr switch_token, tree_ptr xpr_tree, token_ptr rp_token, stmt_ptr stmt)
{
    int switch_node, sfrom, sto, rp_node;
    int has_default;
    stmt_ptr new;
    list_ptr c;

    switch_node = xpr_gen(xpr_tree, &sfrom, &sto);
    rp_node     = get_stmt_no();
    source_map(switch_node, switch_token, rp_token);
    source_map(rp_node, rp_token, rp_token);
    break_cases(stmt, sto, rp_node);
    connect_nodes(stmt->exit, rp_node);
    gen_require(switch_node, rp_node, rp_node);
    gen_require(rp_node, stmt->to, stmt->to);
    c           = stmt->cases;
    has_default = 0;

    while (c)
    {
        if (c->is_default_case)
        {
            has_default = 1;
            break;
        }
        c = c->next;
    }

    if (!has_default)
    {
        connect_nodes(sto, rp_node);
    }

    new            = alloc_stmt();
    new->from      = switch_node;
    new->to        = rp_node;
    new->entry     = sfrom;
    new->exit      = rp_node;
    new->breaks    = NULL;
    new->cases     = NULL;
    new->continues = stmt->continues;
    new->is_jump   = 0;
    new->next      = NULL;
    new->last      = new;

    return new;
}

/*
        | CASE constant_expr ':' statement
                {$$ = case_stmt ($1,$3,$4,False);}
        | DEFAULT ':' statement
                {$$ = case_stmt ($1,$2,$3,True);}
*/
stmt_ptr
case_stmt(token_ptr case_token, token_ptr colon_token, stmt_ptr stmt, int is_default_case)
{
    int node;
    list_ptr list;

    node = get_stmt_no();
    source_map(node, case_token, colon_token);
    connect_nodes(node, stmt->entry);
    gen_require(node, stmt->from, stmt->to);
    stmt->entry           = node;
    stmt->to              = node;
    list                  = alloc_list();
    list->node            = node;
    list->next            = stmt->cases;
    list->is_marked       = False;
    list->is_default_case = is_default_case;
    stmt->cases           = list;

    return stmt;
}

stmt_ptr
return_stmt(token_ptr return_token, tree_ptr xpr_tree, token_ptr semi_token)
{
    int return_node, rfrom, rto;
    stmt_ptr new;
    list_ptr list;

    if (xpr_tree)
    {
        return_node                  = xpr_gen(xpr_tree, &rfrom, &rto);
        current_proc->has_return_xpr = 1;
    }
    else
    {
        return_node = rfrom = rto = get_stmt_no();
    }
    source_map(return_node, return_token, semi_token);
    new          = make_stmt(rfrom, rto, rfrom, rto);
    new->is_jump = 1;

    list       = alloc_list();
    list->next = returns;
    list->node = rto;
    returns    = list;
    fprintf(outfile, "%d(%d,%d)", LIF_RETURN, rto, xpr_tree ? 1 : 0);
    if (z_opt)
        fprintf(outfile, "    return at %d %s", rto, xpr_tree ? "with vaules" : "with no values");
    fprintf(outfile, "\n");

    return new;
}

void
do_returns(int end_stmt)
{
    while (returns)
    {
        connect_nodes(returns->node, end_stmt);
        returns = returns->next;
    }
}

stmt_ptr
fun_stmt(token_ptr t)
{
    int node;
    stmt_ptr new;
    proc_ptr p;

    node = get_stmt_no();
    p    = find_proc(t);
    if (p)
    {
        p->entry_node = node;
        pid           = p->proc_id;
        current_proc  = p;
    }
    else
    {
        max_proc++;
        add_proc(t, node, max_proc);
        pid = max_proc;
    }
    if (t)
        source_map(node, t, t);
    new = make_stmt(node, node, node, node);
    fprintf(outfile, "%d(%d,%d,%s)", LIF_PROC_START, node, pid, t ? t->text : "NO PROC NAME");
    if (z_opt)
    {
        fprintf(outfile, "   Function %s %d entry at %d", t ? t->text : "LOST NAME", pid, node);
    }
    fprintf(outfile, "\n");
    labels = NULL;
    return new;
}
