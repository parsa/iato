// ---------------------------------------------------------------------------
// - Eiq.hpp                                                                 -
// - iato:mac library - expand instruction queue class definition            -
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

#ifndef  IATO_EIQ_HPP
#define  IATO_EIQ_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Eiq is the expand instruction queue. The queue operates like
  /// a fifo. The 'push' method push an instruction in the queue, while
  /// the 'pop' method remove an instruction from the queue. If the queue
  /// is empty, an invalid instruction is returned. If the queue is full
  /// the 'push' method throw an exception.

  class Eiq : public Resource {
  private:
    /// the queue size
    long d_size;
    /// the queue threshold
    long d_qthr;
    /// the instruction buffer
    Dsi* p_inst;
    /// the queue length
    long d_ilen;
    /// the input index
    long d_iidx;
    /// the output index
    long d_oidx;

  public:
    /// create a new queue
    Eiq (void);

    /// create a new queue with a context
    /// @param mtx the architectural context
    Eiq (Mtx* mtx);

    /// destroy this queue
    ~Eiq (void);

    /// reset this queue
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the queue is empty
    bool isempty (void) const;

    /// @return true if the queue is full
    bool isfull (void) const;

    /// @return true if the queue has reached the threshold
    bool isthr (void) const;

    /// push an instruction in the queue
    /// @param dsi the instruction to push
    void push (const Dsi& dsi);

    /// @return the oldest instruction from the queue
    Dsi pop (void);

  private:
    // make the copy constructor private
    Eiq (const Eiq&);
    // make the assignment operator private
    Eiq& operator = (const Eiq&);
  };
}

#endif
