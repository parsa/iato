/*
 * ---------------------------------------------------------------------------
 * - p_0007.c                                                                -
 * - iato:pgm tester : memcpy test module
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

#include <string.h>
#include <stdio.h>

const long SIZE = 30;
int main (int argc, char** argv) {
  int i = 0;
  char src[SIZE];
  char dst[SIZE];
  for (i = 0; i < SIZE; i++) src[i] = i;
  memcpy (dst, src, SIZE);
  for (i = 0; i < SIZE; i++) {
    if (dst[i] != src[i]) {
      fprintf (stdout, "memcpy failure at %d\n", i);
      return 1;
    }
  }
  fprintf (stdout, "memcpy success\n");
  return 0;
}
