// ---------------------------------------------------------------------------
// - Stage.hpp                                                               -
// - iato:mac library - abstract stage class definition                      -
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

#ifndef  IATO_STAGE_HPP
#define  IATO_STAGE_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_ENV_HPP
#include "Env.hpp"
#endif

#ifndef  IATO_DELAYABLE_HPP
#include "Delayable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Stage class is an abstract class that models a pipeline stage.
  /// At construction a stage uses a context object to configure itself.
  /// Once all stages have been created, they can be bounded together with
  /// the 'bind' method. The bind method uses an environment object which
  /// among other things holds the global resources of the processor. A
  /// stage is defined with a name. A hold bit also indicates if a stage 
  /// has been put on hold by another stage. Pipeline stages are linked
  /// together in the mean of the previous and next stage.

  class Stage : public Delayable {
  protected:
    /// the halt bit
    bool d_halt;
    /// the stage index
    long d_sidx;
    /// the previous stage
    Stage* p_pstg;
    /// the next stage
    Stage* p_nstg;

  public:
    /// create a new stage by context and name
    /// @param mtx the architectural context
    /// @param name the stage name
    Stage (Mtx* mtx, const string& name);

    /// create a new stage by context, index and name
    /// @param mtx the architectural context
    /// @param sidx the stage index
    /// @param name the stage name
    Stage (Mtx* mtx, const long sidx, const string& name);

    /// reset this stage
    void reset (void);

    /// flush this stage
    void flush (void);

    /// run this stage
    void run (void);

    /// return true if the stage is halted
    virtual bool ishalted (void) const;

    /// @return the holding bit status
    virtual bool isholding (void) const;

    /// set the previous stage
    /// @param env the execution environment
    /// @param stg the previous stage to set
    virtual void setprev (Env* env, Stage* stg);

    /// set the next stage
    /// @param env the execution environment
    /// @param stg the previous stage to set
    virtual void setnext (Env* env, Stage* stg);

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    virtual void bind (Env* env, Stage* pstg, Stage* nstg);
  };
}

#endif
