// ---------------------------------------------------------------------------
// - Mta.cpp                                                                 -
// - iato:mac library - memory transaction adapter class implementation      -
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

#include "Prn.hpp"
#include "Mac.hpp"
#include "Mta.hpp"
#include "Interrupt.hpp"
#include "Intcode.hpp"

namespace {
  using namespace iato;

  static t_octa gather_integer (Memory* mem, const t_octa addr,
				const long size) {
    assert (mem != nullptr);
    t_octa value = OCTA_0;
    for (long i = 0; i < size; ++i) {
      const t_byte byte = mem->readbyte (addr + i);
      value |= static_cast<t_octa>(byte) << (i * 8);
    }
    return value;
  }

  static void scatter_integer (Memory* mem, const t_octa addr,
			       const long size, const t_octa value) {
    assert (mem != nullptr);
    for (long i = 0; i < size; ++i) {
      const t_byte byte = static_cast<t_byte>((value >> (i * 8)) & 0xFF);
      mem->writebyte (addr + i, byte);
    }
  }

  static void load_real (Memory* mem, const t_octa addr, const long size,
			 t_real& dst,
			 void (t_real::*loader)(const t_byte*)) {
    assert (mem != nullptr);
    assert (size <= t_real::TR_SFSZ);
    t_byte buf[t_real::TR_SFSZ];
    for (long i = 0; i < size; ++i) buf[i] = mem->readbyte (addr + i);
    (dst.*loader) (buf);
  }

  static void store_real (Memory* mem, const t_octa addr, const long size,
			  const t_real& src,
			  void (t_real::*storer)(t_byte*) const) {
    assert (mem != nullptr);
    assert (size <= t_real::TR_SFSZ);
    t_byte buf[t_real::TR_SFSZ];
    (src.*storer) (buf);
    for (long i = 0; i < size; ++i) mem->writebyte (addr + i, buf[i]);
  }

  static bool emulate_unaligned (Mrt& mrt, Memory* dmem, Memory* fmem) {
    const t_octa addr = mrt.getaddr ();
    switch (mrt.gettype ()) {
    case Mrt::REQ_LD2: {
      if (!dmem) return false;
      t_word value = static_cast<t_word>(gather_integer (dmem, addr, 2));
      mrt.setwval (value);
      return true;
    }
    case Mrt::REQ_LD4: {
      if (!dmem) return false;
      t_quad value = static_cast<t_quad>(gather_integer (dmem, addr, 4));
      mrt.setqval (value);
      return true;
    }
    case Mrt::REQ_LD8: {
      if (!dmem) return false;
      t_octa value = gather_integer (dmem, addr, 8);
      mrt.setoval (value);
      return true;
    }
    case Mrt::REQ_ST2:
      if (!dmem) return false;
      scatter_integer (dmem, addr, 2,
		       static_cast<t_octa>(mrt.getwval ()));
      return true;
    case Mrt::REQ_ST4:
      if (!dmem) return false;
      scatter_integer (dmem, addr, 4,
		       static_cast<t_octa>(mrt.getqval ()));
      return true;
    case Mrt::REQ_ST8:
      if (!dmem) return false;
      scatter_integer (dmem, addr, 8, mrt.getoval ());
      return true;
    case Mrt::REQ_LDS: {
      if (!fmem) return false;
      t_real value;
      load_real (fmem, addr, t_real::TR_SISZ, value, &t_real::singleld);
      mrt.setlval (value);
      return true;
    }
    case Mrt::REQ_LDD: {
      if (!fmem) return false;
      t_real value;
      load_real (fmem, addr, t_real::TR_DOSZ, value, &t_real::doubleld);
      mrt.setlval (value);
      return true;
    }
    case Mrt::REQ_LDE: {
      if (!fmem) return false;
      t_real value;
      load_real (fmem, addr, t_real::TR_DESZ, value, &t_real::extendedld);
      mrt.setlval (value);
      return true;
    }
    case Mrt::REQ_LDI: {
      if (!fmem) return false;
      t_real value;
      load_real (fmem, addr, sizeof (t_octa), value, &t_real::integerld);
      mrt.setlval (value);
      return true;
    }
    case Mrt::REQ_LDF: {
      if (!fmem) return false;
      t_real value;
      load_real (fmem, addr, t_real::TR_SFSZ, value, &t_real::fill);
      mrt.setlval (value);
      return true;
    }
    case Mrt::REQ_LPS: {
      if (!fmem) return false;
      t_real lval;
      t_real hval;
      load_real (fmem, addr, t_real::TR_SISZ, lval, &t_real::singleld);
      load_real (fmem, addr + 4, t_real::TR_SISZ, hval, &t_real::singleld);
      mrt.setlval (lval);
      mrt.sethval (hval);
      return true;
    }
    case Mrt::REQ_LPD: {
      if (!fmem) return false;
      t_real lval;
      t_real hval;
      load_real (fmem, addr, t_real::TR_DOSZ, lval, &t_real::doubleld);
      load_real (fmem, addr + 8, t_real::TR_DOSZ, hval, &t_real::doubleld);
      mrt.setlval (lval);
      mrt.sethval (hval);
      return true;
    }
    case Mrt::REQ_LPI: {
      if (!fmem) return false;
      t_real lval;
      t_real hval;
      load_real (fmem, addr, sizeof (t_octa), lval, &t_real::integerld);
      load_real (fmem, addr + 8, sizeof (t_octa), hval, &t_real::integerld);
      mrt.setlval (lval);
      mrt.sethval (hval);
      return true;
    }
    case Mrt::REQ_STS:
      if (!fmem) return false;
      store_real (fmem, addr, t_real::TR_SISZ, mrt.getlval (),
		  &t_real::singlest);
      return true;
    case Mrt::REQ_STD:
      if (!fmem) return false;
      store_real (fmem, addr, t_real::TR_DOSZ, mrt.getlval (),
		  &t_real::doublest);
      return true;
    case Mrt::REQ_STE:
      if (!fmem) return false;
      store_real (fmem, addr, t_real::TR_DESZ, mrt.getlval (),
		  &t_real::extendedst);
      return true;
    case Mrt::REQ_STI:
      if (!fmem) return false;
      store_real (fmem, addr, sizeof (t_octa), mrt.getlval (),
		  &t_real::integerst);
      return true;
    case Mrt::REQ_STF:
      if (!fmem) return false;
      store_real (fmem, addr, t_real::TR_SFSZ, mrt.getlval (),
		  &t_real::spill);
      return true;
    default:
      break;
    }
    return false;
  }
}

namespace iato {

  // create a default mta

  Mta::Mta (void) : Resource (RESOURCE_MTA) {
    d_iwsz = BN_IWSZ;
    d_bwsz = d_iwsz * BN_BYSZ;
    p_bndl = new Bundle[d_iwsz];
    p_bbuf = new t_byte[d_bwsz]; 
    p_imem = 0; 
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
    reset ();
  }

  // create a mta with a context
  
  Mta::Mta (Mtx* mtx) : Resource (RESOURCE_MTA) {
    d_iwsz = mtx->getiwsz (); assert (d_iwsz > 0);
    d_bwsz = mtx->getbwsz (); assert (d_bwsz > 0);
    p_bndl = new Bundle[d_iwsz];
    p_bbuf = new t_byte[d_bwsz]; 
    p_imem = 0; 
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
    reset ();
  }

  // create a mta with a context by name
  
  Mta::Mta (Mtx* mtx, const string& name) : Resource (name) {
    d_iwsz = mtx->getiwsz (); assert (d_iwsz > 0);
    d_bwsz = mtx->getbwsz (); assert (d_bwsz > 0);
    p_bndl = new Bundle[d_iwsz];
    p_bbuf = new t_byte[d_bwsz]; 
    p_imem = 0; 
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
    reset ();
  }

  // destroy this mta
  
  Mta::~Mta (void) {
    delete [] p_bndl;
    delete [] p_bbuf;
  }

  // reset this mta

  void Mta::reset (void) {
    for (long i = 0; i < d_iwsz; i++) p_bndl[i].reset ();
    for (long i = 0; i < d_bwsz; i++) p_bbuf[i] = BYTE_0;
  }
  
  // bind a memory to this mta

  void Mta::bind (Memory* mem) {
    p_imem = mem;
    p_dmem = mem;
    p_fmem = mem;
    p_bmem = mem;
  }

  // bind a memory to this mta by type

  void Mta::bind (t_tmem type, Memory* mem) {
    switch (type) {
    case IMEM:
      p_imem = mem;
      break;
    case DMEM:
      p_dmem = mem;
      break;
    case FMEM:
      p_fmem = mem;
      break;
    case BMEM:
      p_bmem = mem;
      break;
    }
  }

  // process a memory request

  void Mta::process (Mrt& mrt) {
    // check for valid request
    if (mrt.isvalid () == false) return;
    //  process request
    try {
      switch (mrt.gettype ()) {
      case Mrt::REQ_BND:
	update (mrt.getaddr ());
	break;
      case Mrt::REQ_LD1:
	mrt.setbval (p_dmem->readbyte (mrt.getaddr ()));
	break;
      case Mrt::REQ_LD2:
	mrt.setwval (p_dmem->readword (mrt.getaddr ()));
	break;
      case Mrt::REQ_LD4:
	mrt.setqval (p_dmem->readquad (mrt.getaddr ()));
	break;
      case Mrt::REQ_LD8:
	mrt.setoval (p_dmem->readocta (mrt.getaddr ()));
	break;
      case Mrt::REQ_LDS:
	mrt.setlval (p_fmem->readsing (mrt.getaddr ()));
	break;
      case Mrt::REQ_LDD:
	mrt.setlval (p_fmem->readdoub (mrt.getaddr ()));
	break;
      case Mrt::REQ_LDE:
	mrt.setlval (p_fmem->readxten (mrt.getaddr ()));
	break;
      case Mrt::REQ_LDI:
	mrt.setlval (p_fmem->readint (mrt.getaddr ()));
	break;
      case Mrt::REQ_LDF:
	mrt.setlval (p_fmem->readfill (mrt.getaddr ()));
	break;
      case Mrt::REQ_LPS:
	mrt.setlval (p_fmem->readsing (mrt.getaddr ()));
	mrt.sethval (p_fmem->readsing (mrt.getaddr () + 4));
	break;
      case Mrt::REQ_LPD:
	mrt.setlval (p_fmem->readdoub (mrt.getaddr ()));
	mrt.sethval (p_fmem->readdoub (mrt.getaddr () + 8));
	break;
      case Mrt::REQ_LPI:
	mrt.setlval (p_fmem->readint (mrt.getaddr ()));
	mrt.sethval (p_fmem->readint (mrt.getaddr () + 8));
	break;
      case Mrt::REQ_ST1:
	p_dmem->writebyte (mrt.getaddr (), mrt.getbval ());
	break;
      case Mrt::REQ_ST2:
	p_dmem->writeword (mrt.getaddr (), mrt.getwval ());
	break;
      case Mrt::REQ_ST4:
	p_dmem->writequad (mrt.getaddr (), mrt.getqval ());
	break;
      case Mrt::REQ_ST8:
	p_dmem->writeocta (mrt.getaddr (), mrt.getoval ());
	break;
      case Mrt::REQ_STS:
	p_fmem->writesing (mrt.getaddr (), mrt.getlval ());
	break;
      case Mrt::REQ_STD:
	p_fmem->writedoub (mrt.getaddr (), mrt.getlval ());
	break;
      case Mrt::REQ_STI:
	p_fmem->writeint  (mrt.getaddr (), mrt.getlval ());
	break;
      case Mrt::REQ_STE:
	p_fmem->writexten (mrt.getaddr (), mrt.getlval ());
	break;
      case Mrt::REQ_STF:
	p_fmem->writespill (mrt.getaddr (), mrt.getlval ());
	break;
      default:
	break;
      }
    } catch (const Interrupt& vi) {
      if ((vi.getcode () == FAULT_IT_DATA_ALIGN) &&
	  emulate_unaligned (mrt, p_dmem, p_fmem)) {
	return;
      }
      if (mrt.issbit () == true) {
	mrt.setnval (true);
      } else {
	throw vi;
      }
    }
  }

  // update a bundle array at a certain address

  void Mta::update (const t_octa bip) const {
    // check for valid bundle array
    if (!p_bndl) return;
    // try to access the bundle line
    long blen = 0;
    try {
      blen = p_imem->exbuf (bip, d_bwsz, p_bbuf);
    } catch (const Interrupt& vi) {
      // get the offending ip to adjust for buffer length
      t_octa oip = vi.getip ();
      if (oip > bip) {
	blen = (((long) (oip - bip)) / BN_BYSZ) * BN_BYSZ;
      } else {
	throw vi;
      }
    }
    // process the byte buffer into bunddle
    long   ilen = blen / BN_BYSZ; assert ((blen % BN_BYSZ) == 0);
    long   bpos = 0;
    t_octa biip = bip;
    for (long i = 0; i < ilen; i++) {
      p_bndl[i].reset ();
      for (long j = 0; j < BN_BYSZ; j++) {
	long idx = bpos + j;
	p_bndl[i].push (p_bbuf[idx]);
      }
      // set bundle ip if valid
      if (p_bndl[i].isvalid () == true) {
	p_bndl[i].setbip (biip);
	biip += BN_BYSZ;
      }
      // update byte position
      bpos += BN_BYSZ;
    }
    // reset the remaining bundle
    for (long i = ilen; i < d_iwsz; i++) p_bndl[i].reset ();
  }

  // return the mta bundle by index
  
  Bundle Mta::getbndl (const long index) const {
    assert ((index >= 0) && (index < d_iwsz));
    return p_bndl[index];
  }
}
