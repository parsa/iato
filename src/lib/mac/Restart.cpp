// ---------------------------------------------------------------------------
// - Restart.cpp                                                             -
// - iato:mac library - pipeline flush and restart class implementation      -
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
#include "Env.hpp"
#include "Restart.hpp"
#include "Exception.hpp"

namespace iato {

  // create a default restart resource
  
  Restart::Restart (void) : Resource (RESOURCE_PFR) {
    p_rbk  = 0;
    p_pipe = 0;
    reset ();
  }

  // create a new restart with a context
  
  Restart::Restart (Mtx* mtx) : Resource (RESOURCE_PFR) {
    p_rbk  = 0;
    p_pipe = 0;
    reset ();
  }

  // reset this restart

  void Restart::reset (void) {
    d_ipfr = false;
    d_srlz = false;
    d_rip  = OCTA_0;
    d_slot = 0;
  }

  // report this resource

  void Restart::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "pipeline flush and restart" << endl;
  }

  // bind the pipeline with this resource

  void Restart::bind (Runnable* pipe, Env* env) {
    // bind the pipeline
    p_pipe = pipe;
    if (!p_pipe) {
      string msg = "cannot bind pipeline for restart ";
      throw Exception ("bind-error", msg);
    }
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank for restart ";
      throw Exception ("bind-error", msg);
    }
  }

  // @return the serialization restart flag

  bool Restart::getsrlz (void) const {
    if (d_ipfr == false) return false;
    return d_srlz;
  }

  // check if an instruction is valid

  bool Restart::isvalid (const t_octa iip, const long slot) const {
    if (d_ipfr == false) return true;
    if ((iip == d_rip) && (slot < d_slot)) return false;
    return true;
  }

  // check if an instruction must be cancelled, return false if not valid

  bool Restart::check (const t_octa iip, const long slot) {
    if (d_ipfr == false) return true;
    if ((iip == d_rip) && (slot <  d_slot)) return false;
    if ((iip == d_rip) && (slot == d_slot)) reset ();
    return true;
  }

  // flush the pipe by default

  void Restart::pfdef (void) {
    // reset everything
    reset ();
    // flush the resources
    flush ();
  }
    
  // update the restart with a local condition

  void Restart::pflcl (void) {
    // reset everything
    reset ();
    // grab the ip from the register bank
    d_ipfr = true;
    d_rip  = p_rbk->getoval (IPRG,0);
    d_slot = 0;
    // flush the resources
    flush ();
  }

  // update the restart with the current ip

  void Restart::pfstd (const t_octa ip, const long slot) {
    // reset everything
    reset ();
    d_ipfr = true;
    d_rip  = ip;
    d_slot = slot;
    // flush the resources
    flush ();
    // update the register bank
    p_rbk->write (IPRG, 0, d_rip);
  }

  // update the restart with a serialization condition

  void Restart::pfsrl (const t_octa ip, const long slot) {
    pfstd (ip, slot);
    d_srlz = true;
  }

  // update the restart with a next condition

  void Restart::pfnxt (const t_octa ip, const long slot) {
    // reset everything
    reset ();
    // if the offending slot is 2, the restart condition is
    // the next ip at slot 0
    d_ipfr = true;
    d_rip  = (slot == 2) ? ip + BN_BYSZ : ip;
    d_slot = (slot == 2) ? 0 : slot + 1;
    // flush the resources
    flush ();
    // update the register bank
    p_rbk->write (IPRG, 0, d_rip);
  }
}
