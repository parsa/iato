// ---------------------------------------------------------------------------
// - Irb.hpp                                                                 -
// - iato:mac library - instruction result buffer class definition           -
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

#ifndef  IATO_IRB_HPP
#define  IATO_IRB_HPP

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
  using namespace std;

  /// The Irb is the instruction result buffer. The class acts as a buffer
  /// with an index setup in the rob. Each entry is made of the instruction
  /// and the instruction result. The size of the instruction buffer is
  /// as big as the rob.

  class Irb : public Resource {
  private:
    /// the irb size
    long d_size;
    /// the instruction buffer
    Dsi*    p_inst;
    /// the result buffer
    Result* p_resl;

  public:
    /// create a new irb
    Irb (void);

    /// create a new irb with a context
    /// @param mtx the architectural context
    Irb (Mtx* mtx);

    /// destroy this irb
    ~Irb (void);

    /// reset this irb
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the irb is empty
    bool isempty (void) const;

    /// @return true if the irb entry is valid
    bool isvalid (const long index) const;

    /// clear an irb entry by idex
    void clear (const long index);

    /// allocate a new irb entry
    /// @param inst the instruction to buffer
    /// @param resl the instruction result
    long alloc (const Dsi& inst, const Result& resl);

    /// @return an instruction entry by index
    Dsi getinst (const long index) const;

    /// @return an instruction result by index
    Result getresl (const long index) const;

  private:
    // make the copy constructor private
    Irb (const Irb&);
    // make the assignment operator private
    Irb& operator = (const Irb&);
  };
}

#endif
