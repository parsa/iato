// ---------------------------------------------------------------------------
// - Spb.hpp                                                                 -
// - iato:mac library - slot port buffer class definition                    -
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

#ifndef  IATO_SPB_HPP
#define  IATO_SPB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SLOT_HPP
#include "Slot.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Spb class is the slot port buffer. It is a set of ports that are
  /// grouped into a family set. At construction, the context determines
  /// how many slots can be held in the buffer. Each micro-pipeline is
  /// responsible to add the required slot. The Spb is used by an in-order
  /// simulation engine an issue buffer or as an output buffer after the
  /// renaming stage. The port buffer also provides a special slot for 
  /// global interrupt within a group. The interrupt and the offending
  /// instruction order index is stored as well.

  class Spb : public Resource {
  protected:
    /// the M buffer size
    long d_mbsz;
    /// the number of load slots
    long d_ldsn;
    /// the number of store slots
    long d_stsn;
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
    /// the global interrupt
    Interrupt d_intr;
    /// the instruction index
    long d_iioi;

  public:
    /// create a new buffer with a context
    /// @param mtx the architectural context
    Spb (Mtx* mtx);

    /// create a new buffer with a context and a name
    /// @param mtx the architectural context
    /// @param name the buffer name
    Spb (Mtx* mtx, const string& name);

    /// destroy this buffer
    ~Spb (void);

    /// reset this buffer
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return a buffer size by unit
    long getsize (t_unit unit) const;

    /// add a slot in the buffer
    void add (Slot* slot);

    /// @return true if all slots are free
    bool isfree (void) const;

    /// find a free slot by unit and store/load bits
    /// @param unit the unit slot to find
    /// @param stb  the store bit
    /// @param ldb  the load bit
    long find (const t_unit unit, const bool stb, const bool ldb) const;

    /// set the slot instruction by unit, index and instruction
    /// @param unit the unit slot to use
    /// @param slot the slot index
    /// @param ssi  the instruction to set
    void setinst (t_unit unit, const long slot, const Ssi& ssi);

    /// @return an instruction by unit and slot
    Ssi getinst (t_unit unit, const long slot) const;

    /// @return true if the port buffer has been interupted
    bool isintr (void) const;

    /// set the buffer interrupt and index
    /// @param vi   the offending interrupt
    /// @param iioi the offending index
    void setintr (const Interrupt& vi, const long iioi);

  private:
    // make the copy constructor private
    Spb (const Spb&);
    // make the assignment operator private
    Spb& operator = (const Spb&);
  };
}

#endif
