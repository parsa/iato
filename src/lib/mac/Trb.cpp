// ---------------------------------------------------------------------------
// - Trb.cpp                                                                 -
// - iato:mac library - translation register bank class implementation       -
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

#include "Mac.hpp"
#include "Prn.hpp"
#include "Trb.hpp"

namespace iato {
  
  // this function computes the trb size - the trb must be able to handle
  // all isa registers, plus the inflight registers, plus the queued 
  // instructions
  static long get_trb_size (const Mtx* mtx) {
    long result = 0;
    if (mtx) {
      // try to get the setup trb size - if not available it is recomputed
      // from the architectural parameters. 
      result = mtx->getlong ("TRB-SIZE");
      if (result == 0) {
	// get architectural size
	result += mtx->getlong ("LR-GR-SIZE");
	result += mtx->getlong ("LR-FR-SIZE");
	result += mtx->getlong ("LR-PR-SIZE");
	result += mtx->getlong ("LR-BR-SIZE");
	result += mtx->getlong ("LR-AR-SIZE");
	result += mtx->getlong ("LR-CR-SIZE");
	result += mtx->getlong ("PR-UR-SIZE");
	// get queue size and number of units
	result += mtx->getlong ("EIQ-SIZE") * mtx->gettusz ();
      }
    } else {
      result += GR_LRSZ;
      result += FR_LRSZ;
      result += PR_LRSZ;
      result += BR_LRSZ;
      result += AR_LRSZ;
      result += CR_LRSZ;
      result += UR_PRSZ;
      // get queue size and unit defaults
      result += IQ_SIZE * (MK_MUSZ + MK_IUSZ + MK_FUSZ + MK_BUSZ);
    }
    return result;
  }

  // the translation register
  struct t_vtr {
    // the valid bit
    bool d_vbit;
    // the ready bit
    bool d_rbit;
    // force ready bit
    bool d_rfrc;
    // the translation bit
    bool d_tbit;
    // the register number
    long d_tnum;
    // default constructor
    t_vtr (void) {
      reset ();
    }
    // reset a translation
    void reset (void) {
      d_vbit = false;
      d_rbit = false;
      d_rfrc = false;
      d_tbit = false;
      d_tnum = -1;
    }
    // partial flush a trb entry
    void pflsh (void) {
      if (d_vbit == false) return;
      if ((d_tbit == false) && (d_rfrc == false)) return;
      d_tbit = false;
      d_rbit = false;
      d_tnum = -1;
    }

    // allocate a new trb entry
    bool alloc (void) {
      if (d_vbit == true) return false;
      d_vbit = true;
      d_rbit = false;
      d_tbit = false;
      d_tnum = -1;
      return true;
    }
    // clean a translation and mark it ready
    long clean (void) {
      assert (d_vbit == true);
      long result = d_tbit ? d_tnum : -1;
      d_rbit = true;
      d_rfrc = false;
      d_tbit = false;
      d_tnum = -1;
      return result;
    }
    // force ready bit and mark it
    void setrdy (void) {
      assert (d_vbit == true);
      assert (d_tbit == false);
      d_rbit = true;
      d_rfrc = true;
      d_tbit = false;
      d_tnum = -1;
    }
    // cancel a translation, like clean but assert tbit
    void cancel (void) {
      assert (d_vbit == true);
      assert (d_tbit == false);
      d_rbit = true;
      d_tbit = false;
      d_tnum = -1;
    }
  };

  // create a default translation bank

  Trb::Trb (void) : Resource (RESOURCE_TRB) {
    d_size = get_trb_size (0);
    p_vtrb = new t_vtr[d_size];
    reset ();
  }

  // create a translation bank by context

  Trb::Trb (Mtx* mtx) : Resource (RESOURCE_TRB) {
    d_size = get_trb_size (mtx);
    p_vtrb = new t_vtr[d_size];
    reset ();
  }

  // create a translation bank by context and name

  Trb::Trb (Mtx* mtx, const string& name) : Resource (name) {
    d_size = get_trb_size (mtx);
    p_vtrb = new t_vtr[d_size];
    reset ();
  }

  // destroy this translation bank

  Trb::~Trb (void) {
    delete [] p_vtrb;
  }

  // reset this translation bank

  void Trb::reset (void) {
    for (long i = 0; i < d_size; i++) p_vtrb[i].reset ();
  }

  // partial flush this trb

  void Trb::pflsh (void) {
    for (long i = 0; i < d_size; i++) p_vtrb[i].pflsh ();
  }

  // report this resource

  void Trb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "translation register bank" << endl;
    cout << "\tbank size    \t\t: " << d_size << endl;
  }

  // return true if a trb entry is ready

  bool Trb::isready (const long vnum) const {
    assert ((vnum >= 0) && (vnum < d_size));
    assert (p_vtrb[vnum].d_vbit == true);
    return p_vtrb[vnum].d_rbit;
  }

  // return the translation bit

  bool Trb::gettbit (const long vnum) const {
    assert ((vnum >= 0) && (vnum < d_size));
    assert (p_vtrb[vnum].d_vbit == true);
    return p_vtrb[vnum].d_tbit;
  }

  // set the trb translation number

  void Trb::settnum (const long vnum, const long tnum, const bool rbit) {
    assert ((vnum >= 0) && (vnum < d_size));
    assert (p_vtrb[vnum].d_vbit == true);
    p_vtrb[vnum].d_tbit = true;
    p_vtrb[vnum].d_tnum = tnum;
    p_vtrb[vnum].d_rbit = rbit;
  }
    
  // return the translation number

  long Trb::gettnum (const long vnum) const {
    assert ((vnum >= 0) && (vnum < d_size));
    assert (p_vtrb[vnum].d_vbit == true);
    assert (p_vtrb[vnum].d_tbit == true);
    return p_vtrb[vnum].d_tnum;
  }

  // allocate an initial translation

  long Trb::initial (void) {
    // allocate a new entry
    long index = alloc ();
    assert (index != -1);
    // mark it ready without translation
    p_vtrb[index].d_rbit = true;
    return index;
  }
    
  // allocate a new trb and return its index

  long Trb::alloc (void) {
    for (long i = 0; i < d_size; i++) {
      if (p_vtrb[i].alloc () == true) return i;
    }
    assert (false);
    return -1;
  }

  // clean the trb entry and return the old translation

  long Trb::clean (const long vnum, const long onum) {
    assert ((vnum >= 0) && (vnum < d_size));
    assert ((onum >= 0) && (onum < d_size));
    // clean old translation
    p_vtrb[onum].reset ();
    // get translation and reset
    long tnum = p_vtrb[vnum].clean ();
    return tnum;
  }

  // cancel a trb translation

  void Trb::cancel (const long vnum, const long onum) {
    assert ((vnum >= 0) && (vnum < d_size));
    assert ((onum >= 0) && (onum < d_size));
    // clean old translation
    p_vtrb[onum].reset ();
    // cancel translation
    p_vtrb[vnum].cancel ();
  }

  // force an entry to be ready, this is similar
  // to an initial setting, but without allocation

  void Trb::setrdy (const long vnum) {
    assert ((vnum >= 0) && (vnum < d_size));
    p_vtrb[vnum].setrdy ();
  }
}
