// ---------------------------------------------------------------------------
// - Trb.hpp                                                                 -
// - iato:mac library - translation register bank class definition           -
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

#ifndef  IATO_TRB_HPP
#define  IATO_TRB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Trb class is the translation register bank class definition. For
  /// each register type a translation bank is created with a size equal
  /// to the number of physical registers. Each register provides a direct
  /// translation to the physical register. A valid bit, associated with
  /// the register indicates if that translation register is valid. At
  /// reset, the default translation is initialized with a mapping
  /// corresponding to the logical register definition.

  class Trb : public Resource {
  private:
    /// the bank size
    long d_size;
    /// vtr bank
    struct t_vtr* p_vtrb;

  public:
    /// create a default translation bank
    Trb (void);

    /// create a translation bank by context
    /// @param mtx the architectural context
    Trb (Mtx* mtx);

    /// create a translation bank by context and name
    /// @param mtx the architectural context
    /// @param name the rat resource name
    Trb (Mtx* mtx, const string& name);

    /// destroy this bank
    ~Trb (void);

    /// reset this bank
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if a trb entry is ready
    bool isready (const long vnum) const;

    /// @return true if a translation bit is set
    bool gettbit (const long vnum) const;

    /// set the trb translation number
    /// @param vnum the virtual number
    /// @param tnum the translation number
    /// @param rbit the ready bit
    void settnum (const long vnum, const long tnum, const bool rbit);

    /// @return the translation number
    long gettnum (const long vnum) const;

    /// allocate an initial translation
    long initial (void);

    /// @return the index of a new allocated trb entry
    long alloc (void);

    /// clean a trb entry by number and return the old translation
    /// @param vnum the virtual number
    /// @param onum the old virtual number
    long clean (const long vnum, const long onum);

    /// cancel a trb translation by forcing the ready bit
    /// @param vnum the virtual number
    /// @param onum the old virtual number
    void cancel (const long vnum, const long onum);

    /// force the ready bit by index
    /// @param vnum the virtual number
    void setrdy (const long vnum);

  private:
    // make the copy constructor private
    Trb (const Trb&);
    // make the assignement operator private
    Trb& operator =(const Trb&);
  };
}

#endif
