main()
{
	int		input1,input2,input3;
	int		a,b,c,x,y,z;
	int		output1,output2,output3;

	scanf("%d",&input1);
	a = input1;
	scanf("%d",&input2);
	b = input2;
	scanf("%d",&input3);
	c = input3;
	x = b + c;
	y = a + 1;
	z = y + b;
	output2 = z + 1;
	z = z*z;
	output1 = z;
	output3 = x;
	printf ("%d %d %d\n",
		output1,output2,output3);
}
