// ---------------------------------------------------------------------------
// - Ssi.hpp                                                                 -
// - iato:mac library - Statically scheduled instruction class definition    -
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

#ifndef  IATO_SSI_HPP
#define  IATO_SSI_HPP

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

namespace iato {
  
  /// The Ssi class is the sta=tically scheduled instruction that is derived
  /// from the isa instruction class. The class holds additional instruction
  /// information that are used in a speculative environment. One particular
  /// important information is the speculative cfm value that is vaed in the 
  /// instruction.

  class Ssi : public Instr {
  protected:
    /// the reorder index
    long d_ridx;
    /// the iib index
    long d_iidx;
    /// the cancellation flag
    bool d_cnlf;
    /// the interrupt flag
    bool d_intr;
    /// the valid speculation flag
    bool d_vspf;
    /// the predicted predicate value
    bool d_ppvl;
    /// the predicate prediction flag
    bool d_ppfl;
    /// the extra stat flag
    bool d_xflg;
    /// the instruction rse state
    Rse::State  d_iste;
    /// the speculative rse state
    Rse::State  d_sste;

  public:
    /// create a default ssi
    Ssi (void);

    /// create a ssi from an instruction
    /// @param inst the instruction to use
    Ssi (const Instr& inst);

    /// copy construct this ssi
    /// @param that the ssi to copy
    Ssi (const Ssi& that);

    /// assign a ssi to this one
    /// @param that the ssi to assign
    Ssi& operator = (const Ssi& that);

    /// assign an instruction to this ssi
    /// @param that the instruction to assign
    Ssi& operator = (const Instr& that);

    /// reset this ssi
    void reset (void);

    /// @return true if this instruction needs pre-serialization
    bool ispresr (void) const;

    /// @return true if this instruction needs post-serialization
    bool ispostsr (void) const;

    /// set the reorder index
    /// @param index the reorder index
    void setrix (const long index);
    
    /// @return the reorder index
    long getrix (void) const;

    /// set the iib index
    /// @param index the iib index
    void setiib (const long index);
    
    /// @return the iib index
    long getiib (void) const;

    /// set the instruction cancellation flag
    /// @param cnlf the cancel flag to set
    void setcnlf (const bool cnlf);

    /// set the instruction cancellation flag by rid
    /// @param rid  the rid to use for cancellation
    /// @param cnlf the cancel flag to set
    void setcnlf (const Rid& rid, const bool cnlf);

    /// @return the cancellation flag
    bool getcnlf (void) const;

    /// set the interrupt flag flag
    /// @param intr the interrupt flag to set
    void setintr (const bool intr);

    /// @return the interrupt flag
    bool getintr (void) const;

    /// set the valid speculation flag
    /// @param vspf the speculation flag to set
    void setvspf (const bool vspf);

    /// @return the valid speculation flag
    bool getvspf (void) const;

    /// set the predicated predicate value
    /// @param ppvl the predicted value to set
    void setppvl (const bool ppvl);

    /// @return the predicted predicate value
    bool getppvl (void) const;
 
    /// set the predicate prediction flag
    /// @param ppfl the prediction flag to set
    void setppfl (const bool ppfl);

    /// @return the predicate prediction flag
    bool getppfl (void) const;

    /// set the stat extra flag
    /// @param xflg the extra flag to set
    void setxflg (const bool xflg);

    /// @return the stat extra flag
    bool getxflg (void) const;

    /// set the instruction rse state
    /// @param state the state to set
    void setiste (const Rse::State& state);

    /// @return the instruction rse state
    Rse::State getiste (void) const;

    /// @return the instruction cfm
    Cfm geticfm (void) const;

    /// set the speculative rse state
    /// @param state the state to set
    void setsste (const Rse::State& state);

    /// @return the speculative rse state
    Rse::State getsste (void) const;

    /// @return the speculative cfm
    Cfm getscfm (void) const;
  };
}

#endif
