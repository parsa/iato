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
  }

  // create a breakable array by parameters

  ElfBrk::ElfBrk (ElfKernel* ekp) {
    // set break parameters
    d_psize = ekp->getpgsz ();
    d_mode  = ekp->getmode ();
    d_brkta = OCTA_0;
  }

  // add a segment to this elf breakable array

  void ElfBrk::addseg (ElfSegment* seg) {
    if (!seg) return;
    // add the segment
    ElfLoad::addseg (seg);
    // compute upper limit
    t_octa segta = seg->getbase () + (t_octa) seg->getsize ();
    if (segta > d_brkta) d_brkta = page_align (d_psize, segta);
  }

  // set a new break value

  void ElfBrk::setbrkta (const t_octa addr) {
    // do nothing if null
    if (addr == OCTA_0) return;
    // realign the address
    t_octa brk = page_align (d_psize, addr);
    // check that it is not equal
    if (brk == d_brkta) return;
    // first case, the current break limit is not valid - we just have
    // to create a segment with the appropriate size
    if ((isvalid (d_brkta) == false) && (brk > d_brkta)) {
      t_long size = brk - d_brkta;
      ElfSegment* seg = new ElfSegment (size, d_brkta);
      seg->setprot (PROT_FU);
      seg->setmode (d_mode);
      // add segment and update break limit
      addseg (seg);
      d_brkta = brk;
      return;
    }
    assert (false);
  }

  // return the current break value

  t_octa ElfBrk::getbrkta (void) const {
    return d_brkta;
  }
}
