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
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
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
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
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
    p_grat = new long[d_gnum];
    p_frat = new long[d_fnum];
    p_prat = new long[d_pnum];
    p_brat = new long[d_bnum];
    p_arat = new long[d_anum];
    p_crat = new long[d_cnum];
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
  }

  // reset this rat

  void Rat::reset (void) {
    for (long i = 0; i < d_gnum; i++) p_grat[i] = -1;
    for (long i = 0; i < d_fnum; i++) p_frat[i] = -1;
    for (long i = 0; i < d_pnum; i++) p_prat[i] = -1;
    for (long i = 0; i < d_bnum; i++) p_brat[i] = -1;
    for (long i = 0; i < d_anum; i++) p_arat[i] = -1;
    for (long i = 0; i < d_cnum; i++) p_crat[i] = -1;
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
