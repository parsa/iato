// ---------------------------------------------------------------------------
// - ElfSection.hpp                                                          -
// - iato:elf library - elf section class definition                         -
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

#ifndef  IATO_ELFSECTION_HPP
#define  IATO_ELFSECTION_HPP

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_SEGMENT_HPP
#include "Segment.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfSection class is a simple class that holds the data associated
  /// with a particular elf section. The class cannot be constructed directly.
  /// The elf section is described with a name, a size and a set of data.
  
  class ElfSection : public Segment {
  private:
    /// the section name
    string d_name;

  public:
    /// create a new elf section by name and elf section
    /// @param name the section name
    /// @param scn the elf section descriptor
    /// @param mode the endian mode
    ElfSection (const string& name, void* scn, const bool mode);

    /// @return the section name
    string getname (void) const;

  private:
    // make the copy constructor private
    ElfSection (const ElfSection&);
    // make the assignment operator private
    ElfSection& operator = (const ElfSection&);

  public:

    /// the const_iterator class is a constant iterator for the section
    /// class. The iterator returns a bundle object per iteration. If the
    /// iterator is not valid an invalid bundle is returned.

    class const_iterator {
    private:
      /// the section in use
      const ElfSection* p_sect;
      /// the current address
      t_octa d_addr;

    public:
      /// create a default iterator
      const_iterator (void);

      /// create an iterator with a section and address
      /// @param scn the section to iterate
      /// @param addr the address to start
      const_iterator (const ElfSection* scn, const t_octa addr);

      /// copy construct this iterator
      /// @param that the iterator to copy
      const_iterator (const const_iterator& that);

      /// assign an iterator to this one
      /// @param it the iterator to assign
      const_iterator& operator = (const const_iterator& it);
 
      /// move one step the iterator (prefix)
      const_iterator& operator ++ (void);

      /// move one step the iterator (postfix)
      const_iterator operator ++ (int);

      /// @return true if two iterators are equal
      bool operator == (const const_iterator& it) const;

      /// @return true if two iterators are less
      bool operator < (const const_iterator& it) const;

      /// @return true if two iterators are less equal
      bool operator <= (const const_iterator& it) const;

      /// @return true if two iterators are greater
      bool operator > (const const_iterator& it) const;
      
      /// @return true if two iterators are greater equal
      bool operator >= (const const_iterator& it) const;

      /// get a bundle from this iterator
      Bundle operator * (void) const;

      /// @return the current iterator ip
      t_octa getip (void) const;
   };

  public:
    /// @return a bundle iterator pointing at the beginning of the section
    const_iterator begin (void) const;

    /// @return a bundle iterator pointing at the end of the section
    const_iterator end (void) const;
  };
}

#endif
