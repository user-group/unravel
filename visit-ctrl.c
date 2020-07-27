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


#include <stdio.h>
#include <stdlib.h>

//static char sccsid[] = "@(#)visit-ctrl.c	1.1  10/6/94";

int main(int np, char **p) 
{
    int n_procs, n_nodes, start, stop;
    int i;
    FILE *t_file;

    if (np != 2)
        exit(1);
    t_file = fopen(p[1], "r");
    if (!t_file)
        exit(1);
    fscanf(t_file, "%d%d", &n_procs, &n_nodes);
    /*
    11 186
      1    1    5    1 X get_pixel_size
      2   -1    0    0 X scanf
      3    6   22    6 X get_spheres
      4   23   54   10 X get_poly
      5   55   72   15 X intersect_sphere
      6   -1    0    0 X sqrt
      7   73  109   10 X find_sphere
      8  110  118   11 X trace_ray
      9  119  125    6 X cast
     10  126  186    4 X main
     11   -1    0    0 X printf
    16 0 16
         189     612    4025 ray.c
    */
    for (i = 0; i < n_procs; i++) {
        fscanf(t_file, "%*d %d %d %*d %*c %*s", &start, &stop);
        if (start > 0) {
            printf("%d %d\n", start, stop);
        }
    }
    fclose(t_file);
    return 0;
}

