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

/*
static char sccsid[] = "@(#)visit-post.c	1.2  8/16/95";
*/
/*
        force proc entry and exit to be first and last
*/

int first, last, nodes[1000000], i, entry_at, exit_at;

int main(int np, char **p) 
{
    int k, n, entry_seen;
    char buff[2000];

    if (np != 3) {
        fprintf(stderr, "%s: usage visit-post first-node last-node\n", p[0]);
        exit(1);
    }

    first = atoi(p[1]);
    last  = atoi(p[2]);
    i     = 0;
    while (fgets(buff, 2000, stdin)) {
        k = sscanf(buff, "%d", &nodes[i]);
        if (k == 1)
            i++;
    }
    n          = i;
    entry_seen = 0;
    for (i = 0; i < n; i++) {
        if (first == nodes[i]) {
            entry_seen = 1;
        }
        if (entry_seen) {
            if (nodes[i] != last)
                printf("%d\n", nodes[i]);
        }
    }
    entry_seen = 0;
    for (i = 0; i < n; i++) {
        if (first == nodes[i]) {
            break;
        }
        if (nodes[i] != last)
            printf("%d\n", nodes[i]);
    }
    printf("%d\n", last);
    return 0;
}

