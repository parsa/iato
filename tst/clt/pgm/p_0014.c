/*
 * ---------------------------------------------------------------------------
 * - p_0014.c                                                                -
 * - iato:pgm tester : test  mmap                                            -
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
#include <sys/mman.h>

#define TEST_PROT PROT_READ   | PROT_WRITE
#define TEST_FLAG MAP_PRIVATE | MAP_ANON

int main () {
  long  pasi = 0;
  long  j    = 0;
  char  c    = 0;
  void* addr = 0;
  pasi = getpagesize ();
  addr = mmap (0, pasi, TEST_PROT, TEST_FLAG, -1, 0);
  if (addr ==  MAP_FAILED) return 1;
  for (j = 0, c = 0; j < pasi; j++) ((char*) addr)[j] = c++;
  for (j = 0, c = 0; j < pasi; j++) {
    if (((char*) addr)[j] != c++) return 1;
  }
  return 0;
}
