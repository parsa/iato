// ---------------------------------------------------------------------------
// - Register.cpp                                                            -
// - iato:isa library - register class implementation                        -
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
#include "Bits.hpp"
#include "Intcode.hpp"
#include "Register.hpp"

namespace iato {

  // create a new default register bank

  Register::Register (void) : Resource (RESOURCE_RBK) {
    d_gnum = GR_LRSZ;
    d_fnum = FR_LRSZ;
    d_pnum = PR_LRSZ;
    d_bnum = BR_LRSZ;
    d_anum = AR_LRSZ;
    d_cnum = CR_LRSZ;
    p_greg = new t_octa[d_gnum];
    p_gbnk = new t_octa[GR_BKSZ];
    p_nreg = new bool[d_gnum];
    p_nbnk = new bool[GR_BKSZ];
    p_freg = new t_real[d_fnum];
    p_preg = new bool[d_pnum];
    p_breg = new t_octa[d_bnum];
    p_areg = new t_octa[d_anum];
    p_creg = new t_octa[d_cnum];
    reset ();
  }

  // create a register bank with a context

  Register::Register (Ctx* ctx) : Resource (RESOURCE_RBK) {
    d_gnum = ctx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum = ctx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum = ctx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum = ctx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum = ctx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum = ctx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    p_greg = new t_octa[d_gnum];
    p_gbnk = new t_octa[GR_BKSZ];
    p_nreg = new bool[d_gnum];
    p_nbnk = new bool[GR_BKSZ];
    p_freg = new t_real[d_fnum];
    p_preg = new bool[d_pnum];
    p_breg = new t_octa[d_bnum];
    p_areg = new t_octa[d_anum];
    p_creg = new t_octa[d_cnum];
    reset ();
  }

  // create a register bank with a context and name

  Register::Register (Ctx* ctx, const string& name) : Resource (name) {
    d_gnum = ctx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum = ctx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum = ctx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum = ctx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum = ctx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum = ctx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    p_greg = new t_octa[d_gnum];
    p_gbnk = new t_octa[GR_BKSZ];
    p_nreg = new bool[d_gnum];
    p_nbnk = new bool[GR_BKSZ];
    p_freg = new t_real[d_fnum];
    p_preg = new bool[d_pnum];
    p_breg = new t_octa[d_bnum];
    p_areg = new t_octa[d_anum];
    p_creg = new t_octa[d_cnum];
    reset ();
  }

  // destroy this register bank

  Register::~Register (void) {
    delete [] p_greg;
    delete [] p_gbnk;
    delete [] p_nreg;
    delete [] p_nbnk;
    delete [] p_freg;
    delete [] p_preg;
    delete [] p_breg;
    delete [] p_areg;
    delete [] p_creg;
  }

  // reset the register bank

  void Register::reset (void) {
    for (long i = 0; i < d_gnum; i++) {
      p_greg[i] = OCTA_0;
      p_nreg[i] = false;
    }
    for (long i = 0; i < GR_BKSZ; i++) {
      p_gbnk[i] = OCTA_0;
      p_nbnk[i] = false;
    }
    p_freg[0] = 0.0L;
    p_freg[1] = 1.0L;
    for (long i = 2; i < d_fnum; i++) {
      p_freg[i] = 0.0L;
    }
    p_preg[0] = true;
    for (long i = 1; i < d_pnum; i++) {
      p_preg[i] = false;
    }
    for (long i = 0; i < d_bnum; i++) {
      p_breg[i] = OCTA_0;
    }
    for (long i = 0; i < d_anum; i++) {
      p_areg[i] = OCTA_0;
    }
    for (long i = 0; i < d_cnum; i++) {
      p_creg[i] = OCTA_0;
    }
    d_ip.reset  ();
    d_cfm.reset ();
    d_psr.reset ();
  }

  // report this register bank

  void Register::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "register bank" << endl;
    cout << "\tGR bank size \t\t: " << d_gnum << endl;
    cout << "\tFR bank size \t\t: " << d_fnum << endl;
    cout << "\tPR bank size \t\t: " << d_pnum << endl;
    cout << "\tBR bank size \t\t: " << d_bnum << endl;
    cout << "\tAR bank size \t\t: " << d_anum << endl;
    cout << "\tCR bank size \t\t: " << d_cnum << endl;
  }

  // write a register value by type and index

  void Register::write (t_lreg lreg, const long index, const t_octa value) {
    assert (index >= 0);
    switch (lreg) {
    case GREG:
      assert (index < d_gnum);
      // check index for bank selection
      if ((index >= GR_BKBS) && (index < GR_BKTP)) {
	// check active bank
	if (d_psr.getfld (Psr::BN) == true) {
	  p_greg[index] = value;
	} else {
	  p_gbnk[index-GR_BKSZ] = value;
	}
      } else {
	p_greg[index] = value;
      }
      break;
    case BREG:
      assert (index < d_bnum);
      p_breg[index] = value;
      break;
    case AREG:
      assert (index < d_anum);
      p_areg[index] = value;
      break;
    case CREG:
      assert (index < d_cnum);
      p_creg[index] = value;
      break;
    case IPRG:
      assert (index == 0);
      d_ip.setip (value);
      break;
    case CFMR:
      assert (index == 0);
      d_cfm.setcfm (value);
      break;
    case PSRG:
      assert (index == 0);
      d_psr.setpsr (value);
      break;
    case PRRG:
      assert (index == 0);
      for (long i = 1; i < PR_LRSZ; i++) 
	write (PREG, i, bsetget (value, i));
      break;
    case PROT:
      assert (index == 0);
      for (long i = PR_RTBS; i < PR_LRSZ; i++) 
	write (PREG, i, bsetget (value, i));
      break;
    default:
      assert (false);
      break;
    }
  }

  // write a register value by type and index

  void Register::write (t_lreg lreg, const long index, const bool value) {
    assert (index >= 0);
    switch (lreg) {
    case NREG:
      assert (index < d_gnum);
      // check index for bank selection
      if ((index >= GR_BKBS) && (index < GR_BKTP)) {
	// check active bank
	if (d_psr.getfld (Psr::BN) == true) {
	  p_nreg[index] = value;
	} else {
	  p_nbnk[index-GR_BKSZ] = value;
	}
      } else {
	p_nreg[index] = value;
      }
      break;
    case PREG:
      assert (index < d_pnum);
      p_preg[index] = value;
      break;
    default:
      assert (false);
      break;
    }
  }

  // write a register value by type and index

  void Register::write (t_lreg lreg, const long index, const t_real& value) {
    assert (index >= 0);
    switch (lreg) {
    case FREG:
      assert (index < d_fnum);
      p_freg[index] = value;
      break;
    default:
      assert (false);
      break;
    }
  }

  // write a result to the register bank

  void Register::write (const Result& resl) {
    if (resl.isvalid () == false) return;
    for (long i = 0; i < IA_MDST; i++) {
      // do nothing with none or reroute operation
      Result::t_rop rop = resl.getrop (i);
      if ((rop == Result::ROP_NOP) || (rop == Result::REG_RRT)) continue;
      // get rid for validity
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      if (rid.islrnm  () == false) continue;
      t_lreg lreg = rid.gettype ();
      long   pnum = rid.getpnum ();
      switch (lreg) {
      case GREG:
	write (GREG, pnum, resl.getoval (i));
	write (NREG, pnum, resl.getbval (i));
	break;
      case NREG:
	write (NREG, pnum, resl.getbval (i));
	break;
      case FREG:
	write (FREG, pnum, resl.getrval (i));
	break;
      case PREG:
	write (PREG, pnum, resl.getbval (i));
	break;
      case BREG:
	write (BREG, pnum, resl.getoval (i));
	break;
      case AREG:
	write (AREG, pnum, resl.getoval (i));
	break;
      case CREG:
	write (CREG, pnum, resl.getoval (i));
	break;
      case IPRG:
	write (IPRG, 0, resl.getoval (i));
	break;
      case CFMR:
	write (CFMR, 0, resl.getoval (i));
	break;
      case PSRG:
	write (PSRG, 0, resl.getoval (i));
	break;
      case PRRG:
	write (PRRG, 0, resl.getoval (i));
	break;
      case PROT:
	write (PROT, 0, resl.getoval (i));
	break;
      default:
	break;
      }
      // use the tracer to report update
      if (p_tracer) {
	Record rcd    (d_name);
	rcd.setresl   (resl, i);
	p_tracer->add (rcd);
      }
    }
  }

  // read a register value by type and index

  t_octa Register::getoval (t_lreg lreg, const long index) const {
    assert (index >= 0);
    t_octa result = OCTA_0;
    switch (lreg) {
    case GREG:
      assert (index < d_gnum);
      // check index for bank selection
      if ((index >= GR_BKBS) && (index < GR_BKTP)) {
	// check active bank
	if (d_psr.getfld (Psr::BN) == true) {
	  result = p_greg[index];
	} else {
	  result = p_gbnk[index-GR_BKSZ];
	}
      } else {
	result = p_greg[index];
      }
      break;
    case BREG:
      assert (index < d_bnum);
      result = p_breg[index];
      break;
    case AREG:
      assert (index < d_anum);
      result = p_areg[index];
      break;
    case IPRG:
      assert (index == 0);
      result = d_ip.getip ();
      break;
    case CFMR:
      assert (index == 0);
      result = d_cfm.getcfm ();
      break;
    case PSRG:
      assert (index == 0);
      result = d_psr.getpsr ();
      break;
    case CREG:
      assert (index < d_cnum);
      result = p_creg[index];
      break;
    case PRRG:
      for (long i = 0; i < PR_LRSZ; i++) 
	result = bsetocta (result, i, p_preg[i]);
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // read a register value by type and index

  bool Register::getbval (t_lreg lreg, const long index) const {
    assert (index >= 0);
    bool result = false;
    switch (lreg) {
    case NREG:
      assert (index < d_gnum);
      // check index for bank selection
      if ((index >= GR_BKBS) && (index < GR_BKTP)) {
	// check active bank
	if (d_psr.getfld (Psr::BN) == true) {
	  result = p_nreg[index];
	} else {
	  result = p_nbnk[index-GR_BKSZ];
	}
      } else {
	result = p_nreg[index];
      }
      break;
    case PREG:
      assert (index < d_pnum);
      result = p_preg[index];
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // read a register value by type and index

  t_real Register::getrval (t_lreg lreg, const long index) const {
    assert (index >= 0);
    t_real result = 0.0L;
    switch (lreg) {
    case FREG:
      assert (index < d_fnum);
      result = p_freg[index];
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // evaluate a octa register register

  t_octa Register::getoval (const Rid& rid) const {
    assert (rid.isvalid () == true);
    // get register type
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    long   pnum = rid.getpnum ();
    if ((lreg == GREG) && (lnum == 0)) return OCTA_0;
    return getoval (lreg, pnum);
  }

  // evaluate a boolean register

  bool Register::getbval (const Rid& rid) const {
    assert (rid.isvalid () == true);
    // get register type
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    long   pnum = rid.getpnum ();
    if (lnum == 0) return true;
    return getbval (lreg, pnum);
  }

  // evaluate a real register register

  t_real Register::getrval (const Rid& rid) const {
    assert (rid.isvalid () == true);
    // get register type
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    long   pnum = rid.getpnum ();
    if ((lreg == FREG) && (lnum == 0)) return 0.0L;
    if ((lreg == FREG) && (lnum == 1)) return 1.0L;
    return getrval (lreg, pnum);
  }

  // evaluate an operand from the register bank

  void Register::eval (Operand& oprd) const {
    for (long i = 0; i < IA_MSRC; i++) {
      if (oprd.isvalid (i) == true) continue;
      Rid rid = oprd.getrid (i);
      if (rid.isvalid () == false) continue;
      t_lreg lreg = rid.gettype ();
      long   lnum = rid.getlnum ();
      long   pnum = rid.getpnum ();
      switch (lreg) {
      case GREG:
	if (lnum == 0) {
	  oprd.setoval (i, OCTA_0);
	  oprd.setbval (i, false);
	} else {
	  oprd.setoval (i, getoval (GREG, pnum));
	  oprd.setbval (i, getbval (NREG, pnum));
	}
	break;
      case NREG:
	if (lnum == 0) {
	  oprd.setbval (i, false);
	} else {
	  oprd.setbval (i, getbval (NREG, pnum));
	}
	break;
      case FREG:
	if (lnum == 0) {
	  oprd.setrval (i, 0.0L);
	} else if (lnum == 1) {
	  oprd.setrval (i, 1.0L);
	} else {
	  oprd.setrval (i, getrval (FREG, pnum));
	}
	break;
      case PREG:
	if (lnum == 0) {
	  oprd.setbval (i, true);
	} else {
	  oprd.setbval (i, getbval (PREG, pnum));
	}
	break;
      case BREG:
	oprd.setoval (i, getoval (BREG, pnum));
	break;
      case AREG:
	oprd.setoval (i, getoval (AREG, pnum));
	break;
      case CREG:
	oprd.setoval (i, getoval (CREG, pnum));
	break;
      case IPRG:
	oprd.setoval (i, getoval (IPRG, pnum));
	break;
      case CFMR:
	oprd.setoval (i, getoval (CFMR, pnum));
	break;
      case PSRG:
	oprd.setoval (i, getoval (PSRG, pnum));
	break;
      case PRRG:
	oprd.setoval (i, getoval (PRRG, pnum));
	break;
      case UMRG:
	{
	  Psr psr = getoval    (PSRG, pnum);	  
	  Umr umr = psr.getumr ();
	  oprd.setoval         (i, umr.getumr ());
	}
	break;
      default:
	break;
      }
      // use the tracer to report regread
      if (p_tracer) {
	Record rcd    (d_name);
	rcd.setoprd   (oprd, i);
	p_tracer->add (rcd);
      }
    }
  }

  // check one record against the register file

  bool Register::check (const Record& rcd) const {
    if (rcd.gettype () != Record::REGCHK) return false;
    // get record info
    t_lreg lreg = rcd.chklreg ();
    long   pnum = rcd.chkpnum ();
    bool   bval = rcd.chkbval ();
    t_octa oval = rcd.chkoval ();
    t_real rval = rcd.chkrval ();
    // check value
    bool result = false;
    switch (lreg) {
    case GREG:
      {
	bool   brbk = getbval (NREG, pnum);
	t_octa orbk = getoval (GREG, pnum);
	if (brbk == true) {
	  result = (bval == brbk);
	} else {
	  result = (bval == brbk) && (oval == orbk);
	}
      }
      break;
    case NREG:
      {
	bool brbk = getbval (NREG, pnum);
	result = (bval == brbk);
      }
      break;
    case FREG:
      {
	t_real rrbk = getrval (FREG, pnum);
	if ((rrbk.isnan () == true) && (rval.isnan () == true)) return true;
	else if ((rrbk.getsign () == true) && (rrbk.getsign () == false))
	  return false;
	else if ((rrbk.getsign () == false) && (rrbk.getsign () == true)) 
	  return false;
	result = (rval == rrbk);
      }
      break;
    case PREG:
      {
	bool brbk = getbval (PREG, pnum);
	result = (bval == brbk);
      }
      break;
    case CREG:
      {
	t_octa orbk = getoval (CREG, pnum);
	result = (oval == orbk);
      }
      break;
    default:
      {
	t_octa orbk = getoval (lreg, pnum);
	result = (oval == orbk);
      }
      break;	
    }
    return result;
  }
}
