// ---------------------------------------------------------------------------
// - Mbn.cpp                                                                 -
// - iato:mac library - memory bypass network class implementation           -
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

#include "Prn.hpp"
#include "Mbn.hpp"

namespace iato {

  // create an empty bypass
  
  Mbn::Mbn (void) : Resource (RESOURCE_MBN) {
    reset ();
  }

  // create an empty bypass by context
  
  Mbn::Mbn (Mtx* mtx) : Resource (RESOURCE_MBN) {
    reset ();
  }

  // create an empty bypass by context and name
  
  Mbn::Mbn (Mtx* mtx, const string& name) : Resource (name) {
    reset ();
  }

  // reset this bypass network

  void Mbn::reset (void) {
    long size = d_vmbe.size ();
    for (long i = 0; i < size; i++) d_vmbe[i]->reset ();
  }

  // report this resource

  void Mbn::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "memory bypass network" << endl;
  }

  // add a bypass element

  void Mbn::add (Mbe* mbe) {
    if (mbe) d_vmbe.push_back (mbe);
  }

  // update an mrt with the bypass data

  Mrt Mbn::update (const Mrt& mrt) const {
    Mrt result;
    // check for valid mrt
    if (mrt.isvalid () == false) return result;
    // check for load to update
    if (mrt.isload () == false) return result;
    // get the load mrt mask and address
    t_octa lmsk = mrt.getmask ();
    t_octa ladr = mrt.getaddr () & lmsk;
    // get the network size and iterate
    long size = d_vmbe.size ();
    for (long i = 0; i < size; i++) {
      // the index is valid, get the bypass mrt
      Mrt bmrt = d_vmbe[i]->getmrt ();
      if (bmrt.isvalid () == false) continue;
      // get the bypass mrt address and check
      t_octa addr = bmrt.getaddr () & lmsk;
      if (addr == ladr) {
	result = mrt;
	result.setmv (bmrt);
	return result;
      }
    }
    return result;
  }

  // update an mrt with the bypass data and index

  Mrt Mbn::update (const Mrt& mrt, const long mix) const {
    Mrt result;
    // check for valid mrt
    if (mrt.isvalid () == false) return result;
    // check for load to update
    if (mrt.isload () == false) return result;
    // get the load mrt mask and address
    t_octa lmsk = mrt.getmask ();
    t_octa ladr = mrt.getaddr () & lmsk;
    // get the network size and iterate
    long size = d_vmbe.size ();
    for (long i = 0; i < size; i++) {
      // get the ordering flag and check for index
      long eix = d_vmbe[i]->geteix ();
      if ((eix == -1) || (eix >= mix)) continue;
      // the index is valid, get the bypass mrt
      Mrt bmrt = d_vmbe[i]->getmrt ();
      if (bmrt.isvalid () == false) continue;
      // get the bypass mrt address and check
      t_octa addr = bmrt.getaddr () & lmsk;
      if (addr == ladr) {
	result = mrt;
	result.setmv (bmrt);
	return result;
      }
    }
    return result;
  }
}
