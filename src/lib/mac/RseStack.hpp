// ---------------------------------------------------------------------------
// - RseStack.hpp                                                            -
// - iato:mac library - rse stack class definition                           -
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

#ifndef  IATO_RSESTACK_HPP
#define  IATO_RSESTACK_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

namespace iato {
  using namespace std;

  /// The RseStack class implements a finite stack of rse state. When a
  /// rse dependant branch is made, the rse state is placed on the stack
  /// or reterived from it. The rse state contains mostly the cfm register
  /// that defines the state of the renaming circuitry. The rse stack is an
  /// essential piece of code that permits to operate with a branch predictor.

  class RseStack : public Resource {
  private:
    /// the stack size
    long d_size;
    /// number of entries
    long d_rlen;
    /// rse state data
    Rse::State* p_rstk;

  public:
    /// create a default rse stack
    RseStack (void);

    /// create a new rse stack with a context
    /// @param mtx the architectural context
    RseStack (Mtx* mtx);

    /// destroy this rse stack
    ~RseStack (void);

    /// reset this rse stack
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the stack is empty
    bool isempty (void) const;

    /// @return true if the stack is full
    bool isfull (void) const;

    /// push a new rse state on the stack
    /// @param state the state to push
    void push (const Rse::State& state);

    /// @return the latest irb and clean rob
    Rse::State pop (void);

    /// shift the stack by one element
    void shift (void);

  private:
    // make the copy constructor private
    RseStack (const RseStack&);
    // make the assignment operator private
    RseStack& operator = (const RseStack&);
  };
}

#endif
