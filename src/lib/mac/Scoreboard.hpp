// ---------------------------------------------------------------------------
// - Scoreboard.hpp                                                          -
// - iato:mac library - register scoreboarding class definition              -
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
  /// By default, all Itanium ISa register can be monitored by the scoreboard
  /// with a simple lock or unlock mode. When a register instruction is marked
  /// in the scoreboard, the whole issue group might be put on hold until such
  /// register is unlocked. The locking model is by default the full model
  /// that provides scoreboarding for all register. Other models might be
  /// implemented lto support relazed model. The scoreboard also operates
  /// with an interrupted instruction index. The index is the oldest
  /// interrupted instruction in the instruction group.
  
  
  class Scoreboard : public Resource {
  public:
    /// the scoreboard model
    enum t_model {
      MDEF, // default model
      MALL  // all register locking
    };
    
  private:
    /// the scoreboard model
    t_model d_mode;
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
    /// the umrg busy lock
    bool  d_umrg;
    /// the psrg busy lock
    bool  d_psrg;
    /// the serialize flag
    bool  d_srlz;

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

    /// @return true if a register is marked
    bool ismarked (const Rid& rid) const;
    
    /// mark a register by rid and flag
    /// @param rid  the register id to set
    /// @param flag the register flag to set
    void mark (const Rid& rid, const bool flag);
    
    /// set a serialization condition
    /// @param ssi the instruction to use for serialization
    void setsr (const Ssi& ssi);

    /// unset a serialization condition
    /// @param ssi the instruction to use for serialization
    void unsetsr (const Ssi& ssi);

    /// lock all registers associated with an instruction
    /// @param ssi the instruction to use for locking
    void lock (const Ssi& ssi);

    /// unlock all registers associated with an instruction
    /// @param ssi the instruction to use for locking
    void unlock (const Ssi& ssi);
 
    /// @return true if the instruction presents a hazard
    bool ishazard (const Ssi& ssi) const;

  private:
    // make the copy constructor private
    Scoreboard (const Scoreboard&);
    // make the assignment operator private
    Scoreboard& operator = (const Scoreboard&);
  };
}

#endif
