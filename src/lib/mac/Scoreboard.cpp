// ---------------------------------------------------------------------------
// - Scoreboard.cpp                                                          -
// - iato:mac library - register scoreboarding class implementation          -
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
#include "Mac.hpp"
#include "Scoreboard.hpp"

namespace iato {
  using namespace std;

  // create a default register scoreboard

  Scoreboard::Scoreboard (void) : Resource (RESOURCE_PSB) {
    d_mode = MDEF;
    d_gnum    = GR_LRSZ;
    d_fnum    = FR_LRSZ;
    d_pnum    = PR_LRSZ;
    d_bnum    = BR_LRSZ;
    d_anum    = AR_LRSZ;
    d_cnum    = CR_LRSZ;
    d_rrnum   = RR_LRSZ;
    d_dbrnum  = DBR_LRSZ;
    d_ibrnum  = IBR_LRSZ;
    d_pkrnum  = PKR_LRSZ;
    d_pmcnum  = PMC_LRSZ;
    d_pmdnum  = PMD_LRSZ;
    d_cpuidnum= CPUID_LRSZ;
    d_msrnum  = MSR_LRSZ;
    d_dtrnum  = DTR_LRSZ;
    d_itrnum  = ITR_LRSZ;
    p_glok = new bool[d_gnum];
    p_flok = new bool[d_fnum];
    p_plok = new bool[d_pnum];
    p_blok = new bool[d_bnum];
    p_alok = new bool[d_anum];
    p_clok = new bool[d_cnum];
    p_rrlok = new bool[d_rrnum];
    p_dbrlok = new bool[d_dbrnum];
    p_ibrlok = new bool[d_ibrnum];
    p_pkrlok = new bool[d_pkrnum];
    p_pmclok = new bool[d_pmcnum];
    p_pmdlok = new bool[d_pmdnum];
    p_cpuidlok = new bool[d_cpuidnum];
    p_msrlok = new bool[d_msrnum];
    p_dtrlok = new bool[d_dtrnum];
    p_itrlok = new bool[d_itrnum];
    reset ();
  }

  // create a register scoreboard with a context

  Scoreboard::Scoreboard (Mtx* mtx) : Resource (RESOURCE_PSB) {
    d_mode = MDEF;
    d_gnum    = mtx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum    = mtx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum    = mtx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum    = mtx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum    = mtx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum    = mtx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    d_rrnum   = mtx->getlong ("LR-RR-SIZE"); assert (d_rrnum > 0);
    d_dbrnum  = mtx->getlong ("LR-DBR-SIZE"); assert (d_dbrnum > 0);
    d_ibrnum  = mtx->getlong ("LR-IBR-SIZE"); assert (d_ibrnum > 0);
    d_pkrnum  = mtx->getlong ("LR-PKR-SIZE"); assert (d_pkrnum > 0);
    d_pmcnum  = mtx->getlong ("LR-PMC-SIZE"); assert (d_pmcnum > 0);
    d_pmdnum  = mtx->getlong ("LR-PMD-SIZE"); assert (d_pmdnum > 0);
    d_cpuidnum= mtx->getlong ("LR-CPUID-SIZE"); assert (d_cpuidnum > 0);
    d_msrnum  = mtx->getlong ("LR-MSR-SIZE"); assert (d_msrnum > 0);
    d_dtrnum  = mtx->getlong ("LR-DTR-SIZE"); assert (d_dtrnum > 0);
    d_itrnum  = mtx->getlong ("LR-ITR-SIZE"); assert (d_itrnum > 0);
    p_glok = new bool[d_gnum];
    p_flok = new bool[d_fnum];
    p_plok = new bool[d_pnum];
    p_blok = new bool[d_bnum];
    p_alok = new bool[d_anum];
    p_clok = new bool[d_cnum];
    p_rrlok = new bool[d_rrnum];
    p_dbrlok = new bool[d_dbrnum];
    p_ibrlok = new bool[d_ibrnum];
    p_pkrlok = new bool[d_pkrnum];
    p_pmclok = new bool[d_pmcnum];
    p_pmdlok = new bool[d_pmdnum];
    p_cpuidlok = new bool[d_cpuidnum];
    p_msrlok = new bool[d_msrnum];
    p_dtrlok = new bool[d_dtrnum];
    p_itrlok = new bool[d_itrnum];
    reset ();
  }

  // create a register scoreboard with a context and a name

  Scoreboard::Scoreboard (Mtx* mtx, const string& name) : Resource (name) {
    d_mode = MDEF;
    d_gnum    = mtx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum    = mtx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum    = mtx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum    = mtx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum    = mtx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum    = mtx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    d_rrnum   = mtx->getlong ("LR-RR-SIZE"); assert (d_rrnum > 0);
    d_dbrnum  = mtx->getlong ("LR-DBR-SIZE"); assert (d_dbrnum > 0);
    d_ibrnum  = mtx->getlong ("LR-IBR-SIZE"); assert (d_ibrnum > 0);
    d_pkrnum  = mtx->getlong ("LR-PKR-SIZE"); assert (d_pkrnum > 0);
    d_pmcnum  = mtx->getlong ("LR-PMC-SIZE"); assert (d_pmcnum > 0);
    d_pmdnum  = mtx->getlong ("LR-PMD-SIZE"); assert (d_pmdnum > 0);
    d_cpuidnum= mtx->getlong ("LR-CPUID-SIZE"); assert (d_cpuidnum > 0);
    d_msrnum  = mtx->getlong ("LR-MSR-SIZE"); assert (d_msrnum > 0);
    d_dtrnum  = mtx->getlong ("LR-DTR-SIZE"); assert (d_dtrnum > 0);
    d_itrnum  = mtx->getlong ("LR-ITR-SIZE"); assert (d_itrnum > 0);
    p_glok = new bool[d_gnum];
    p_flok = new bool[d_fnum];
    p_plok = new bool[d_pnum];
    p_blok = new bool[d_bnum];
    p_alok = new bool[d_anum];
    p_clok = new bool[d_cnum];
    p_rrlok = new bool[d_rrnum];
    p_dbrlok = new bool[d_dbrnum];
    p_ibrlok = new bool[d_ibrnum];
    p_pkrlok = new bool[d_pkrnum];
    p_pmclok = new bool[d_pmcnum];
    p_pmdlok = new bool[d_pmdnum];
    p_cpuidlok = new bool[d_cpuidnum];
    p_msrlok = new bool[d_msrnum];
    p_dtrlok = new bool[d_dtrnum];
    p_itrlok = new bool[d_itrnum];
    reset ();
  }
  
  // destroy this register scoreboard

  Scoreboard::~Scoreboard (void) {
    delete [] p_glok;
    delete [] p_flok;
    delete [] p_plok;
    delete [] p_blok;
    delete [] p_alok;
    delete [] p_clok;
    delete [] p_rrlok;
    delete [] p_dbrlok;
    delete [] p_ibrlok;
    delete [] p_pkrlok;
    delete [] p_pmclok;
    delete [] p_pmdlok;
    delete [] p_cpuidlok;
    delete [] p_msrlok;
    delete [] p_dtrlok;
    delete [] p_itrlok;
  }

  // reset the register scoreboard

  void Scoreboard::reset (void) {
    for (long i = 0; i < d_gnum; i++) p_glok[i] = false;
    for (long i = 0; i < d_fnum; i++) p_flok[i] = false;
    for (long i = 0; i < d_pnum; i++) p_plok[i] = false;
    for (long i = 0; i < d_bnum; i++) p_blok[i] = false;
    for (long i = 0; i < d_anum; i++) p_alok[i] = false;
    for (long i = 0; i < d_cnum; i++) p_clok[i] = false;
    for (long i = 0; i < d_rrnum; i++) p_rrlok[i] = false;
    for (long i = 0; i < d_dbrnum; i++) p_dbrlok[i] = false;
    for (long i = 0; i < d_ibrnum; i++) p_ibrlok[i] = false;
    for (long i = 0; i < d_pkrnum; i++) p_pkrlok[i] = false;
    for (long i = 0; i < d_pmcnum; i++) p_pmclok[i] = false;
    for (long i = 0; i < d_pmdnum; i++) p_pmdlok[i] = false;
    for (long i = 0; i < d_cpuidnum; i++) p_cpuidlok[i] = false;
    for (long i = 0; i < d_msrnum; i++) p_msrlok[i] = false;
    for (long i = 0; i < d_dtrnum; i++) p_dtrlok[i] = false;
    for (long i = 0; i < d_itrnum; i++) p_itrlok[i] = false;
    d_umrg = false;
    d_psrg = false;
    d_srlz = false;
  }

  // report this resource

  void Scoreboard::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: " << "register scoreboard" << endl;
  }

  // return a true if a register is marked

  bool Scoreboard::ismarked (const Rid& rid) const {
    if (rid.isvalid () == false) return false;
    // extract rid info
    t_lreg lreg = rid.gettype ();
    long   pnum = rid.getpnum (); assert (pnum >= 0);
    // process from the register type
    bool result = false;
    switch (lreg) {
    case GREG:
      result = pnum < d_gnum ? p_glok[pnum] : false;
      break;
    case FREG:
      result = pnum < d_fnum ? p_flok[pnum] : false;
      break;
    case PREG:
      result = pnum < d_pnum ? p_plok[pnum] : false;
      break;
    case BREG:
      result = pnum < d_bnum ? p_blok[pnum] : false;
      break;
    case AREG:
      result = pnum < d_anum ? p_alok[pnum] : false;
      break;
    case CREG:
      result = pnum < d_cnum ? p_clok[pnum] : false;
      break;
    case RRRG:
      result = pnum < d_rrnum ? p_rrlok[pnum] : false;
      break;
    case DBRG:
      result = pnum < d_dbrnum ? p_dbrlok[pnum] : false;
      break;
    case IBRG:
      result = pnum < d_ibrnum ? p_ibrlok[pnum] : false;
      break;
    case PKRG:
      result = pnum < d_pkrnum ? p_pkrlok[pnum] : false;
      break;
    case PMCR:
      result = pnum < d_pmcnum ? p_pmclok[pnum] : false;
      break;
    case PMDR:
      result = pnum < d_pmdnum ? p_pmdlok[pnum] : false;
      break;
    case CPIDR:
      result = pnum < d_cpuidnum ? p_cpuidlok[pnum] : false;
      break;
    case MSRG:
      result = pnum < d_msrnum ? p_msrlok[pnum] : false;
      break;
    case DTRG:
      result = pnum < d_dtrnum ? p_dtrlok[pnum] : false;
      break;
    case ITRG:
      result = pnum < d_itrnum ? p_itrlok[pnum] : false;
      break;
    case IPRG:
    case CFMR:
      result = false;
      break;
    case PRRG:
      for (long i = 0; i < PR_LRSZ; i++) result |= p_plok[i];
      break;
    case PROT:
      for (long i = PR_RTBS; i < PR_LRSZ; i++) result |= p_plok[i];
      break;
    case UMRG:
      result = d_umrg;
      break;
    case PSRG:
      result = d_psrg;
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // set a register scoreboard by rid

  void Scoreboard::mark (const Rid& rid, const bool flag) {
    if (rid.isvalid () == false) return;
    // extract rid info
    t_lreg lreg = rid.gettype ();
    long   pnum = rid.getpnum (); assert (pnum >= 0);
    // process from the register type
    switch (lreg) {
    case GREG:
      assert (pnum < d_gnum);
      p_glok[pnum] = flag;
      break;
    case FREG:
      assert (pnum < d_fnum);
      p_flok[pnum] = flag;
      break;
    case PREG:
      assert (pnum < d_pnum);
      p_plok[pnum] = flag;
      break;
    case BREG:
      assert (pnum < d_bnum);
      p_blok[pnum] = flag;
      break;
    case AREG:
      assert (pnum < d_anum);
      p_alok[pnum] = flag;
      break;
    case CREG:
      assert (pnum < d_cnum);
      p_clok[pnum] = flag;
      break;
    case RRRG:
      assert (pnum < d_rrnum);
      p_rrlok[pnum] = flag;
      break;
    case DBRG:
      assert (pnum < d_dbrnum);
      p_dbrlok[pnum] = flag;
      break;
    case IBRG:
      assert (pnum < d_ibrnum);
      p_ibrlok[pnum] = flag;
      break;
    case PKRG:
      assert (pnum < d_pkrnum);
      p_pkrlok[pnum] = flag;
      break;
    case PMCR:
      assert (pnum < d_pmcnum);
      p_pmclok[pnum] = flag;
      break;
    case PMDR:
      assert (pnum < d_pmdnum);
      p_pmdlok[pnum] = flag;
      break;
    case CPIDR:
      assert (pnum < d_cpuidnum);
      p_cpuidlok[pnum] = flag;
      break;
    case MSRG:
      assert (pnum < d_msrnum);
      p_msrlok[pnum] = flag;
      break;
    case DTRG:
      assert (pnum < d_dtrnum);
      p_dtrlok[pnum] = flag;
      break;
    case ITRG:
      assert (pnum < d_itrnum);
      p_itrlok[pnum] = flag;
      break;
    case IPRG:
    case CFMR:
      break;
    case PRRG:
      for (long i = 0; i < PR_LRSZ; i++) p_plok[i] = flag;
      break;
    case PROT:
      for (long i = PR_RTBS; i < PR_LRSZ; i++) p_plok[i] = flag;
      break;
    case UMRG:
      d_umrg = flag;
      break;
    case PSRG:
      d_psrg = flag;
      break;
    default:
      assert (false);
      break;
    }
  }

  // set a serialization condition

  void Scoreboard::setsr (const Ssi& ssi) {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return;
    // check for post serialize condition
    if (ssi.ispostsr () == true) d_srlz = true;
  }

  // unset a serialization condition

  void Scoreboard::unsetsr (const Ssi& ssi) {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return;
    // check for post serialize condition
    if (ssi.ispostsr () == true) d_srlz = false;
  }

  // lock an instruction with the scoreboard

  void Scoreboard::lock (const Ssi& ssi) {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return;
    // lock destination registers
    for (long i = 0; i < IA_MDST; i++) {
      Rid rid = ssi.getdnum (i);
      mark (rid, true);
    }
    // set a serialization condition
    setsr (ssi);
  }

  // unlock an instruction with the scoreboard

  void Scoreboard::unlock (const Ssi& ssi) {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return;
    // lock destination registers
    for (long i = 0; i < IA_MDST; i++) {
      Rid rid = ssi.getdnum (i);
      mark (rid, false);
    }
  }

  // check if an instruction has a hazard

  bool Scoreboard::ishazard (const Ssi& ssi) const {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return false;
    // check serialize flag
    if (d_srlz == true) return true;
    // check predicate first
    Rid pred = ssi.getpnum ();
    if (ismarked (pred) == true) return true;
    // check source registers
    for (long i = 0; i < IA_MSRC; i++) {
      Rid sreg = ssi.getsnum (i);
      if (ismarked (sreg) == true) return true;
    }
    // no hazard at all
    return false;
  }
}
