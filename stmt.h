#ifndef _STMT_H_
#define _STMT_H_

#include "ansi_parse.h"

void do_returns(int end_stmt);
void source_map (int n, token_ptr from, token_ptr to);


void connect_nodes (int from, int to);

void gen_require (int node_required, int from, int to);

#endif


