// ---------------------------------------------------------------------------
// - Bpe.cpp                                                                 -
// - iato:mac library - bypass network element class implementation          -
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
#include "Bpe.hpp"

namespace iato {
  
  // the bypass group
  struct t_bpg {
    // the value rid
    Rid d_rid;
    // the uvr value
    Uvr d_uvr;
    // reset the group
    void reset (void) {
      d_rid.reset ();
      d_uvr.reset ();
    }
  };

  // create a default bypass element

  Bpe::Bpe (void) : Resource (RESOURCE_BNE) {
    d_size = IA_MDST;
    p_vbpg = new t_bpg[d_size];
    reset ();
  }

  // create a bypass element by context

  Bpe::Bpe (Mtx* mtx) : Resource (RESOURCE_BNE) {
    d_size = IA_MDST;
    p_vbpg = new t_bpg[d_size];
    reset ();
  }

  // create a bypass element by context and name

  Bpe::Bpe (Mtx* mtx, const string& name) : Resource (name) {
    d_size = IA_MDST;
    p_vbpg = new t_bpg[d_size];
    reset ();
  }

  // destroy this bypass network

  Bpe::~Bpe (void) {
    delete [] p_vbpg;
  }

  // reset this bypass network

  void Bpe::reset (void) {
    for (long i = 0; i < d_size; i++) p_vbpg[i].reset ();
  }

  // report this resource

  void Bpe::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "bypass element" << endl;
    cout << "\telement size \t\t: " << d_size << endl;
  }

  // set a bypass value by index, rid and value

  void Bpe::setuvr (const long index, const Rid& rid, const Uvr& uvr) {
    assert ((index >= 0) && (index < d_size));
    p_vbpg[index].d_rid = rid;
    p_vbpg[index].d_uvr = uvr;
  }

  // find the index of a matching bypass element

  long Bpe::find (const Rid& rid) const {
    for (long  i = 0; i < d_size; i++) {
      if (p_vbpg[i].d_rid == rid) return i;
    }
    return -1;
  }

  // get the rid associated with a bypass element

  Rid Bpe::getrid (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vbpg[index].d_rid;
  }

  // get the uvr associated with a bypass element

  Uvr Bpe::getuvr (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vbpg[index].d_uvr;
  }

  // update the bpe with a result

  void Bpe::update (const Result& resl) {
    if (resl.isvalid () == false) return;
    for (long i = 0; i < IA_MDST; i++) {
      if (resl.getrop (i) == Result::REG_UPD) {
	Rid rid = resl.getrid (i);
	if (rid.isvalid () == false) continue;
	if (rid.islrnm  () == false) continue;
	Uvr uvr = resl.getuvr (i);
	setuvr (i, rid, uvr);
      } else {
	p_vbpg[i].reset ();
      }
    }
  }

  // clear a bpe by rid

  void Bpe::clear (const Rid& rid) {
    // try to find the rid
    if (rid.isvalid () == false) return;
    long index = find (rid);
    if (index == -1) return;
    // clear the bpe
    p_vbpg[index].reset ();
  }
}
