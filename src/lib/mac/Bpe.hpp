// ---------------------------------------------------------------------------
// - Bpe.hpp                                                                 -
// - iato:mac library - Bypass network element class definition              -
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

#ifndef  IATO_BPE_HPP
#define  IATO_BPE_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Bpe class is the bypass network element. It is used to model the 
  /// operations of the bypass network across multiple functional units.
  /// One bypass element is used by a stage to forward the instruction
  /// results. All bypass element are merged into a global resource in
  /// order to form the bypass network. The bypass element size is determined
  /// by the maximum number of results a unit can produce.

  class Bpe : public Resource {
  private:
    /// the element size
    long d_size;
    /// the bypass group
    struct t_bpg* p_vbpg;

  public:
    /// create a default bypass element
    Bpe (void);

    /// create a bypass network by context
    /// @param mtx the architectural context
    Bpe (Mtx* mtx);

    /// create a bypass network by context and name
    /// @param mtx the architectural context
    /// @param name the rat resource name
    Bpe (Mtx* mtx, const string& name);

    /// destroy this bypass element
    ~Bpe (void);

    /// reset this bypass element
    void reset (void);

    /// report this resource
    void report (void) const;

    /// set a bypass value by index, rid and value
    /// @param index the element index
    /// @param rid   the rid to map
    /// @param uvr   the uvr to map
    void setuvr (const long index, const Rid& rid, const Uvr& uvr);

    /// @return the index of a matching bypass element
    long find (const Rid& rid) const;
    
    /// @return the rid associated with a bypass element
    Rid getrid (const long index) const;
    
    /// @return the uvr associated with a bypass element
    Uvr getuvr (const long index) const;
    
    /// update the bpe with a result
    /// @param resl the result used to update the bpe
    void update (const Result& resl);

    /// clear a bypass element by rid
    /// @param rid the bpe rid to clear
    void clear (const Rid& rid);

  private:
    // make the copy constructor private
    Bpe (const Bpe&);
    // make the assignement operator private
    Bpe& operator =(const Bpe&);
  };
}

#endif
