// ---------------------------------------------------------------------------
// - Pipelane.cpp                                                            -
// - iato:mac library - generic parallel pipeline class implementation       -
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
#include "Pipelane.hpp"

namespace iato {

  // create a new pipelane by context

  Pipelane::Pipelane (Mtx* mtx) : Stage (mtx, RESOURCE_PLN) {
    d_depth = 0;
    d_block = false;
    p_rctrl = 0;
    reset ();
  }

  // create a new pipelane by context and mode

  Pipelane::Pipelane (Mtx* mtx, const bool mode) : Stage (mtx, RESOURCE_PLN) {
    d_depth = 0;
    d_block = false;
    p_rctrl = 0;
    reset ();
    setmode (mode);
  }

  // create a new pipelane by context and name

  Pipelane::Pipelane (Mtx* mtx, const string& name) : Stage (mtx, name) {
    d_depth = 0;
    d_block = false;
    p_rctrl = 0;
    reset ();
  }

  // destroy this pipelane

  Pipelane::~Pipelane (void) {
    clear ();
    for (long i = 0; i < d_depth; i++) {
      if (p_rctrl) delete p_rctrl[i];
    }
    delete [] p_rctrl;
  }

  // reset this pipelane

  void Pipelane::reset (void) {
    long size = d_lane.size ();
    for (long i = 0; i < size; i++) d_lane[i]->reset ();
    if (p_rctrl) {
      for (long i = 0; i < d_depth; i++) {
	Runnable* ctrl = p_rctrl[i];
	if (ctrl) ctrl->reset ();
      }
    }
  }

  // flush this pipelane

  void Pipelane::flush (void) {
    long size = d_lane.size ();
    for (long i = 0; i < size; i++) d_lane[i]->flush ();
    if (p_rctrl) {
      for (long i = 0; i < d_depth; i++) {
	Runnable* ctrl = p_rctrl[i];
	if (ctrl) ctrl->flush ();
      }
    }
  }

  // return true if one pipeline first stage is holding in blocking mode
  // or false in non blocking mode

  bool Pipelane::isholding (void) const {
    return false;
  }

  // activate all pipelines

  void Pipelane::activate (void) {
    long size = d_lane.size ();
    for (long i = d_depth - 1; i >= 0; i--) {
      // check for blocking stage line
      if (d_block == true) {
	for (long j = 0; j < size; j++) {
	  Pipeline* pipe = d_lane[j];
	  if (pipe->isholding (i) == true) return;
	}
      }
      // run the pipe controller
      Runnable* ctrl = p_rctrl[i];
      if (ctrl) ctrl->run ();
      // run or activate each stage in parallel
      for (long j = 0; j < size; j++) {
	Pipeline* pipe = d_lane[j];
	if (d_block == true) {
	  pipe->activate (i);
	} else {
	  pipe->run (i);
	}
      }
    }
  }

  // report all pipeline

  void Pipelane::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: "  << "pipelane" << endl;
    cout << "\tnumber of pipes\t: "  << depth ()   << endl;
    cout << endl;
    typedef t_plane::const_iterator t_pit;
    for (t_pit it = d_lane.begin (); it < d_lane.end (); it++) {
      Pipeline* pipe = *it;
      pipe->report ();
    }
  }

  // set the stat collector to all pipelines

  void Pipelane::setstc (Stat* stc) {
    if (!stc) return;
    Resource::setstc (stc);
    typedef t_plane::iterator t_pit;
    for (t_pit it = d_lane.begin (); it < d_lane.end (); it++) {
      Resource* res = *it;
      res->setstc (stc);
    }
  }

  // set the resource tracer to all pipelines

  void Pipelane::settrc (Tracer* tracer) {
    if (!tracer) return;
    Resource::settrc (tracer);
    typedef t_plane::iterator t_pit;
    for (t_pit it = d_lane.begin (); it < d_lane.end (); it++) {
      Resource* res = *it;
      res->settrc (tracer);
    }
  }

  // clear this pipelane

  void Pipelane::clear (void) {
    long size = d_lane.size ();
    for (long i = 0; i < size; i++) delete d_lane[i];
    d_lane.clear ();
  }

  // set the blocking mode

  void Pipelane::setmode (const bool mode) {
    d_block = mode;
  }

  // get the blocking mode

  bool Pipelane::getmode (void) const {
    return d_block;
  }

  // return the pipelane depth

  long Pipelane::depth (void) const {
    return d_lane.size ();
  }

  // add a new pipeline in this pipelane
  
  void Pipelane::add (Pipeline* pipe) {
    if (!pipe) return;
    d_lane.push_back (pipe);
    // update the maximum depth
    long pd = pipe->depth ();
    if (pd > d_depth) {
      // copy old resource controler
      Runnable** rctrl = new (Runnable*)[pd];
      for (long i = 0; i < pd; i++) rctrl[i] = 0;
      for (long i = 0; i < d_depth; i++) rctrl[i] = p_rctrl[i];
      delete [] p_rctrl;
      // adjust depth and control
      d_depth = pd;
      p_rctrl = rctrl;
    }
  }

  // set a new contol resource by index

  void Pipelane::setclog (const long index, Runnable* pclog) {
    assert ((index >= 0) && (index < d_depth));
    if (p_rctrl) p_rctrl[index] = pclog;
  }

  // bind this pipelane with the environment and the enclosing stages

  void Pipelane::bind (Env* env, Stage* pstg, Stage* nstg) {
    long size = d_lane.size ();
    for (long i = 0; i < size; i++) {
      Pipeline* pipe = d_lane[i];
      pipe->bind (env, pstg, nstg);
    }
  }
}
