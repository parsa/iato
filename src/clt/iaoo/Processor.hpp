// ---------------------------------------------------------------------------
// - Processor.hpp                                                           -
// - iato:iapa client - processor class definition                           -
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

#ifndef  IATO_PROCESSOR_HPP
#define  IATO_PROCESSOR_HPP

#ifndef  IATO_ENV_HPP
#include "Env.hpp"
#endif

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_SYSTEM_HPP
#include "System.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_PIPELINE_HPP
#include "Pipeline.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Processor class is the architectural processor class. All processor
  /// components are defined here to build a detailed out of order 
  /// architecture. Ideally the processor is a set of resources and a
  /// pipeline. At construction, the selected resources are placed into
  /// an execution environment. Also, the pipeline is created by allocating
  /// all stages. During the binding process, all stages are connected to
  /// their resources and neighrbouring stage. A special case occurs for 
  /// input and output stages that communicates with system resources.

  class Processor : public Runnable {
  private:
    /// the processor environment
    Env* p_env;
    /// the processor pipeline
    Pipeline* p_pipe;
    /// initial entry point
    t_octa d_entry;
    /// the top of stack
    t_octa d_stkva;
    /// the backing store
    t_octa d_bspva;
    
  public:
    /// create a new processor by context
    /// @param stx the simulation context
    Processor (Stx* stx);

    /// destroy this processor
    ~Processor (void);

    /// reset this processor
    void reset (void);

    /// flush this processor
    void flush (void);

    /// run this processor
    void run (void);

    /// set an external interrupt
    /// @param vi the external interrupt to set
    void extint (const Interrupt& vi);

    /// report this processor
    void report (void) const;

    /// @return true if the processor is halted
    bool ishalted (void) const;

    /// set the processor with a stat collector
    /// @param stc the stat collectoor to set
    void setstc (Stat* stc);

    /// set the processor with a tracer
    /// @param tracer the trace monitor
    void settrc (Tracer* tracer);

    /// bind the processor with the system interface
    /// @param sys the system interface
    void bind (System* sys);

    /// @return the processor register bank
    Register* getrbk (void) const;

  private:
    // make the copy constructor private
    Processor (const Processor&);
    // make the assignment operator private
    Processor& operator = (const Processor&);
  };
}

#endif

