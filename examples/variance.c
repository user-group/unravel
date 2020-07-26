#include <stdio.h>
#define MAX 1024
main()
{
      float x[MAX];
      float var2, var3, var4 ;
      float var5, var1;
      float t1, t2;
      float ssq;
      float avg;
      float dev;
      int  i, n;
         t2 = 0 ;
         t1 = 0 ;
         ssq = 0 ;
         dev = 0;
         scanf ("%d", &n);
         for ( i = 0 ; i < n ; i = i + 1)
         {
           scanf ("%f", &x[i]);
           t1 = t1 + x[i];
           ssq = ssq + x[i] * x[i];
          }
         avg = t1 / n;
         var3 = (ssq  - n * avg * avg) / (n - 1);
         var4 = (ssq  - t1 * avg) / (n - 1);
         t1 = t1 * t1 / n;
         var2 = (ssq  -  t1 ) / (n - 1);
         t1 = 0 ;
         for ( i = 0 ; i < n ; i = i + 1)
         {
           dev = x[i] - avg ;
           t2 = t2 + dev ;
           t1 = t1 + dev * dev ;
         }
         var5 = (t1 - t2 * t2 / n ) / (n -1);
         var1 = t1 / (n - 1);
          printf("variance (one pass, using square of sum): %f \n",var2);
          printf("variance (one pass, using average): %f \n",var3);
          printf("variance (one pass, using average, sum): %f \n",var4);
          printf("variance (two pass, corrected): %f \n",var5);
          printf("variance (two pass): %f \n",var1);
}
