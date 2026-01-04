// ---------------------------------------------------------------------------
// - MemLogic.hpp                                                            -
// - iato:isa library - memlogic class implementation                        -
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

#include "Lrn.hpp"
#include "Isa.hpp"
#include "MemLogic.hpp"
#include "Interrupt.hpp"

namespace iato {

  // helper that writes the compared value back to ar.ccv
  static void update_ccv (Result& resl, const t_octa value) {
    Rid ccv;
    ccv.setlnum (AREG, AR_CCV);
    resl.setbval (ccv, false);
    resl.setoval (ccv, value);
  }

  // create a default memory logic

  MemLogic::MemLogic (void) {
    p_alat = new Alat;
    p_mem  = 0; 
    d_name = RESOURCE_MLG;
    reset ();
  }

  // create a memory logic with a context
  
  MemLogic::MemLogic (Ctx* ctx) {
    p_alat = new Alat (ctx);
    d_name = RESOURCE_MLG;
    p_mem  = 0;
    reset ();
  }

  // destroy this memory logic
  
  MemLogic::~MemLogic (void) {
    delete p_alat;
  }

  // reset this memory logic
  void MemLogic::reset (void) {
    p_alat->reset ();
  }
  
  // bind a memory to this logic

  void MemLogic::bind (Memory* mem) {
    p_mem = mem;
  }

  // update the memory, the ALAT and a result

  void MemLogic::update (Result& resl) {
    for (long i = 0; i < IA_MDST; i++) {
      switch (resl.getrop (i)) {
      case Result::REG_CHK:
	p_alat->check (resl, i);
	break;
      case Result::REG_LD1:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setoval (i, p_mem->readbyte (resl.getaddr (i)));
	  // add an entrie to the alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 1);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  resl.setbval (i, true);
	}
	// add a memory read access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setrmem (resl.getaddr (i), resl.getoval (i), 0x01);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_LD2:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setoval (i, p_mem->readword (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 2);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  resl.setbval (i, true);
	}
	// add a memory read access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setrmem (resl.getaddr (i), resl.getoval (i), 0x02);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_LD4:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setoval (i, p_mem->readquad (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 4);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  resl.setbval (i, true);
	}
	// add a memory read access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setrmem (resl.getaddr (i), resl.getoval (i), 0x04);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_LD8:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setoval (i, p_mem->readocta (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 8);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  resl.setbval (i, true);
	}
	// add a memory read access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setrmem (resl.getaddr (i), resl.getoval (i), 0x08);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_LDS:
      case Result::RPL_LDS:
      case Result::RPH_LDS:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setrval (i, p_mem->readsing (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 4);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  t_real rval = 0.0; 
	  rval.setnat ();
	  resl.setrval (i, rval);
	}
	break;
      case Result::REG_LDD:
      case Result::RPL_LDD:
      case Result::RPH_LDD:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setrval (i, p_mem->readdoub (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 8);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  t_real rval = 0.0; 
	  rval.setnat ();
	  resl.setrval (i, rval);
	}
	break;
      case Result::REG_LDE:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setrval (i, p_mem->readxten (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 10);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  t_real rval = 0.0; 
	  rval.setnat ();
	  resl.setrval (i, rval);
	}
	break;
      case Result::REG_LDI:
      case Result::RPL_LDI:
      case Result::RPH_LDI:
	if (resl.getachk (i) == true)
	  if (p_alat->load (resl, i) == false) break;
	try {
	  resl.setrval (i, p_mem->readint (resl.getaddr (i)));
	  // add an entrie to alat if needed
	  if (resl.getaset (i) == true)
	    p_alat->add (resl.getaddr (i), resl.getrid (i), 8);
	} catch (const Interrupt& vi) {
	  if (resl.getspec (i) == false) throw vi;
	  t_real  rval; 
	  rval.setnat ();
	  resl.setrval (i, rval);
	}
	break;
      case Result::REG_LDF:
	resl.setrval (i, p_mem->readfill (resl.getaddr (i)));
	break;
      case Result::REG_ST1:
	p_mem->writebyte (resl.getaddr (i), resl.getimmv (i));
	p_alat->memupd (resl.getaddr (i), 1);
	// add a memory write record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setwmem (resl.getaddr (i), resl.getimmv (i), 0x01);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_ST2:
	p_mem->writeword (resl.getaddr (i), resl.getimmv (i));
	p_alat->memupd (resl.getaddr (i), 2);
	// add a memory write access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setwmem (resl.getaddr (i), resl.getimmv (i), 0x02);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_ST4:
	p_mem->writequad (resl.getaddr (i), resl.getimmv (i));
	p_alat->memupd (resl.getaddr (i), 4);
	// add a memory write access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setwmem (resl.getaddr (i), resl.getimmv (i), 0x04);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_ST8:
	p_mem->writeocta (resl.getaddr (i), resl.getimmv (i));
	p_alat->memupd (resl.getaddr (i), 8);
	// add a memory write access record
	if (p_tracer) {
	  Record rcd (d_name);
	  rcd.setwmem (resl.getaddr (i), resl.getimmv (i), 0x08);
	  p_tracer->add (rcd);
	}
	break;
      case Result::REG_STS:
	p_mem->writesing (resl.getaddr (i), resl.getrimv (i));
	p_alat->memupd (resl.getaddr (i), 4);
	break;
      case Result::REG_STD:
	p_mem->writedoub (resl.getaddr (i), resl.getrimv (i));
	p_alat->memupd (resl.getaddr (i), 8);
	break;
      case Result::REG_STI:
	p_mem->writeint (resl.getaddr (i), resl.getrimv (i));
	p_alat->memupd (resl.getaddr (i), 8);
	break;
      case Result::REG_STE:
	p_mem->writexten (resl.getaddr (i), resl.getrimv (i));
	p_alat->memupd (resl.getaddr (i), 10);
	break;
      case Result::REG_STF:
	p_mem->writespill (resl.getaddr (i), resl.getrimv (i));
	p_alat->memupd (resl.getaddr (i), 16);
	break;
      case Result::REG_CX1: {
	t_byte addrv = p_mem->readbyte (resl.getaddr (i));
	t_byte oval  = static_cast<t_byte> (resl.getoval (0));
	t_octa nval  = static_cast<t_octa> (addrv);
	resl.updoval (0, nval);
	update_ccv (resl, nval);
	if (addrv == oval) {
	  p_mem->writebyte (resl.getaddr (i),
			    static_cast<t_byte> (resl.getimmv (i)));
	  p_alat->memupd (resl.getaddr (i), 1);
	}
	break;
      }
      case Result::REG_CX2: {
	t_word addrv = p_mem->readword (resl.getaddr (i));
	t_word oval  = static_cast<t_word> (resl.getoval (0));
	t_octa nval  = static_cast<t_octa> (addrv);
	resl.updoval (0, nval);
	update_ccv (resl, nval);
	if (addrv == oval) {
	  p_mem->writeword (resl.getaddr (i),
			    static_cast<t_word> (resl.getimmv (i)));
	  p_alat->memupd (resl.getaddr (i), 2);
	}
	break;
      }
      case Result::REG_CX4: {
	t_quad addrv = p_mem->readquad (resl.getaddr (i));
	t_quad oval  = static_cast<t_quad> (resl.getoval (0));
	t_octa nval  = static_cast<t_octa> (addrv);
	resl.updoval (0, nval);
	update_ccv (resl, nval);
	if (addrv == oval) {
	  p_mem->writequad (resl.getaddr (i),
			    static_cast<t_quad> (resl.getimmv (i)));
	  p_alat->memupd (resl.getaddr (i), 4);
	}
	break;
      }
      case Result::REG_CX8: {
	t_octa oval  = resl.getoval (0);
	t_octa addrv = p_mem->readocta (resl.getaddr (i));
	resl.updoval (0, addrv);
	update_ccv (resl, addrv);
	if (addrv == oval) {
	  p_mem->writeocta (resl.getaddr (i), resl.getimmv (i));
	  p_alat->memupd (resl.getaddr (i), 8);
	}
	break;
      }
      case Result::ALT_INV:
	p_alat->reset ();
	resl.setinv (i);
      default:
	break;
      }
    }
  }
}
