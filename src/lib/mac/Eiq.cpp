// ---------------------------------------------------------------------------
// - Eiq.cpp                                                                 -
// - iato:mac library - expand instruction queue class implementation        -
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

#include "Eiq.hpp"
#include "Mac.hpp"
#include "Prn.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new queue
  
  Eiq::Eiq (void) : Resource (RESOURCE_EIQ) {
    d_size = IQ_SIZE;
    d_qthr = d_size - BN_IWSZ * BN_SLSZ; assert (d_qthr > 0);
    p_inst = new Dsi[d_size];
    reset ();
  }

  // create a new queue ith a context
  
  Eiq::Eiq (Mtx* mtx) : Resource (RESOURCE_EIQ) {
    d_size = mtx->getlong ("EIQ-SIZE");          assert (d_size > 0);
    d_qthr = d_size - mtx->getswsz (); assert (d_qthr > 0);
    p_inst = new Dsi[d_size];
    reset ();
  }

  // destroy this eiq

  Eiq::~Eiq (void) {
    delete [] p_inst;
  }

  // reset this eiq

  void Eiq::reset (void) {
    d_ilen = 0;
    d_iidx = 0;
    d_oidx = 0;
    for (long i = 0; i < d_size; i++) p_inst[i].reset ();
  }

  // report this resource

  void Eiq::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "expand instruction queue" << endl;
    cout << "\tqueue size   \t\t: " << d_size << endl;
    cout << "\tthreshold    \t\t: " << d_qthr << endl;
  }

  // return true if the queue is empty

  bool Eiq::isempty (void) const {
    if (d_ilen == 0) return true;
    return false;
  }

  // return true if the queue is full

  bool Eiq::isfull (void) const {
    if (d_ilen == d_size) return true;
    return false;
  }

  // return true if the queue has reached the threshold

  bool Eiq::isthr (void) const {
    if (d_ilen >= d_qthr) return true;
    return false;
  }

  // push a new instruction in the queue

  void Eiq::push (const Dsi& dsi) {
    // do not push invalid instruction
    if (dsi.isvalid () == false) return;
    // check that we are not full
    if (isfull () == true) {
      throw Exception ("eiq-error", "instruction queue is full");
    }
    // save the instruction
    p_inst[d_iidx] = dsi;
    d_iidx = ++d_iidx % d_size;
    d_ilen++;
  }

  // pop an instruction from the queue

  Dsi Eiq::pop (void) {
    Dsi result;
    // check that we are not empty
    if (isempty () == false) {
      // save result and reset slot
      result = p_inst[d_oidx];
      p_inst[d_oidx].reset ();
      d_oidx = ++d_oidx % d_size;
      d_ilen--; 
      assert (d_ilen >= 0);
    }
    return result;
  }
}
