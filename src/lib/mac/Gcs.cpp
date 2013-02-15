// ---------------------------------------------------------------------------
// - Gcs.cpp                                                                 -
// - iato:mac library - grand central station resource class implementation  -
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

#include "Gcs.hpp"
#include "Prn.hpp"

namespace iato {

  // create a new gcs with a context
  
  Gcs::Gcs (Mtx* mtx) : Resource (RESOURCE_GCS) {
    d_size = mtx->gettusz (); assert (d_size > 0);
    d_slen = 0;
    p_vsta = new Station*[d_size];
    for (long i = 0; i < d_size; i++) p_vsta[i] = 0;
    reset ();
  }

  // destroy this gcs

  Gcs::~Gcs (void) {
    delete [] p_vsta;
  }

  // reset this gcs

  void Gcs::reset (void) {
    for (long i = 0; i < d_size; i++) {
      if (p_vsta[i]) p_vsta[i]->reset ();
    }
  }

  // report this resource

  void Gcs::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "grand central station" << endl;
    cout << "\tgcs size     \t\t: " << d_size << endl;
  }

  // set the stat collector to all stations

  void Gcs::setstc (Stat* stc) {
    for (long i = 0; i < d_size; i++) {
      if (p_vsta[i]) p_vsta[i]->setstc (stc);
    }
  }

  // bind the tracer to all stations

  void Gcs::settrc (Tracer* tracer) {
    for (long i = 0; i < d_size; i++) {
      if (p_vsta[i]) p_vsta[i]->settrc (tracer);
    }
  }

  // add a new station to this central

  long Gcs::add (Station* sta) {
    long igcs = -1;
    if (sta) {
      assert (d_slen < d_size);
      assert (p_vsta[d_slen] == 0);
      igcs = d_slen;
      p_vsta[d_slen++] = sta;
    }
    return igcs;
  }

  // clear a station entry by gcs and station index

  void Gcs::clear (const long igcs, const long sidx) {
    assert ((igcs >= 0) && (igcs <= d_size));
    Station* sta = p_vsta[igcs];
    sta->clear (sidx);
  }

  // clear a station entry by instruction

  void Gcs::clear (const Dsi& dsi) {
    if (dsi.isvalid () == false) return;
    long igcs = dsi.getgcs ();
    long sidx = dsi.getsid ();
    if ((igcs == -1) || (sidx == -1)) return;
    clear (igcs, sidx);
  }

  // broadcast a cancel flag to all stations

  void Gcs::setcnl (const Result& resl) {
    if (resl.isvalid () == false) return;
    for (long i = 0; i < IA_MDST; i++) {
      // do nothing if not reg update
      if (resl.getrop (i) != Result::REG_UPD) continue;
      // check rid for validity
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      if (rid.islrnm  () == false) continue;
      if (rid.gettype () != PREG)  continue;
      // get boolean value
      bool cnlf = !resl.getbval (i);
      // update all stations
      for (long i = 0; i < d_size; i++) {
	if (p_vsta[i]) p_vsta[i]->setcnl (rid, cnlf);
      }
    }    
  }

  // broadcast a rid to all station

  void Gcs::setrdy (const Rid& rid) {
    if (rid.isvalid () == false) return;
    for (long i = 0; i < d_size; i++) 
      if (p_vsta[i]) p_vsta[i]->setrdy (rid);
  }

  // broadcast a result to all stations

  void Gcs::setrdy (const Result& resl) {
    for (long i = 0; i < IA_MDST; i++) {
      // check rid for validity
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      // update all stations
      setrdy (rid);
    }
  }

  // broadcast an instruction to all station

  Dsi Gcs::setrdy (const Dsi& inst) {
    // make sure we have something to do
    if (inst.isvalid () == false) return inst;
    // check for latency - at 1 and below we broadcast
    long elat = inst.getelat ();
    if (elat <= 1) {
      for (long i = 0; i < IA_MDST; i++) {
	Rid rid = inst.getdnum (i);
	setrdy (rid);
      }
      if (elat == 0) return inst;
    }
    // update the latency
    Dsi result = inst;
    result.setelat (elat - 1);
    return result;
  }

  // reschedule an instruction in a station

  void Gcs::resched (const Dsi& dsi) {
    if (dsi.isvalid () == false) return;
    // get station id and index
    long igcs = dsi.getgcs ();
    long sidx = dsi.getsid ();
    if ((igcs == -1) || (sidx == -1)) return;
    // grab the station and re-schedul
    Station* sta = p_vsta[igcs];
    if (sta) sta->resched (sidx);
  }

  // dump the contents of all stations

  void Gcs::dump (void) const {
    for (long i = 0; i < d_size; i++) {
      Station* sta = p_vsta[i];
      if (sta) sta->dump ();
    }
  }
}
