// ---------------------------------------------------------------------------
// - Urf.hpp                                                                 -
// - iato:mac library - universal register file class definition             -
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

#ifndef  IATO_URF_HPP
#define  IATO_URF_HPP

#ifndef  IATO_RAT_HPP
#include "Rat.hpp"
#endif

#ifndef  IATO_TRB_HPP
#include "Trb.hpp"
#endif

#ifndef  IATO_URB_HPP
#include "Urb.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

#ifndef  IATO_OPERAND_HPP
#include "Operand.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Urf class is the universal register file class definition. The Urf
  /// is built with a translation register bank (trb), the universal register
  /// bank (urb), and the register alias table (rat). The main reason to group
  /// these three resources into one, is to simplify the operand evalution 
  /// and the result update as well as the speculative flush of the renaming
  /// logic. Each individual resources can be accessed by getting them.

  class Urf : public Resource {
  private:
    /// the rat
    Rat* p_rat;
    /// the trb
    Trb* p_trb;
    /// the urb
    Urb* p_urb;

  public:
    /// create a default urf
    Urf (void);

    /// create a urf by context
    /// @param mtx the architectural context
    Urf (Mtx* mtx);

    /// create a urf by context and name
    /// @param mtx the architectural context
    /// @param name the rat resource name
    Urf (Mtx* mtx, const string& name);

    /// destroy this urf
    ~Urf (void);

    /// reset this urf
    void reset (void);

    /// report this resource
    void report (void) const;

    /// flush this urf
    void flush (void);

    /// partial flush this urf
    void pflsh (void);

    /// @return the associated rat
    Rat* getrat (void) const;

    /// @return the associated trb
    Trb* gettrb (void) const;

    /// @return the associated trb
    Urb* geturb (void) const;

    /// @return true if a trb entry is ready
    bool isready (const Rid& rid) const;

    /// @return true if an operand is ready
    bool isready (const Operand& oprd) const;

    /// evaluate a rid in the urf
    /// @param rid the rid to evaluate
    Uvr eval (const Rid& rid) const;

    /// evaluate an operand in the urf
    /// @param oprd the operand to evaluate
    void eval (Operand& oprd) const;

    /// reroute a rid in the urf
    /// @param rid the rid to reroute
    void reroute (const Rid& rid);

    /// reroute all result registers
    /// @param resl the result used for rerouting
    void reroute (const Result& resl);

    /// update the urf with a result
    /// @param resl the result used for update
    void update (const Result& resl);

    /// clean the urf with a result
    /// @param resl the result used for cleanup
    void clean (const Result& resl);

    /// cancel the urf with a result
    /// @param resl the result used for update
    void cancel (const Result& resl);

  private:
    // make the copy constructor private
    Urf (const Urf&);
    // make the assignement operator private
    Urf& operator =(const Urf&);
  };
}

#endif
