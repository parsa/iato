// ---------------------------------------------------------------------------
// - Mta.hpp                                                                 -
// - iato:mac library - memory transaction adapter class definition          -
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

#ifndef  IATO_MTA_HPP
#define  IATO_MTA_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_MRT_HPP
#include "Mrt.hpp"
#endif

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Mta class is a memory transaction adapter class that adapts the 
  /// memory transaction to with a result object. The adapter supports also 
  /// bundle related transactions. Additionally, the adapter is designed to 
  /// respond to alat request.

  class Mta : public Resource {
  public:
    /// the mta memory types
    enum t_tmem {
      IMEM, // instruction memory
      DMEM, // data memory
      FMEM, // floating point memory
      BMEM  // backing-store memory
    };

  private:
    /// the bundle window size
    long d_iwsz;
    /// the bytes window size
    long d_bwsz;
    /// the bundle array
    Bundle* p_bndl;
    /// the bundle buffer
    t_byte* p_bbuf;
    /// the instruction memory
    Memory* p_imem;
    /// the data memory
    Memory* p_dmem;
    /// the floating point memory
    Memory* p_fmem;
    /// the backing store memory
    Memory* p_bmem;

  public:
    /// create a default mta
    Mta (void);

    /// create a mta with a context
    /// @param mtx the architectural context
    Mta (Mtx* mtx);

    /// create a mta with a context and a name
    /// @param mtx the architectural context
    /// @param name the resource name
    Mta (Mtx* mtx, const string& name);

    /// destroy this mta
    ~Mta (void);

    /// reset this mta
    void reset (void);

    /// bind a memory to the mta
    /// @param mem the memory to bind
    void bind (Memory* mem);

    /// bind a memory to the mta by type
    /// @param type the memory type to bind
    /// @param mem the memory to bind
    void bind (t_tmem type, Memory* mem);

    /// process a memory request
    /// @param mrt the memory request to process
    void process (Mrt& mrt);
  
    /// update a bundle array at a certain address
    /// @param bip the bunlde ip to use
    void update (const t_octa bip) const;

    /// @return the mta bundle by index
    Bundle getbndl (const long index) const;

  private:
    // make the copy constructor private
    Mta (const Mta&);
    // make the assignment operator private
    Mta& operator = (const Mta&);
  };
}

#endif
