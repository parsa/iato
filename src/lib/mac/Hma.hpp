// ---------------------------------------------------------------------------
// - Hma.hpp                                                                 -
// - iato:mac library - hierarchical memory architecture class definition    -
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

#ifndef  IATO_HMA_HPP
#define  IATO_HMA_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_MTA_HPP
#include "Mta.hpp"
#endif

#ifndef  IATO_RUNNABLE_HPP
#include "Runnable.hpp"
#endif

namespace iato {

  /// The Hma class is the hierarchical memory architecture. The memory model
  /// is build with or without caches. In bypass mode, no cache are installed
  /// and a direct access to the memory is perfomed via the mta. In non
  /// bypass mode, the memory access is performed via a memory request
  /// interface that is normally part of a port request.

  class Hma : public Runnable {
  protected:
    /// the mta
    Mta*    p_mta;
    /// the memory image
    Memory* p_mem;
    /// the memory bypass bit
    bool    d_mbb;

  public:
    /// create a memory architecture with a memory image
    /// @param mem the memory image to use
    Hma (Memory* mem);

    /// create a memory interface with a context and memory image
    /// @param mtx the architectural context
    /// @param mem the memory image to use
    Hma (Mtx* mtx, Memory* mem);

    /// reset this interface
    void reset (void);

    /// flush this interface
    void flush (void);

    /// report this resource
    void report (void) const;

    /// run this runnable object
    void run (void);

    /// @return the memory adapter
    Mta* getmta (void) const;

  private:
    // make the copy constructor private
    Hma (const Hma&);
    // make the assignment operator private
    Hma& operator = (const Hma&);
  };
}

#endif
