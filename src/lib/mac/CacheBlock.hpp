// ---------------------------------------------------------------------------
// - CacheBlock.hpp                                                          -
// - iato:isa library - cache block class definition                         -
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

#ifndef  IATO_CACHEBLOCK_HPP
#define  IATO_CACHEBLOCK_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

#include <vector>

namespace iato {

  /// The CacheBlock class is a simple class that implements the functionality
  /// of a cache block. A cache block is defined as an array of N entries
  /// with a valid bit, a tag and a line. For simplicity, the tag is stored
  /// as an octa with a mask associated with the block. The line is defined
  /// as a set of bytes.

  class CacheBlock {
  private:
    /// the block size
    long d_size;
    /// the line size
    long d_blsz;
    /// the tag mask
    t_octa d_mask;
    /// the block data structure
    std::vector<struct s_block*> p_blok;

  public:
    /// create a cache block by size and mask
    /// @param size the block size
    /// @param blsz the block line size
    CacheBlock (const long size, const long blsz);

    /// destroy this cache block
    ~CacheBlock (void);

    /// reset this cache block
    void reset (void);

    /// @return true if the line address is valid
    bool isvalid (const long index, const t_octa addr) const;

    /// read a line byte at a certain address
    /// @param index the line index in the block
    /// @param addr  the address to read the byte
    t_byte readbyte (const long index, const t_octa addr) const;

    /// write a line byte at a certain address
    /// @param index the line index in the block
    /// @param addr  the address to read the byte
    /// @param byte  the byte to write
    void writebyte (const long index, const t_octa addr, const t_byte byte);

    /// update a line with a buffer
    /// @param index the line index in the block
    /// @param addr the base address to update
    /// @param line the line buffer to update
    void update (const long index, const t_octa addr, const t_byte* line);

  private:
    // make the copy constructor private
    CacheBlock (const CacheBlock&);
    // make the assignment operator private
    CacheBlock& operator = (const CacheBlock&);
  };
}

#endif
