// ---------------------------------------------------------------------------
// - t_fpsr.cpp                                                              -
// - iato:isa testers - fpsr class tester                                    -
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

#include "Isa.hpp"
#include "Fpsr.hpp"

int main (int, char**) {
  using namespace std;
  using namespace iato;
  
  // constatnt
  const t_octa val = 0x1234567890abcdefULL;

  // test constructor
  Fpsr d_fpsr;
  assert (d_fpsr.getfpsr() == DEF_FPSR);
  Fpsr d_fpsr2 (val);
  assert (d_fpsr2.getfpsr() == val);

  // test fiel setting
  d_fpsr.setfld (Fpsr::TRAPS, Fpsr::VD, true);
  d_fpsr.setfld (Fpsr::TRAPS, Fpsr::DD, true);
  assert (d_fpsr.getbfld (Fpsr::TRAPS, Fpsr::VD) == true);
  assert (d_fpsr.getbfld (Fpsr::TRAPS, Fpsr::DD) == true);

  d_fpsr.setfld (Fpsr::SF1, Fpsr::V, true);
  d_fpsr.setfld (Fpsr::SF0, Fpsr::PC, (t_byte) 0x02);
  d_fpsr.setfld (Fpsr::SF3, Fpsr::RC, (t_byte) 0x01);
  assert (d_fpsr.getbfld (Fpsr::SF1, Fpsr::V) == true);
  assert (d_fpsr.getbsfld (Fpsr::SF0, Fpsr::PC) == (t_byte) 0x02);
  assert (d_fpsr.getbsfld (Fpsr::SF3, Fpsr::RC) == (t_byte) 0x01);

  // success
  return 0;
}
