// ---------------------------------------------------------------------------
// - CacheDirect.hpp                                                         -
// - iato:isa library - direct cache memory class definition                 -
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

#ifndef  IATO_CACHEDIRECT_HPP
#define  IATO_CACHEDIRECT_HPP

#ifndef  IATO_CACHE_HPP
#include "Cache.hpp"
#endif

#ifndef  IATO_CACHEBLOCK_HPP
#include "CacheBlock.hpp"
#endif

namespace iato {

  /// The CacheDirect class is cache memory class that implements the direct
  /// mapping strategy. With a direct cache, there is only one set and a
  /// unique position for a cache line. This is the simplest cache memory.

  class CacheDirect : public Cache {
  private:
    /// the cache size in bytes
    long   d_size;
    /// the line size in bytes
    long   d_clsz;
    /// the line shift
    long   d_lsht;
    /// the block size
    long   d_bksz;
    /// the block mask
    t_octa d_bmsk;
    /// the cache block
    CacheBlock* p_blok;

  public:
    /// create a direct cache by size and line size
    /// @param size the cache size
    /// @param clsz the block line size
    CacheDirect (const long size, const long clsz);

    /// destroy this cache
    ~CacheDirect (void);
 
    /// reset this cache
    void reset (void);

    /// @return true if the address is valid
    bool isvalid (const t_octa addr) const;

    /// read a byte at a certain address
    /// @param addr the address to read
    t_byte readbyte (const t_octa addr) const;

    /// write a byte at a certain address
    /// @param addr the address to write the byte
    /// @param byte the byte to write
    void writebyte (const t_octa addr, const t_byte byte);

    /// update a line with a buffer
    /// @param addr the base address to update
    /// @param line the line buffer to update
    void update (const t_octa addr, const t_byte* line);

  private:
    // make the copy constructor private
    CacheDirect (const CacheDirect&);
    // make the assignment operator private
    CacheDirect& operator = (const CacheDirect&);
  };
}

#endif
