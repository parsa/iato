// ---------------------------------------------------------------------------
// - t_ip.cpp                                                                -
// - iato:isa testers - ip class tester                                      -
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

#include "Ip.hpp"

int main (int, char**) {
  using namespace iato;

  // create a new ip
  Ip ip;
  assert (ip.getip () == 0x0000000000000000ULL);
 
  // set, check and reset
  ip.setip (0x1234567891234567LL);
  assert   (ip.getip () == 0x1234567891234567ULL);
  ip.reset ();
  assert   (ip.getip () == 0x0000000000000000ULL);

  // success
  return 0;
}
