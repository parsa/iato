// ---------------------------------------------------------------------------
// - ElfMap.hpp                                                              -
// - iato:elf library - elf mappable memory class definition                 -
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

#ifndef  IATO_ELFMAP_HPP
#define  IATO_ELFMAP_HPP

#ifndef  IATO_ELFLOAD_HPP
#include "ElfLoad.hpp"
#endif

#ifndef  IATO_ELFKERNEL_HPP
#include "ElfKernel.hpp"
#endif

namespace iato {

  /// The ElfMap class is a specialized elf load object that maintain the
  /// memory mappable interface. The full 'mmap' system call is implemented
  /// via this class. The starting address for a memory mappable  is defined
  /// as a parameters. The memory can be mapped or unmapped. For a memory
  /// mapping, the meory size needs to be alligned to a page size or an
  /// exception is raised.

  class ElfMap : public ElfLoad {
  private:
    /// the page size
    t_octa d_psize;
    /// the memory mode
    bool   d_mode;
    /// the base address
    t_octa d_mbase;

  public:
    /// create a new mappable memory
    ElfMap (void);
 
    /// create a new mappable memory by parameters
    /// @param ekp the elf kernel parameters
    ElfMap (ElfKernel* ekp);

    /// @return the top available address
    t_octa findtop (void) const;

    /// map some memory anonymously with some protection
    /// @param size the memory size to map
    /// @param prot the memory protection to use
    /// @param type the memory type to map
    t_octa mmap (const t_long size, const t_byte prot, 
		 ElfSegment::t_stype type);

    /// unmap some memory at a certan address and size
    /// @param addr the base memory address
    /// @param size the memory size to unmap
    bool munmap (const t_octa addr, const t_long size);

  private:
    // make the copy constructor private
    ElfMap (const ElfMap&);
    // make the assignment operator private
    ElfMap& operator = (const ElfMap&); 
  };
}

#endif
