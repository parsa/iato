/*
 * ---------------------------------------------------------------------------
 * - p_0006.c                                                                -
 * - iato:pgm tester : argument check and prints                             -
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

int main (int argc, char** argv) {
  int i = 0;
  char** envp = &argv[argc+1];

  /* print arguments */
  for (i = 0; i < argc; i++) 
    fprintf (stdout, "argv[%d] = %s\n", i, argv[i]);

  /* print environment */
  i = 0;
  while (*envp != NULL) {
    fprintf (stdout, "envp[%d] = %s\n", i++,*envp++);
  }
  return 0;
}

