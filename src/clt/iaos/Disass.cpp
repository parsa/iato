// ---------------------------------------------------------------------------
// - Disass.cpp                                                              -
// - iato:iaos client - text disassemble class implementation                -
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

#include "Disass.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // this procedure throws an exception with a message and a ip
  static void disass_error (const string& msg, const t_octa ip) {
    ostringstream os;
    os << msg << "at ip 0x";
    os << hex << setw (16) << setfill ('0') << ip;
    throw Exception ("disassemble-error", os.str ());
  }

  // this procedure map a slot to a data8 directive
  static string disass_data8 (const t_octa inst) {
    ostringstream os;
    os << "data8 0x" << hex << inst;
    return os.str ();
  }

  // this procedure disassemble a bundle
  static void disass_bundle (const Bundle& bndl, const bool dadr,
			     const bool dbnd, const bool dins, 
			     const bool dfrc) {
    // check valid bundle
    if (bndl.isvalid () == false) return;
    // disassemble address
    if (dadr == true) {
      cout << "0x" ;
      cout << hex << setw (16) << setfill ('0') << bndl.getbip () << dec;
      cout << ' ';
    }
    // disassemble bundle
    if (dbnd == true) cout << bndl.tostring () << ' ';
    // disassemble instructions
    if (dins == true) {
      try {
	// slot 0
	Instr i0 = bndl.getinstr (0);
	if (i0.isvalid () == true) {
	  cout << i0.recode () << endl;
	}
      } catch (...) {
	if (dfrc == true) {
	  cout << "      " << disass_data8 (bndl.getslot (0)) << endl;
	} else {
	  throw;
	}
      }
      // slot 1
      try {
	Instr i1 = bndl.getinstr (1);
	if (i1.isvalid () == true) {
	  if (dadr == true) cout << "                  " << ' ';
	  if (dbnd == true) cout << "     "              << ' ';
	  cout << i1.recode () << endl;
	}
      } catch (...) {
	if (dfrc == true) {
	  if (dadr == true) cout << "                  " << ' ';
	  if (dbnd == true) cout << "     "              << ' ';
	  cout << "      " << disass_data8 (bndl.getslot (1)) << endl;
	} else {
	  throw;
	}
      }
      // slot 2
      try {
	Instr i2 = bndl.getinstr (2);
	if (i2.isvalid () == true) {
	  if (dadr == true) cout << "                  " << ' ';
	  if (dbnd == true) cout << "     "              << ' ';
	  cout << i2.recode () << endl;
	}
      } catch (...) {
	if (dfrc == true) {
	  if (dadr == true) cout << "                  " << ' ';
	  if (dbnd == true) cout << "     "              << ' ';
	  cout << "      " << disass_data8 (bndl.getslot (2)) << endl;
	} else {
	  throw;
	}
      }
    } else {
      cout << endl;
    }
  }

  // this procedure disassemble a section
  static void disass_section (ElfSection* scn, const bool dadr,
			      const bool dbnd, const bool dins, 
			      const bool dfrc) {
    if (!scn) return;
    // iterate throw the section
    typedef ElfSection::const_iterator t_bit;
    for (t_bit bit = scn->begin (); bit < scn->end (); bit++) {
      Bundle bndl = *bit;
      // disassemble bundle
      if (bndl.isvalid () == false)
	disass_error ("invalid bundle", bit.getip ());
      disass_bundle (bndl, dadr, dbnd, dins, dfrc);
    }
  }

  // create a new disassemble object with an image
  
  Disass::Disass (ElfImage* elf) {
    p_elf  = elf;
    d_dadr = false;
    d_dbnd = false;
    d_dins = false;
  }

  // set the report flag

  void Disass::setflag (const bool dadr,const bool dbnd,const bool dins,
			const bool dfrc) {
    d_dadr = dadr;
    d_dbnd = dbnd;
    d_dins = dins;
    d_dfrc = dfrc;
  }

  // disassemble this class
  
  void Disass::report (void) const {
    // try to get an elf text image
    ElfText* text = p_elf ? p_elf->gettxt () : 0;
    if (!text || (!d_dadr && !d_dbnd && !d_dins)) return;
    // get the number of section and loop
    long slen = text->length ();
    for (long i = 0; i < slen; i++) {
      ElfSection* scn = text->getscn (i);
      disass_section (scn, d_dadr, d_dbnd, d_dins, d_dfrc);
    }
  }
}
