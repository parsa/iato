// ---------------------------------------------------------------------------
// - Mli.cpp                                                                 -
// - iato:mac library - memory logic interface class implementation          -
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
#include "Mli.hpp"

namespace iato {
  namespace {
    inline bool is_ld_rop (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_LD1:
      case Result::REG_LD2:
      case Result::REG_LD4:
      case Result::REG_LD8:
        return true;
      default:
        return false;
      }
    }

    inline bool is_st_rop (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_ST1:
      case Result::REG_ST2:
      case Result::REG_ST4:
      case Result::REG_ST8:
        return true;
      default:
        return false;
      }
    }

    inline long rop_size (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_LD1:
      case Result::REG_ST1: return 1;
      case Result::REG_LD2:
      case Result::REG_ST2: return 2;
      case Result::REG_LD4:
      case Result::REG_ST4: return 4;
      case Result::REG_LD8:
      case Result::REG_ST8: return 8;
      default:              return 0;
      }
    }

    inline Mrt::t_mrtt rop_to_ldreq (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_LD1: return Mrt::REQ_LD1;
      case Result::REG_LD2: return Mrt::REQ_LD2;
      case Result::REG_LD4: return Mrt::REQ_LD4;
      case Result::REG_LD8: return Mrt::REQ_LD8;
      default:              return Mrt::REQ_NUL;
      }
    }

    inline bool is_fetchadd_rop (const Result::t_rop rop) {
      return (rop == Result::REG_FAD4) || (rop == Result::REG_FAD8);
    }

    inline long fad_size (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_FAD4: return 4;
      case Result::REG_FAD8: return 8;
      default:               return 0;
      }
    }

    inline Mrt::t_mrtt fad_load (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_FAD4: return Mrt::REQ_LD4;
      case Result::REG_FAD8: return Mrt::REQ_LD8;
      default:               return Mrt::REQ_NUL;
      }
    }

    inline Mrt::t_mrtt fad_store (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_FAD4: return Mrt::REQ_ST4;
      case Result::REG_FAD8: return Mrt::REQ_ST8;
      default:               return Mrt::REQ_NUL;
      }
    }
  }

  // create a default memory logic

  Mli::Mli (void) : Resource (RESOURCE_MLI) {
    p_mpr = new Mpr;
    p_mob = 0;
  }

  // create a memory logic by context

  Mli::Mli (Mtx* mtx) : Resource (RESOURCE_MLI) {
    p_mpr = new Mpr (mtx);
    p_mob = 0;
  }

  // create a memory logic by context and name

  Mli::Mli (Mtx* mtx, const string& name) : Resource (name) {
    p_mpr = new Mpr (mtx);
    p_mob = 0;
  }
 
  // reset this memory logic

  void Mli::reset (void) {
    if (p_mpr) p_mpr->reset ();
    if (p_mob) p_mob->reset ();
  }

  // report some resource information

  void Mli::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: memory logic interface" << endl;
  }

  // bind a memory request port

  void Mli::bind (Mta* mta, Mob* mob) {
    p_mpr->bind (mta);
    p_mob = mob;
  }

  // request a memory transaction by instruction and result

  void Mli::preset (const Dsi& inst, Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // Handle fetchadd inline (atomic fetch-and-add). We intentionally bypass
    // Mob bookkeeping here since Mob cannot represent a single instruction that
    // is both a load and a store.
    for (long i = 0; i < IA_MDST; ++i) {
      const Result::t_rop rop = resl.getrop (i);
      if (is_fetchadd_rop (rop) == false) continue;
      const long size = fad_size (rop);
      if (size == 0) continue;
      const t_octa addr   = resl.getaddr (i);
      const t_octa addend = resl.getimmv (i);
      // 1) load old value into destination register
      Mrt lmrt;
      lmrt.setld (fad_load (rop), addr, false, resl.getrid (i));
      p_mpr->preset (lmrt);
      if (p_mpr->istack () == true) resl.update (p_mpr->getmrt ());
      const t_octa oldv = resl.getoval (i);
      // 2) compute new value and store it
      Mrt smrt;
      if (size == 4) {
	const t_quad ov = static_cast<t_quad> (oldv);
	const t_quad av = static_cast<t_quad> (addend);
	const t_quad nv = static_cast<t_quad> (ov + av);
	const t_octa newv = static_cast<t_octa> (static_cast<t_quad> (nv));
	smrt.setst (fad_store (rop), addr, newv);
	smrt.setqval (nv);
      } else {
	const t_octa newv = oldv + addend;
	smrt.setst (fad_store (rop), addr, newv);
	smrt.setoval (newv);
      }
      p_mpr->update (smrt);
      return;
    }
    // Handle xchg-style combined load+store operations without calling
    // Result::getmrt() (which asserts if it sees multiple requests).
    long ld_idx = -1;
    long st_idx = -1;
    for (long i = 0; i < IA_MDST; ++i) {
      const Result::t_rop rop = resl.getrop (i);
      if ((ld_idx < 0) && is_ld_rop (rop)) ld_idx = i;
      if ((st_idx < 0) && is_st_rop (rop)) st_idx = i;
    }
    if ((ld_idx >= 0) && (st_idx >= 0)) {
      const long size = rop_size (resl.getrop (ld_idx));
      if ((size != 0) && (rop_size (resl.getrop (st_idx)) == size) &&
	  (resl.getaddr (ld_idx) == resl.getaddr (st_idx))) {
	Mrt lmrt;
	lmrt.setld (rop_to_ldreq (resl.getrop (ld_idx)),
		    resl.getaddr (ld_idx), false, resl.getrid (ld_idx));
	p_mob->preset (inst.getmob (), lmrt);
	p_mpr->preset (lmrt);
	if (p_mpr->istack () == true) resl.update (p_mpr->getmrt ());
	return;
      }
    }
    // chek for load
    if (inst.getldb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // preset the mob and mpr
    p_mob->preset (inst.getmob (), mrt);
    p_mpr->preset (mrt);
    // eventually update the result
    if (p_mpr->istack () == true) resl.update (p_mpr->getmrt ());	
  }

  // update a memory transaction by instruction and result

  void Mli::update (const Dsi& inst, const Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // fetchadd already handled during preset
    for (long i = 0; i < IA_MDST; ++i) {
      if (is_fetchadd_rop (resl.getrop (i)) == true) return;
    }
    // Handle xchg-style combined load+store stores without calling getmrt.
    long ld_idx = -1;
    long st_idx = -1;
    for (long i = 0; i < IA_MDST; ++i) {
      const Result::t_rop rop = resl.getrop (i);
      if ((ld_idx < 0) && is_ld_rop (rop)) ld_idx = i;
      if ((st_idx < 0) && is_st_rop (rop)) st_idx = i;
    }
    if ((ld_idx >= 0) && (st_idx >= 0)) {
      const long size = rop_size (resl.getrop (ld_idx));
      if ((size != 0) && (rop_size (resl.getrop (st_idx)) == size) &&
	  (resl.getaddr (ld_idx) == resl.getaddr (st_idx))) {
	const t_octa addr = resl.getaddr (st_idx);
	const t_octa newv = resl.getimmv (st_idx);
	Mrt smrt;
	switch (size) {
	case 1:
	  smrt.setst (Mrt::REQ_ST1, addr, static_cast<t_octa> (newv & 0xFFULL));
	  smrt.setbval (static_cast<t_byte> (newv & 0xFFULL));
	  break;
	case 2:
	  smrt.setst (Mrt::REQ_ST2, addr, static_cast<t_octa> (newv & 0xFFFFULL));
	  smrt.setwval (static_cast<t_word> (newv & 0xFFFFULL));
	  break;
	case 4:
	  smrt.setst (Mrt::REQ_ST4, addr, static_cast<t_octa> (newv & 0xFFFFFFFFULL));
	  smrt.setqval (static_cast<t_quad> (newv & 0xFFFFFFFFULL));
	  break;
	case 8:
	  smrt.setst (Mrt::REQ_ST8, addr, newv);
	  smrt.setoval (newv);
	  break;
	default:
	  break;
	}
	p_mob->update (inst.getmob (), smrt);
	p_mpr->update (smrt);
	return;
      }
    }
    // chek for store
    if (inst.getstb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // update the mob
    p_mob->update (inst.getmob (), mrt);
    // update the port
    p_mpr->update (mrt);
  }
}
