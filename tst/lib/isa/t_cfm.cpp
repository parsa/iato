// ---------------------------------------------------------------------------
// - t_cfm.cpp                                                               -
// - iato:isa testers - real class tester                                    -
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
#include "Cfm.hpp"

int main (int, char**) {
  using namespace std;
  using namespace iato;


  // test create a default cfm
  Cfm cfm;
  assert (cfm.getcfm () == DEF_CFM);

  // test create a cfm with a value
  Cfm cfm2 (OCTA_0);
  assert (cfm2.getcfm () == OCTA_0);

  // test copy contruct 
  Cfm cfm3 = cfm;
  assert (cfm3.getcfm () == DEF_CFM);

  // test assign
  cfm3 = cfm2;
  assert (cfm3.getcfm () == OCTA_0);

  // test equality
  assert (cfm3 == cfm2);
  assert (cfm2 != cfm);
  
  assert (cfm  == DEF_CFM); 
  assert (cfm2 != DEF_CFM);
  
  // test reset 
  cfm2.reset ();
  assert (cfm2.getcfm () == DEF_CFM);

  // test set the cfm value
  cfm2.setcfm (OCTA_0);
  assert (cfm2.getcfm () == OCTA_0);
  
  // test rotate this cfm by decrementing each register rename register base
  // modulo the rotating region
  cfm.rotate ();
  
  // set and check rrb fields values
  cfm.setrrb (Cfm :: RGR, 0);
  assert (cfm.getrrb (Cfm :: RGR) == 0);
  cfm.setrrb(Cfm :: RFR, 0);
  assert (cfm.getrrb (Cfm :: RFR) == 0);
  cfm.setrrb(Cfm :: RPR, 0);
  assert (cfm.getrrb (Cfm :: RPR) == 0);
  
  // success
  return 0;
}
