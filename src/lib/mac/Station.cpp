// ---------------------------------------------------------------------------
// - Station.cpp                                                             -
// - iato:mac library - reservation station table class implementation       -
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
#include "Mac.hpp"
#include "Utils.hpp"
#include "Station.hpp"

namespace iato {
  using namespace std;

  // the reservation station table
  struct t_rsen {
    // the valid bit
    bool d_valid;
    // the wakeup bit
    bool d_wakup;
    // the instruction
    Dsi d_inst;
    // the scheduler index
    long d_schi;
    // initialize this station
    t_rsen (void) {
      reset ();
    }
    // reset this station
    void reset (void) {
      d_valid = false;
      d_wakup = false;
      d_schi  = -1;
      d_inst.reset ();
    }
    // insert a new instruction
    void insert (const Dsi& dsi, const long sidx, const long igcs, 
		 const long schi) {
      assert (d_valid == false);
      d_valid = true;
      d_wakup = false;
      d_schi  = schi;
      d_inst  = dsi;
      // set station index
      d_inst.setsid (sidx);
      d_inst.setgcs (igcs);
    }
    // check if the instruction is ready
    bool isready (void) {
      // if the entry, the instruction or the
      // wakeup is set, the instruction is not
      // ready for execution or has been scheduled
      if (d_valid == false) return false;
      // check the instruction
      bool result = d_inst.isready () && (d_wakup == false);
      return result;
    }
    // mark instruction operands ready by rid
    void setready (const Rid& rid) {
      // do not mark if not valid
      if (d_valid == false) return;
      // do nothing if the wakeup bit is set
      if (d_wakup == true) return;
      // set the instruction ready bit
      d_inst.setrdy (rid);
    }
    // mark instruction cancelled
    void setcnl (const Rid& rid, const bool value) {
      // do not mark if not valid
      if (d_valid == false) return;
      // do nothing if the wakeup bit is set
      if (d_wakup == true) return;
      // set the instruction cancel flag
      d_inst.setcnlf (rid, value);
    }
    // reschedule an instruction
    void resched (void) {
      assert (d_valid == true);
      assert (d_wakup == true);
      // clean wakeup it and mark rescheduled
      d_wakup = false;
      d_inst.setrsch (true);
    }
  };

  // create a station table by type

  Station::Station (const t_unit unit) : Resource (RESOURCE_RST) {
    d_type = unit;
    d_igcs = -1;
    d_size = RS_TBSZ;
    p_stbl = new t_rsen[d_size];
    p_rsst = new long[d_size];
    reset ();
  }

  // create a station table by context and type

  Station::Station (Mtx* mtx, const t_unit unit) : Resource (RESOURCE_RST) {
    d_type = unit;
    d_igcs = -1;
    d_size = mtx->getlong ("RST-SIZE"); assert (d_size > 0);
    p_stbl = new t_rsen[d_size];
    p_rsst = new long[d_size];
    reset ();
  }

  // create a station table by context, type and name

  Station::Station (Mtx* mtx, const t_unit unit, const string& name) : 
    Resource (RESOURCE_RST) {
    d_type = unit;
    d_igcs = -1;
    d_size = mtx->getlong ("RST-SIZE"); assert (d_size > 0);
    p_stbl = new t_rsen[d_size];
    p_rsst = new long[d_size];
    reset ();
  }

  // destroy this station

  Station::~Station (void) {
    delete [] p_stbl;
    delete [] p_rsst;
  }

  // reset this station

  void Station::reset (void) {
    for (long  i = 0; i < d_size; i++) {
      p_stbl[i].reset ();
      p_rsst[i] = -1;
    }
  }

  // report this resource

  void Station::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: " << "reservation station table" << endl;
    cout << "\ttable size   \t\t: " << d_size << endl;
  }

  // set the station gcs index

  void Station::setgcs (const long index) {
    d_igcs = index;
  }

  // get the station gcs index

  long Station::getgcs (void) const {
    return d_igcs;
  }

  // return true if the table is full

  bool Station::isfull (void) const {
    for (long  i = 0; i < d_size; i++) {
      if (p_stbl[i].d_valid == false) return false;
    }
    return true;
  }

  // return true if the table is empty

  bool Station::isempty (void) const {
    for (long  i = 0; i < d_size; i++) {
      if (p_stbl[i].d_valid == true) return false;
    }
    return true;
  }

  // insert a new entry in the station

  long Station::alloc (const Dsi& dsi) {
    assert (d_type == dsi.getsunit ());
    // find a free reservation station
    long index = findsta ();
    if (index == -1) return -1;
    // find a free scheduler entry
    long schi = findsch ();
    if (index == -1) return -1;
    // insert it in the station
    p_stbl[index].insert (dsi, index, d_igcs, schi);
    // update the scheduler
    p_rsst[schi] = index;
    // update the tracer
    if (p_tracer) {
      Record rcd (d_name, dsi);
      p_tracer->add (rcd);
    }
    return index;
  }

  // clear an entry in the station by index

  void Station::clear (const long index) {
    assert ((index >= 0) && (index < d_size));
    long schi = p_stbl[index].d_schi;
    // clear the station entry
    p_stbl[index].reset ();
    // clear the scheduler entry
    p_rsst[schi] = -1;
    // repack the scheduler table
    repack ();
  }

  // return the next instruction ready for execution

  Dsi Station::getrdy (void) {
    Dsi dsi;
    // loop in the scheduler table to find the next ready instruction
    for (long  i = 0; i < d_size; i++) {
      long stai = p_rsst[i];
      if (stai == -1) continue;
      // check for the instruction beeing ready
      if (p_stbl[stai].isready () == true) {
	assert (p_stbl[stai].d_schi == i);
	dsi = p_stbl[stai].d_inst;
	p_stbl[stai].d_wakup = true;
	break;
      }
    }
    // return an invalid instruction if nothing is found
    return dsi;
  }

  // mark all operands ready by rid

  void Station::setrdy (const Rid& rid) {
    for (long  i = 0; i < d_size; i++) {
      p_stbl[i].setready (rid);
    }
  }

  // mark the instruction cancelled by rid and value

  void Station::setcnl (const Rid& rid, const bool value) {
    for (long  i = 0; i < d_size; i++) {
      p_stbl[i].setcnl (rid, value);
    }
  }

  // reschedule an instruction by station index

  void Station::resched (const long index) {
    assert ((index >= 0) && (index < d_size));
    p_stbl[index].resched ();
  }

  // return a free reservation station index or -1

  long Station::findsta (void) const {
    for (long  i = 0; i < d_size; i++) {
      if (p_stbl[i].d_valid == false) return i;
    }
    return -1;
  }

  // return a free scheduler entry or -1

  long Station::findsch (void) const {
    for (long  i = 0; i < d_size; i++) {
      if (p_rsst[i] == -1) return i;
    }
    return -1;
  }

  // repack the scheduler table

  void Station::repack (void) {
    // loop in the scheduling table and find a hole
    // the reservation station table must be updated when packing the
    // entries
    for (long  i = 0; i < d_size; i++) {
      if (p_rsst[i] != -1) continue;
      // here there is a hole - so pack it up to the next one
      for (long j = i + 1; j < d_size; j++) {
	if (p_rsst[j] == -1) continue;
	// here we found an entry, so move it
	long stai = p_rsst[j];
	p_rsst[i] = stai;
	p_rsst[j] = -1;
	// update the station scheduler index
	p_stbl[stai].d_schi = i;
	// force continuation with next index
	break;
      }      
    }
  }

  // dump the station contents, this is a procedure for debug

  void Station::dump (void) const {
    cerr << "station[" << d_igcs << "] {" << endl;
    for (long i = 0; i < d_size; i++) {
      cerr << "  [" << dec << setw (2) << setfill ('0') << i << "] ";
      if (p_stbl[i].d_valid == false)
	cerr << "INVALID" << endl;
      else {
	if (p_stbl[i].isready () == true)
	  cerr << "[R] ";
	else
	  cerr << "[P] ";
	long   sh = p_stbl[i].d_schi;
	cerr << "  [" << dec << setw (2) << setfill ('0') << sh << "] 0x";
	t_octa ip = p_stbl[i].d_inst.getiip ();
	cerr << hex << setw (16) << setfill ('0') << ip << dec;
	cerr << ' ' << trim (p_stbl[i].d_inst.recode ());
	if (p_stbl[i].d_inst.isready () == true) 
	  cerr << " [true]" << endl;
	else
	  cerr << " [false]" << endl;
      }
    }
    cerr << '}' << endl;
  }
}
