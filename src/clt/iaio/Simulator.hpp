// ---------------------------------------------------------------------------
// - Simulator.hpp                                                           -
// - iato:iaio client - in-order simulator class definition                  -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#ifndef  IATO_SIMULATOR_HPP
#define  IATO_SIMULATOR_HPP

#ifndef  IATO_STAT_HPP
#include "Stat.hpp"
#endif

#ifndef  IATO_SYSTEM_HPP
#include "System.hpp"
#endif

#ifndef  IATO_PROCESSOR_HPP
#include "Processor.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Simulator class is an integrated class that associates a processor
  /// core with various components like the memory interface, the information
  /// units and/or the statistical monitors. At construction, all simulation
  /// unit are created according to the context definition. Then all units
  /// are binded together with the "bind" method. Finally the simulation is
  /// run with the "run" method. A simulator class is highly dependant on
  /// the architecture. The core unit is the processor that integrates
  /// all processors stages and resources used during the simulation.
  /// The memory unit integrates the instruction memory and/or caches.
  /// Initially, the constructor takes a context object that has been defined
  /// from the program options. The program name to simulate is used to build
  /// a memory image prior simulating it.

  class Simulator {
  private:
    /// the program name to simulate
    string d_name;
    /// the verbose flag
    bool d_vflag;
    /// the tracer flag
    bool d_tflag;
    /// the cycle counter
    long d_cycle;
    /// the maximum cycle count
    long d_maxcc;
    /// the maximum instruction count
    long d_maxic;
    /// the check flag
    bool d_cflag;
    /// the stat flag
    bool d_sflag;
    /// the stat cycle count
    long d_sccnt;
    /// the stat file name
    string d_sname;
    /// the system interface
    System*    p_psys;
    /// the processor core
    Processor* p_proc;
    /// the stat collection
    Stat*      p_pstc;
    /// the emulator tracer
    Tracer*    p_ptrc;
    /// the emulator checker
    Checker*   p_pchk;

  public:
    /// create a simulator with a context and a program
    /// @param stx the simulation context
    /// @param name the program name to simulate
    Simulator (Stx* ctx, const string& name);

    /// create a simulator with a context, a program and arguments
    /// @param stx the simulation context
    /// @param name the program name to simulate
    /// @param argv the program arguments
    Simulator (Stx* ctx, const string& name, const vector<string>& argv);
    
    /// destroy this simulator
    ~Simulator (void);

    /// @return the cycle counter
    long getcycle (void) const;    

    /// reset the simulator
    void reset (void);

    /// bind the simulator
    void bind (void);

    /// run the simulator
    int run (void);

    /// loop on the simulator
    void loop (void);

    /// report this simulator
    void report (void) const;

    /// print some simulator stats
    void pstat (void);

    /// execute a complete simulation
    void exec (void);

  private:
    // make the copy constructor private
    Simulator (const Simulator&);
    // make the assignment operator private
    Simulator& operator = (const Simulator&);
  };
}

#endif
