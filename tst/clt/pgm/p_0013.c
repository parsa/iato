/*
 * ---------------------------------------------------------------------------
 * - p_0013.c                                                                -
 * - iato:pgm tester : stat test                                             -
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
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (void) {
  struct stat sts;
  int    fd   = -1;

  /* check stat and lstat */
  if (stat ("p_0013.c", &sts) != 0) return 1;
  fprintf (stdout, "p_0013: stat success\n");
  if (lstat ("p_0013.c", &sts) != 0) return 1;
  fprintf (stdout, "p_0013: lstat success\n");

  /* check fstat */
  fd = open ("p_0013.c", O_RDONLY);
  if (fstat (fd, &sts) != 0) return 1;
  close (fd);
  fprintf (stdout, "p_0013: fstat success\n");

  // check for access */
  if (access ("p_0013", R_OK | X_OK) != 0) return 1;
  fprintf (stdout, "p_0013: access success\n");
  return 0;
}
