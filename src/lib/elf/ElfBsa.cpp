// ---------------------------------------------------------------------------
// - ElfBsa.cpp                                                              -
// - iato:elf library - elf backing store area class implementation          -
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

#include "ElfBsa.hpp"

namespace iato {

  // create a default bsa

  ElfBsa::ElfBsa (void) {
    // create a default kernel parameter
    ElfKernel ekp;
    // set bsa parameters
    d_size  = ekp.getbsasz ();
    p_data  = new t_byte[d_size];
    // set base, protection and mode
    setbase (ekp.getbsava ());
    setprot (PROT_FU);
    setmode (ekp.getmode ());
  }

  // create a new bsa by kernel parameters
  
  ElfBsa::ElfBsa (ElfKernel* ekp) {
    // set bsa parameters
    d_size  = ekp->getbsasz ();
    p_data  = new t_byte[d_size];
    // set base, protection and mode
    setbase (ekp->getbsava ());
    setprot (PROT_FU);
    setmode (ekp->getmode ());
  }
}
