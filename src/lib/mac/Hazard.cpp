// ---------------------------------------------------------------------------
// - Hazard.cpp                                                              -
// - iato:mac library - hazard detection logic class implementation          -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#include "Mac.hpp"
#include "Prn.hpp"
#include "Hazard.hpp"

namespace iato {

  // create a default hazard detection logic

  Hazard::Hazard (void) : Resource (RESOURCE_HDL) {
    p_spb = 0;
    p_psb = 0;
    reset ();
  }

  // create a hazard detection logic by name

  Hazard::Hazard (const string& name) : Resource (name) {
    p_spb = 0;
    p_psb = 0;
    reset ();
  }

  // create a hazard detection logic by context

  Hazard::Hazard (Mtx* mtx) : Resource (RESOURCE_HDL) {
    p_spb = 0;
    p_psb = 0;
    reset ();
  }
  
  // create a hazard detection logic by context and name

  Hazard::Hazard (Mtx* mtx, const string& name) : Resource (name) {
    p_spb = 0;
    p_psb = 0;
    reset ();
  }

  // reset this dispersal engine

  void Hazard::reset (void) {
    if (p_spb) p_spb->reset ();
    if (p_psb) p_psb->reset ();
  }

  // report some resource information

  void Hazard::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: hazard detectino logic" << endl;
    cout << "\thazard type\t\t: default" << endl;
  }

  // return true if there is a hazard

  bool Hazard::ishazard (void) const {
    // check for resources
    if (!p_spb) return false;
    if (!p_psb) return false;
    // check the M unit
    long msiz = p_spb->getsize (MUNIT);
    for (long i = 0; i < msiz; i++) {
      Ssi ssi = p_spb->getinst (MUNIT, i);
      if (p_psb->ishazard (ssi) == true) return true;
    }    
    // check the I unit
    long isiz = p_spb->getsize (IUNIT);
    for (long i = 0; i < isiz; i++) {
      Ssi ssi = p_spb->getinst (IUNIT, i);
      if (p_psb->ishazard (ssi) == true) return true;
    }
    // check the F unit
    long fsiz = p_spb->getsize (FUNIT);
    for (long i = 0; i < fsiz; i++) {
      Ssi ssi = p_spb->getinst (FUNIT, i);
      if (p_psb->ishazard (ssi) == true) return true;
    }
    // check the B unit
    long bsiz = p_spb->getsize (BUNIT);
    for (long i = 0; i < bsiz; i++) {
      Ssi ssi = p_spb->getinst (BUNIT, i);
      if (p_psb->ishazard (ssi) == true) return true;
    }
    // no hazard
    return false;
  }

  // bind the hazard resource
  
  void Hazard::bind (Spb* spb, Scoreboard* psb) {
    p_spb = spb;
    p_psb = psb;
  } 
}
