// ---------------------------------------------------------------------------
// - CacheBlock.cpp                                                          -
// - iato:isa library - cache block class implementation                     -
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

#include "Utils.hpp"
#include "Exception.hpp"
#include "CacheBlock.hpp"

namespace iato {

  // the block structure
  struct s_block {
    // this block size
    long    d_blsz;
    // the valid bit
    bool    d_valid;
    // the block line tag
    t_octa  d_tag;
    // the line data
    t_byte* p_line;
    // create a block data by line size
    s_block (const long blsz) {
      d_blsz = blsz; assert (d_blsz > 0);
      p_line  = new t_byte[d_blsz];
      reset ();
    }
    // destroy this block data
    ~s_block (void) {
      delete [] p_line;
    }
    // reset this block data
    void reset (void) {
      d_valid = false;
      d_tag   = OCTA_0;
      for (long i = 0; i < d_blsz; i++) p_line[i] = BYTE_0;
    }
    // check if an address is valid
    bool isvalid (const t_octa addr) const {
      if (d_valid == false) return false;
      return (d_tag == addr);
    }
    // read a byte at a certain index
    t_byte readbyte (const long offset) const {
      assert (d_valid == true);
      assert ((offset >= 0) && (offset < d_blsz));
      return p_line[offset];
    }
    // write a byte at a certain index
    void writebyte (const long offset, const t_byte byte) {
      assert (d_valid == true);
      assert ((offset >= 0) && (offset < d_blsz));
      p_line[offset] = byte;
    }
    // update a line with a buffer and a size
    void update (const t_octa tag, const t_byte* line) {
      d_valid = true;
      d_tag   = tag;
      for (long i = 0; i < d_blsz; i++) p_line[i] = line[i];
    }
  };

  // create a cach block by size and mask
  
  CacheBlock::CacheBlock (const long size, const long blsz) {
    d_size = size; assert (d_size > 0);
    d_blsz = blsz; assert (d_blsz > 0);
    d_mask = getsmsk (d_size * d_blsz);
    p_blok.reserve(size);
    for (size_t i = 0; i < size; ++i)
    {
      p_blok.emplace_back(new s_block(d_blsz));
    }
  }

  // destroy this cache block

  CacheBlock::~CacheBlock (void) {
    for (s_block* b : p_blok)
    {
      delete b;
    }
  }

  // reset this cache block

  void CacheBlock::reset (void) {
    for (long i = 0; i < d_size; i++) p_blok[i]->reset ();
  }

  // check if a cache line is valid

  bool CacheBlock::isvalid (const long index, const t_octa addr) const {
    assert ((index >= 0) && (index < d_size));
    return p_blok[index]->isvalid (addr & d_mask);
  }

  // read a byte line at a certain address

  t_byte CacheBlock::readbyte (const long index, const t_octa addr) const {
    // check for valid address
    if (isvalid (index, addr) == false)
      throw Exception ("block-error", "invalid byte read with invalid line");
    // compute offset from address
    long offset = addr % (t_octa) d_blsz;
    return p_blok[index]->readbyte (offset);
  }

  // write a byte line at a certain address

  void CacheBlock::writebyte (const long index, const t_octa addr, 
			      const t_byte byte) {
    // check for valid address
    if (isvalid (index, addr) == false)
      throw Exception ("block-error", "invalid byte write with invalid line");
    // compute offset from address
    long offset = addr % (t_octa) d_blsz;
    p_blok[index]->writebyte (offset, byte);
  }

  // update a cache block line by index

  void CacheBlock::update (const long index, const t_octa addr, 
			   const t_byte* line) {
    assert ((index >= 0) && (index < d_size));
    // update the line
    p_blok[index]->update (addr & d_mask, line);
  }
}
