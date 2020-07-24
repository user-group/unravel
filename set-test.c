# include "sets.h"
int is_subset();
main ()
{
	int		i;
	bit_set	a,b,c,d;

	a = create_bit_set (35);
	b = create_bit_set (35);
	c = create_bit_set (35);
	d = create_bit_set (35);

	for (i = 0; i < 35; i++){
		bit_on (a,i);
		bit_on (b,2*(i/2));
		bit_on (c,i);
		bit_on (d,2*(i/2));
	}
	printf ("a == b %d  a sub b %d  b sub a %d\n",
		bit_set_equal (a,b),is_subset(a,b),is_subset(b,a));
	printf ("a == c %d  a sub c %d  c sub a %d\n",
		bit_set_equal (a,c),is_subset(a,c),is_subset(c,a));
	printf ("a == d %d  a sub d %d  d sub a %d\n",
		bit_set_equal (a,d),is_subset(a,d),is_subset(d,a));
	printf ("b == c %d  b sub c %d  c sub b %d\n",
		bit_set_equal (b,c),is_subset(b,c),is_subset(c,b));
	printf ("b == d %d  b sub d %d  d sub b %d\n",
		bit_set_equal (b,d),is_subset(b,d),is_subset(d,b));
	printf ("c == d %d  c sub d %d  d sub c %d\n",
		bit_set_equal (c,d),is_subset(c,d),is_subset(d,c));

	bit_off (d,6);
	bit_off (c,7);

	printf ("\n\na == b %d  a sub b %d  b sub a %d\n",
		bit_set_equal (a,b),is_subset(a,b),is_subset(b,a));
	printf ("a == c %d  a sub c %d  c sub a %d\n",
		bit_set_equal (a,c),is_subset(a,c),is_subset(c,a));
	printf ("a == d %d  a sub d %d  d sub a %d\n",
		bit_set_equal (a,d),is_subset(a,d),is_subset(d,a));
	printf ("b == c %d  b sub c %d  c sub b %d\n",
		bit_set_equal (b,c),is_subset(b,c),is_subset(c,b));
	printf ("b == d %d  b sub d %d  d sub b %d\n",
		bit_set_equal (b,d),is_subset(b,d),is_subset(d,b));
	printf ("c == d %d  c sub d %d  d sub c %d\n",
		bit_set_equal (c,d),is_subset(c,d),is_subset(d,c));
}
