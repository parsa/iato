// ---------------------------------------------------------------------------
// - Disperse.hpp                                                            -
// - iato:mac library - bundle dispersion class definition                   -
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

#ifndef  IATO_DISPERSE_HPP
#define  IATO_DISPERSE_HPP

#ifndef  IATO_SPB_HPP
#include "Spb.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// the Disperse class is a generic class that can disperse a bundle
  /// into an issue port buffer. The class is a generic one and the simplest
  /// implementation can disperse an instruction to the first available unit
  /// only. More complex rule can be added by derivation if needed. In the
  /// base class, A type instruction are not disperse to either M or I. For
  /// that kind of dispersal, another disperser is required. The expand method
  /// disperse a bundle into the issue port buffer. If the bundle has been
  /// succesfully dispersed, the method returns true. If the bundle is
  /// partially dispersed, the dispersed instruction are marked invalid in
  /// the bundle and the method returns false.

  class Disperse : public Resource {
  protected:
    /// the issue port buffer
    Spb* p_ipb;
    /// the interrupt buffer
    Iib* p_iib;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a default dispersal engine
    Disperse (void);

    /// create a dispersal engine by name
    /// @param name the dispersal engine resource name
    Disperse (const string& name);

    /// create a new dispersal engine system by context
    /// @param mtx the architectural context
    Disperse (Mtx* mtx);

    /// create a new dispersal engine system by context and name
    /// @param mtx  the architectural context
    /// @param name the dispersal engine resource name
    Disperse (Mtx* mtx, const string& name);

    /// reset this resource
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if a bundle must be pushed back
    virtual bool isback (const Bundle& bndl) const;

    /// @return true if a bundle can be followed by another one
    virtual bool isnext (const Bundle& bnld) const;

    /// disperse a bundle into an issue port buffer
    /// @param bndl the bundle to expand
    /// @param boix the bundle index
    virtual void expand (Bundle& bndl, const long boix);

    /// bind the ipb and the scoreboard
    /// @param iib the interrupt buffer
    /// @param ipb the ipb to bind
    /// @param psb the scoreboard to bind
    virtual void bind (Spb* ipb, Iib* iib, Scoreboard* psb);

  private:
    // make the copy constructor private
    Disperse (const Disperse&);
    // make the assignment operator private
    Disperse& operator = (const Disperse&);
  };
}

#endif
