/*
 * ---------------------------------------------------------------------------
 * - p_0008.c                                                                -
 * - iato:pgm tester : malloc, calloc test module                            -
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

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char** argv) {
  void*  addr = NULL;
  size_t size = 0;
  size_t i    = 0;

  for (i = 1; i < 10; i++) {
    size = 1024 * i;
    if ((addr = malloc (size)) == NULL) {
      fprintf (stdout, "malloc size = %ld failed\n", size);
    } else {
      fprintf (stdout, "malloc size = %ld at %p sucess\n", size, addr);
    }
  }
  for (i = 1; i < 10; i++) {
    size = 1024 * i;
    if ((addr = calloc (i, size)) == NULL) {
      fprintf (stdout, "calloc size = %ld failed\n", size);
    } else {
      fprintf (stdout, "calloc size = %ld at %p sucess\n", size, addr);
    }
  }

  for (i = 1; i < 10; i++) {
    size = 1024 * 1024 * i;
    if ((addr = malloc (size)) == NULL) {
      fprintf (stdout, "malloc size = %ld failed\n", size);
    } else {
      fprintf (stdout, "malloc size = %ld at %p sucess\n", size, addr);
    }
  }
  for (i = 1; i < 10; i++) {
    size = 1024 * 1024 * i;
    if ((addr = calloc (i, size)) == NULL) {
      fprintf (stdout, "calloc size = %ld failed\n", size);
    } else {
      fprintf (stdout, "calloc size = %ld at %p sucess\n", size, addr);
    }
  }
  return 0;
}
