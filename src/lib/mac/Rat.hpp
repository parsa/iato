// ---------------------------------------------------------------------------
// - Rat.hpp                                                                 -
// - iato:mac library - ram based register alias table class definition      -
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

#ifndef  IATO_RAT_HPP
#define  IATO_RAT_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RID_HPP
#include "Rid.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Rat class is the ram implementation of the Register Alias Table
  /// The class establishes a binding between a logical register number and
  /// another one. For each register types, a rat is created those size is 
  /// defined by the isa. A special case is induced by the gr bank, that can
  /// have more registers as defined by the isa.

  class Rat : public Resource {
  private:
    /// number of general register (and nat)
    long d_gnum;
    /// number of floating registers
    long d_fnum;
    /// number of predicate registers
    long d_pnum;
    /// number of branch registers
    long d_bnum;
    /// number of application registers
    long d_anum;
    /// number of control registers
    long d_cnum;
    /// gr rat array
    long* p_grat;
    /// fr rat array
    long* p_frat;
    /// pr rat array
    long* p_prat;
    /// br rat array
    long* p_brat;
    /// ar rat array
    long* p_arat;
    /// cr rat array
    long* p_crat;

  public:
    /// create a default rat
    Rat (void);

    /// create a new rat by context
    /// @param mtx the architectural context
    Rat (Mtx* mtx);

    /// create a new rat by context and name
    /// @param mtx the architectural context
    /// @param name the rat resource name
    Rat (Mtx* mtx, const string& name);

    /// destroy this rat
    ~Rat (void);

    /// reset this rat
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return the number of registers by type
    long getsize (t_lreg lreg) const;

    /// get the register mapping by register type and id
    /// @param lreg the register type
    /// @param rnum the register number
    long getmap (t_lreg lreg, const long rnum) const;

    /// get the register mapping by rid
    /// @param rid the register id to use
    long getmap (const Rid& rid) const;

    /// set the register mapping by type and id and return the old one
    /// @param lreg the register type to set
    /// @param rnum the register number to set
    /// @param mnum the mapping register number
    long setmap (t_lreg lreg, const long rnum, const long mnum);

    /// set the register mapping by rid and return the old one
    /// @param rid the register id to set
    /// @param mnum the mapping register number
    long setmap (const Rid& rid, const long mnum);

  private:
    // make the copy constructor private
    Rat (const Rat&);
    // make the assignement operator private
    Rat& operator =(const Rat&);
  };
}

#endif
