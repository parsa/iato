// ---------------------------------------------------------------------------
// - Mli.hpp                                                                 -
// - iato:mac library - memory logic interface class definition              -
// ---------------------------------------------------------------------------
// - inria 2002-2004                                                         -
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

#ifndef  IATO_MLI_HPP
#define  IATO_MLI_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

#ifndef  IATO_MOB_HPP
#include "Mob.hpp"
#endif

#ifndef  IATO_MPR_HPP
#include "Mpr.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Mli class is a special resource used to perform read operations
  /// from a result object with a memory port request. This class acts as a
  /// simple logic.

  class Mli : public Resource {
  private:
    /// the port request
    Mpr* p_mpr;
    /// the memory ordering buffer
    Mob* p_mob;

  public:
    /// create a default memory read logic
    Mli (void);

    /// create a new memory read logic by context
    /// @param mtx  the architectural context
    Mli (Mtx* mtx);

    /// create a new memory read logic by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Mli (Mtx* mtx, const string& name);
 
    /// reset this memory read logic
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// bind a memory request port
    /// @param mta the memory transaction adapter
    /// @param mob the memory ordering buffer
    void bind (Mta* mta, Mob* mob);

    /// preset a memory operation with a result
    /// @param inst  the instruction to process
    /// @param resl the result used for request
    void preset (const Ssi& inst, Result& resl);

    /// update a memory operation with a result
    /// @param inst  the instruction to process
    /// @param resl the result used for request
    void update (const Ssi& inst, const Result& resl);

  private:
    // make the copy constructor private
    Mli (const Mli&);
    // make the assignment oerator private
    Mli& operator = (const Mli&);
  };
}

#endif
