// ---------------------------------------------------------------------------
// - t_uvr.cpp                                                               -
// - iato:isa testers - uvr class tester                                     -
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

#include "Uvr.hpp"

int main () {
  using namespace std;
  using namespace iato;

  // test create a default uvr
  Uvr uvr;
  assert (uvr.isvalid () == false);
  uvr.setbval (false);
  assert (uvr.isvalid () == true);

  // test copy constructor
  Uvr uvr2 = uvr;
  assert (uvr2.isvalid () == true);

  // reset an uvr
  uvr2.reset();

  // test assignement operator
  uvr2 = uvr;
  assert      (uvr2.isvalid () == true);
   
  // test set and get a real value
  uvr.setrval (1.0);
  assert      (uvr.getrval () == 1.0L);

  // test set and get  an octa value
  uvr.setoval (OCTA_1 );
  assert      (uvr.getoval () == OCTA_1);
  
  // test the uvr type
  assert      (uvr.gettype () == Uvr::OBV);

  // test set and get a dual value
  uvr.setdval (OCTA_1, false);
  assert      (uvr.getbval () == false);

  // success
  return 0;
}
