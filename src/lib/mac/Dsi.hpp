// ---------------------------------------------------------------------------
// - Dsi.hpp                                                                 -
// - iato:mac library - Dynamically scheduled instruction class definition   -
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

#ifndef  IATO_DSI_HPP
#define  IATO_DSI_HPP

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

namespace iato {
  
  /// The Dsi class is the dynamically scheduled instruction that is derived
  /// from the statically instruction class. The class holds additional 
  /// information that are used in a dynamically scheduled environment. 
  /// Such information includes in particular the the reorder buffer (ROB) 
  /// entry.

  class Dsi : public Ssi {
  private:
    /// the station index
    long d_sidx;
    /// the gcs index
    long d_igcs;
    /// the execution latency
    long d_elat;
    /// the reschedule flag
    bool d_rsch;
    /// the predicate not ready flag
    bool d_pnrd;

  public:
    /// create a default dsi
    Dsi (void);

    /// create a dsi from an instruction
    /// @param inst the instruction to use
    Dsi (const Instr& inst);

    /// copy construct this dsi
    /// @param that the dsi to copy
    Dsi (const Dsi& that);

    /// assign a dsi to this one
    /// @param that the dsi to assign
    Dsi& operator = (const Dsi& that);

    /// assign an instruction to this dsi
    /// @param that the instruction to assign
    Dsi& operator = (const Instr& that);

    /// reset this dsi
    void reset (void);

    /// @return true if this instruction needs pre-serialization
    bool ispresr (void) const;

    /// @return true if this instruction needs post-serialization
    bool ispostsr (void) const;

    /// @return true if the instruction is ready for selection
    bool isready (void) const;

    /// set an instruction operand ready by rid
    /// @param rid the rid use to mark the operand ready
    void setready (const Rid& rid);

    /// set the instruction reschedule flag
    /// @param rsch the reschedule flag to set
    void setrsch (const bool rsch);

    /// @return the reschedule flag
    bool getrsch (void) const;

    /// set the predicate not ready flag
    /// @param pnrd the not ready flag 
    void setpnrd (const bool pnrd);

    /// @return the predicate not ready flag
    bool getpnrd (void) const;

    /// set the station entry
    /// @param index the station index
    void setsid (const long index);
    
    /// @return the station entry
    long getsid (void) const;

    /// set the gcs index
    /// @param index the gcs index
    void setgcs (const long index);
    
    /// @return the gcs index
    long getgcs (void) const;

    /// set the execution latency
    /// @param elat the execution latency
    void setelat (const long elat);

    /// @return the execution latency
    long getelat (void) const;
  };
}

#endif
