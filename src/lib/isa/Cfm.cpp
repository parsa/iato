// ---------------------------------------------------------------------------
// - Cfm.cpp                                                                 -
// - iato:isa library - current frame marker resource class implementation   -
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
#include "Cfm.hpp"

namespace iato {

  // create a default cfm

  Cfm::Cfm (void) {
    setcfm (DEF_CFM);
  }

  // create a cfm with a value

  Cfm::Cfm (const t_octa value) {
    setcfm (value);
  }

  // copy construct this cfm

  Cfm::Cfm (const Cfm& that) {
    d_cfm = that.d_cfm;
  }

  // assign a cfm to this one

  Cfm& Cfm::operator = (const Cfm& that) {
    d_cfm = that.d_cfm;
    return *this;
  }

  // return true if two cfm are equal

  bool Cfm::operator == (const Cfm& cfm) const {
    return (d_cfm == cfm.d_cfm);
  }

  // return true if a cfm match an octa

  bool Cfm::operator == (const t_octa val) const {
    return (d_cfm == val);
  }

  // return true if two cfm are not equal

  bool Cfm::operator != (const Cfm& cfm) const {
    return (d_cfm != cfm.d_cfm);
  }

  // return true if a cfm do not match an octa

  bool Cfm::operator != (const t_octa val) const {
    return (d_cfm != val);
  }

  // reset this cfm

  void Cfm::reset (void) {
    d_cfm = DEF_CFM;
  }

  // set the cfm value

  void Cfm::setcfm (const t_octa cfm) {
    d_cfm = cfm & 0x0000003FFFFFFFFFLL;
  }

  // return the current cfm

  t_octa Cfm::getcfm (void) const {
    return d_cfm;
  }

  // rotate this cfm by decrementing each register rename register base
  // modulo the rotating region

  t_octa Cfm::rotate (void) {
    // rotate the general register - if and only if sor is not 0
    t_byte sor = 8 * getfld (SOR);
    if (sor != 0) {
      t_byte rgr = getfld (RGR);
      t_byte ngr = (rgr == BYTE_0) ? GR_RRBM - 1 : 
	           sor - ((sor - rgr + 1) % sor);
      setfld (RGR, ngr);
    }
    // rotate the floating registers
    t_byte rfr = getfld (RFR);
    t_byte nfr = (rfr == BYTE_0) ? FR_RRBM - 1 : 
                 FR_RTSZ - ((FR_RTSZ - rfr + 1) % FR_RTSZ);
    setfld (RFR, nfr);
    // rotate the predicate registers
    t_byte rpr = getfld (RPR);
    t_byte npr = (rpr == BYTE_0) ? PR_RRBM - 1 : 
                 PR_RTSZ - ((PR_RTSZ - rpr + 1) % PR_RTSZ);
    setfld (RPR, npr);
    // return the new cfm value
    return d_cfm;
  }

  // this procedure returns the cfm mask by type
  static inline t_octa get_cfm_mask (Cfm::t_field fld) {
    t_octa mask = OCTA_0;
    switch (fld) {
    case Cfm::SOF:
      mask = ((t_octa) 0x7F);
      break;
    case Cfm::SOL:
      mask = ((t_octa) 0x7F) << 7;
      break;
    case Cfm::SOR:
      mask = ((t_octa) 0x0F) << 14;
      break;
    case Cfm::RGR:
      mask = ((t_octa) 0x7F) << 18;
      break;
    case Cfm::RFR:
      mask = ((t_octa) 0x7F) << 25;
      break;
    case Cfm::RPR:
      mask = ((t_octa) 0x3F) << 32;
      break;
    }
    return mask;
  }

  // set the cfm field

  void Cfm::setfld (t_field fld, t_byte val) {
    // mask cfm value
    t_octa mask = get_cfm_mask (fld);
    // compute value shift
    t_octa fval = val;
    switch (fld) {
    case SOF:
      break;
    case SOL:
      fval = fval << 7;
      break;
    case SOR:
      fval = fval << 14;
      break;
    case RGR:
      fval = fval << 18;
      break;
    case RFR:
      fval = fval << 25;
      break;
    case RPR:
      fval = fval << 32;
      break;
    }
    // update cfm value
    d_cfm = (d_cfm & ~mask) | (fval & mask);
  }

  // return a cfm field value

  t_byte Cfm::getfld (t_field fld) const {
    t_octa mask = get_cfm_mask (fld);
    t_octa fval = d_cfm & mask;
    switch (fld) {
    case SOF:
      break;
    case SOL:
      fval = fval >> 7;
      break;
    case SOR:
      fval = fval >> 14;
      break;
    case RGR:
      fval = fval >> 18;
      break;
    case RFR:
      fval = fval >> 25;
      break;
    case RPR:
      fval = fval >> 32;
      break;
    }
    return (t_byte) fval;
  }

  // return a rrb field value

  long Cfm::getrrb (t_field fld) const {
    long result = 0;
    t_byte rrb = getfld (fld);
    t_byte sor = getfld (SOR) == 0 ? GR_RRBM : getfld (SOR) * 8;
    switch (fld) {
    case RGR:
      result = (rrb == 0) ? 0 : ((long) rrb - GR_RRBM) % (long) sor;
      break;
    case RFR:
      result = (rrb == 0) ? 0 : ((long) rrb - FR_RRBM) % FR_RTSZ;
      break;
    case RPR:
      result = (rrb == 0) ? 0 : ((long) rrb - PR_RRBM) % PR_RTSZ;
      break;
    default:
      assert (false);
      break;
    }
    assert (result <= 0);
    return result;
  }

  // set a rrb field value

  void Cfm::setrrb (t_field fld, const long val) {
    switch (fld) {
    case RGR:
    case RFR:
    case RPR:
      setfld (fld, (t_byte) val);
      break;
    default:
      assert (false);
      break;
    }
  }
}
