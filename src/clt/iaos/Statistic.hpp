// ---------------------------------------------------------------------------
// - Statistic.hpp                                                           -
// - iato:iaos client - static statistic class definition                    -
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

#ifndef  IATO_STATISTIC_HPP
#define  IATO_STATISTIC_HPP

#ifndef  IATO_STAT_HPP
#include "Stat.hpp"
#endif

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {
  
  /// This Statistic class is a simple class to handle static analyse of
  /// elf text objects

  class Statistic : public Stat {
  private:
    /// the elf text to examine
    ElfImage* p_elf;

  public:
    /// create a new statistic object by name and context
    /// @param name the trace file name
    Statistic (ElfImage* elf);

    /// compute statistic acording to flags
    void compute (void);

  private:
    // make the copy constructor private
    Statistic (const Statistic&);
    // make the assignment operator private
    Statistic& operator = (const Statistic&);
  };
}

#endif
