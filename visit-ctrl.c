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
