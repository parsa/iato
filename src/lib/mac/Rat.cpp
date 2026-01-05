// ---------------------------------------------------------------------------
// - Rat.cpp                                                                 -
// - iato:mac library - ram based register alias table class implementation  -
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
#include "Rat.hpp"
#include "Interrupt.hpp"

namespace iato {

  // create a default rat

  Rat::Rat (void) : Resource (RESOURCE_RAT) {
    d_gnum = GR_LRSZ;
    d_fnum = FR_LRSZ;
    d_pnum = PR_LRSZ;
    d_bnum = BR_LRSZ;
    d_anum = AR_LRSZ;
    d_cnum = CR_LRSZ;
    d_rrnum = RR_LRSZ;
    d_dbrnum = DBR_LRSZ;
    d_ibrnum = IBR_LRSZ;
    d_pkrnum = PKR_LRSZ;
    d_pmcnum = PMC_LRSZ;
    d_pmdnum = PMD_LRSZ;
    d_cpidnum = CPUID_LRSZ;
    d_msrnum = MSR_LRSZ;
    d_dtrnum = DTR_LRSZ;
    d_itrnum = ITR_LRSZ;
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
    p_rrat = new long[d_rrnum];
    p_dbrat = new long[d_dbrnum];
    p_ibrat = new long[d_ibrnum];
    p_pkrat = new long[d_pkrnum];
    p_pmcat = new long[d_pmcnum];
    p_pmdat = new long[d_pmdnum];
    p_cpidat = new long[d_cpidnum];
    p_msrat = new long[d_msrnum];
    p_dtrat = new long[d_dtrnum];
    p_itrat = new long[d_itrnum];
    reset ();
  }

  // create a rat by context

  Rat::Rat (Mtx* mtx) : Resource (RESOURCE_RAT) {
    d_gnum = mtx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum = mtx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum = mtx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum = mtx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum = mtx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum = mtx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    d_rrnum = mtx->getlong ("LR-RR-SIZE"); assert (d_rrnum > 0);
    d_dbrnum = mtx->getlong ("LR-DBR-SIZE"); assert (d_dbrnum > 0);
    d_ibrnum = mtx->getlong ("LR-IBR-SIZE"); assert (d_ibrnum > 0);
    d_pkrnum = mtx->getlong ("LR-PKR-SIZE"); assert (d_pkrnum > 0);
    d_pmcnum = mtx->getlong ("LR-PMC-SIZE"); assert (d_pmcnum > 0);
    d_pmdnum = mtx->getlong ("LR-PMD-SIZE"); assert (d_pmdnum > 0);
    d_cpidnum = mtx->getlong ("LR-CPUID-SIZE"); assert (d_cpidnum > 0);
    d_msrnum = mtx->getlong ("LR-MSR-SIZE"); assert (d_msrnum > 0);
    d_dtrnum = mtx->getlong ("LR-DTR-SIZE"); assert (d_dtrnum > 0);
    d_itrnum = mtx->getlong ("LR-ITR-SIZE"); assert (d_itrnum > 0);
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
    p_rrat = new long[d_rrnum];
    p_dbrat = new long[d_dbrnum];
    p_ibrat = new long[d_ibrnum];
    p_pkrat = new long[d_pkrnum];
    p_pmcat = new long[d_pmcnum];
    p_pmdat = new long[d_pmdnum];
    p_cpidat = new long[d_cpidnum];
    p_msrat = new long[d_msrnum];
    p_dtrat = new long[d_dtrnum];
    p_itrat = new long[d_itrnum];
    reset ();
  }

  // create a rat by context and name

  Rat::Rat (Mtx* mtx, const string& name) : Resource (name) {
    d_gnum = mtx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum = mtx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum = mtx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum = mtx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum = mtx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum = mtx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    d_rrnum = mtx->getlong ("LR-RR-SIZE"); assert (d_rrnum > 0);
    d_dbrnum = mtx->getlong ("LR-DBR-SIZE"); assert (d_dbrnum > 0);
    d_ibrnum = mtx->getlong ("LR-IBR-SIZE"); assert (d_ibrnum > 0);
    d_pkrnum = mtx->getlong ("LR-PKR-SIZE"); assert (d_pkrnum > 0);
    d_pmcnum = mtx->getlong ("LR-PMC-SIZE"); assert (d_pmcnum > 0);
    d_pmdnum = mtx->getlong ("LR-PMD-SIZE"); assert (d_pmdnum > 0);
    d_cpidnum = mtx->getlong ("LR-CPUID-SIZE"); assert (d_cpidnum > 0);
    d_msrnum = mtx->getlong ("LR-MSR-SIZE"); assert (d_msrnum > 0);
    d_dtrnum = mtx->getlong ("LR-DTR-SIZE"); assert (d_dtrnum > 0);
    d_itrnum = mtx->getlong ("LR-ITR-SIZE"); assert (d_itrnum > 0);
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
    p_rrat = new long[d_rrnum];
    p_dbrat = new long[d_dbrnum];
    p_ibrat = new long[d_ibrnum];
    p_pkrat = new long[d_pkrnum];
    p_pmcat = new long[d_pmcnum];
    p_pmdat = new long[d_pmdnum];
    p_cpidat = new long[d_cpidnum];
    p_msrat = new long[d_msrnum];
    p_dtrat = new long[d_dtrnum];
    p_itrat = new long[d_itrnum];
    reset ();
  }

  // destroy this rat

  Rat::~Rat (void) {
    delete [] p_grat;
    delete [] p_frat;
    delete [] p_prat;
    delete [] p_brat;
    delete [] p_arat;
    delete [] p_crat;
    delete [] p_rrat;
    delete [] p_dbrat;
    delete [] p_ibrat;
    delete [] p_pkrat;
    delete [] p_pmcat;
    delete [] p_pmdat;
    delete [] p_cpidat;
    delete [] p_msrat;
    delete [] p_dtrat;
    delete [] p_itrat;
  }

  // reset this rat

  void Rat::reset (void) {
    for (long i = 0; i < d_gnum; i++) p_grat[i] = -1;
    for (long i = 0; i < d_fnum; i++) p_frat[i] = -1;
    for (long i = 0; i < d_pnum; i++) p_prat[i] = -1;
    for (long i = 0; i < d_bnum; i++) p_brat[i] = -1;
    for (long i = 0; i < d_anum; i++) p_arat[i] = -1;
    for (long i = 0; i < d_cnum; i++) p_crat[i] = -1;
    for (long i = 0; i < d_rrnum; i++) p_rrat[i] = -1;
    for (long i = 0; i < d_dbrnum; i++) p_dbrat[i] = -1;
    for (long i = 0; i < d_ibrnum; i++) p_ibrat[i] = -1;
    for (long i = 0; i < d_pkrnum; i++) p_pkrat[i] = -1;
    for (long i = 0; i < d_pmcnum; i++) p_pmcat[i] = -1;
    for (long i = 0; i < d_pmdnum; i++) p_pmdat[i] = -1;
    for (long i = 0; i < d_cpidnum; i++) p_cpidat[i] = -1;
    for (long i = 0; i < d_msrnum; i++) p_msrat[i] = -1;
    for (long i = 0; i < d_dtrnum; i++) p_dtrat[i] = -1;
    for (long i = 0; i < d_itrnum; i++) p_itrat[i] = -1;
  }

  // report this resource

  void Rat::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "ram based rat" << endl;
    cout << "\tgr rat size  \t\t: " << d_gnum << endl;
    cout << "\tfr rat size  \t\t: " << d_fnum << endl;
    cout << "\tpr rat size  \t\t: " << d_pnum << endl;
    cout << "\tbr rat size  \t\t: " << d_bnum << endl;
    cout << "\tar rat size  \t\t: " << d_anum << endl;
    cout << "\tcr rat size  \t\t: " << d_cnum << endl;
    cout << "\trr rat size  \t\t: " << d_rrnum << endl;
    cout << "\tdbr rat size \t\t: " << d_dbrnum << endl;
    cout << "\tibr rat size \t\t: " << d_ibrnum << endl;
    cout << "\tpkr rat size \t\t: " << d_pkrnum << endl;
    cout << "\tpmc rat size \t\t: " << d_pmcnum << endl;
    cout << "\tpmd rat size \t\t: " << d_pmdnum << endl;
    cout << "\tcpuid rat size\t\t: " << d_cpidnum << endl;
    cout << "\tmsr rat size \t\t: " << d_msrnum << endl;
    cout << "\tdtr rat size \t\t: " << d_dtrnum << endl;
    cout << "\titr rat size \t\t: " << d_itrnum << endl;
  }

  // return the number of registers by type

  long Rat::getsize (t_lreg lreg) const {
    long size = 0;
    switch (lreg) {
    case GREG:
      size = d_gnum;
      break;
    case FREG:
      size = d_fnum;
      break;
    case PREG:
      size = d_pnum;
      break;
    case BREG:
      size = d_bnum;
      break;
    case AREG:
      size = d_anum;
      break;
    case CREG:
      size = d_cnum;
      break;
    case RRRG:
      size = d_rrnum;
      break;
    case DBRG:
      size = d_dbrnum;
      break;
    case IBRG:
      size = d_ibrnum;
      break;
    case PKRG:
      size = d_pkrnum;
      break;
    case PMCR:
      size = d_pmcnum;
      break;
    case PMDR:
      size = d_pmdnum;
      break;
    case CPIDR:
      size = d_cpidnum;
      break;
    case MSRG:
      size = d_msrnum;
      break;
    case DTRG:
      size = d_dtrnum;
      break;
    case ITRG:
      size = d_itrnum;
      break;
    default:
      break;
    }
    return size;
  }

  // get the register mapping by register type and id

  long Rat::getmap (t_lreg lreg, const long rnum) const {
    assert (rnum >= 0);
    long result = -1;
    switch (lreg) {
    case GREG:
      assert (rnum < d_gnum);
      result = p_grat[rnum];
      break;
    case FREG:
      assert (rnum < d_fnum);
      result = p_frat[rnum];
      break;
    case PREG:
      assert (rnum < d_pnum);
      result = p_prat[rnum];
      break;
    case BREG:
      assert (rnum < d_bnum);
      result = p_brat[rnum];
      break;
    case AREG:
      assert (rnum < d_anum);
      result = p_arat[rnum];
      break;
    case CREG:
      assert (rnum < d_cnum);
      result = p_crat[rnum];
      break;
    case RRRG:
      assert (rnum < d_rrnum);
      result = p_rrat[rnum];
      break;
    case DBRG:
      assert (rnum < d_dbrnum);
      result = p_dbrat[rnum];
      break;
    case IBRG:
      assert (rnum < d_ibrnum);
      result = p_ibrat[rnum];
      break;
    case PKRG:
      assert (rnum < d_pkrnum);
      result = p_pkrat[rnum];
      break;
    case PMCR:
      assert (rnum < d_pmcnum);
      result = p_pmcat[rnum];
      break;
    case PMDR:
      assert (rnum < d_pmdnum);
      result = p_pmdat[rnum];
      break;
    case CPIDR:
      assert (rnum < d_cpidnum);
      result = p_cpidat[rnum];
      break;
    case MSRG:
      assert (rnum < d_msrnum);
      result = p_msrat[rnum];
      break;
    case DTRG:
      assert (rnum < d_dtrnum);
      result = p_dtrat[rnum];
      break;
    case ITRG:
      assert (rnum < d_itrnum);
      result = p_itrat[rnum];
      break;
    default:
      {
	string msg = "illegal register rat mapping";
	throw Interrupt (FAULT_IT_OPER_LEGAL, msg);
      }
      break;
    }
    return result;
  }  

  // get the register mapping by rid

  long Rat::getmap (const Rid& rid) const {
    // check for valid rid
    if (rid.isvalid () == false) return -1;
    // map the rid
    t_lreg lreg = rid.gettype ();
    long   pnum = rid.getpnum ();
    return getmap (lreg, pnum);
  }

  // set the register mapping by type and id and return the old one
  
  long Rat::setmap (t_lreg lreg, const long rnum, const long mnum) {
    assert ((rnum >= 0) && (mnum >= 0));
    long result = -1;
    switch (lreg) {
    case GREG:
      assert (rnum < d_gnum);
      result = p_grat[rnum];
      p_grat[rnum] = mnum;
      break;
    case FREG:
      assert (rnum < d_fnum);
      result = p_frat[rnum];
      p_frat[rnum] = mnum;
      break;
    case PREG:
      assert (rnum < d_pnum);
      result = p_prat[rnum];
      p_prat[rnum] = mnum;
      break;
    case BREG:
      assert (rnum < d_bnum);
      result = p_brat[rnum];
      p_brat[rnum] = mnum;
      break;
    case AREG:
      assert (rnum < d_anum);
      result = p_arat[rnum];
      p_arat[rnum] = mnum;
      break;
    case CREG:
      assert (rnum < d_cnum);
      result = p_crat[rnum];
      p_crat[rnum] = mnum;
      break;
    case RRRG:
      assert (rnum < d_rrnum);
      result = p_rrat[rnum];
      p_rrat[rnum] = mnum;
      break;
    case DBRG:
      assert (rnum < d_dbrnum);
      result = p_dbrat[rnum];
      p_dbrat[rnum] = mnum;
      break;
    case IBRG:
      assert (rnum < d_ibrnum);
      result = p_ibrat[rnum];
      p_ibrat[rnum] = mnum;
      break;
    case PKRG:
      assert (rnum < d_pkrnum);
      result = p_pkrat[rnum];
      p_pkrat[rnum] = mnum;
      break;
    case PMCR:
      assert (rnum < d_pmcnum);
      result = p_pmcat[rnum];
      p_pmcat[rnum] = mnum;
      break;
    case PMDR:
      assert (rnum < d_pmdnum);
      result = p_pmdat[rnum];
      p_pmdat[rnum] = mnum;
      break;
    case CPIDR:
      assert (rnum < d_cpidnum);
      result = p_cpidat[rnum];
      p_cpidat[rnum] = mnum;
      break;
    case MSRG:
      assert (rnum < d_msrnum);
      result = p_msrat[rnum];
      p_msrat[rnum] = mnum;
      break;
    case DTRG:
      assert (rnum < d_dtrnum);
      result = p_dtrat[rnum];
      p_dtrat[rnum] = mnum;
      break;
    case ITRG:
      assert (rnum < d_itrnum);
      result = p_itrat[rnum];
      p_itrat[rnum] = mnum;
      break;
    default:
      result = -1;
      break;
    }
    return result;
  }

  // set the register mapping by rid

  long Rat::setmap (const Rid& rid, const long mnum) {
    // check for valid rid
    if (rid.isvalid () == false) return -1;
    // set the rid
    t_lreg lreg = rid.gettype ();
    long   pnum = rid.getpnum ();
    return setmap (lreg, pnum, mnum);
  }
}
