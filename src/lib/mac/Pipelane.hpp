// ---------------------------------------------------------------------------
// - Pipelane.hpp                                                            -
// - iato:mac library - generic parallel pipeline class definition           -
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

#ifndef  IATO_PIPELANE_HPP
#define  IATO_PIPELANE_HPP

#ifndef  IATO_PIPELINE_HPP
#include "Pipeline.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Pipelane class is a collection of micro pipeline that operates in
  /// parallel. At each cycle, all parallel stages are run in parallel. Each
  /// micro pipeline do not need to have the same depth. by default, the
  /// pipelane operates in non-blocking mode. This means that if a stage
  /// is holding, onnly thoise previous stage in the same micro-pipeline
  /// will be stalled. If the pipelane operates in blocking mode, a holding
  /// stage causes all other stages to block. Such mode is used with an
  /// in-order machine.

  class Pipelane : public Stage {
  private:
    /// the pipelane type
    typedef vector<Pipeline*> t_plane;
    /// the pipelane vector
    t_plane d_lane;
    /// the pipeline controler
    Runnable** p_rctrl;
    /// the maximum depth
    long d_depth;
    /// the blocking mode
    bool d_block;

  public:
    /// create a default pipelane with a context
    /// @param mtx the architectural context
    Pipelane (Mtx* mtx);

    /// create a new pipelane with a context and name
    /// @param mtx the architectural context
    /// @param name the pipelane name
    Pipelane (Mtx* mtx, const string& name);

    /// destroy this pipelane
    ~Pipelane (void);

    /// reset this pipelane
    void reset (void);

    /// flush this pipelane
    void flush (void);

    /// return true if one stage is holding
    bool isholding (void) const;

    /// activate all pipelines
    void activate (void);

    /// report on all pipelane stages
    void report (void) const;

    /// set the stat collector to all pilelines
    /// @param stc the stat collector to set
    void setstc (Stat* stc);

    /// set the resource tracer to all pipelines
    /// @param tracer the resource tracer to bind
    void settrc (Tracer* tracer);

    /// set the blocking mode
    /// @param mode the mode to set
    void setblock (const bool mode);

    /// @return the blocking mode
    bool getblock (void) const;

    /// clear this pipelane
    virtual void clear (void);

    /// @return the pipelane maximum depth
    virtual long depth (void) const;

    /// add a new pipeline in this pipelane
    /// @param pipe the pipeline to add
    virtual void add (Pipeline* pipe);

    /// set the pipeline control logic by index
    /// @param index the pipeline index
    /// @param pclog the pipeline control logic
    virtual void setclog (const long index, Runnable* pclog);

    /// bind this pipelane with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

  private:
    // make the copy constructor private
    Pipelane (const Pipelane&);
    // make the assignement operator private
    Pipelane& operator = (const Pipelane&);
  };
}

#endif
