// ---------------------------------------------------------------------------
// - t_umr.cpp                                                               -
// - iato:isa testers - umr class tester                                     -
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

#include "Umr.hpp"

int main (int, char**) {
  using namespace iato;

  // create a new umr
  Umr umr;
  assert (umr.getumr () == 0x00);
 
  // check each bit
  umr.setfld (Umr::BE, true);
  assert (umr.getumr () == 0x02);
  umr.setfld (Umr::UP, true);
  assert (umr.getumr () == 0x06);
  umr.setfld (Umr::AC, true);
  assert (umr.getumr () == 0x0e);
  umr.setfld (Umr::MFL, true);
  assert (umr.getumr () == 0x1e);
  umr.setfld (Umr::MFH, true);
  assert (umr.getumr () == 0x3e);
 
  // success
  return 0;
}
