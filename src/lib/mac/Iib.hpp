// ---------------------------------------------------------------------------
// - Iib.hpp                                                                 -
// - iato:mac library - instruction  interrupt buffer class definition       -
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

#ifndef  IATO_IIB_HPP
#define  IATO_IIB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Iib is the instruction interrupt buffer. The interrupt buffer
  /// allocation is generally done when the instruction is decoded and 
  /// the associated index is part of the decoded instruction.

  class Iib : public Resource {
  private:
    /// the iib size
    long d_size;
    /// the iib threshold
    long d_ithr;
    /// the iib length
    long d_ilen;
    /// the valid alloc bit
    bool* p_vldb;
    /// the interrupt buffer
    Interrupt* p_intr;

  public:
    /// create a new buffer
    Iib (void);

    /// create a new buffer with a context
    /// @param mtx the architectural context
    Iib (Mtx* mtx);

    /// destroy this buffer
    ~Iib (void);

    /// reset this buffer
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the buffer is empty
    bool isempty (void) const;

    /// @return true if the buffer is full
    bool isfull (void) const;

    /// @return true if the iib has reached the threshold
    bool isthr (void) const;

    /// @return true if the buffer entry is valid
    bool isvalid (const long index) const;

    /// clear an iib entry by index
    void clear (const long index);

    /// allocate a new iib entry
    long alloc (void);

    /// @return an interrupt by index
    Interrupt getintr (const long index) const;

    /// set an interrupt by index
    /// @param index the iib index
    /// @param intr the interrupt to set
    void setintr (const long index, const Interrupt& intr);

  private:
    // make the copy constructor private
    Iib (const Iib&);
    // make the assignment operator private
    Iib& operator = (const Iib&);
  };
}

#endif
