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
    // dispatch request
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
