// ---------------------------------------------------------------------------
// - Gcs.hpp                                                                 -
// - iato:mac library - grand central station resource class definition      -
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

#ifndef  IATO_GCS_HPP
#define  IATO_GCS_HPP

#ifndef  IATO_STATION_HPP
#include "Station.hpp"
#endif

namespace iato {
  
  /// The Gcs class is the Grand Central Station class, that is a collection
  /// of reservation station tables. The sole purpose of creating such 
  /// resource is to permit a particular pipelane stage to broadcast ready
  /// operands to all stations.

  class Gcs : public Resource {
  private:
    /// the number of station
    long d_size;
    /// the station length
    long d_slen;
    /// the vector of stations
    Station** p_vsta;

  public:
    /// create an empty central with a context
    /// @param mtx the architectural context
    Gcs (Mtx* mtx);

    /// destroy this gcs
    ~Gcs (void);

    /// reset this central
    void reset (void);

    /// report this resource
    void report (void) const;

    /// set the stat collector to all stations
    /// @param stc the stat collector to set
    void setstc (Stat* stc);

    /// bind the tracer to all stations
    /// @param tracer the resource tracer to bind
    void settrc (Tracer* tracer);

    /// add a station to this central and return its index
    /// @param sta the station to add
    long add (Station* sta);

    /// clear a station entry by gcs and station index
    /// @param igcs the gcs index
    /// @param sidx the station index
    void clear (const long igcs, const long sidx);

    /// clear a station etry by instruction
    /// @param dsi the instruction used to clear
    void clear (const Dsi& dsi);

    /// broadcast a cancel flag to all stations
    /// @param resl the result to broadcast
    void setcnl (const Result& resl);

    /// broadcast a rid to all station
    /// @param rid the rid to broadcast
    void setrdy (const Rid& rid);

    /// broadcast a result to all station
    /// @param resl the result to broadcast
    void setrdy (const Result& resl);

    /// broadcast an instruction to all station or decrease latency
    /// @param inst the instruction to check
    Dsi setrdy (const Dsi& inst);

    /// reschedule an instruction in a station
    void resched (const Dsi& dsi);

    /// dump all stations contents
    void dump (void) const;

  private:
    // make the copy constructor private
    Gcs (const Gcs&);
    // make the assignment operator private
    Gcs& operator = (const Gcs&);
  };
}

#endif
