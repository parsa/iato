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
  int fd = 0;

  if ( lstat ("p_0013.c", & sts) != 0) {
    fprintf (stderr, "stat erreur\n");
    return 1;
  }
  fprintf (stdout,
	   "Noeud : %ld\n",
	   sts . st_ino);
  fprintf (stdout,
	   "Taille : %lu octets\n",
	   sts . st_size);
  if ( stat ("p_0013.c", & sts) != 0) {
    fprintf (stderr, "stat erreur\n");
    return 2;
  }
  fprintf (stdout,
	   "Noeud : %ld\n",
	   sts . st_ino);
  fprintf (stdout,
	   "Taille : %lu octets\n",
	   sts . st_size);

  fd = open ("p_0013.c", O_RDONLY);
  if ( fstat (fd, & sts) != 0) {
    fprintf (stderr, "stat erreur\n");
    return 3;
  }
  fprintf (stdout,
	   "Noeud : %ld\n",
	   sts . st_ino);
  fprintf (stdout,
	   "Taille : %lu octets\n",
	   sts . st_size);

  if ( access ("p_0013", R_OK | X_OK) != 0) {
    fprintf (stderr, "access erreur\n");
    return 4;
  }
  return 0;
}
