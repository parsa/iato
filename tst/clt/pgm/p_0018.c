/*
 * ---------------------------------------------------------------------------
 * - p_0018.c                                                                -
 * - iato:pgm tester : iaio tester with floating point number                -
 * ---------------------------------------------------------------------------
 * - (c) inria 2002-2004                                                     -
 * ---------------------------------------------------------------------------
 * - authors                                      Amaury Darsch    2002:2004 -
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

static float f1 (const int i) {
  return (float) i*2;
}

int main(int argc, char** argv) {

  /* check for cast first */
  fprintf (stdout, "f1 test : %f\n", f1 (20));

  /* success */
  return 0;
}
