/*
 * ---------------------------------------------------------------------------
 * - p_0009.c                                                                -
 * - iato:pgm tester : sscanf test module                                    -
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
#include <stdarg.h>
#include <stdlib.h>

static int check_varags (const long lt, const long lh, ...) {
  va_list arg;
  long    t = 0, h = 0;
  va_start (arg, lh);
  t = va_arg (arg, long); 
  if (t != lt) return 0;
  h = va_arg (arg, long);
  if (h != lh) return 0;
  va_end (arg);
  return 1;
}

static int check_strtol (const char* data, const long val) {
  long result = strtol (data, NULL, 10);
  if (result != val) {
    fprintf (stdout, "strtol failure, result = %ld\n", result);
    return 0;
  }
  return 1;
}

static int check_sscanf (const char* data, const long lt, const long lh) {
  long t = 0, h = 0;
  if (sscanf (data, "%ld %ld", &t, &h) != 2) {
    fprintf (stdout, "sscanf argument count failure\n");
    return 0;
  }
  if (t != lt) {
    fprintf (stdout, "sscanf argument failure, t = %ld\n", t);
    return 0;
  }
  if (h != lh) {
    fprintf (stdout, "sscanf argument failure, h = %ld\n", h);
    return 0;
  }
  return 1;
}

int main (int argc, char** argv) {
  if (check_varags (1,2,1,2) == 0) {
    fprintf (stdout, "varags failure\n");
    return 1;
  } else {
    fprintf (stdout, "varags success\n");
  }
  if (check_strtol ("1", 1) == 0) {
    fprintf (stdout, "strtol failure\n");
    return 1;
  } else {
    fprintf (stdout, "strtol success\n");
  }
  if (check_sscanf ("1 2",1,2) == 0) {
    fprintf (stdout, "sscanf failure\n");
    return 1;
  } else {
    fprintf (stdout, "sscanf success\n");
  }
  return 0;
}
