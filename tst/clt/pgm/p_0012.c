/*
 * ---------------------------------------------------------------------------
 * - p_0012.c                                                                -
 * - iato:pgm tester : sigaction and kill test                               -
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
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>

void handler (int sig) {
  if (sig != SIGINT) {
    fprintf (stderr, "bad handler call\n");
    exit (3);
  }
}

int main () {
  int result = 0;
  struct sigaction act;
  act.sa_handler = handler;
  result = sigaction (SIGINT, &act, NULL);
  if (result == -1) {
    fprintf (stderr, "sigaction error\n");
    return 1;
  }
  result = kill (0, SIGINT);
  if (result == -1) {
    fprintf (stderr, "kill error\n");
    return 2;
  }
  return 0;
}
