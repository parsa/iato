// ---------------------------------------------------------------------------
// - Irb.cpp                                                                 -
// - iato:mac library - instruction result buffer class implementation       -
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

#include "Irb.hpp"
#include "Mac.hpp"
#include "Prn.hpp"

namespace iato {

  // create a new irb
  
  Irb::Irb (void) : Resource (RESOURCE_IRB) {
    d_size = IR_SIZE;
    p_inst = new Dsi[d_size];
    p_resl = new Result[d_size];
    reset ();
  }

  // create a new irb ith a context
  
  Irb::Irb (Mtx* mtx) : Resource (RESOURCE_IRB) {
    d_size = mtx->getlong ("IRB-SIZE"); assert (d_size > 0);
    p_inst = new Dsi[d_size];
    p_resl = new Result[d_size];
    reset ();
  }

  // destroy this irb

  Irb::~Irb (void) {
    delete [] p_inst;
    delete [] p_resl;
  }

  // reset this irb

  void Irb::reset (void) {
    for (long i = 0; i < d_size; i++) {
      p_inst[i].reset ();
      p_resl[i].reset ();
    }
  }

  // report this resource

  void Irb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "instruction result buffer" << endl;
    cout << "\tirb size     \t\t: " << d_size << endl;
  }

  // return true if the irb is empty

  bool Irb::isempty (void) const {
    for (long i = 0; i < d_size; i++) {
      if (p_inst[i].isvalid () == true) return false;
    }
    return true;
  }

  // check if the irb entry is valid

  bool Irb::isvalid (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_inst[index].isvalid ();
  }

  // clear the irb entry by index

  void Irb::clear (const long index) {
    assert ((index >= 0) && (index < d_size));
    p_inst[index].reset ();
    p_resl[index].reset ();
  }
  
  // allocate a new irb entry

  long Irb::alloc (const Dsi& inst, const Result& resl) {
    // find a new entry
    for (long i = 0; i < d_size; i++) {
      if (p_inst[i].isvalid () == false) {
	p_inst[i] = inst;
	p_resl[i] = resl;
	return i;
      }
    }
    return -1;
  }

  // get the irb intruction by index

  Dsi Irb::getinst (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_inst[index];
  }

  // get the irb result by index

  Result Irb::getresl (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_resl[index];
  }  
}
