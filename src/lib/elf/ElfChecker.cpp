// ---------------------------------------------------------------------------
// - ElfChecker.cpp                                                          -
// - iato:elf library - elf checker class implementation                     -
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
#include "ElfChecker.hpp"

namespace iato {
  using namespace std;

  // create an elf checker by section

  ElfChecker::ElfChecker (const string& name, void* scn, const bool mode) :
    ElfSection (name, scn, mode) {
  }

  // return a check object from this section

  Checker* ElfChecker::getchecker (void) const {
    // create a new checker
    Checker* chk = new Checker;
    // get memory base address
    t_octa addr = getbase ();
    // create a record check
    Record rcd ("CHK");
    // loop until we have valid data
    while (isvalid (addr) == true) {
      // get register type
      t_byte lreg = readbyte (addr++);
      t_byte pnum = readbyte (addr++);
      // align address and read value
      addr += 6;
      t_octa data = readocta (addr);
      if (lreg == FREG) {
	addr += 8;
	t_octa fdata = readocta (addr);
	rcd.setrchk (lreg, pnum, data, fdata);
      } else rcd.setrchk (lreg, pnum, data);
      addr += ABI_ADDR_ALIGN;
      chk->add (rcd);
    }
    return chk;
  }
}
