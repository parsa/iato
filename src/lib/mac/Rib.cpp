// ---------------------------------------------------------------------------
// - Rib.cpp                                                                 -
// - iato:mac library - replay identification buffer class implementation    -
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

#include "Rib.hpp"
#include "Mac.hpp"
#include "Prn.hpp"

namespace iato {

  // create a new rib
  
  Rib::Rib (void) : Resource (RESOURCE_RIB) {
    d_size = RB_SIZE;
    p_rids = new Rid[d_size];
    reset ();
  }

  // create a new rib ith a context
  
  Rib::Rib (Mtx* mtx) : Resource (RESOURCE_RIB) {
    d_size = mtx->getlong ("RIB-SIZE"); assert (d_size > 0);
    p_rids = new Rid[d_size];
    reset ();
  }

  // destroy this rib

  Rib::~Rib (void) {
    delete [] p_rids;
  }

  // reset this rib

  void Rib::reset (void) {
    for (long i = 0; i < d_size; i++) {
      p_rids[i].reset ();
    }
    d_over = false;
  }

  // report this resource

  void Rib::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "replay identification buffer" << endl;
    cout << "\trib size     \t\t: " << d_size << endl;
  }

  // return true if the rib is empty

  bool Rib::isempty (void) const {
    for (long i = 0; i < d_size; i++) {
      if (p_rids[i].isvalid () == true) return false;
    }
    return true;
  }

  // check if the rib entry is valid

  bool Rib::isvalid (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_rids[index].isvalid ();
  }

  // clear the rib entry by index

  void Rib::clear (const long index) {
    assert ((index >= 0) && (index < d_size));
    p_rids[index].reset ();
  }

  // check if a rid exists in this buffer

  bool Rib::exists (const Rid& rid) const {
    // check for valid rid
    if (rid.isvalid () == false) return false;
    // loop in the buffer
    for (long i = 0; i < d_size; i++) {
      if (p_rids[i] == rid) return true;
    }
    return false;
  }

  // add a new rid in this buffer

  void Rib::add (const Rid& rid) {
    // check for overflow
    if (d_over == true) return;
    // check for valid rid
    if (rid.isvalid () == false) return;
    // check for non renamble rid
    if (rid.islrnm () == true) return;
    // check for a non present rid
    if (exists (rid) == true) return;
    // look for an empty slot
    for (long i = 0; i < d_size; i++) {
      if (p_rids[i].isvalid () == false) {
	p_rids[i] = rid;
	return;
      }
    }
    // overflow detected
    d_over = true;
  }

  // add an instruction dependency 

  void Rib::add (const Dsi& dsi) {
    // check for valid instruction
    if (dsi.isvalid () == false) return;
    // loop in the destination
    for (long i = 0; i < IA_MDST; i++) {
      Rid drid = dsi.getdnum (i);
      add (drid);
    }
  }

  // check if an instruction is dependent

  bool Rib::isdep (const Dsi& dsi) const {
    // check for valid instruction
    if (dsi.isvalid () == false) return false;
    // check the predicate
    Rid prid = dsi.getpnum ();
    if (prid.islrnm () == false) {
      if (exists (prid) == true) return true;
    }
    // check the operands
    for (long i = 0; i < IA_MSRC; i++) {
      Rid srid = dsi.getsnum (i);
      if (srid.islrnm () == false) {
	if (exists (srid) == true) return true;
      }
    }
    // no match
    return false;
  }
}
