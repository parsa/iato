// ---------------------------------------------------------------------------
// - t_bundle.cpp                                                            -
// - iato:isa testers - bundle class tester                                  -
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

#include "Bundle.hpp"

int main () {
  using namespace std;
  using namespace iato;
  
  // test create a default bundle 
  Bundle bundle;
  assert (bundle.length() == 0);
  
  bundle.setsip (OCTA_1, 0);
  assert (bundle.getsip () == OCTA_1);

  {
      // test copy construct a bundle
      Bundle bundle2 = bundle ;
      assert (bundle2.getsip() == OCTA_1);

      // test destroy this bundle
  }

  {
      // test assign a bundle 
      Bundle bundle3;
      bundle3 = bundle;
      assert (bundle3.getsip () == OCTA_1);
      
      // test reset a bundle
      bundle3.reset();
      assert (bundle3.getsip() == OCTA_0);
  }

  // test push a byte in this bundle field
  for (size_t i = 0; i < 16; ++i)
  {
      bundle.push (0x01U);
  }

  // check bundle content
  assert (bundle.length ()   == 16);
  assert (bundle.isvalid ()  == true);
  assert (bundle.gettmpl ()  == 0x01U);
  assert (bundle.tostring () == "[MII]");
  assert (bundle.repr ()     ==  "MII| ");

  // success
  return 0;
}
