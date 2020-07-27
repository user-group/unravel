#include "sets.h"
#include <stdio.h>
#include <stdlib.h>
#define WORD_SIZE 32
#define FALSE 0
#define TRUE 1

static char *sccs_h_id = SETS_SCCS_ID;
static char *sccs_id   = "@(#)sets.c	1.2  8/16/95";

void clear_id_set(head) id_set_ptr head;
{
    id_set_ptr at;

    while (head) {
        at = head->next;
        free((char *)head);
        head = at;
    }
}

int is_in_id_set(head, pid, id) id_set_ptr head;
int id, pid;
{
    id_set_ptr at;

    if (head) {
        at = head;
        while (at) {
            if ((at->id == id) && (at->pid == pid))
                return 1;
            at = at->next;
        }
    }
    return 0;
}

int add_to_id_set(head, pid, id) id_set_ptr *head;
int id, pid;
{
    id_set_ptr last, new, at;

    if (*head) {
        at = *head;
        while (at) {
            if ((at->id == id) && (at->pid == pid))
                return 0;
            last = at;
            at   = at->next;
        }
    }
    new = (id_set_ptr)malloc(sizeof(id_set_node));
    if (!new) {
        fprintf(stderr, "out of memory in add_to_id_set\n");
        exit(1);
    }
    new->id   = id;
    new->pid  = pid;
    new->next = NULL;
    if (*head)
        last->next = new;
    else
        *head = new;
    return 1;
}

var_ptr add_to_var_set(s, id, level, code) var_ptr s;
int id, level, code;
{
    var_ptr new;

    new = s;
    while (new) {
        if ((id == new->id) && (level == new->level) && (code == new->code))
            return s;
        new = new->next;
    }
    make_var(new, id, level, code);
    new->next = s;
    return new;
}

set_ptr add_to_set(s, v) set_ptr s;
int v;
{
    set_ptr new;

    new = s;
    while (new) {
        if (v == new->id)
            return s;
        new = new->next;
    }
    make_set(new, v);
    new->next = s;
    return new;
}

bit_set create_bit_set(size) int size;
{
    bit_set b;
    int i, w;
    unsigned int *start;

    w = (size + WORD_SIZE - 1) / WORD_SIZE;
    /* ******* FIX FIX FIX ***********/
    /* bug when size = 32            */
    /*
    w = w + 1;  KLUDGE
    printf ("Create %d words for size = %d with bpw(%d)\n",w,
            size,WORD_SIZE);
    */

    start = (unsigned int *)malloc((unsigned int)(w * sizeof(int)));
    if (start)
        for (i = 0; i < w; i++)
            start[i] = 0;

    b = (bit_set)malloc(sizeof(bit_set_node));
    if (b) {
        b->bits = start;
        b->max  = size;
    }
    return b;
}

void clear_bit_set(set) bit_set set;
{
    int i;

    if (!set)
        return;

    /*
    printf ("Clear %d:",set->max);
    printf ("set %x max %x bits %x:",set,&(set->max),(set->bits));
    for (i = 0; i <= set->max/WORD_SIZE; i++)
    for (i = 0; i <= set->max/WORD_SIZE; i++) set->bits[i] = 0;
    for (i = 0; i < (set->max + WORD_SIZE - 1)/WORD_SIZE; i++)
            printf (" (%d,%x)",i,&(set->bits[i]));
    printf ("\n");
    */

    for (i = 0; i < (set->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        set->bits[i] = 0;
}

void bit_on(b, at) bit_set b;
int at;
{
    if (at < 0 || !b || b->max < at)
        return;
    b->bits[at / WORD_SIZE] |= 1 << (at % WORD_SIZE);
}

void bit_off(b, at) bit_set b;
int at;
{
    if (at < 0 || !b || b->max < at)
        return;
    b->bits[at / WORD_SIZE] &= ~(1 << (at % WORD_SIZE));
}

int is_bit_on(b, at) bit_set b;
int at;
{
    if (at < 0 || !b || b->max < at)
        return 0;
    if (b->bits[at / WORD_SIZE] & 1 << (at % WORD_SIZE))
        return 1;
    return 0;
}

void copy_bit_set(to, from) bit_set to, from;
{
    int i;

    if (to->max > from->max) {
        fprintf(stderr, "Bad call to copy_bit_set");
        exit(1);
    }

    for (i = 0; i < (from->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        to->bits[i] = from->bits[i];
}

int bit_set_equal(a, b) bit_set a, b;
{
    int i;

    if (a->max != b->max) {
        fprintf(stderr, "Bad call to bit_set_equal");
        exit(1);
    }

    for (i = 0; i < (a->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        if (a->bits[i] != b->bits[i])
            return FALSE;
    return TRUE;
}

void union_bit_set(to, from) bit_set to, from;
{
    int i;

    if (to->max != from->max) {
        fprintf(stderr, "Bad call to union_bit_set");
        fprintf(stderr, " to(%d) vs from(%d)\n", to->max, from->max);
        exit(4);
    }

    for (i = 0; i < (to->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        to->bits[i] |= from->bits[i];
}

int cunion_bit_set(to, from) bit_set to, from;
{
    int i;
    int change = 0, old;

    if (to->max != from->max) {
        fprintf(stderr, "Bad call to union_bit_set");
        fprintf(stderr, " to(%d) vs from(%d)\n", to->max, from->max);
        exit(4);
    }

    for (i = 0; i < (to->max + WORD_SIZE - 1) / WORD_SIZE; i++) {
        old = to->bits[i];
        to->bits[i] |= from->bits[i];
        if (to->bits[i] != old)
            change = 1;
    }
    return change;
}

int is_subset(sub, set) bit_set sub, set;
{
    int i;
    int proper = 0;

    if (!sub || !set) {
        fprintf(stderr, "Bad call to is_subset");
        exit(1);
    }

    if (sub->max != set->max) {
        fprintf(stderr, "Bad call to is_subset");
        fprintf(stderr, " sub(%d) vs set(%d)\n", sub->max, set->max);
        exit(4);
    }

    for (i = 0; i < (sub->max + WORD_SIZE - 1) / WORD_SIZE; i++) {
        if (sub->bits[i] & (~set->bits[i]))
            return 0;
        if (sub->bits[i] != set->bits[i])
            proper = 1;
    }
    return proper;
}

void not_bit_set(to) bit_set to;
{
    int i;

    if (!to) {
        fprintf(stderr, "Bad call to not_bit_set");
        exit(1);
    }

    for (i = 0; i < (to->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        to->bits[i] = ~to->bits[i];
}

void and_bit_set(to, from) bit_set to, from;
{
    int i;

    if (to->max != from->max) {
        fprintf(stderr, "Bad call to and_bit_set");
        exit(1);
    }

    for (i = 0; i < (to->max + WORD_SIZE - 1) / WORD_SIZE; i++)
        to->bits[i] &= from->bits[i];
}

int get_prev_member(set, at) bit_set set;
int at;
{
    int end, i, j, start;

    at--;
    if (at < 0 || !set || at > set->max)
        return -1;
    end = (at / WORD_SIZE) * WORD_SIZE;
    if ((set->bits[at / WORD_SIZE]) << (WORD_SIZE - (at % WORD_SIZE)))
        /* more in this word */
        for (i = at; i >= end; i--)
            if (is_bit_on(set, i))
                return i;
    start = end / WORD_SIZE - 1;
    for (j = start; j >= 0; j--)
        if (set->bits[j])
            for (i = (j + 1) * WORD_SIZE - 1; i >= j * WORD_SIZE; i--)
                if (is_bit_on(set, i))
                    return i;
    return -1;
}

int get_next_member(set, last) bit_set set;
int last;
{
    int end, i, j, start;

    last++;
    if (last < 0 || !set || last > set->max)
        return -1;
    end = (last / WORD_SIZE + 1) * WORD_SIZE;
    if ((set->bits[last / WORD_SIZE]) >> (last % WORD_SIZE))
        /* more in this word */
        for (i = last; i < end; i++)
            if (is_bit_on(set, i))
                return i;
    start = end / WORD_SIZE;
    end   = set->max / WORD_SIZE;
    for (j = start; j <= end; j++)
        if (set->bits[j])
            for (i = j * WORD_SIZE; i < (j + 1) * WORD_SIZE; i++)
                if (is_bit_on(set, i))
                    return i;
    return -1;
}
