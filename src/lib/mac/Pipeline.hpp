// ---------------------------------------------------------------------------
// - Pipeline.hpp                                                            -
// - iato:mac library - generic pipeline class definition                    -
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

#ifndef  IATO_PIPELINE_HPP
#define  IATO_PIPELINE_HPP

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Pipeline class is a genric container class that holds the pipeline
  /// stages of the selected micro-architecture. Simply speaking, a pipeline
  /// is a vector of stages that would be run sequentially. When adding a
  /// new stage, the pipeline stage is added at the end of the vector. Once
  /// created, the pipeline is connected with the "bind" method. Running
  /// the pipeline is done by running all stages in reverse order. Normal
  /// order is done for flushing and reseting.

  class Pipeline : public Runnable {
  private:
    /// the pipeline type
    typedef vector<Stage*> t_pline;
    /// the pipeline vector
    t_pline d_pipe;

  public:
    /// create a default pipeline
    /// @param mtx the architectural context
    Pipeline (Mtx* mtx);

    /// create a new pipeline with a context and name
    /// @param mtx the architectural context
    /// @param name the pipeline name
    Pipeline (Mtx* mtx, const string& name);

    /// destroy this pipeline
    ~Pipeline (void);

    /// reset this pipeline
    void reset (void);

    /// flush this pipeline
    void flush (void);

    /// return true if one stage is holding
    bool isholding (const long index) const;

    /// run all pipeline stages
    void run (void);

    /// run one pipeline stage by index
    /// @param index the stage index to run
    void run (const long index);

    /// report on all pipeline stages
    void report (void) const;

    /// set the stat collector to all stages
    /// @param stc the stat collector to set
    void setstc (Stat* stc);

    /// set the resource tracer to all resources
    /// @param tracer the resource tracer to bind
    void settrc (Tracer* tracer);

    /// clear this pipeline
    virtual void clear (void);

    /// @return the pipeline depth
    virtual long depth (void) const;

    /// add a new stage in this pipeline
    /// @param stg the stage to add
    virtual void add (Stage* stg);

    /// @return a pipeline stage by name
    virtual Stage* get (const string& name) const;

    /// bind this pipeline with an environment and the enclosing stages
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    virtual void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return true if the pipeline is halted
    virtual bool ishalted (void) const;

  private:
    // make the copy constructor private
    Pipeline (const Pipeline&);
    // make the assignement operator private
    Pipeline& operator = (const Pipeline&);
  };
}

#endif
