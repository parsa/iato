/*
 * ---------------------------------------------------------------------------
 * - p_0015.c                                                                -
 * - iato:pgm tester : times test module                                     -
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
#include <sys/times.h>

int main () {
  struct tms buf;
  if (times(&buf) == -1) return 1;
  fprintf (stdout, "tms_utime  : %ld\n", buf.tms_utime);
  fprintf (stdout, "tms_stime  : %ld\n", buf.tms_stime);
  fprintf (stdout, "tms_cutime : %ld\n", buf.tms_cutime);
  fprintf (stdout, "tms_cstime : %ld\n", buf.tms_cstime);
  return 0;
}

