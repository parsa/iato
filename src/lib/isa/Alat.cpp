// ---------------------------------------------------------------------------
// - Alat.cpp                                                                -
// - iato:isa library - alat class implementation                            -
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
#include "Lrn.hpp"
#include "Alat.hpp"

namespace iato {

  // the alat structure
  struct t_alat {
    // the register tag
    t_word d_tag;
    // the load address
    t_octa d_addr;
    // the load size, 0 for invalid
    t_byte d_ldsz;
    // default constructor
    t_alat (void) {
      reset ();
    }
    // reset the alat structure
    void t_alat::reset (void) {
      d_tag   = WORD_0;
      d_addr  = OCTA_0;
      d_ldsz  = BYTE_0;
    }
  };

  // this function convert a memory request size to an address mask
  static t_octa get_addr_mask (const t_byte size) {
    t_octa mask = OCTA_0;
    switch (size) {
    case 0x01:
      mask = 0x0000000000000000ULL;
      break;
    case 0x02:
      mask = 0x0000000000000001ULL;
      break;
    case 0x04:
      mask = 0x0000000000000003ULL;
      break;
    case 0x08:
      mask = 0x0000000000000007ULL;
      break;
    case 0x0a:
    case 0x10:
      mask = 0x000000000000000fULL;
      break;      
    default:
      assert (false);
      break;
    }
    return mask;
  }

  // create a default alat

  Alat::Alat (void) : Resource (RESOURCE_ALT) {
    d_size = AL_SIZE;
    p_alat = new t_alat[d_size];
  }

  // create an alat with a context

  Alat::Alat (Ctx* ctx) : Resource (RESOURCE_ALT) {
    d_size = ctx->getlong ("ALAT-SIZE"); assert (d_size > 0);
    p_alat = new t_alat[d_size];
  }

  // create an alat with a context and name

  Alat::Alat (Ctx* ctx, const string& name) : Resource (name) {
    d_size = ctx->getlong ("ALAT-SIZE"); assert (d_size > 0);
    p_alat = new t_alat[d_size];
  }
  
  // destroy this alat
  
  Alat::~Alat (void) {
    delete [] p_alat;
  }

  // reset this alat

  void Alat::reset (void) {
    for (long i = 0; i < d_size; i++) {
      if ((p_tracer) && (isvalid (i) == true)) {
	Record rcd    (d_name);
	rcd.setalat   (p_alat[i].d_tag, p_alat[i].d_addr, p_alat[i].d_ldsz,
		       false);
	p_tracer->add (rcd);
      }
      p_alat[i].reset ();
    }
  }
  
  // report this alat

  void Alat::report (void) const {
    using namespace std;
    cout << "\tresource type\t\t: " << "alat" << endl;
    cout << "\talat size    \t\t: " << d_size << endl;
  }

  // return true if alat is full

  bool Alat::isfull (void) const {
    for (long i = 0; i < d_size; i++)
      if (p_alat[i].d_ldsz == BYTE_0)
	return false;
    return true;
  }

  // return true if the line ind is valid 
  bool Alat::isvalid (const t_word ind) const {
    assert (ind < d_size);
    return (p_alat[ind].d_ldsz != BYTE_0);
  }

  // get a tag

  t_word Alat::gettag (const Rid reg) const {
    t_lreg typ  = reg.gettype ();
    long   pnum = reg.getpnum (); 
    assert ((typ == FREG) || (typ == GREG));
    assert ((2 * pnum) < 0x000000000000FFFF);
    return (typ == FREG) ? 2*(t_word) pnum : (t_word) pnum;
  }

  // add an entrie to the alat

  void Alat::add (const t_octa addr, const Rid reg, const t_byte size) {
    t_word tag = gettag (reg);
    remove (reg);
    for (long i = 0; i < d_size; i++) 
      if (isvalid (i) == false) {
	p_alat[i].d_tag  = tag;
	p_alat[i].d_addr = addr;
	p_alat[i].d_ldsz = size;
	if (p_tracer) {
	  Record rcd    (d_name);
	  rcd.setalat   (tag, addr, size, true);
	  p_tracer->add (rcd);
	}
	return;
      }
  }

  // notify to alat that size bytes at memory address addr have been changed

  void Alat::memupd (const t_octa addr, const t_byte size) {
    t_octa mask = OCTA_0;
    for (long i = 0; i < d_size; i++) {
      if (isvalid (i) == false) continue;
      mask = (p_alat[i].d_ldsz < size) ? 
	get_addr_mask (size) : get_addr_mask (p_alat[i].d_ldsz);
      if ((addr & ~mask) == (p_alat[i].d_addr & ~mask)) {
	if (p_tracer) {
	  Record rcd    (d_name);
	  rcd.setalat   (p_alat[i].d_tag, p_alat[i].d_addr, p_alat[i].d_ldsz,
			 false);
	  p_tracer->add (rcd);
	}
	p_alat[i].reset ();
      }
    }
  }

  // remove an entry from the alat

  void Alat::remove (const Rid reg) {
    t_word tag = gettag (reg);
    for (long i = 0; i < d_size; i++) 
      if ((isvalid (i) == true) && (p_alat[i].d_tag == tag)) {
	if (p_tracer) {
	  Record rcd    (d_name);
	  rcd.setalat   (tag, p_alat[i].d_addr, p_alat[i].d_ldsz, false);
	  p_tracer->add (rcd);
	}
	p_alat[i].reset ();
      }
  }

  // check if this Rid matches into the Alat
  
  bool Alat::check (Rid reg) const {
    t_word tag = gettag (reg);
    for (long i = 0; i < d_size; i++) 
      if ((isvalid (i) == true) && (p_alat[i].d_tag == tag))
	return true;
    return false;
  }

  // determine if the load have to be done

  bool Alat::load (Result& resl, const long indx) {
    bool   result = false;
    long   size   = 0;
    // verify the result type correctness and set the size
    Result::t_rop type = resl.getrop (indx);
    switch (type) {
    case Result::REG_LD1:
      size = 1;
      break;
    case Result::REG_LD2:
      size = 2;
      break;
    case Result::REG_LD4:
      size = 4;
      break;
    case Result::REG_LD8:
      size = 8;
      break;
    case Result::REG_LDS:
      size = 4;
      break;
    case Result::REG_LDD:
      size = 8;
      break;
    case Result::REG_LDE:
      size = 10;
      break;
    case Result::REG_LDI:
      size = 8;
      break;
    default:
      assert (false);
      break;
    }
    // verify that check bit is set
    assert (resl.getachk (indx) == true);
    // does the load have to be done
    if (check (resl.getrid (indx)) == false) result = true;
    // does the alat have to clear this entrie
    if (resl.getaclr (indx) == true)
      remove (resl.getrid (indx));
    // does a new entrie have to be added
    if ((result == true) && (resl.getaclr (indx) == false)) {
      add (resl.getaddr (indx), resl.getrid  (indx), size);
    }
    if (result == false) resl.setinv (indx);
    return result;
  }

  // check and update result

  void Alat::check (Result& resl, const long indx) {
    assert (resl.getrop (indx) == Result::REG_CHK);
    if (check (resl.getrid (indx)) == true) {
      if (resl.getaclr (indx) == true)
	remove (resl.getrid (indx));
      for (long j = 0; j < IA_MDST; j++)
	if (resl.isreg (j, IPRG) == true)
	  resl.setinv (j);
      resl.setinv (indx);
    }
  }
}
