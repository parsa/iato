// ---------------------------------------------------------------------------
// - Ipb.hpp                                                                 -
// - iato:mac library - issue port buffer class definition                   -
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

#ifndef  IATO_IPB_HPP
#define  IATO_IPB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SLOT_HPP
#include "Slot.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Ipb class is the issue port buffer. It is a set of ports that are
  /// grouped into a family set. At construction, the context determines
  /// how many slots can be held in the buffer. Each micro-pipeline is
  /// responsible to add the required slot. The Ipb is used by an in-order
  /// simulation engine.

  class Ipb : public Resource {
  private:
    /// the M buffer size
    long d_mbsz;
    /// the M slot buffer
    Slot** p_mbuf;
    /// the I buffer size
    long   d_ibsz;
    /// the I slot buffer
    Slot** p_ibuf;
    /// the F buffer size
    long   d_fbsz;
    /// the F slot buffer
    Slot** p_fbuf;
    /// the B buffer size
    long   d_bbsz;
    /// the B slot buffer
    Slot** p_bbuf;

  public:
    /// create a new buffer with a context
    /// @param mtx the architectural context
    Ipb (Mtx* mtx);

    /// create a new buffer with a context and a name
    /// @param mtx the architectural context
    /// @param name the buffer name
    Ipb (Mtx* mtx, const string& name);

    /// destroy this buffer
    ~Ipb (void);

    /// reset this buffer
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return a buffer size by unit
    long getsize (t_unit unit) const;

    /// add a slot in the buffer
    void add (Slot* slot);

    /// find a free slot by unit
    long find (t_unit unit) const;

    /// set the slot instruction by unit, index and instruction
    /// @param unit the unit slot to use
    /// @param slot the slot index
    /// @param ssi  the instruction to set
    void setinst (t_unit unit, const long slot, const Ssi& ssi);

    /// @return an instruction by unit and slot
    Ssi getinst (t_unit unit, const long slot) const;

  private:
    // make the copy constructor private
    Ipb (const Ipb&);
    // make the assignment operator private
    Ipb& operator = (const Ipb&);
  };
}

#endif
