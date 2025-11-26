// ---------------------------------------------------------------------------
// - ElfBrk.cpp                                                              -
// - iato:elf library - elf breakable load segment class implementation      -
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

#include "ElfBrk.hpp"
#include "Exception.hpp"

namespace iato {
  // this procedure realign a an address to the next page boundary
  static t_octa page_align (const t_octa psize, const t_octa addr) {
    if ((addr % psize) == OCTA_0) return addr;
    t_octa np = addr / psize;
    t_octa result = (np + 1) * psize;
    return result;
  }

  // create a default elf breakable array

  ElfBrk::ElfBrk (void) {
    // create a default kernel parameter
    ElfKernel ekp;
    // set break parameters
    d_psize = ekp.getpgsz ();
    d_mode  = ekp.getmode ();
    d_brkta = OCTA_0;
    d_heapb = OCTA_0;
    d_guard = OCTA_0;
  }

  // create a breakable array by parameters

  ElfBrk::ElfBrk (ElfKernel* ekp) {
    // set break parameters
    d_psize = ekp->getpgsz ();
    d_mode  = ekp->getmode ();
    d_brkta = OCTA_0;
    d_heapb = OCTA_0;
    d_guard = OCTA_0;
  }

  // add a segment to this elf breakable array

  void ElfBrk::addseg (ElfSegment* seg) {
    if (!seg) return;
    // add the segment
    ElfLoad::addseg (seg);
    // compute upper limit
    t_octa segta = seg->getbase () + (t_octa) seg->getsize ();
    if (segta > d_brkta) {
      d_brkta = page_align (d_psize, segta);
      d_heapb = d_brkta;
      d_guard = d_brkta + d_psize;
    }
  }

  // set a new break value

  void ElfBrk::setbrkta (const t_octa addr) {
    if (addr == OCTA_0) return;
    // make sure we know where the heap starts
    if (d_heapb == OCTA_0) d_heapb = d_brkta;
    // align the requested break
    t_octa want = page_align (d_psize, addr);
    // never shrink below the heap base
    if (want < d_heapb) want = d_heapb;
    // if we already have enough space, just update the guard
    if (want <= d_brkta) {
      d_brkta = want;
      d_guard = d_brkta + d_psize;
      return;
    }
    // extend page by page so we keep a guard immediately after the heap
    t_octa cur = d_brkta;
    if (cur == OCTA_0) cur = d_heapb;
    while (cur < want) {
      t_octa next = cur + d_psize;
      t_long size = static_cast<t_long> (next - cur);
      ElfSegment* seg = new ElfSegment (size, cur);
      seg->setmode (d_mode);
      seg->setprot (Memory::PROT_RW);
      ElfLoad::addseg (seg);
      cur = next;
    }
    d_brkta = cur;
    d_guard = d_brkta + d_psize;
  }

  // return the current break value

  t_octa ElfBrk::getbrkta (void) const {
    return d_brkta;
  }

  void ElfBrk::bootstrap (const long pages) {
    long count = (pages <= 0) ? 1 : pages;
    t_octa extra = d_psize * static_cast<t_octa> (count);
    setbrkta (d_brkta + extra);
  }

  t_octa ElfBrk::getheapbase (void) const {
    return d_heapb;
  }

  t_octa ElfBrk::getheaplimit (void) const {
    return d_brkta;
  }

  t_octa ElfBrk::getguard (void) const {
    return d_guard;
  }

  t_octa ElfBrk::getpgsz (void) const {
    return d_psize;
  }
}
