// ---------------------------------------------------------------------------
// - Pipeline.cpp                                                            -
// - iato:mac library - generic pipeline class implementation                -
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
#include "Pipeline.hpp"

namespace iato {

  // create a default pipeline with a context

  Pipeline::Pipeline (Mtx* mtx) : Runnable (RESOURCE_PPL) {
    reset ();
  }

  // create a new pipeline by context and name

  Pipeline::Pipeline (Mtx* mtx, const string& name) : Runnable (name) {
    reset ();
  }

  // destroy this pipeline

  Pipeline::~Pipeline (void) {
    clear ();
  }

  // reset this pipeline

  void Pipeline::reset (void) {
    long size = d_pipe.size ();
    for (long i = 0; i < size; i++) d_pipe[i]->reset ();
  }

  // flush this pipeline

  void Pipeline::flush (void) {
    long size = d_pipe.size ();
    for (long i = 0; i < size; i++) d_pipe[i]->flush ();
  }

  // return true if one stage is holding 

  bool Pipeline::isholding (const long index) const {
    long size = d_pipe.size ();
    if ((index < 0) || (index >= size)) return false;
    return d_pipe[index]->isholding ();
  }

  // run all pipeline stages

  void Pipeline::run (void) {
    typedef t_pline::reverse_iterator t_pit;
    for (t_pit it = d_pipe.rbegin (); it < d_pipe.rend (); it++) {
      Stage* stg = *it;
      stg->run ();
    }
  }

  // run one pipeline stage by index - if the index is out of bound
  // do nothing.

  void Pipeline::run (const long index) {
    long size = d_pipe.size ();
    if ((index < 0) || (index >= size)) return;
    d_pipe[index]->run ();
  }

  // report all pipeline stages

  void Pipeline::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: "  << "pipeline" << endl;
    cout << "\tnumber of stages\t: " << depth ()   << endl;
    cout << endl;
    typedef t_pline::const_iterator t_pit;
    for (t_pit it = d_pipe.begin (); it < d_pipe.end (); it++) {
      Stage* stg = *it;
      stg->report ();
    }
  }

  // set the stat collector to all stages

  void Pipeline::setstc (Stat* stc) {
    if (!stc) return;
    Resource::setstc (stc);
    typedef t_pline::iterator t_pit;
    for (t_pit it = d_pipe.begin (); it < d_pipe.end (); it++) {
      Resource* res = *it;
      res->setstc (stc);
    }
  }

  // set the resource tracer to all stages

  void Pipeline::settrc (Tracer* tracer) {
    if (!tracer) return;
    Resource::settrc (tracer);
    typedef t_pline::iterator t_pit;
    for (t_pit it = d_pipe.begin (); it < d_pipe.end (); it++) {
      Resource* res = *it;
      res->settrc (tracer);
    }
  }

  // clear this pipeline

  void Pipeline::clear (void) {
    long size = d_pipe.size ();
    for (long i = 0; i < size; i++) delete d_pipe[i];
    d_pipe.clear ();
  }

  // return the pipeline depth

  long Pipeline::depth (void) const {
    return d_pipe.size ();
  }

  // add a new stage in this pipeline
  
  void Pipeline::add (Stage* stg) {
    if (!stg) return;
    d_pipe.push_back (stg);
  }

  // get a pipeline stage by name
  
  Stage* Pipeline::get (const string& name) const {
    long size = d_pipe.size ();
    for (long i = 0; i < size; i++) {
      Stage* stg = d_pipe[i];
      if (stg && (stg->getname () == name)) return stg;
    }
    return 0;
  }

  // bind this pipeline with an environment and the enclosing stages
  
  void Pipeline::bind (Env* env, Stage* pstg, Stage* nstg) {
    // check for pipeline depth
    long size = d_pipe.size ();
    if (size == 0) return;
    // bind the first stage
    if (size == 1) {
      d_pipe[0]->bind (env, pstg, nstg);
      return;
    }
    for (long i = 0; i < size; i++) {
      Stage* ps = (i == 0) ? pstg : d_pipe[i-1];
      Stage* ns = (i == (size - 1)) ? nstg : d_pipe[i+1];
      d_pipe[i]->bind (env, ps, ns);
    }
  }

  // return true if the pipeline is halted - that is we check the last
  // stage for halt

  bool Pipeline::ishalted  (void) const {
    long index = depth () - 1;
    return d_pipe[index]->ishalted ();
  }
}
