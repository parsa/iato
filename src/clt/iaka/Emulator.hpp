// ---------------------------------------------------------------------------
// - Emulator.hpp                                                            -
// - iato:iaka client - emulator class definition                            -
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

#ifndef  IATO_EMULATOR_HPP
#define  IATO_EMULATOR_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_IRT_HPP
#include "Irt.hpp"
#endif

#ifndef  IATO_STAT_HPP
#include "Stat.hpp"
#endif

#ifndef  IATO_RENAME_HPP
#include "Rename.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

#ifndef  IATO_FETCHER_HPP
#include "Fetcher.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_MEMLOGIC_HPP
#include "MemLogic.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Emulator class is a complete IA64 ISA emulator engine that
  /// reads a memory image and emulate such program. The emulator maintains
  /// register bank image that can be used to dump a program state

  class Emulator {
  private:
    /// the program name to emulate
    string d_name;
    /// the elf image
    ElfImage*   p_elf;
    /// the elf executable image
    ElfExec*    p_mem;
    /// the rse
    Rename*     p_rse;
    /// the irt
    Irt*        p_irt;
    /// the register bank
    Register*   p_rbk;
    /// the memory logic
    MemLogic*   p_meml;
    /// the fetch buffer
    Fetcher*    p_ftch;
    /// the branch predictor
    Branch*     p_bprd;
    /// the M execute unit
    Executable* p_munit;
    /// the I execute unit
    Executable* p_iunit;
    /// the F execute unit
    Executable* p_funit;
    /// the B execute unit
    Executable* p_bunit;
    /// the trace flag
    bool d_tflag;
    /// the verbose flag
    bool d_vflag;
    /// the branch record flag
    bool d_bflag;
    /// the branch only flag
    bool d_boflg;
    /// the predicate only flag
    bool d_poflg;
    /// the check flag
    bool d_cflag;
    /// the stat flag
    bool d_sflag;
    /// the disperse flag
    bool d_dflag;
    /// the cycle counter
    long d_cycle;
    /// the maximum cycle count
    long d_maxcc;
    /// the branch pipeline refill
    long d_bpfil;
    /// the stat collection
    Stat*    p_stat;
    /// the emulator tracer
    Tracer*  p_tracer;
    /// the emulator checker
    Checker* p_checker;

  public:
    /// create a emulator with a context and a program
    /// @param atx the application context
    /// @param name the program name to emulate
    Emulator (Atx* atx, const string& name);

    /// create a emulator with a context, a name and arguments
    /// @param atx the application context
    /// @param name the program name to emulate
    /// @param argv the program arguments
    Emulator (Atx* atx, const string& name, const vector<string>& argv);
    
    /// destroy this emulator
    ~Emulator (void);

    /// reset the emulator
    void reset (void);

    /// print some emulator stats
    void pstat (void);

    /// bind this emulator
    void bind (void);

    /// run the emulator
    int run (void);

    /// execute a bundle
    /// @param bnd the bundle to execute
    void exec (const Bundle& bnd);

  private:
    // make the copy constructor private
    Emulator (const Emulator&);
    // make the assignment operator private
    Emulator& operator = (const Emulator&);
    /// loop in the emulator
    void loop (void);
    // execute an instruction
    bool exec (Instr& inst);
    // update the emulator with a result
    void update (Result& resl);
  };
}

#endif
