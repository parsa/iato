// ---------------------------------------------------------------------------
// - t_rid.cpp                                                               -
// - iato:isa testers - rid class tester                                     -
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

#include "Rid.hpp"

int main () {
  using namespace std;
  using namespace iato;
  
  // test create a new register number
  Rid rid;
  assert (rid.isready () == true);
  
  // test copy constructor
  Rid cri = rid;
  assert (cri.isready () == true);
  
  // perform several tests
  rid.seterdy (true);
  rid.setreg  (GREG, 1, 1);
  assert      (rid.tostring () == "type = GREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (NREG, 1, 1);
  assert      (rid.tostring () == "type = NREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (FREG, 1, 1);
  assert      (rid.tostring () == "type = FREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (PREG, 1, 1);
  assert      (rid.tostring () == "type = PREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (BREG, 1, 1);
  assert      (rid.tostring () == "type = BREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (AREG, 1, 1);
  assert      (rid.tostring () == "type = AREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (IPRG, 1, 1);
  assert      (rid.tostring () == "type = IREG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (CFMR, 1, 1);
  assert      (rid.tostring () == "type = CFMR lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (PRRG, 1, 1);
  assert      (rid.tostring () == "type = PRRG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (PROT, 1, 1);
  assert      (rid.tostring () == "type = PROT lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (UMRG, 1 ,1);
  assert      (rid.tostring () == "type = UMRG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (PSRG, 1, 1);
  assert      (rid.tostring () == "type = PSRG lnum = 1 pnum = 1 ");
  
  rid.reset   ();
  rid.seterdy (true);
  rid.setreg  (CREG, 1, 1);
  assert      (rid.tostring () == "type = CREG lnum = 1 pnum = 1 ");
    
  // success
  return 0;
}
