// ---------------------------------------------------------------------------
// - ElfKernel.cpp                                                           -
// - iato:elf library - elf kernel parameters class implementation           -
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

#include "Abi.hpp"
#include "ElfKernel.hpp"

namespace iato {

  // create a default elf kernel parameters
  
  ElfKernel::ElfKernel (void) {
    // set default parameters
    d_pshft = PAGE_SHFT;
    d_stksz = VSTK_SIZE;
    d_mbase = ABI_MMAP_BASE;
    d_bsasz = VBSA_SIZE;
    // initialize everything
    initialize ();
  }

  // create an elf kernel parameters with a context
  
  ElfKernel::ElfKernel (Etx* etx) {
    // set default parameters
    d_mbase = ABI_MMAP_BASE;
    d_pshft = etx->getlong ("PAGE-SHIFT");
    d_stksz = etx->getlong ("STACK-SIZE");         assert (d_stksz > 0);
    d_bsasz = etx->getlong ("BACKING-STORE-SIZE"); assert (d_bsasz > 0);
    // initialize everything
    initialize ();
  }

  void ElfKernel::initialize (void) {
    // set default little endian
    d_mode  = false;
    d_clktk = ELF_AUX_CLKTK;
    // recompute page size
    if (d_pshft < 13) d_pshft = 13;
    d_psize = OCTA_1 << d_pshft;
    // recompute stack parameters
    t_octa soff = (OCTA_1 << (4 * d_pshft - 12)) - d_psize;
    d_stkva = SPBS_BASE + soff;
    assert ((d_stkva % ABI_VSTK_ALIGN) == OCTA_0);
    // recompute bsa parameters
    d_bsava = d_stkva - VBSA_BREF + d_psize;
  }

  // return the endian mode

  bool ElfKernel::getmode (void) const {
    return d_mode;
  }

  // set the endian mode

  void ElfKernel::setmode (const bool mode) {
    d_mode = mode;
  }

  // return the page size

  t_octa ElfKernel::getpgsz (void) const {
    return d_psize;
  }

  // return the mappable base address

  t_octa ElfKernel::getmapb (void) const {
    return d_mbase;
  }

  // return the top stack address

  t_octa ElfKernel::getstkva (void) const {
    return d_stkva;
  }

  // return the stack size

  long ElfKernel::getstksz (void) const {
    return d_stksz;
  }
  
  // return the bsa base address

  t_octa ElfKernel::getbsava (void) const {
    return d_bsava;
  }

  // return the bsa size

  long ElfKernel::getbsasz (void) const {
    return d_bsasz;
  }

  // return the times clock ticks

  long ElfKernel::getclktk (void) const {
    return d_clktk;
  }
}
