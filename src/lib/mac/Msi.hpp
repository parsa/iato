// ---------------------------------------------------------------------------
// - Msi.hpp                                                                 -
// - iato:mac library - memory synchro interface class definition            -
// ---------------------------------------------------------------------------
// - inria 2002-2004                                                         -
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

#ifndef  IATO_MSI_HPP
#define  IATO_MSI_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

#ifndef  IATO_MPR_HPP
#include "Mpr.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Msi class is a special resource used to perform in-order memory
  /// operations. The class operates with a memory port and combines a
  /// memory bypass network that can be used for subsequent load.

  class Msi : public Resource {
  private:
    /// the port request
    Mpr* p_mpr;

  public:
    /// create a default memory synchro logic
    Msi (void);

    /// create a new memory synchro logic by context
    /// @param mtx  the architectural context
    Msi (Mtx* mtx);

    /// create a new memory logic by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Msi (Mtx* mtx, const string& name);
 
    /// reset this memory logic
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// bind a memory request port
    /// @param mta the memory transaction adapter
    void bind (Mta* mta);

    /// preset a memory operation with a result
    /// @param inst the instruction to process
    /// @param resl the result used for request
    void preset (const Ssi& inst, Result& resl);

    /// update a memory operation with a result
    /// @param inst the instruction to process
    /// @param resl the result used for request
    void update (const Ssi& inst, const Result& resl);

  private:
    // make the copy constructor private
    Msi (const Msi&);
    // make the assignment oerator private
    Msi& operator = (const Msi&);
  };
}

#endif
