// ---------------------------------------------------------------------------
// - Bundle.cpp                                                              -
// - iato:isa library - bundle class implementation                          -
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

#include "Isa.hpp"
#include "Bundle.hpp"
#include "Interrupt.hpp"

namespace iato {
  using namespace std;

  // the bundle mask
  const t_byte BN_MASK = 0x1F;

  // the bundle encoding
  static const char* BN_CODE[] = {
    "MII" , "MII", "MII", "MII", 
    "MLX" , "MLX", "-f-", "-f-",
    "MMI" , "MMI", "MMI", "MMI",
    "MFI" , "MFI", "MMF", "MMF",
    "MIB" , "MIB", "MBB", "MBB",
    "-f-" , "-f-", "BBB", "BBB",
    "MMB" , "MMB", "-f-", "-f-",
    "MFB" , "MFB", "-f-", "-f-"
  };

  // the bundle encoding
  static const char* BN_REPR[] = {
    "MII  " , "MII| ", "MI|I ", "MI|I|", 
    "MLX  " , "MLX| ", "-f-  ", "-f-  ",
    "MMI  " , "MMI| ", "M|MI ", "M|MI|",
    "MFI  " , "MFI| ", "MMF  ", "MMF| ",
    "MIB  " , "MIB| ", "MBB  ", "MBB| ",
    "-f-  " , "-f-  ", "BBB  ", "BBB| ",
    "MMB  " , "MMB| ", "-f-  ", "-f-  ",
    "MFB  " , "MFB| ", "-f-  ", "-f-  "
  };

  // return the template string repesentation of a template with stop bits
  
  string Bundle::tostrws (const t_byte tmpl){
    return BN_REPR[tmpl & BN_MASK];
  }

  // return the template string repesentation of a template without stop bits

  string Bundle::tostrwo (const t_byte tmpl){
    return BN_CODE[tmpl & BN_MASK];
  }

  // create a default bundle

  Bundle::Bundle (void) {
    p_data = new t_byte[BN_BYSZ];
    reset ();
  }

  // copy construct a bundle

  Bundle::Bundle (const Bundle& that) {
    d_bip  = that.d_bip;
    d_sip  = that.d_sip;
    d_ssl  = that.d_ssl;
    d_blen = that.d_blen;
    p_data = new t_byte[BN_BYSZ];
    for (long i = 0; i < BN_BYSZ; i++) p_data[i] = that.p_data[i];
    for (long i = 0; i < BN_SLSZ; i++) d_vsb[i]  = that.d_vsb[i];
  }

  // destroy this bundle

  Bundle::~Bundle (void) {
    delete [] p_data;
  }
   
  // assign a bundle to this one

  Bundle& Bundle::operator = (const Bundle& that) {
    // check for equality and clean
    if (this == &that) return *this;
    delete [] p_data;
    // copy to assign
    d_bip  = that.d_bip;
    d_sip  = that.d_sip;
    d_ssl  = that.d_ssl;
    d_blen = that.d_blen;
    p_data = new t_byte[BN_BYSZ];
    for (long i = 0; i < BN_BYSZ; i++) p_data[i] = that.p_data[i];
    for (long i = 0; i < BN_SLSZ; i++) d_vsb[i]  = that.d_vsb[i];
    return *this;
  }

  // reset this bundle

  void Bundle::reset (void) {
    d_blen = 0;
    d_bip  = OCTA_0;
    d_sip  = OCTA_0;
    d_ssl  = -1;
    for (long i = 0; i < BN_BYSZ; i++) p_data[i] = BYTE_0;
    for (long i = 0; i < BN_SLSZ; i++) d_vsb[i] = true;
  }

  // return true if the bundle is valid

  bool Bundle::isvalid (void) const {
    // check the buffer width
    if ((d_blen == 0) || ((d_blen % BN_BYSZ) != 0)) return false;
    return true;
  }

  // return true if the bundle slot is a branch

  bool Bundle::isbr (const long slot) const {
    if (isvalid () == false) return false;
    try {
      Instr inst = getinstr (slot);
      if (inst.isvalid () == false) return false;
      return inst.isbr ();
    } catch (...) {
      return false;
    }
  }

  // set the bundle ip

  void Bundle::setbip (const t_octa ip) {
    d_bip = ip;
    d_sip = ip + BN_BYSZ;
  }

  // get the bundle ip

  t_octa Bundle::getbip (void) const {
    return d_bip;
  }

  // set the bundle speculative ip

  void Bundle::setsip (const t_octa sip, const long ssl) {
    // set speculative info
    d_sip = sip;
    d_ssl = ssl; assert ((d_ssl >= 0) && (d_ssl < BN_SLSZ));
    // invalidate remaining slots and exit
    for (long i = d_ssl+1; i < BN_SLSZ; i++) setvsb (i, false);
  }

  // get the bundle speculative ip

  t_octa Bundle::getsip (void) const {
    return d_sip;
  }

  // set the valid slot bit by index

  void Bundle::setvsb (const long slot, const bool flag) {
    assert ((slot >= 0) && (slot < BN_SLSZ));
    d_vsb[slot] = flag;
  }

  // return the valid slot bit

  bool Bundle::getvsb (const long slot) const {
    assert ((slot >= 0) && (slot < BN_SLSZ));
    return d_vsb[slot];
  }

  // return the bundle length

  long Bundle::length (void) const {
    return d_blen;
  }

  // push a byte in this bundle field

  void Bundle::push (const t_byte byte) {
    assert (d_blen < BN_BYSZ);
    p_data[d_blen++] = byte;
  }

  // get a byte value by index

  t_byte Bundle::get (const long index) const {
    assert ((index >= 0) && (index < d_blen));
    return p_data[index];
  }

  // get the bundle template

  t_byte Bundle::gettmpl (void) const {
    assert (isvalid () == true);
    return p_data[0] & BN_MASK;
  }

  // get the bundle instruction by slot

  t_octa Bundle::getslot (const long slot) const {
    assert (isvalid () == true);
    assert ((slot == 0) || (slot == 1) || (slot == 2));
    t_octa result = 0;
    long   bstart = 0;
    long   bstop  = 0;
    if (slot == 0) {
      bstart = 5;
      bstop  = 45;
    }
    if (slot == 1) {
      bstart = 46;
      bstop  = 86;
    }
    if (slot == 2) {
      bstart = 87;
      bstop  = 127;
    }
    // copy instruction
    for (long i = bstart, j = 0; i <= bstop; i+=8, j++) {
      t_octa byte = (t_octa) p_data[i/8];
      result |= (byte << j*8);
    }
    // normalize locally
    if (slot == 0) result = (result >> 5) & 0x000001FFFFFFFFFFLL;
    if (slot == 1) result = (result >> 6) & 0x000001FFFFFFFFFFLL;
    if (slot == 2) result = (result >> 7) & 0x000001FFFFFFFFFFLL;
    return result;
  }

  // get an instruction by slot index

  Instr Bundle::getinstr (const long slot) const {
    // get the bundle template
    t_byte tmpl = gettmpl ();
    t_octa ival = getslot (slot);
    // check for valid slot bit
    Instr inst;
    if (d_vsb[slot] == false) return inst;
    inst.setiip (d_bip);
    if ((d_ssl != -1) && (d_ssl == slot)) inst.setsip (d_sip);
    switch (tmpl) {
    case BN_MxIxIx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, false, ival);
      break;
    case BN_MxIxIs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, true,  ival);
      break;
    case BN_MxIsIx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, true,  ival);
      if (slot == 2) inst.decode (IUNIT, 2, false, ival);
      break;
    case BN_MxIsIs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, true,  ival);
      if (slot == 2) inst.decode (IUNIT, 2, true, ival);
      break;
    case BN_MxLxXx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 2) inst.decode (XUNIT, 2, false, ival, getslot (1));
      break;
    case BN_MxLxXs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 2) inst.decode (XUNIT, 2, true,  ival, getslot (1));
      break;
    case BN_MxMxIx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, false, ival);
      break;
    case BN_MxMxIs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, true,  ival);
      break;
    case BN_MsMxIx:
      if (slot == 0) inst.decode (MUNIT, 0, true,  ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, false, ival);
      break;
    case BN_MsMxIs:
      if (slot == 0) inst.decode (MUNIT, 0, true,  ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, true,  ival);
      break;
    case BN_MxFxIx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (FUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, false, ival);
      break;
    case BN_MxFxIs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (FUNIT, 1, false, ival);
      if (slot == 2) inst.decode (IUNIT, 2, true,  ival);
      break;
    case BN_MxMxFx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (FUNIT, 2, false, ival);
      break;
    case BN_MxMxFs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (FUNIT, 2, true,  ival);
      break;
    case BN_MxIxBx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, false, ival);
      break;
    case BN_MxIxBs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (IUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, true,  ival);
      break;
    case BN_MxBxBx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (BUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, false, ival);
      break;
    case BN_MxBxBs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (BUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, true,  ival);
      break;
    case BN_BxBxBx:
      if (slot == 0) inst.decode (BUNIT, 0, false, ival);
      if (slot == 1) inst.decode (BUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, false, ival);
      break;
    case BN_BxBxBs:
      if (slot == 0) inst.decode (BUNIT, 0, false, ival);
      if (slot == 1) inst.decode (BUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, true,  ival);
      break;
    case BN_MxMxBx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, false, ival);
      break;
    case BN_MxMxBs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (MUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, true,  ival);
      break;
    case BN_MxFxBx:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (FUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, false, ival);
      break;
    case BN_MxFxBs:
      if (slot == 0) inst.decode (MUNIT, 0, false, ival);
      if (slot == 1) inst.decode (FUNIT, 1, false, ival);
      if (slot == 2) inst.decode (BUNIT, 2, true,  ival);
      break;
    default:
      {
	ostringstream os;
	os << "invalid bundle template at 0x";
	os << hex << setw (16) << setfill ('0') << d_bip << dec;  
	throw Interrupt (FAULT_IT_OPER_LEGAL, os.str ());
      }
      break;
    }
    return inst;
  }

  // get a bundle representation

  string Bundle::tostring (void) const {
    t_byte tmpl = gettmpl ();
    ostringstream os;
    os << '[' <<  BN_CODE[tmpl] << ']';
    return os.str ();
  }

  // get a bundle representation with stop bit

  string Bundle::repr (void) const {
    t_byte tmpl = gettmpl ();
    return BN_REPR[tmpl];
  }  
}
