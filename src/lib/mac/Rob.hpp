// ---------------------------------------------------------------------------
// - Rob.hpp                                                                 -
// - iato:mac library - reorder buffer class definition                      -
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

#ifndef  IATO_ROB_HPP
#define  IATO_ROB_HPP

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

  /// The Rob class implements a reorder buffer as a fifo. The rob is used
  /// to retire instruction in order and process interrupt. This rob
  /// implementation is instruction based. Using an instruction based
  /// rob simplifies the architecture, since there is no need to handle
  /// MLX bundles as well as implied dependencies. The interrupt processing
  /// is also simplified. The entry allocation is on the other end complex.

  class Rob : public Resource {
  private:
    // rob size
    long   d_size;
    // number of entries
    long   d_rlen;
    // rob data
    struct t_rob* p_rbuf;
    // input push index
    long d_iptr;
    // output pop index
    long d_optr;

  public:
    /// create a new rob
    Rob (void);

    /// create a new rob with a context
    /// @param mtx the architectural context
    Rob (Mtx* mtx);

    /// destroy this rob
    ~Rob (void);

    /// reset this rob
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the rob is empty
    bool isempty (void) const;

    /// @return true if the rob is full
    bool isfull (void) const;

    /// @return true if the rob entry is valid
    bool isvalid (const long ridx) const;

    /// @return true if the latest rob entry is valid
    bool isvalid (void) const;

    /// @return true if the latest rob entry is valid
    bool ispop (void) const;

    /// @return true if the latest rob entry is a serialization
    bool issrlz (void) const;

    /// @return true if the latest rob entry is a nop
    bool isnop (void) const;

    /// @return true if the latest rob entry is an interrupt
    bool isintr (void) const;

    /// @return true if the latest rob entry has been cancelled
    bool iscancel (void) const;

    /// allocate a new rob entry with an interrupt index
    /// @param iiib the interrupt index
    void alloc (const long iiib);

    /// allocate a serialize rob entry with an ip and a slot
    /// @param ip    the ip to restart
    /// @param slot the slot to restart
    /// @param unit the instruction unit
    /// @param srlz the serialize bit
    /// @param nopb the nop bit
    void alloc (const t_octa ip, const long slot, t_unit unit,
		const bool srlz, const bool nopb);

    /// allocate a new rob entry
    /// @param inst the instruction
    /// @param imob the memory ordering buffer index
    /// @param iiib the instruction interrupt buffer index
    long alloc (const Dsi& inst, const long imob, const long iiib);

    /// @return the latest irb and clean rob
    long pop (void);

    /// pop the latest serialize rob entry
    void spop (void);

    /// pop the latest nop rob entry
    void npop (void);

    /// @return the latest interrupt index and clean rob
    long ipop (void);

    /// @return the latest instruction ip
    t_octa getiip (void) const;

    /// @return the latest instruction slot
    long getslot (void) const;

    /// @return the latest instruction unit
    t_unit getunit (void) const;

    /// @return the latest irb index
    long getiidx (void) const;

    /// @return the latest memory ordering index
    long getimob (void) const;

    /// @return the latest iib index
    long getiiib (void) const;

    /// @return the latest speculative bit
    bool getsbit (void) const;

    /// @return the latest branch speculative status
    bool getbbss (void) const;

    /// set the execute flag
    /// @param ridx the rob index
    /// @param flag the flag to set
    void setexe (const long ridx, const bool flag);

    /// set the cancel flag
    /// @param ridx the rob index
    /// @param flag the flag to set
    void setcnlf (const long ridx, const bool flag);

    /// set the irb index entry
    /// @param ridx the rob index
    /// @param index the irb index
    void setirb (const long ridx, const long index);

    /// set the interrupt entry
    /// @param ridx the rob index
    /// @param iiib the interrupt index
    void setintr (const long ridx, const long iiib);

    /// set the interrupt entry with speculative info
    /// @param ridx the rob index
    /// @param iiib the virtual interrupt
    /// @param sbit the speculative bit
    void setintr (const long ridx, const long iiib, const bool sbit);

    /// set the speculative bit
    /// @param ridx the rob index
    /// @param sbit the speculative bit
    void setsbit (const long ridx, const bool sbit);

    /// set the bundle branch speculative status
    /// @param ridx the rob index
    /// @param bbss the speculative status
    void setbbss (const long ridx, const bool bbss);

  private:
    // make the copy constructor private
    Rob (const Rob&);
    // make the assignment operator private
    Rob& operator = (const Rob&);
  };
}

#endif
