/*
 * ---------------------------------------------------------------------------
 * - p_0011.c                                                                -
 * - iato:pgm tester : unlink file                                           -
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
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main () {
  int f1 = 0, f2 = 0, f3 = 0;
  char *s1, *s2, *s3;
  s1 = strdup ("/tmp/iatoXXXXXX"); f1 = mkstemp (s1); close (f1);
  s2 = strdup ("/tmp/iatoXXXXXX"); f2 = mkstemp (s2); close (f2);
  s3 = strdup ("/tmp/iatoXXXXXX"); f3 = mkstemp (s3); close (f3);
  if (unlink (s2) != 0){
    fprintf (stdout, "unlink error %s\n", s2);
    return 1;
  }
  if (unlink (s3) != 0){
    fprintf (stdout, "unlink error %s\n", s3);
    return 1;
  }
  if (link (s1, s2) == -1){
    fprintf (stdout, "link error %s\n", s2);
    return 1;
  }
  if (rename (s2, s3) == -1){
    fprintf (stdout, "rename error %s\n", s3);
    return 1;
  } 
  if (unlink (s1) != 0){
    fprintf (stdout, "unlink error %s\n", s1);
    return 1;
  }
  if (unlink (s3) != 0){
    fprintf (stdout, "unlink error %s\n", s3);
    return 1;
  }
  return 0;
}
