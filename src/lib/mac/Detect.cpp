// ---------------------------------------------------------------------------
// - Detect.cpp                                                              -
// - iato:mac library - speculative detection logic class implementation     -
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

#include "Prn.hpp"
#include "Detect.hpp"

namespace iato {

  // create a default detection logic

  Detect::Detect (void) : Resource (RESOURCE_DTL) {
    p_rbk = 0;
  }

  // create a speculation logic by context

  Detect::Detect (Mtx* mtx) : Resource (RESOURCE_DTL) {
    p_rbk = 0;
  }
  
  // create a speculation logic by context and name

  Detect::Detect (Mtx* mtx, const string& name) : Resource (name) {
    p_rbk = 0;
  }
 
  // reset this detection logic

  void Detect::reset (void) {
  }

  // report some resource information

  void Detect::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: detection logic" << endl;
  }

  // check the sip speculation

  bool Detect::chksip (const Ssi& ssi, const Result& resl) const {
    // if the instruction is not valid, the verification is ok
    if (ssi.isvalid () == false) return true;
    // check if we are speculating
    if (ssi.getsfl () == false) return true;
    // if the result is not valid, the verification is not ok since
    // the instruction might have been cancelled
    if (resl.isvalid () == false) return false;
    // here we are speculating - get the instruction speculative ip
    // and check if the result is the same. If the result does not
    // have an IP register, we assume the worse and return false
    if (resl.isreg (IPRG) == false) return false;
    // compare speculative ip with real one
    t_octa sip = ssi.getsip  ();
    t_octa rip = resl.getrip ();
    return (sip == rip);
  }

  // check for valid predicate speculation

  bool Detect::chkspp (const Ssi& ssi) const {
    assert (p_rbk);
    // if the instruction is not valid, the test succeeds
    if (ssi.isvalid () == false) return true;
    // check if we were speculating
    if (ssi.getppfl () == false) return true;
    // get the predicate rid and value
    Rid  prid = ssi.getpnum (); assert (prid.isvalid () == true);
    bool pval = p_rbk->getbval (prid);
    // get the instruction cancel flag and check
    bool sval = !ssi.getcnlf ();
    return (pval == sval);
  }

  // bind the register bank

  void Detect::bind (Register* rbk) {
    p_rbk = rbk;
  }
}
