// ---------------------------------------------------------------------------
// - Urb.cpp                                                                 -
// - iato:mac library - universal register bank class implementation         -
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

#include "Mac.hpp"
#include "Prn.hpp"
#include "Urb.hpp"

namespace iato {
  
  // the urb entry
  struct t_urb {
    // the valid bit
    bool d_vbit;
    // the uvr value
    Uvr  d_uvr;
    // default constructor
    t_urb (void) {
      reset ();
    }
    // reset the urb entry
    void reset (void) {
      d_vbit = false;
      d_uvr.reset ();
    }
    // allocate a new urb entry
    bool alloc (void) {
      if (d_vbit == true) return false;
      d_uvr.reset ();
      return true;
    }
  };

  // create a default universal bank

  Urb::Urb (void) : Resource (RESOURCE_URB) {
    d_size = UR_PRSZ;
    p_vurb = new t_urb[d_size];
    reset ();
  }

  // create a universal bank by context

  Urb::Urb (Mtx* mtx) : Resource (RESOURCE_URB) {
    d_size = mtx->getlong ("PR-UR-SIZE"); assert (d_size > 0);
    p_vurb = new t_urb[d_size];
    reset ();
  }

  // create a universal bank by context and name

  Urb::Urb (Mtx* mtx, const string& name) : Resource (name) {
    d_size = mtx->getlong ("PR-UR-SIZE"); assert (d_size > 0);
    p_vurb = new t_urb[d_size];
    reset ();
  }

  // destroy this universal bank

  Urb::~Urb (void) {
    delete [] p_vurb;
  }

  // reset this universal bank

  void Urb::reset (void) {
    for (long i = 0; i < d_size; i++) p_vurb[i].reset ();
  }

  // report this resource

  void Urb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "universal register bank" << endl;
    cout << "\tbank size    \t\t: " << d_size << endl;
  }

  // set the urb value by uvr

  void Urb::setuvr (const long rnum, const Uvr& uvr) {
    assert ((rnum >= 0) && (rnum < d_size));
    assert (p_vurb[rnum].d_vbit == false);
    p_vurb[rnum].d_vbit = true;
    p_vurb[rnum].d_uvr  = uvr;
  }

  // return the uvr value by index

  Uvr Urb::getuvr (const long rnum) const {
    assert ((rnum >= 0) && (rnum < d_size));
    assert (p_vurb[rnum].d_vbit == true);
    return p_vurb[rnum].d_uvr;
  }

  // allocate a new urb and return its index

  long Urb::alloc (void) {
    for (long i = 0; i < d_size; i++) {
      if (p_vurb[i].alloc () == true) return i;
    }
    assert (false);
    return -1;
  }

  // clean an urb entry

  void Urb::clean (const long rnum) {
    assert ((rnum >= 0) && (rnum < d_size));
    p_vurb[rnum].reset ();
  }
}
