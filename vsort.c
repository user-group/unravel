# include <stdio.h>
typedef struct ll_struct ll_node,*ll;
struct ll_struct {
	int value;
	ll	next;
	};
typedef struct {
	int		is_branch;
	int		is_join;
	int		visit;
	ll		branches_to,joins_from;
	} node, *node_ptr;
node	nodes[10000];
int		n_nodes = 0;

output(k)
	int		k;
{
	ll		go_list;

	if (nodes[k].visit) return;
	nodes[k].visit = 1;
	go_list = nodes[k].branches_to;
	while (go_list){
		output(go_list->value);
		go_list = go_list -> next;
	}
	printf ("%d\n",k);
}

output1(k)
	int		k;
{
	int		at;
	ll		from;

	/* printf ("output %d\n",k); */
	if (nodes[k].visit) return;
	at = k;
	while (at){
		nodes[at].visit = 1;
		printf ("%d\n",at);
		from = nodes[at].joins_from;
		at = 0;
		while(from){
			if (from->next) {
				output(from->value);
			}
			else at = from->value;
			from = from->next;
		}
		if (nodes[at].visit) at = 0;
	}
}

int go_deep (k)
	int		k;
{
	int	at;
	at = k;
	while (at){
		nodes[at].visit = -1;

	}
}

main()
{
	int		i;
	int		from,to;
	ll		new_list,sort_at;
	int		hold;
	int		min,max;

	for (i = 0; i < 10000; i++){
		nodes[i].is_branch = 0;
		nodes[i].is_join = 0;
		nodes[i].visit = 1;
		nodes[i].branches_to = NULL;
		nodes[i].joins_from  = NULL;
	}
	max = 0; min = 1000000000;
	while (EOF != scanf("%d %d",&from,&to)){
		if (from > n_nodes) n_nodes = from;
		if (to > n_nodes) n_nodes = to;
		if (from < min) min = from;
		if (from > max) max = from;
		if (to < min) min = to;
		if (to > max) max = to;
		nodes[from].visit = nodes[to].visit = 0;
		new_list = (ll) malloc (sizeof(ll_node));
		new_list -> value = to;
		new_list -> next = NULL;
		if (nodes[from].branches_to){
			new_list -> next = nodes[from].branches_to;
			nodes[from].branches_to = new_list;
			nodes[from].is_branch = 1;
		}
		else {
			nodes[from].branches_to = new_list;
		}
		new_list = (ll) malloc (sizeof(ll_node));
		new_list -> value = from;
		new_list -> next = NULL;
		if (nodes[to].joins_from){
			new_list -> next = nodes[to].joins_from;
			nodes[to].joins_from = new_list;
			nodes[to].is_join = 1;
			sort_at = new_list;
			while (sort_at->next){
				if (sort_at->value < sort_at->next->value){
					hold = sort_at->next->value;
					sort_at->next->value = sort_at -> value;
					sort_at -> value = hold;
				}
				sort_at = sort_at->next;
			}
		}
		else {
			nodes[to].joins_from = new_list;
		}
	}

/*	
	printf ("%d nodes\n",n_nodes);
	for (i = 1; i <= n_nodes; i++){
		if (nodes[i].is_branch) printf   ("branch at %d\n",i);
		if (nodes[i].is_join) printf     ("join   at %d\n",i);
		if (!nodes[i].branches_to) printf("exit   at %d\n",i);
		if (!nodes[i].joins_from)  printf("entry  at %d\n",i);
	}
	*/
	
	for (i = min; i <= max; i++){
		if (nodes[i].visit){
			printf ("%d\n",i);
		}
	}
	for (i = 1; i <= n_nodes; i++){
		if (!nodes[i].joins_from) output (i);
	}
	for (i = 1; i <= n_nodes; i++)
	{
		if (!nodes[i].visit){
			fprintf (stderr,"no visit to %d\n",i);
			output (i);
		}
	}
	
	/*
		if (!nodes[i].branches_to) output (i);
	for (i = 1; i <= n_nodes; i++)
	for (i = n_nodes; i > 0; i--)
	{
		if (!nodes[i].visit){
			fprintf (stderr,"no visit to %d\n",i);
			go_deep (i);
		}
	}
	*/
}
