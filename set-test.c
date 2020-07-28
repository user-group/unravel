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
#include "sets.h"
int is_subset();
main()
{
    int i;
    bit_set a, b, c, d;

    a = create_bit_set(35);
    b = create_bit_set(35);
    c = create_bit_set(35);
    d = create_bit_set(35);

    for (i = 0; i < 35; i++)
    {
        bit_on(a, i);
        bit_on(b, 2 * (i / 2));
        bit_on(c, i);
        bit_on(d, 2 * (i / 2));
    }
    printf("a == b %d  a sub b %d  b sub a %d\n",
        bit_set_equal(a, b),
        is_subset(a, b),
        is_subset(b, a));
    printf("a == c %d  a sub c %d  c sub a %d\n",
        bit_set_equal(a, c),
        is_subset(a, c),
        is_subset(c, a));
    printf("a == d %d  a sub d %d  d sub a %d\n",
        bit_set_equal(a, d),
        is_subset(a, d),
        is_subset(d, a));
    printf("b == c %d  b sub c %d  c sub b %d\n",
        bit_set_equal(b, c),
        is_subset(b, c),
        is_subset(c, b));
    printf("b == d %d  b sub d %d  d sub b %d\n",
        bit_set_equal(b, d),
        is_subset(b, d),
        is_subset(d, b));
    printf("c == d %d  c sub d %d  d sub c %d\n",
        bit_set_equal(c, d),
        is_subset(c, d),
        is_subset(d, c));

    bit_off(d, 6);
    bit_off(c, 7);

    printf("\n\na == b %d  a sub b %d  b sub a %d\n",
        bit_set_equal(a, b),
        is_subset(a, b),
        is_subset(b, a));
    printf("a == c %d  a sub c %d  c sub a %d\n",
        bit_set_equal(a, c),
        is_subset(a, c),
        is_subset(c, a));
    printf("a == d %d  a sub d %d  d sub a %d\n",
        bit_set_equal(a, d),
        is_subset(a, d),
        is_subset(d, a));
    printf("b == c %d  b sub c %d  c sub b %d\n",
        bit_set_equal(b, c),
        is_subset(b, c),
        is_subset(c, b));
    printf("b == d %d  b sub d %d  d sub b %d\n",
        bit_set_equal(b, d),
        is_subset(b, d),
        is_subset(d, b));
    printf("c == d %d  c sub d %d  d sub c %d\n",
        bit_set_equal(c, d),
        is_subset(c, d),
        is_subset(d, c));
}
