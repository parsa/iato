// ---------------------------------------------------------------------------
// - Alat.hpp                                                                -
// - iato:isa library - alat class definition                                -
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

#ifndef  IATO_ALAT_HPP
#define  IATO_ALAT_HPP

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

namespace iato {

  /// The Alat class is an abstract class definition of the Alat. It
  /// defines the basic interface for any Alat, a default implementation
  /// for each interface is given.

  class Alat : public Resource {
  private:
    /// the alat number of entries
    long d_size;
    /// the alat structure
    struct t_alat* p_alat;

  public:
    /// create a default alat
    Alat (void);

    /// create an alat with a context
    /// @param ctx the current context
    Alat (Ctx* ctx);

    /// create an alat with a context and name
    /// @param ctx the current context
    /// @param name the resource name
    Alat (Ctx* ctx, const string& name);

    /// destroy this alat
    ~Alat (void);

    /// reset this alat
    void reset (void);

    /// report this alat
    void report (void) const;

    /// @return true if the alat is full
    bool isfull (void) const;

    /// @return true if the line is valid
    bool isvalid (const t_word ind) const;

    /// get a tag according to register type and number
    /// @param reg the regiter rid
    /// @return the tag value
    virtual t_word gettag (const Rid reg) const;

    /// add a new entry in the alat
    void add (const t_octa addr, const Rid reg, const t_byte size);

    /// notify to alat that a memory address have been changed
    /// @param addr the memory address write accessed
    /// @param size the size of the access
    void memupd (const t_octa addr, const t_byte size);

    /// remove an entry from the alat
    /// @param reg the Rid to remove
    void remove (const Rid reg);

    /// @param reg the Rid to look for
    /// @return true if an entry matches
    bool check (const Rid reg) const;

    /// determine if the load have to be done and update alat state
    /// @param resl the result which contains the load
    /// @param indx the load index in the result
    /// @return true if the load have to be done 
    bool  load (Result& resl, const long indx);

    /// check and update result
    /// @param resl the result which contains the check
    /// @param indx the check index in the result
    void check (Result& resl, const long indx);

  private:
    // make the copy constructor private
    Alat (const Alat&);
    // make the assignment operator private
    Alat& operator = (const Alat&);
  };
}

#endif
