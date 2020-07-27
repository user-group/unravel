#include <stdio.h>
#include <string.h>

#include "ansi_parse.h"
#include "lif.h"

int max_chain        = 0;
static char sccsid[] = "@(#)chain.c	1.3  5/5/94";

int current_chain() { return max_chain; }

int find_ptr(tree_ptr t, chain_ptr c);
void assemble_dot(char *buff, tree_ptr t, chain_ptr c);

var_ste_ptr type_member(t) type_ste_ptr t;
{
    if (!t)
        return NULL;
    switch (t->detail) {
    case STE_TYPEDEF:
        return type_member(t->type_entry);
    case STE_TAGGED_STRUCT:
        return t->tag_entry ? t->tag_entry->vars : NULL;
    case STE_ANON_STRUCT:
        return t->anon_entry;
    }
    return NULL;
}

var_ste_ptr find_members(var) var_ste_ptr var;
{
    if (!var)
        return NULL;
    /*
    printf ("Find members for %s\n",var->token->text);
    fflush(stdout);
    */
    switch (var->detail) {
    case STE_TYPEDEF:
        return type_member(var->type_entry);
    case STE_TAGGED_STRUCT:
        return var->tag_entry ? var->tag_entry->vars : NULL;
    case STE_ANON_STRUCT:
        return var->anon_entry;
    }
    return NULL;
}

int assemble_chain(t, c) tree_ptr t;
chain_ptr c;
{
    char var_name[2000];
    int status = 0;
    int scope;
    int code;
    var_ste_ptr var;

    switch (t->op_code) {
    case DOT_OP:
        assemble_dot(var_name, t, c);
        /*
        printf("chain %d after dot: %s\n",c->chain_id,var_name);
        */
        if (c->current_seq == -1) { /* 0 was -1???? */
            /*
            printf ("search in symbol table\n");
            */
            var = look_up_id((var_ste_ptr)NULL, var_name, &scope);
        } else {
            /*printf ("should search in member table\n");
            {
                    var_ste_ptr	v;
                    v = c->fields;
                    while(v){
                            printf ("\t? %s\n",v->token->text);
                            v = v-> next;
                    }
            } */
            var = look_up_id(c->fields, var_name, &scope);
            /*
            printf("chain %d after dot: %s var %d %s\n",
                    c->chain_id,var_name,var,var?var->token->text:
            */
            if (var == NULL)
                return 1;
            ++c->current_seq;
            c->fields = find_members(var);
            if (!c->no_output) {
                fprintf(outfile,
                    "%d(%d,%d,%d,%s)",
                    LIF_FIELD,
                    c->chain_id,
                    c->current_seq,
                    var->id,
                    var->token->text);
                if (z_opt)
                    fprintf(outfile,
                        "    field %d (%s) of chain %d",
                        c->current_seq,
                        var->token->text,
                        c->chain_id);
                fprintf(outfile, "\n");
            }
            return 0;
        }
        /*
        printf("CHAIN %d after dot: %s var %d %s\n",
                c->chain_id,var_name,var,var?var->token->text:
        */
    case ID_OP:
        if (t->op_code == ID_OP)
            var = look_up_id((var_ste_ptr)NULL, t->token->text, &scope);
        if (scope > 1)
            code = LIF_CHAIN;
        else
            code = LIF_GCHAIN;
        if (var == NULL)
            return 1;
        c->fields = find_members(var);
        /*
        printf ("fields %d\n",c->fields);
        */
        if (c->fields == NULL)
            return 1;
        if (!c->no_output) {
            fprintf(outfile, "%d(%d,%d)", code, c->chain_id, var->id);
            if (z_opt)
                fprintf(outfile,
                    "    %s chain %d on %s",
                    code == LIF_CHAIN ? "local" : "global",
                    c->chain_id,
                    var->token->text);
            fprintf(outfile, "\n");
        }
        c->current_seq = 0;
        return 0;
    case POINTER_OP:
        status = assemble_chain(t->left, c);
        if (status)
            return status;
        var = look_up_id(c->fields, t->right->token->text, &scope);
        if (x_opt)
            printf("line %d, add field, %s var %p\n", lineno, t->right->token->text, var);
        if (var == NULL)
            return 3;
        ++c->current_seq;
        c->fields = find_members(var);
        if (!c->no_output) {
            fprintf(outfile,
                "%d(%d,%d,%d,%s)",
                LIF_FIELD,
                c->chain_id,
                c->current_seq,
                var->id,
                var->token->text);
            if (z_opt)
                fprintf(outfile,
                    "    field %d (%s) of chain %d",
                    c->current_seq,
                    var->token->text,
                    c->chain_id);
            fprintf(outfile, "\n");
        }
        return 0;
    case LEFTP_OP:
    case RIGHTP_OP:
    case ARRAY_OP:
        /*
                return assemble_chain (t->left,c);
                */
        return find_ptr(t->left, c);
    case BIN_OP:
    case UN_OP:
    case PRE_OP:
    case POST_OP:
    case RELASGN_OP:
    case ASGN_OP:
        return find_ptr(t, c);
    case CALL_OP:
    case ADDR_OP:
    case DEREF_OP:
    case CAST_OP:
    case CON_OP:
    case COMMA_OP:
    default:
        break;
    }
    return 2;
}
int find_ptr(t, c) tree_ptr t;
chain_ptr c;
{
    int left, right;
    chain_rec c_left, c_right;

    if (!t)
        return 1;
    switch (t->op_code) {
    case BIN_OP:
    case UN_OP:
    case PRE_OP:
    case POST_OP:
    case RELASGN_OP:
    case ASGN_OP:
        c_left            = *c;
        c_right           = *c;
        c_left.no_output  = 1;
        c_right.no_output = 1;
        left              = find_ptr(t->left, &c_left);
        right             = find_ptr(t->right, &c_right);
        /*
        printf ("line %d, chain %d left %d right %d\n",lineno,
                c->chain_id,left,right);
        printf ("l mem %d r mem %d\n",c_left.fields,
                c_right.fields);
                */
        c_left.no_output  = c->no_output;
        c_right.no_output = c->no_output;
        if (c_left.fields && c_right.fields)
            return 1;
        if (c_right.fields) {
            *c = c_right;
            return find_ptr(t->right, c);
        }
        *c = c_left;
        if (c_left.fields)
            return find_ptr(t->left, c);
        ;
        return 1;
    case DOT_OP:
    case POINTER_OP:
        return assemble_chain(t, c);
    default:
        return assemble_chain(t, c);
    }
}

int get_chain(t) tree_ptr t;
{
    chain_rec c;
    int status = 0;

    c.chain_id    = ++max_chain;
    c.is_global   = -1;
    c.current_seq = -1;
    c.no_output   = 0;
    c.type        = NULL;
    c.fields      = NULL;
    status        = assemble_chain(t, &c);
    if (status)
        return 0;
    return c.chain_id;
}

void print_local_ptr_tab() { printf("\nLocal Pointer Table\n"); }

void print_global_ptr_tab() { printf("\nGlobal Pointer Table\n"); }

void dot_really_ptr(buff, t, c) char *buff;
tree_ptr t;
chain_ptr c;
{
    var_ste_ptr var;
    int code, scope = 0, is_ptr, is_array;

    var = look_up_id(NULL, buff, &scope);
    if (var) {
        c->is_array    = 0;
        c->chain_id    = ++max_chain;
        c->current_seq = 0;
        c->fields      = find_members(var);
        strcpy(buff, t->right->token->text);
        if (scope > 1)
            code = LIF_CHAIN;
        else
            code = LIF_GCHAIN;
        if (!c->no_output) {
            fprintf(outfile, "%d(%d,%d)", code, c->chain_id, var->id);
            if (z_opt)
                fprintf(outfile,
                    "    %s chain %d on %s",
                    code == LIF_CHAIN ? "local" : "global",
                    c->chain_id,
                    var->token->text);
            fprintf(outfile, "\n");
        }
    } else {
        printf("Warning: var %s in %s at line %d, not found\n", buff, the_current_file, lineno);
    }
}

void assemble_dot(buff, t, c) char *buff;
tree_ptr t;
chain_ptr c;
{
    int status = 0;
    var_ste_ptr var;
    int scope, is_ptr, is_array;
    char hold[2000];

    switch (t->op_code) {
    case ID_OP:
        strcpy(buff, t->token->text);
        return;
    case DOT_OP:
        /*
        printf ("dot dot down (%s) at line %d \n",buff,lineno);
        */
        c->is_array = 0;
        assemble_dot(buff, t->left, c);
        /*
        printf ("dot dot back %s %s\n",buff,c->is_array?"(array)":"");
        */
        if (c->is_array) {
            c->is_array = 0;
            strcpy(hold, buff);
            strcat(hold, ".");
            strcat(hold, t->right->token->text);
            if ((c->chain_id == -1)) {
                var = look_up_id(NULL, hold, &scope);
                if (var) {
                } else {
                    /*
                    printf ("dot really ptr at line %d\n",lineno);
                    */
                    dot_really_ptr(buff, t, c);
                    return;
                }
            } else {
                var = look_up_id(c->fields, hold, &scope);
                if (!var) {
                    /*
                    printf ("buff (%s) hold (%s) ",buff,hold);
                    printf ("at line %d, array on chain %d\n",lineno,
                            c->chain_id);
                    */
                    var = look_up_id(c->fields, buff, &scope);
                    if (var) {
                        ++c->current_seq;
                        c->fields = find_members(var);
                        if (!c->no_output) {
                            fprintf(outfile,
                                "%d(%d,%d,%d,%s)",
                                LIF_FIELD,
                                c->chain_id,
                                c->current_seq,
                                var->id,
                                var->token->text);
                            if (z_opt)
                                fprintf(outfile,
                                    "    field %d (%s) of chain %d",
                                    c->current_seq,
                                    var->token->text,
                                    c->chain_id);
                            fprintf(outfile, "\n");
                        }
                        strcpy(buff, t->right->token->text);
                        return;
                    }
                }
            }
        }
        strcat(buff, ".");
        strcat(buff, t->right->token->text);
        return;
    case LEFTP_OP:
    case RIGHTP_OP:
        assemble_dot(buff, t->left, c);
        return;
    case ARRAY_OP:
        /*
        printf ("dot array down %s\n",buff);
        */
        assemble_dot(buff, t->left, c);
        /*
        printf ("dot array back %s chain_id %d\n",buff,c->chain_id);
        var = look_up_id (NULL,buff,&scope);
        printf ("look up %s %d\n",buff,var);
        */
        c->is_array = 1;
        return;
    case POINTER_OP:
        /*
        printf ("dot pointer down %s\n",buff);
        */
        if ((c->chain_id == -1) || (c->chain_id == 0)) {
            if (c->no_output)
                c->chain_id = 0;
            else
                c->chain_id = ++max_chain;
            c->is_global   = -1;
            c->current_seq = -1;
            c->type        = NULL;
            c->fields      = NULL;
        }
        status = assemble_chain(t->left, c);
        strcpy(buff, t->right->token->text);
        /*
        printf ("dot pointer back %s\n",buff);
        */

    default:;
    }
    return;
}
