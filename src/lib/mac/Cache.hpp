// ---------------------------------------------------------------------------
// - Cache.hpp                                                               -
// - iato:cpt library - abstract cache memory class definition               -
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

#ifndef  IATO_CACHE_HPP
#define  IATO_CACHE_HPP

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

namespace iato {

  /// The Cache class is an abstract class that defines the basic interface
  /// for a cache memory subsystem. The class is derived from the memory
  /// interface and shall be used to design larger system.

  class Cache : public Memory {
  public:
    /// the cache types
    enum t_cmtype {
      UNIFIED, // unified cache memory
      INSTR,   // instruction cache memory
      DATA     // data cache memory
    }; 
    /// cache update policy
    enum t_cmupd {
      WRITETH, // write-through
      WRITEBK  // write-back
    };
    /// cache write policy
    enum t_cmwrt {
      WRNOALC, // no write allocate
      WRALLOC  // write allocate
    };

  protected:
    /// the cache type
    t_cmtype d_type;
    /// cache update
    t_cmupd  d_cupd;
    /// cache write
    t_cmwrt  d_cwrt;
    /// the cache level
    long     d_level;

  public:
    /// create a default cache memory
    Cache (void);

    /// set the cache parameters
    /// @param level the cache level
    /// @param type  the cache type
    /// @param cupd  the cache update policy
    /// @param cwrt  the cache write policy
    virtual void setparam (const long    level, const t_cmtype type, 
			   const t_cmupd cupd,  const t_cmwrt  cwrt);

    /// @return the cache type
    virtual t_cmtype gettype (void) const;

    /// @return the cache level
    virtual long getlevel (void) const;

    /// update a line with a buffer
    /// @param addr the base address to update
    /// @param line the line buffer to update
    virtual void update (const t_octa addr, const t_byte* line) =0;

  private:
    // make the copy constructor private
    Cache (const Cache&);
    // make the assignment operator private
    Cache& operator = (const Cache&);
  };
}

#endif
