// ---------------------------------------------------------------------------
// - Statistic.cpp                                                           -
// - iato:iaos client - static statistic class implementation                -
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

#include "Exception.hpp"
#include "Statistic.hpp"

namespace iato {

  // create a new static statistic object
  
  Statistic::Statistic (ElfImage* elf) {
    p_elf = elf;
  }

  // compute statistics

  void Statistic::compute (void) {    
    // try to get an elf text image
    ElfText* text = p_elf ? p_elf->gettxt () : 0;
    if (!text) return;
    // get the number of section and loop
    long slen = text->length ();
    for (long i = 0; i < slen; i++) {
      ElfSection* scn = text->getscn (i);
      if (!scn) continue;
      // iterate throw the section
      typedef ElfSection::const_iterator t_bit;
      for (t_bit bit = scn->begin (); bit < scn->end (); bit++) {
	try {
	  Bundle bndl = *bit;
	  // disassemble bundle
	  if (bndl.isvalid () == false) continue;
	  if (d_bflg == true) addbndl (bndl);
	  for (long j = 0; j < BN_SLSZ; j++) {
	    Instr inst = bndl.getinstr (j);
	    addinst (inst);
	  }
	} catch (const Exception& e) {
	}
      }
    }
  }
}
