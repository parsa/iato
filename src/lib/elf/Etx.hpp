// ---------------------------------------------------------------------------
// - Etx.hpp                                                                 -
// - iato:elf library - elf context class definition                         -
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

#ifndef  IATO_ETX_HPP
#define  IATO_ETX_HPP

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

namespace iato {

  /// The Etx class is the micro-architecture context class. This class
  /// is derived from the isa context class and provides additional
  /// parameters that are used the elf engine. Most of these parameters
  /// are used to derive a correct process image.
  
  class Etx : public Ctx {
  public:
    /// create a new context
    Etx (void);

    /// reset this context
    void reset (void);

    /// update this context with a particular architecture
    void update (const t_arch arch);

  private:
    // make the copy constructor private
    Etx (const Etx&);
    // make the assignment operator private
    Etx& operator = (const Etx&);
  };
}

#endif

