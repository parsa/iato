// ---------------------------------------------------------------------------
// - ElfChecker.hpp                                                          -
// - iato:elf library - elf checker class definition                         -
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

#ifndef  IATO_ELFCHECKER_HPP
#define  IATO_ELFCHECKER_HPP

#ifndef  IATO_CHECKER_HPP
#include "Checker.hpp"
#endif

#ifndef  IATO_ELFSECTION_HPP
#include "ElfSection.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfChecker class is a special section that is designed to handle
  /// the special checker section. Once the section has been read, it is
  /// possible to extract a checker object. The checker object is built
  /// by scaning the section and building register check records. Once the
  /// checker object has been built, that section can be destroyed.
  
  class ElfChecker : public ElfSection {
  private:
    /// create a new elf checker by name and elf section
    /// @param name the section name
    /// @param scn the elf section descriptor
    /// @param mode the endian mode
    ElfChecker (const string& name, void* scn, const bool mode);

  public:
    /// @return the checker object
    Checker* getchecker (void) const;

  private:
    // make the copy constructor private
    ElfChecker (const ElfChecker&);
    // make the assignment operator private
    ElfChecker& operator = (const ElfChecker&);
    // make the elf image a friend
    friend class ElfImage;
  };
}

#endif
