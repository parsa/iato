// ---------------------------------------------------------------------------
// - Stat.hpp                                                                -
// - iato:isa library - isa statistics collection class definition           -
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


#ifndef  IATO_STAT_HPP
#define  IATO_STAT_HPP

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Stat class is a collection class that can be used to collect various
  /// isa statistics and even more. The class operates by filling various
  /// information like the program name, its size and then the instruction
  /// beeing executed. The class updates its information on the fly, so at
  /// the end, all data are available.

  class Stat {
  protected:
    /// collection start time
    t_long d_stim;
    /// collection end time
    t_long d_etim;
    /// total number of cycles
    t_long d_ncyc;
    /// total number of bundles
    t_long d_nbnd;
    /// total number of instructions
    t_long d_nins;
    /// total number of predicated instruction
    t_long d_nprd;
    /// number of predicated non branch instruction
    t_long d_nbpd;
    /// total number of cancel instruction
    t_long d_ncan;
    /// total number of cancel non branch instruction
    t_long d_nbcn;
    /// total number of rescheduled instructions
    t_long d_nrsh;
    /// number of predicted branches
    t_long d_npbr;
    /// number of successfull branch prediction
    t_long d_npbs;
    /// number of predicted predicates
    t_long d_nppr;
    /// number of successfull predicate prediction
    t_long d_npps;
    /// the bunble distribution array
    t_long d_bndl[Bundle::BN_MAXTPL];
    /// the instruction distribution array
    t_long d_inst[OPCODE_MAX];
    /// the bundle flag
    bool   d_bflg;
    /// the instruction flag
    bool   d_iflg;
    /// the nop flag
    bool   d_nflg;

  public:
    /// create a default stat collection
    Stat (void);

    /// destroy this stat object
    virtual ~Stat (void);

    /// reset this stat object
    virtual void reset (void);

    /// set report selection flags
    /// @param bflg the bundle flag
    /// @param iflg the instruction flag
    /// @param nflg the stop flag
    virtual void setflg (const bool bflg, const bool iflg, const bool nflg);

    /// mark a simulation cycle
    virtual void marksc (void);

    /// mark the end of the simulation
    virtual void markes (void);

    /// mark the branch prediction stat
    /// @parm bflg the successfull branch prediction
    virtual void markbp (const bool bflg);

    /// mark the predicate prediction stat
    /// @parm pflg the successfull predicate prediction
    virtual void markpp (const bool pflg);

    /// add a new bundle for stat collection
    /// @param bndl the bundle to add
    virtual void addbndl (const Bundle& bndl);

    /// add an instruction for stat collection
    /// @param inst the instruction to add
    virtual void addinst (const Instr& inst);

    /// add an instruction for stat collection
    /// @param inst the instruction to add
    /// @param cnlf the cancel flag
    /// @param rsch the reschedule flag
    virtual void addinst (const Instr& inst, const bool cnlf, const bool rsch);

    /// @return the number of nop instructions
    virtual t_long getnnop (void) const;

    /// print the bundle information
    virtual void printb (void) const;

    /// print the instruction information
    virtual void printi (void) const;
  
    /// print the nop information
    virtual void printn (void) const;

    /// print the stat summary
    virtual void summary (void) const;

    /// print a stat report
    virtual void print (void) const;

  private:
    // make the copy constructor private
    Stat (const Stat&);
    // make the assignment operator private
    Stat& operator = (const Stat&);
  };
}

#endif
