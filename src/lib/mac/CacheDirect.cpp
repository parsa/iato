// ---------------------------------------------------------------------------
// - CacheDirect.cpp                                                         -
// - iato:isa library - direct cache memory class implementation             -
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
#include "CacheDirect.hpp"

namespace iato {

  // create a direct cache by size and line size

  CacheDirect::CacheDirect (const long size, const long clsz) {
    d_size = size; assert (d_size > 0);
    // compute line parameters
    d_clsz = clsz; assert (d_clsz > 0);
    d_lsht = getmsbi (d_clsz);
    // compute block parameters
    d_bksz = d_size / d_clsz; assert (d_bksz > 0);
    d_bmsk = !getsmsk (d_bksz);
    // compute the tag mask
    p_blok = new CacheBlock (d_bksz, d_clsz);
  }

  // destroy this direct cache

  CacheDirect::~CacheDirect (void) {
    delete p_blok;
  }

  // reset this direct cache

  void CacheDirect::reset (void) {
    p_blok->reset ();
  }

  // check if an address is valid

  bool CacheDirect::isvalid (const t_octa addr) const {
    // compute the cache block index
    long index = (addr >> d_lsht) & d_bmsk;
    // check for valid entry
    return p_blok->isvalid (index, addr);
  }

  // read a byte at a certain address

  t_byte CacheDirect::readbyte (const t_octa addr) const {
    // compute the cache block index
    long index = (addr >> d_lsht) & d_bmsk;
    // get the cache byte
    return p_blok->readbyte (index, addr);
  }

  // write a byte at a certain address

  void CacheDirect::writebyte (const t_octa addr, const t_byte byte) {
    // if the cache uses a no write allocate policy and the address
    // is not valid, simply ignore the write
    if ((d_cwrt == Cache::WRNOALC) && (isvalid (addr) == false)) return;    
    // compute the cache block index
    long index = (addr >> d_lsht) & d_bmsk;
    // write the cache byte
    p_blok->writebyte (index, addr, byte);
  }

  // update a line with a buffer

  void CacheDirect::update (const t_octa addr, const t_byte* line) {
    // compute the cache block index
    long index = (addr >> d_lsht) & d_bmsk;
    // update the cache block line
    p_blok->update (index, addr, line);
  }
}
