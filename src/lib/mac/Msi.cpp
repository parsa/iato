// ---------------------------------------------------------------------------
// - Msi.cpp                                                                 -
// - iato:mac library - memory synchro interface class implementation        -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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
#include "Msi.hpp"
#include "Result.hpp"

namespace iato {

  namespace {
    inline bool is_cmpxchg_rop (const Result::t_rop rop) {
      return (rop == Result::REG_CX1) || (rop == Result::REG_CX2) ||
	     (rop == Result::REG_CX4) || (rop == Result::REG_CX8);
    }

    inline long cx_size (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_CX1: return 1;
      case Result::REG_CX2: return 2;
      case Result::REG_CX4: return 4;
      case Result::REG_CX8: return 8;
      default:              return 0;
      }
    }

    inline Mrt::t_mrtt cx_load (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_CX1: return Mrt::REQ_LD1;
      case Result::REG_CX2: return Mrt::REQ_LD2;
      case Result::REG_CX4: return Mrt::REQ_LD4;
      case Result::REG_CX8: return Mrt::REQ_LD8;
      default:              return Mrt::REQ_NUL;
      }
    }

    inline Mrt::t_mrtt cx_store (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_CX1: return Mrt::REQ_ST1;
      case Result::REG_CX2: return Mrt::REQ_ST2;
      case Result::REG_CX4: return Mrt::REQ_ST4;
      case Result::REG_CX8: return Mrt::REQ_ST8;
      default:              return Mrt::REQ_NUL;
      }
    }

    inline t_octa mask_value (const t_octa value, const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_CX1: return value & 0xFFULL;
      case Result::REG_CX2: return value & 0xFFFFULL;
      case Result::REG_CX4: return value & 0xFFFFFFFFULL;
      case Result::REG_CX8: return value;
      default:              return value;
      }
    }
  }

  // create a default memory logic

  Msi::Msi (void) : Resource (RESOURCE_MSI) {
    p_mpr = new Mpr;
  }

  // create a memory logic by context

  Msi::Msi (Mtx* mtx) : Resource (RESOURCE_MSI) {
    p_mpr = new Mpr (mtx);
  }

  // create a memory logic by context and name

  Msi::Msi (Mtx* mtx, const string& name) : Resource (name) {
    p_mpr = new Mpr (mtx);
  }
 
  // reset this memory logic

  void Msi::reset (void) {
    if (p_mpr) p_mpr->reset ();
  }

  // report some resource information

  void Msi::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: memory synchro interface" << endl;
  }

  // bind a memory request port

  void Msi::bind (Mta* mta) {
    p_mpr->bind (mta);
  }

  // request a memory transaction by instruction and result

  void Msi::preset (const Ssi& inst, Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
  // handle cmpxchg instructions inline
  for (long i = 0; i < IA_MDST; ++i) {
    Result::t_rop rop = resl.getrop (i);
    if (is_cmpxchg_rop (rop) == false) continue;
    long size = cx_size (rop);
    if (size == 0) continue;
    t_octa addr = resl.getaddr (i);
    t_octa cmpv = mask_value (resl.getoval (0), rop);
    t_octa newv = mask_value (resl.getimmv (i), rop);
    // issue load request and update destination register
    Mrt lmrt;
    lmrt.setld (cx_load (rop), addr, false, resl.getrid (0));
    p_mpr->preset (lmrt);
    if (p_mpr->istack () == true) {
      Mrt tmrt = p_mpr->getmrt ();
      resl.update (tmrt);
    }
    t_octa memv = mask_value (resl.getoval (0), rop);
    resl.setbval (0, false);
    // update AR_CCV if the instruction declared it
    Rid ccv = resl.getrid (1);
    if (ccv.isvalid () == true) {
      resl.setbval (ccv, false);
      resl.setoval (ccv, memv);
    }
    // perform the conditional store when compare succeeds
    if (memv == cmpv) {
      Mrt smrt;
      switch (rop) {
      case Result::REG_CX1:
	smrt.setst (cx_store (rop), addr, static_cast<t_octa> (newv & 0xFFULL));
	smrt.setbval (static_cast<t_byte> (newv & 0xFFULL));
	break;
      case Result::REG_CX2:
	smrt.setst (cx_store (rop), addr, static_cast<t_octa> (newv & 0xFFFFULL));
	smrt.setwval (static_cast<t_word> (newv & 0xFFFFULL));
	break;
      case Result::REG_CX4:
	smrt.setst (cx_store (rop), addr, static_cast<t_octa> (newv & 0xFFFFFFFFULL));
	smrt.setqval (static_cast<t_quad> (newv & 0xFFFFFFFFULL));
	break;
      case Result::REG_CX8:
	smrt.setst (cx_store (rop), addr, newv);
	smrt.setoval (newv);
	break;
      default:
	break;
      }
      p_mpr->update (smrt);
    }
    return;
  }
    // chek for load
    if (inst.getldb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // preset the mpr
    p_mpr->preset (mrt);
    // eventually update the result
    if (p_mpr->istack () == true) {
      Mrt tmrt = p_mpr->getmrt ();
      resl.update (tmrt);
    }
  }

  // update a memory transaction by instruction and result

  void Msi::update (const Ssi& inst, const Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // cmpxchg already handled during preset
    for (long i = 0; i < IA_MDST; ++i) {
      if (is_cmpxchg_rop (resl.getrop (i)) == true) return;
    }
    // chek for store
    if (inst.getstb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // update the port
    p_mpr->update (mrt);
  }
}
