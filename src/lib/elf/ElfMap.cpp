// ---------------------------------------------------------------------------
// - ElfMap.cpp                                                              -
// - iato:elf library - elf mappable memory class implementation             -
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

#include "ElfMap.hpp"
#include "Exception.hpp"

namespace iato {
  // this procedure realign a size to a page size
  static t_long size_align (const t_long psize, const t_long size) {
    if ((size % psize) == 0) return size;
    t_long np = size / psize;
    t_long sp = (np + 1) * psize;
    return sp;
  }

  // this procedure realign an address to a page size
  static t_octa page_align (const t_octa psize, const t_octa addr) {
    if ((addr % psize) == 0) return addr;
    t_long np = addr / psize;
    t_long ra = (np + 1) * psize;
    return ra;
  }

  // create a default elf mappable memory

  ElfMap::ElfMap (void) {
    // create a default kernel parameter
    ElfKernel ekp;
    // set mappable parameters
    d_psize = ekp.getpgsz ();
    d_mode  = ekp.getmode ();
    d_mbase = ekp.getmapb ();
  }

  // create a mappable memory by parameters

  ElfMap::ElfMap (ElfKernel* ekp) {
    // set mappable parameters
    d_psize = ekp->getpgsz ();
    d_mode  = ekp->getmode ();
    d_mbase = ekp->getmapb ();
  }

  // find the next available page aligned address

  t_octa ElfMap::findtop (void) const {
    // get the number of elf segments
    long slen = length ();
    // the base address is used if nothing is mapped
    if (slen == 0) return d_mbase;
    // loop in the segment and compute max address
    t_octa result = d_mbase;
    for (long i = 0; i < slen; i++) {
      ElfSegment* seg = getseg (i);
      if (!seg) continue;
      t_octa base = seg->getbase ();
      t_octa size = seg->getsize ();
      t_octa addr = page_align (d_psize, base + size);
      if (addr > result) result = addr;
    }
    return result;
  }

  // map some memory anonymously

  t_octa ElfMap::mmap (const t_long size, const t_byte prot, 
		       ElfSegment::t_stype type) {
    // align the size to a page length
    t_long mlen = size_align (d_psize, size);
    // get the next base address
    t_octa base = findtop ();
    // create the segment
    ElfSegment* seg = new ElfSegment (mlen, base, type);
    seg->setprot (prot);
    // add the segment
    addseg (seg);
    return base;
  }

  // unmap some memory at a certain address and size

  bool ElfMap::munmap (const t_octa addr, const t_long size) {
    // check if the adress exists
    if (isvalid (addr) == false) return false;
    // get the segment by address and check the size
    long index = find (addr);
    if (index == -1) return false;
    ElfSegment* seg = getseg (index);
    if (!seg) return false;
    assert (size == seg->getsize ());
    // remove the element
    return remove (index);
  }
}
