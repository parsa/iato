// ---------------------------------------------------------------------------
// - t_huge.cpp                                                              -
// - iato:isa testers - huge class tester                                    -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
// -                                              Pierre Villalon  2002:2003 -
// -                                              Andre  Seznec    2002:2004 -
// ---------------------------------------------------------------------------
// - This program  is  free software;  you can redistribute it and/or modify -
// - it  under  the  terms  of the GNU  General Public License version 2, as -
// - published by the Free Software Foundation.                              -
// -                                                                         -
// - This  program  is  distributed  in  the  hope  that it  will be useful, -
// - but   without  any  warranty; without  even  the  implied  warranty  of -
// - merchantability or fitness for a particular purpose.                    -
// -                                                                         -
// - See the GNU General Public License version 2 for more details           -
// ---------------------------------------------------------------------------

#include "Types.hpp"

int main (int, char**) {
  using namespace std;
  using namespace iato;

  // create a default huge
  t_huge h0;
  assert (h0.getlow  () == OCTA_0);
  assert (h0.gethigh () == OCTA_0);

  // set from an octa
  t_huge h1 = OCTA_1;
  assert (h1.getlow  () == OCTA_1);
  assert (h1.gethigh () == OCTA_0);

  // test for add
  t_huge h2 = h1 + h0;
  assert (h2.getlow  () == OCTA_1);
  assert (h2.gethigh () == OCTA_0);

  // check for add
  h0 = 1024;
  h1 = 2048;
  h2 = h0 + h1;
  assert (h2.getlow  () == 3072);
  assert (h2.gethigh () == OCTA_0);

  // check for mul
  h0 = OCTA_0;
  h1 = OCTA_1;
  h2 = h0 * h1;
  assert (h2.getlow  () == OCTA_0);
  assert (h2.gethigh () == OCTA_0);

  // check for mul
  h0 = OCTA_1;
  h1 = OCTA_1;
  h2 = h0 * h1;
  assert (h2.getlow  () == OCTA_1);
  assert (h2.gethigh () == OCTA_0);

  // check for mul
  h0 = 1024;
  h1 = 1024;
  h2 = h0 * h1;
  assert (h2.getlow  () == 1048576);
  assert (h2.gethigh () == OCTA_0);
  
  // success
  return 0;
}
