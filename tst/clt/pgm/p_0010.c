/*
 * ---------------------------------------------------------------------------
 * - p_0010.c                                                                -
 * - iato:pgm tester : long division test module                             -
 * ---------------------------------------------------------------------------
 * - (c) inria 2002-2004                                                     -
 * ---------------------------------------------------------------------------
 * - authors                                      Amaury Darsch    2002:2004 -
 * -                                              Pierre Villalon  2002:2003 -
 * -                                              Andre  Seznec    2002:2004 -
 * ---------------------------------------------------------------------------
 * - This program is  free software;  you can redistribute  it and/or modify -
 * - it  under  the  terms of the  GNU  General Public License version 2, as -
 * - published by the Free Software Foundation.                              -
 * -                                                                         -
 * - This  program  is  distributed  in  the  hope that it  will be  useful, -
 * - but   without  any  warranty;  without  even  the implied  warranty  of -
 * - merchantability or fitness for a particular purpose.                    -
 * -                                                                         -
 * - See the GNU General Public License version 2 for more details           -
 * ---------------------------------------------------------------------------
 */

#include <stdio.h>

static unsigned long long make_udiv (const unsigned long long base) {
  return (0xFFFFFFFFFFFFFFFFULL / base);
}

int main () {
  unsigned long long x = make_udiv (10);
  if (x != 0x1999999999999999ULL) {
    fprintf (stdout, "udiv3 failure\n");
    return 1;
  }
  fprintf (stdout, "udiv3 sucess\n");
  return 0;
}

