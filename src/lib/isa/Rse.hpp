// ---------------------------------------------------------------------------
// - Rse.hpp                                                                 -
// - iato:isa library - register stack engine class definition               -
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

#ifndef  IATO_RSE_HPP
#define  IATO_RSE_HPP

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

#ifndef  IATO_CFM_HPP
#include "Cfm.hpp"
#endif

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Rse class is a complete Register Stack Engine class that is
  /// responsible to manage register renaming as well as spilling and filling
  /// of the stacked registers. Since the RSE is affected both by instructions
  /// and its internal state, the class separate both functions by operating
  /// virtualy with instruction or results. For exemple, with an emulator
  /// the rse is affected by instructions (like alloc) or by results (like
  /// br.ret). With a simulator, the rse must be able to operate speculatively.
  /// For this reason, the rse operates with a state object that defines the
  /// current rse state (real or speculative).

  class Rse : public Resource {
  public:
    /// the rse state
    class State {
    private:
      /// number of general registers
      long d_ngr;
      /// the size of frame
      long d_sof;
      /// the size of local
      long d_sol;
      /// the size of rotating
      long d_sor;
      /// the rotating gr base
      long d_rgr;
      /// the rotating fr base
      long d_rfr;
      /// the rotating pr base
      long d_rpr;
      /// the bottom of frame
      long d_bof;
      /// the load register number
      long d_ldr;
      /// the store register number
      long d_str;
      /// the number of dirty registers
      long d_dty;

    public:
      /// create a default rse state
      State (void);

      /// create a rse state with a register size
      /// @param ngr the number of logical register
      State (const long ngr);

      /// create a rse state with a register size and a cfm
      /// @param ngr the number of logical register
      /// @param cfm the cfm used to set the state
      State (const long ngr, const Cfm& cfm);

      /// reset this rse state
      void reset (void);

      /// compare a state with another one
      bool operator == (const State& that) const;

      /// set the number of general register
      /// @param ngr the number of general registers
      void setngr (const long ngr);

      /// @return the cfm associated with this state
      Cfm getcfm (void) const;

      /// set the state with a cfm
      /// @param cfm the cfm used to set the rse state
      void setcfm (const Cfm& cfm);

      /// @return the state bottom of frame
      long getbof (void) const;

      /// set the state with a bottom of frame
      /// @param val the bottom of frame value.
      void setbof (const long val);

      /// check that the cfm state equal the calling cfm
      /// @param cfm the cfm to check
      bool chkcfm (const Cfm& cfm) const;

      /// update the rse state for an alloc
      /// @param cfm the cfm used to update the rse state
      void alloc (const Cfm& cfm);

      /// update the rse state for a call
      /// @param cfm the cfm used to update the rse state
      void call (const Cfm& cfm);

      /// update the rse state for a return
      /// @param cfm the cfm used to update the rse state
      void retn (const Cfm& cfm);

      /// update the rse state for a branch loop
      /// @param cfm the cfm used to update the rse state
      void loop (const Cfm& cfm);

      /// map the general register index
      /// @param lnum the logical register number
      /// @param rwf the read write flag (false = read)
      long mapgr (const long lnum, const bool rwf) const;

      /// map the floating register index
      /// @param lnum the logical register number
      long mapfr (const long lnum) const;

      /// map the predicate register index
      /// @param lnum the logical register number
      long mappr (const long lnum) const;

      /// map a rid by doing register renaming
      /// @param rid the rid to rename
      /// @param rwf the read-write flag
      Rid maprid (const Rid& rid, const bool rwf) const;

      /// spill register if needed
      void spill (void);

      /// fill register if needed
      void fill (void);

      /// dump the rse state (for debug)
      void dump (const string& prefix) const;
    };

  protected:
    /// the number of logical register
    long  d_ngr;
    /// the rse state
    State d_state;

  public:
    /// create a default rse
    Rse (void);

    /// create a rse with a context
    /// @param ctx the current context
    Rse (Ctx* ctx);

    /// create a rse with a context and a name
    /// @param ctx the current context
    /// @param name the resource name
    Rse (Ctx* ctx, const string& name);

    /// reset the rse
    void reset (void);

    /// flush this rse
    virtual void flush (void);

    /// set the rse state by state
    /// @param state the state to set
    virtual void setste (const State& state);

    /// @return the current rse state
    virtual State getste (void) const;

    /// set the speculative rse state by state
    /// @param state the state to set
    virtual void setsst (const State& state);

    /// @return the speculative  rse state
    virtual State getsst (void) const;

    /// validate a state against the rse state
    virtual bool validate (const State& state) const;

    /// validate a cfm against the rse state
    virtual bool validate (const Cfm& cfm) const;

    /// rename an instruction with the rse
    /// @param inst the instruction to rename
    virtual void rename (Instr& inst) const;

    /// preset the rse state with an instruction
    /// @param inst the instruction used to preset
    virtual void preset (const Instr& inst) =0;

    /// after set the rse state with an instruction
    /// @param inst the instruction used to after set
    virtual void aftset (const Instr& inst) =0;

    /// update the rse state with a result
    /// @param resl the result used to update
    virtual void update (const Result& resl) =0;

  private:
    // make the copy constructor private
    Rse (const Rse&);
    // make the assignment operator private
    Rse& operator = (const Rse&);
  };
}
#endif
