// ---------------------------------------------------------------------------
// - Scoreboard.hpp                                                          -
// - iato:mac library - register scoreboarding class definition              -
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

#ifndef  IATO_SCOREBOARD_HPP
#define  IATO_SCOREBOARD_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Scoreboard class implements a register scoreboarding facility.
  /// The primary scoreboard structure is the instruction list with their
  /// associated states. Instruction are placed in-order in the list
  /// ands their state is updated by the appropriate stages.
  /// For a given register , indicated by its rid, the scoreboard structure
  /// list the registers those value are produced by multicycle instruction.
  /// Such register are those with L1D misses as well as floating or multi
  /// media instructions.

  class Scoreboard : public Resource {
  public:
    /// the instruction scoreboard states
    enum t_state {
      ISSUED = 0, // instruction issued
      EXECED = 1, // instruction executed
      WRBACK = 2, // instruction write-backed
      INTERP = 3, // instruction interrupted
      CANCEL = 4, // instruction cancelled
      PFSPEC = 5, // pipe flush by speculation
      PFBRTK = 6, // pipe flush branch taken
    };
    /// max number of instruction states
    static const long MAX_STATES = 7;

  private:
    /// The Is class is the instruction scoreboarding, a subunit of the
    /// scoreboard resource. The instruction scoreboard track the instruction
    /// activity across the pipeline. Additionaly, the IS tracks the state
    /// of the instruction with respect to the interrupt status. The IS
    /// is implemented as a cirular buffer

    class Is {
    private:
      /// the buffer size
      long d_size;
      /// buffer structure
      struct t_isb* p_visb;
      /// the input index
      long d_iptr;
      /// the output index
      long d_optr;
      /// the length
      long d_ilen;

    public:
      /// create a default instruction scoreboard
      Is (void);

      /// create a new instruction scoreboard by context
      /// @param mtx the architectural context
      Is (Mtx* mtx);

      /// destroy this instruction scoreboard
      ~Is (void);

      /// reset this instruction scorebard
      void reset (void);

      /// @return true if the instruction scoreboard is empty
      bool isempty (void) const;

      /// @return true if the instruction scoreboard is full
      bool isfull (void) const;

      /// @return true if the index is the current pop index
      bool ispop (const long index) const;

      /// allocate a new entry by instruction
      long alloc (const Ssi& ssi);

      /// allocate a new entry by interrupt
      void alloc (const Interrupt& svi);

      /// @return true if the current instruction has been cleaned
      bool clean (void);

      /// set the instruction state
      /// @param ridx  the scoreboard index
      /// @param state the instruction state to set
      void setstate (const long ridx, t_state state);

      /// @return the instruction state by index and state
      bool getstate (const long ridx, t_state state) const;

    private:
      // make the copy constructor private
      Is (const Is&);
      // make the assignment operator private
      Is& operator = (const Is&);
    };

    /// The Rs class is the register scoreboarding, a subunit of the 
    /// scoreboard resource. The register scoreboard tracks all register
    /// avaibility with the help of a busy bit.

    class Rs {
    private:
      /// number of general register (and nat)
      long  d_gnum;
      /// number of floating registers
      long  d_fnum;
      /// number of predicate registers
      long  d_pnum;
      /// number of branch registers
      long  d_bnum;
      /// number of application registers
      long  d_anum;
      /// number of control registers
      long  d_cnum;
      /// gr busy lock
      bool* p_glok;
      /// fr busy lock
      bool* p_flok;
      /// pr busy lock
      bool* p_plok;
      /// br busy lock
      bool* p_blok;
      /// ar busy lock
      bool* p_alok;
      /// cr busy lock
      bool* p_clok;
      
    public:
      /// create a new register scoreboard
      Rs (void);
      
      /// create a new register  with a context
      /// @param mtx the architectural context
      Rs (Mtx* mtx);
      
      /// destroy this register scoreboard
      ~Rs (void);
      
      /// reset this register scoreboard
      void reset (void);
      
      /// @return true if a register is marked
      bool ismarked (const Rid& rid) const;
      
      /// set a register by rid
      /// @param rid  the register id to set
      /// @param flag the register flag to set
      void mark (const Rid& rid, const bool flag);
      
    private:
      // make the copy constructor private
      Rs (const Rs&);
      // make the assignment operator private
      Rs& operator = (const Rs&);
    };

  private:
    /// the instruction scoreboard
    Is* p_is;
    /// the register scoreboard
    Rs* p_rs;
    /// max instructions to clean
    long d_swsz;

  public:
    /// create a new scoreboard
    Scoreboard (void);
      
    /// create a new scoreboard with a context
    /// @param mtx the architectural context
    Scoreboard (Mtx* mtx);
    
    /// create a new scoreboard with a context and a name
    /// @param mtx the architectural context
    /// @param name the resource name
    Scoreboard (Mtx* mtx, const string& name);
    
    /// destroy this scoreboard
    ~Scoreboard (void);
    
    /// reset this scoreboard
    void reset (void);
    
    /// report this resource
    void report (void) const;

    /// allocate an instruction in the scoreboard
    long alloc (const Ssi& ssi);

    /// allocate an interrupt in the scoreboard
    void alloc (const Interrupt& svi);

    /// clean the scoreboard with write-backed instructions
    void clean (void);

    /// set the instruction state
    /// @param ridx  the scoreboard index
    /// @param state the instruction state to set
    void setstate (const long ridx, t_state state);

    /// set the instruction state
    /// @param ssi   the instruction to mark
    /// @param state the instruction state to set
    void setstate (const Ssi& ssi, t_state state);
    
    /// @return the scoreboard instruction state by index
    bool getstate (const long ridx, t_state state) const;

    /// @return the scoreboard instruction state by instruction
    bool getstate (const Ssi& ssi, t_state state) const;

    /// retire an instruction and clean the scoreboard
    /// @param ssi the instruction to mark
    void retire (const Ssi& ssi);

    /// @return true if a register is marked
    bool ismarked (const Rid& rid) const;
    
    /// set a register by rid
    /// @param rid  the register id to set
    /// @param flag the register flag to set
    void mark (const Rid& rid, const bool flag);
  
    /// @return true if the instruction presents a hazard
    bool ishazard (const Ssi& ssi) const;

    /// @return true if an instruction is holding
    bool isholding (const Ssi& ssi) const;

  private:
    // make the copy constructor private
    Scoreboard (const Scoreboard&);
    // make the assignment operator private
    Scoreboard& operator = (const Scoreboard&);
  };
}

#endif
