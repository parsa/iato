// ---------------------------------------------------------------------------
// - Station.hpp                                                             -
// - iato:mac library - reservation station table class definition           -
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

#ifndef  IATO_STATION_HPP
#define  IATO_STATION_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Station class is a resource that is designed to act as a reservation
  /// station table. A reservation station is one entry in the table. The
  /// sole purpose of a resrvationn station is to park instruction waiting
  /// for their operands. When the operands are ready, the instruction is
  /// waked-up and delivered to the next stage. The complete administration
  /// task is actually quite complexe. First, a valid bit indicates if the
  /// entry is valid. Second a wake-up bit indicates if the instruction has
  /// been waked-up. The instruction remains in the station until it has
  /// been write-backed or cancelled by local or global flush. The station
  /// also maintains a priority table that is used by the scheduler to
  /// select the ready instructions.

  class Station : public Resource {
  private:
    /// the station type
    t_unit d_type;
    /// the station gcs index
    long   d_igcs;
    /// the number of entries
    long d_size;
    /// the reservation station table
    struct t_rsen* p_stbl;
    /// the scheduling table
    long*  p_rsst;

  public:
    /// create a station table by type
    /// @param unit the station unit type
    Station (const t_unit unit);

    /// create a station table by type and context
    /// @param mtx the architectural context
    /// @param unit the station unit type
    Station (Mtx* mtx, const t_unit unit);
 
    /// create a station table by type and context and name
    /// @param mtx the architectural context
    /// @param unit the station unit type
    /// @param name the station name
    Station (Mtx* mtx, const t_unit unit, const string& name);

    /// destroy this station table
    ~Station (void);

    /// reset the station table
    void reset (void);

    /// report this resource
    void report (void) const;

    /// set the station gcs index
    /// @param index the index to set
    void setgcs (const long index);

    /// @return the station gcs index
    long getgcs (void) const;

    /// @return true if the table is full
    bool isfull (void) const;

    /// @return true if the table is empty
    bool isempty (void) const;

    /// alloc a new entry in the table by instruction
    /// @param dsi the instruction to insert
    long alloc (const Dsi& dsi);

    /// clear a station entry by index
    /// @param index the station index
    void clear (const long index);

    /// set the station ready bit by rid
    /// @param rid the rid to compare
    void setrdy (const Rid& rid);

    /// @return the next instruction ready for execution
    Dsi getrdy (void);

    /// mark instruction cancelled by rid and value
    /// @param rid the rid to check
    /// @param value the cancel flag
    void setcnl (const Rid& rid, const bool value);

    /// reschedule an instruction by index
    /// @param index the instruction station index
    void resched (const long index);

    /// mark the predicate not ready flag
    /// @param index the instruction station index
    /// @param pnrd the predicate not ready flag
    void setpnrd (const long index, const bool pnrd);

    /// dump the station content
    void dump (void) const;

  private:
    // make the copy constructor private
    Station (const Station&);
    // make the assignment operator private
    Station& operator = (const Station&);
    /// find a free reservation station index or -1
    long findsta (void) const;
    /// find a free scheduler index or -1
    long findsch (void) const;
    /// repack the scheduler table
    void repack (void);
  };
}

#endif
