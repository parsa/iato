// ---------------------------------------------------------------------------
// - Mapper.cpp                                                              -
// - iato:mac library - memory mapper class implementation                   -
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

#include "Urn.hpp"
#include "Ecu.hpp"
#include "Mapper.hpp"
#include "Interrupt.hpp"

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

    inline bool is_xchg_rop (const Result::t_rop rop) {
      return (rop == Result::REG_XCH1) || (rop == Result::REG_XCH2) ||
	     (rop == Result::REG_XCH4) || (rop == Result::REG_XCH8);
    }

    inline long xchg_size (const Result::t_rop rop) {
      switch (rop) {
      case Result::REG_XCH1: return 1;
      case Result::REG_XCH2: return 2;
      case Result::REG_XCH4: return 4;
      case Result::REG_XCH8: return 8;
      default:               return 0;
      }
    }

    inline t_octa mask_value (const t_octa value, const long size) {
      switch (size) {
      case 1: return value & 0xFFULL;
      case 2: return value & 0xFFFFULL;
      case 4: return value & 0xFFFFFFFFULL;
      default: return value;
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
  }
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
  }

  // create a default mapper

  Mapper::Mapper (void) : Resource (RESOURCE_MMP) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // create a mapper with a context
  
  Mapper::Mapper (Mtx* mtx) : Resource (RESOURCE_MMP) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // create a mapper with a context by name
  
  Mapper::Mapper (Mtx* mtx, const string& name) : Resource (name) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // reset this mapper
  
  void Mapper::reset (void) {
  }

  // bind a memory to this mapper

  void Mapper::bind (Memory* mem) {
    p_dmem = mem;
    p_fmem = mem;
    p_bmem = mem;
  }

  // bind a memory to this mapper by type

  void Mapper::bind (t_tmem type, Memory* mem) {
    switch (type) {
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

  void Mapper::process (Mrt& mrt) {
    // check for valid request
    if (mrt.isvalid () == false) return;
    // process request
    try {
      switch (mrt.gettype ()) {
      case Mrt::REQ_BND:
	assert (false);
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
      if (mrt.issbit () == true) {
	mrt.setnval (true);
      } else {
	throw vi;
      }
    }
  }

  // update a result with a memory request

  void Mapper::update (Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;

    // Handle cmpxchg/xchg results without going through Result::getmrt().
    // These instructions can be encoded as combined load+store semantics,
    // but Result::getmrt() only supports a single request and asserts if it
    // sees both.
    //
    // This mapper is used by the in-order emulator, so we directly perform
    // the memory effects and patch the register result.
    for (long i = 0; i < IA_MDST; ++i) {
      const Result::t_rop rop = resl.getrop (i);
      if (is_cmpxchg_rop (rop) == false) continue;
      const long size = cx_size (rop);
      if (size == 0) continue;
      const t_octa addr = resl.getaddr (i);
      const t_octa cmpv = mask_value (resl.getoval (0), size);
      const t_octa newv = mask_value (resl.getimmv (i), size);

      t_octa memv = OCTA_0;
      switch (size) {
      case 1: memv = static_cast<t_octa> (p_dmem->readbyte (addr)); break;
      case 2: memv = static_cast<t_octa> (p_dmem->readword (addr)); break;
      case 4: memv = static_cast<t_octa> (p_dmem->readquad (addr)); break;
      case 8: memv = p_dmem->readocta (addr); break;
      default: break;
      }
      memv = mask_value (memv, size);
      resl.updoval (0, memv);
      // update AR_CCV if declared as a destination (slot 1 by convention)
      Rid ccv = resl.getrid (1);
      if (ccv.isvalid () == true) resl.setoval (ccv, memv);

      if (memv == cmpv) {
        switch (size) {
        case 1: p_dmem->writebyte (addr, static_cast<t_byte> (newv)); break;
        case 2: p_dmem->writeword (addr, static_cast<t_word> (newv)); break;
        case 4: p_dmem->writequad (addr, static_cast<t_quad> (newv)); break;
        case 8: p_dmem->writeocta (addr, newv); break;
        default: break;
        }
      }
      return;
    }

    // Handle fetchadd directly (atomic fetch-and-add).
    for (long i = 0; i < IA_MDST; ++i) {
      const Result::t_rop rop = resl.getrop (i);
      if (is_fetchadd_rop (rop) == false) continue;
      const long size = fad_size (rop);
      if (size == 0) continue;
      const t_octa addr   = resl.getaddr (i);
      const t_octa addend = resl.getimmv (i);
      if (size == 4) {
	const t_quad oldq = p_dmem->readquad (addr);
	resl.setoval (i, static_cast<t_octa> (oldq));
	const t_quad newq = static_cast<t_quad> (oldq + static_cast<t_quad> (addend));
	p_dmem->writequad (addr, newq);
      } else {
	const t_octa oldv = p_dmem->readocta (addr);
	resl.setoval (i, oldv);
	const t_octa newv = oldv + addend;
	p_dmem->writeocta (addr, newv);
      }
      return;
    }

    // Detect an xchg-style combined load+store: one integer load and one
    // integer store of the same size to the same address.
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
        const t_octa addr = resl.getaddr (ld_idx);
        t_octa oldv = OCTA_0;
        switch (size) {
        case 1: oldv = static_cast<t_octa> (p_dmem->readbyte (addr)); break;
        case 2: oldv = static_cast<t_octa> (p_dmem->readword (addr)); break;
        case 4: oldv = static_cast<t_octa> (p_dmem->readquad (addr)); break;
        case 8: oldv = p_dmem->readocta (addr); break;
        default: break;
        }
        const t_octa newv = mask_value (resl.getimmv (st_idx), size);
        switch (size) {
        case 1: p_dmem->writebyte (addr, static_cast<t_byte> (newv)); break;
        case 2: p_dmem->writeword (addr, static_cast<t_word> (newv)); break;
        case 4: p_dmem->writequad (addr, static_cast<t_quad> (newv)); break;
        case 8: p_dmem->writeocta (addr, newv); break;
        default: break;
        }
        // xchg returns the old value in the load destination register
        resl.setoval (ld_idx, mask_value (oldv, size));
        return;
      }
    }

    // get associated mrt
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // process the mrt
    process (mrt);
    // update the result
    resl.update (mrt);
  } 
}
