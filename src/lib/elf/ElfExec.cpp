// ---------------------------------------------------------------------------
// - ElfExec.cpp                                                             -
// - iato:elf library - elf exec segment class implementation                -
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

#include "ElfExec.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new elf executable memory
  
  ElfExec::ElfExec (ElfKernel* ekp) {
    // create the stack
    add (p_stk = new ElfStack (ekp));
    // create the bsa
    add (p_bsa = new ElfBsa (ekp));
    // create the map area
    add (p_map = new ElfMap (ekp));
    // reset breakable memory
    p_brk = 0;
  }

  // set the breakable memory

  void ElfExec::setbrkm (ElfBrk* brk) {
    if (!brk)  return;
    if (p_brk) throw Exception ("memory-error", "breakable memory is set");
    add (p_brk = brk);
  }

  // return the elf breakable memory

  ElfBrk* ElfExec::getbrkm (void) const {
    return p_brk;
  }

  // return the elf stack memory

  ElfStack* ElfExec::getstkm (void) const {
    return p_stk;
  }
  
  // return the elf bsa memory

  ElfBsa* ElfExec::getbsam (void) const {
    return p_bsa;
  }

  // return the elf mappable memory

  ElfMap* ElfExec::getmapm (void) const {
    return p_map;
  }
}
