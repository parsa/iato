// ---------------------------------------------------------------------------
// - ExpStg.cpp                                                              -
// - iato:iaoo client - expand stage class implementation                    -
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

#include "Srn.hpp"
#include "PrnStg.hpp"
#include "ExpStg.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new expand stage by context and name

  ExpStg::ExpStg (Stx* stx) : Stage (stx, RESOURCE_EXP) {
    d_ilen = stx->getiwsz () * BN_SLSZ; assert (d_ilen > 0);
    reset ();
  }

  // create a new expand stage by context and name

  ExpStg::ExpStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_ilen = stx->getiwsz () * BN_SLSZ; assert (d_ilen > 0);
    reset ();
  }

  // reset this expand stage

  void ExpStg::reset (void) {
    Stage::reset ();
    clear ();
  }
    
  // flush this expand stage

  void ExpStg::flush (void) {
    Stage::flush ();
    clear ();
  }

  // return true if the stage is holding

  bool ExpStg::isholding (void) const {
    // return true if one queue has reached the threshold
    bool qthr = d_mbuf.isthr () || d_ibuf.isthr () ||
                d_fbuf.isthr () || d_bbuf.isthr ();
    return qthr;
  }

  // activate this expand stage - the instructions are coming from the
  // previous stage and are assumed to be physically renamed.

  void ExpStg::activate (void) {
    // get the previous stage and map it to prn
    PrnStg* prn = dynamic_cast <PrnStg*> (p_pstg);
    assert (prn);
    // loop in the physical renamed instruction buffer
    for (long i = 0; i < d_ilen; i++) {
      Dsi dsi = prn->getinst (i);
      if (dsi.isvalid () == false) continue;
      // get instruction slot type
      t_unit unit = dsi.getsunit ();
      // switch to the buffer
      switch (unit) {
      case MUNIT:
	d_mbuf.push (dsi);
	break;
      case IUNIT:
	d_ibuf.push (dsi);
	break;
      case FUNIT:
	d_fbuf.push (dsi);
	break;
      case BUNIT:
	d_bbuf.push (dsi);
	break;	
      default:
	assert (false);
	break;
      }
      // create a record for the tracer
      if (p_tracer) {
	Record rcd (d_name, dsi);
	p_tracer->add (rcd);
      }
    }
    // check if the previous stage is halted - if it is halted, we check
    // that all queue are empty before going into halt mode
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = isempty ();
    }
    // update the tracer
    if (p_tracer) {
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void ExpStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: multi-unit expand stage" << endl;
    cout << "\tinstruction buffer \t: " << d_ilen     << endl;
    cout << "\tnumer of queues    \t: " << 4          << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // clear the unit queues 
  
  void ExpStg::clear (void) {
    d_mbuf.reset ();
    d_ibuf.reset ();
    d_fbuf.reset ();
    d_bbuf.reset ();
  }

  // return true if all queues are empty

  bool ExpStg::isempty (void) const {
    bool flag = d_mbuf.isempty () && d_ibuf.isempty () && 
                d_fbuf.isempty () && d_bbuf.isempty ();
    return flag;
  }

  // return true if a queue is empty by unit

  bool ExpStg::isempty (const t_unit unit) const {
    bool result = false;
    switch (unit) {
    case MUNIT:
      result = d_mbuf.isempty ();
      break;
    case IUNIT:
      result = d_ibuf.isempty ();
      break;
    case FUNIT:
      result = d_fbuf.isempty ();
      break;
    case BUNIT:
      result = d_bbuf.isempty ();
      break;
    default:
      break;
    }
    return result;
  }

  // return true if all queues are full

  bool ExpStg::isfull (void) const {
    bool flag = d_mbuf.isfull () || d_ibuf.isfull () ||
                d_fbuf.isfull () || d_bbuf.isfull ();
    return flag;
  }

  // return true if a queue is full by unit

  bool ExpStg::isfull (const t_unit unit) const {
    bool result = false;
    switch (unit) {
    case MUNIT:
      result = d_mbuf.isfull ();
      break;
    case IUNIT:
      result = d_ibuf.isfull ();
      break;
    case FUNIT:
      result = d_fbuf.isfull ();
      break;
    case BUNIT:
      result = d_bbuf.isfull ();
      break;
    default:
      break;
    }
    return result;
  }

  // pop an instruction by unit

  Dsi ExpStg::pop (const t_unit unit) {
    Dsi dsi;
    switch (unit) {
    case MUNIT:
      dsi = d_mbuf.pop ();
      break;
    case IUNIT:
      dsi = d_ibuf.pop ();
      break;
    case FUNIT:
      dsi = d_fbuf.pop ();
      break;
    case BUNIT:
      dsi = d_bbuf.pop ();
      break;
    default:
      assert (false);
      break;
    }
    return dsi;
  }
}
