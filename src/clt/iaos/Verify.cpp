// ---------------------------------------------------------------------------
// - Verify.cpp                                                             -
// - iato:iaos client - text verify class implementation                     -
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

#include "Verify.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // this procedure throws an exception with a message and a ip
  static void verify_error (const string& msg, const t_octa ip) {
    ostringstream os;
    os << msg << "at ip 0x";
    os << hex << setw (16) << setfill ('0') << ip;
    throw Exception ("verify-error", os.str ());
  }

  // this procedure verify that an instruction van be decoded
  static void verify_instruction (const Bundle& bndl) {
    if (bndl.isvalid () == false) return;
    // check the instruction for a bundle, if the bundle is of
    // type MLX, only slot 0 and 2 needs to be verified
    t_byte tmpl = bndl.gettmpl ();
    t_octa  bip = bndl.getbip  ();
    if ((tmpl == Bundle::BN_MxLxXx) || (tmpl == Bundle::BN_MxLxXs)) {
      Instr i0 = bndl.getinstr (0);
      if (i0.isvalid () == false) 
	verify_error ("invalid slot 0 instruction", bip);
      Instr i2 = bndl.getinstr (2);
      if (i2.isvalid () == false) 
	verify_error ("invalid slot 2 instruction", bip);
    } else {
      Instr i0 = bndl.getinstr (0);
      if (i0.isvalid () == false) 
	verify_error ("invalid slot 0 instruction", bip);
      Instr i1 = bndl.getinstr (1);
      if (i1.isvalid () == false) 
	verify_error ("invalid slot 1 instruction", bip);
      Instr i2 = bndl.getinstr (2);
      if (i2.isvalid () == false) 
	verify_error ("invalid slot 2 instruction", bip);
    }
  }

  // this procedure verify the bundle for a section
  static void verify_section (ElfSection* scn, const bool vins) {
    if (!scn) return;
    // iterate throw the section
    typedef ElfSection::const_iterator t_bit;
    for (t_bit bit = scn->begin (); bit < scn->end (); bit++) {
      Bundle bndl = *bit;
      // verify bundle
      if (bndl.isvalid () == false)
	verify_error ("invalid bundle", bit.getip ());
      // verify instructions
      if (vins == true) verify_instruction (bndl);
    }
  }

  // create a new verify object with an image
  
  Verify::Verify (ElfImage* elf) {
    p_elf  = elf;
    d_vbnd = false;
    d_vins = false;
  }

  // set the report flag

  void Verify::setflag (const bool vbnd, const bool vins) {
    d_vins = vins;
    d_vbnd = vbnd || d_vins;
  }

  // verify this class
  
  void Verify::verify (void) const {
    // try to get an elf text image
    ElfText* text = p_elf ? p_elf->gettxt () : 0;
    if (!text || (d_vbnd == false)) return;
    // get the number of section and loop
    long slen = text->length ();
    for (long i = 0; i < slen; i++) {
      ElfSection* scn = text->getscn (i);
      verify_section (scn, d_vins);
    }
  }
}
