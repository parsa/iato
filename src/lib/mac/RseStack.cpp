// ---------------------------------------------------------------------------
// - RseStack.cpp                                                            -
// - iato:mac library - rse stack class implementation                       -
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
#include "RseStack.hpp"

namespace iato {
  using namespace std;

  // create a new rse stack
  
  RseStack::RseStack (void) : Resource (RESOURCE_RTK) {
    d_size = BP_RTKS;
    p_rstk = new Rse::State[d_size];
    reset ();
  }

  // create a new rse stack with a context
  
  RseStack::RseStack (Mtx* mtx) : Resource (RESOURCE_RTK) {
    d_size = mtx->getlong ("RTK-SIZE"); assert (d_size > 0);
    p_rstk = new Rse::State[d_size];
    reset ();
  }

  // destroy this rse stack

  RseStack::~RseStack (void) {
    delete [] p_rstk;
  }

  // reset this rse stack

  void RseStack::reset (void) {
    for (long i = 0; i < d_size; i++) p_rstk[i].reset ();
    d_rlen = 0;
  }

  // report this resource

  void RseStack::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "rse stack buffer" << endl;
    cout << "\tstack size   \t\t: " << d_size << endl;
  }

  // return true if the stack is empty

  bool RseStack::isempty (void) const {
    return (d_rlen == 0) ? true : false;
  }

  // return true if the stack is full

  bool RseStack::isfull (void) const {
    return (d_rlen >= d_size) ? true : false;
  }

  // push a new state in the stack

  void RseStack::push (const Rse::State& state) {
    // shift first the stack if full
    if (isfull () == true) return;
    // add the element
    p_rstk[d_rlen++] = state;
  }

  // pop the oldest state

  Rse::State RseStack::pop (void) {
    // prepare the oldest state
    Rse::State state;
    // select between empty or last one
    if (isempty () == true) {
      state.reset ();
    } else {
      state = p_rstk[--d_rlen];
    }
    // return the oldest state
    return state;
  }
}
