/*
 * ---------------------------------------------------------------------------
 * - p_0016.c                                                                -
 * - iato:pgm tester : mkdir & rmdir test module                             -
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

#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main () {
  char *s1;
  s1 = strdup ("/tmp/iatoXXXXXX");
  if (mkdir (s1, S_IRUSR|S_IWUSR|S_IXUSR) == -1) return 2;
  if (rmdir (s1) == -1)   return 1;  
  return 0;
}
