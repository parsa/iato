// ---------------------------------------------------------------------------
// - Mpr.cpp                                                                 -
// - iato:mac library - memory port request class implementation             -
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
#include "Mpr.hpp"
#include "Interrupt.hpp"

namespace iato {

  // create a default port request

  Mpr::Mpr (void) : Resource (RESOURCE_MPR) {
    p_mta = 0;    
    reset ();
  }

  // create a new port request

  Mpr::Mpr (Mtx* mtx) : Resource (RESOURCE_MPR) {
    p_mta = 0;
    reset ();
  }

  // create a new requestor by context, type and name

  Mpr::Mpr (Mtx* mtx, const string& name) : Resource (name) {
    p_mta = 0;
    reset ();
  }
  
  // reset this requestor

  void Mpr::reset (void) {
    d_busy = false;
    d_tack = false;
    d_mrt.reset ();
  }

  // flush this requestor

  void Mpr::flush (void) {
    reset ();
  }

  // report this resource

  void Mpr::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "memory port request" << endl;
  }

  // return the busy bit

  bool Mpr::isbusy (void) const {
    return d_busy;
  }

  // return the ackwoledge bit

  bool Mpr::istack (void) const {
    return d_tack;
  }

  // return the memory request type

  Mrt Mpr::getmrt (void) const {
    return d_mrt;
  }

  // return a transaction bundle by index

  Bundle Mpr::getbndl (const long index) const {
    Bundle result;
    if (p_mta) result = p_mta->getbndl (index);
    return result;
  }

  // issue a port request by mrt

  void Mpr::request (const Mrt& mrt) {
    // save request and mark busy
    d_mrt = mrt;
    d_busy = true;
    d_tack = false;
    // check for immediate reply
    p_mta->process (d_mrt);
    d_busy = false;
    d_tack = true;
  }

  // issue a port load request by mrt

  void Mpr::preset (const Mrt& mrt) {
    // check for valid mrt
    if (mrt.isload () == false) return;
    // save request and mark busy
    d_mrt = mrt;
    d_busy = true;
    d_tack = false;
    // check for immediate reply
    p_mta->process (d_mrt);
    d_busy = false;
    d_tack = true;
  }

  // issue a port update request by mrt

  void Mpr::update(const Mrt& mrt) {
    // check for valid mrt
    if (mrt.isstore () == false) return;
    // save request and mark busy
    d_mrt = mrt;
    d_busy = true;
    d_tack = false;
    // check for immediate reply
    p_mta->process (d_mrt);
    d_busy = false;
    d_tack = true;
  }

  // bind the port request

  void Mpr::bind (Mta* mta) {
    p_mta = mta;
  }
}
