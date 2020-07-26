main()
{
	int		input1,input2,input3,cond1,cond2;
	int		a,b,c;
	int		*x,*y,*z;
	int		output1,output2,output3;

	scanf("%d",&input1);
	a = input1;
	scanf("%d",&input2);
	b = input2;
	scanf("%d",&input3);
	c = input3;
	scanf("%d",&cond1);
	scanf("%d",&cond2);
	x = &a;
	if (cond1) x = &b;
	y = &c;
	z = &b;
	output2 = *z + 1;
	*z = *y + *x;
	output1 = *z;
	output3 = *x;
	printf ("%d %d %d\n",
		output1,output2,output3);
}
