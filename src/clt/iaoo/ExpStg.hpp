// ---------------------------------------------------------------------------
// - ExpStg.hpp                                                              -
// - iato:iaoo client - expand stage class definition                        -
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

#ifndef  IATO_EXPSTG_HPP
#define  IATO_EXPSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_EIQ_HPP
#include "Eiq.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ExpStg is the expand stage. It operates with instructions that have
  /// been renamed physically. After the physical renaming, the instructions
  /// are ready to operate out of order. The theory of operations is actually
  /// quite simple. Each unit is associated with a buffer that acts as a fifo.
  /// instructions are place in the unit queue and wait to be disperse to
  /// the execution stages.
  
  class ExpStg : public Stage {
  private:
    /// the instruction buffer size
    long d_ilen;
    /// the M unit queue
    Eiq d_mbuf;
    /// the I unit queue
    Eiq d_ibuf;
    /// the F unit queue
    Eiq d_fbuf;
    /// the B unit queue
    Eiq d_bbuf;

  public:
    /// create a new expand stage by context
    /// @param stx the simulation context
    ExpStg (Stx* stx);

    /// create a new expand by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    ExpStg (Stx* stx, const string& name);

    /// reset this expand stage
    void reset (void);

    /// flush this expand stage
    void flush (void);

    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this expand stage
    void activate (void);

    /// report some stage information
    void report (void) const;
 
    /// clear the expand resources
    void clear (void);

    /// @return true if all queues are empty
    bool isempty (void) const;

    /// @return true if a queue is empty by unit
    bool isempty (const t_unit unit) const;
 
    /// @return true if all queues are full
    bool isfull (void) const;

    /// @return true if a queue is full by unit
    bool isfull (const t_unit unit) const;

    /// @return an instruction in the queue by unit
    Dsi pop (const t_unit unit);

  private:
    // make the copy constructor private
    ExpStg (const ExpStg&);
    // make the assignement operator private
    ExpStg& operator = (const ExpStg&);
  };
}

#endif
