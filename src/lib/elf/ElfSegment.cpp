// ---------------------------------------------------------------------------
// - ElfSegment.cpp                                                          -
// - iato:elf library - elf segment class implementation                     -
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

#include "Elf.hxx"
#include "Exception.hpp"
#include "ElfSegment.hpp"

namespace iato {

  // this procedure return the segment protection mask
  static t_byte get_seg_prot (Elf64_Phdr* phdr) {
    t_byte     result = '\000';
    Elf64_Word flags  = phdr ? phdr->p_flags : '\000';
    if ((flags & PF_R) == PF_R) result |= Memory::PROT_RD;
    if ((flags & PF_W) == PF_W) result |= Memory::PROT_WR;
    if ((flags & PF_X) == PF_X) result |= Memory::PROT_EX;
    return result;
  }

  // create an elf segment by size and base address

  ElfSegment::ElfSegment (const t_long size, const t_octa base) : 
                          Segment (size) {
    // set segment base
    setbase (base);
    // make the segment private
    d_type = PRIVATE;
  }

  // create an elf segment by size, base address and type

  ElfSegment::ElfSegment (const t_long size, const t_octa base, 
			  t_stype type ) : Segment (size) {
    // set segment base
    setbase (base);
    // set the segment type
    d_type = type;
  }


  // create a new elf segment from a file descriptor
  
  ElfSegment::ElfSegment (const int sid, void* seg, const bool mode) {
    // remap the segment header
    Elf64_Phdr* phdr = reinterpret_cast <Elf64_Phdr*> (seg);
    // get the file/memory size and initialize the memory
    long fsize = phdr->p_filesz;
    long msize = phdr->p_memsz;
    // compute null memory size
    long nsize = msize - fsize;
    assert (nsize >= 0);
    // initialize the byte array and copy
    t_byte* data = new t_byte[msize];
    if (fsize > 0) {
      lseek (sid, phdr->p_offset, SEEK_SET);
      if (read (sid, data, fsize) != fsize)
	throw Exception ("elf-error", "cannot read elf segment");
    }
    // initialize null byte
    for (long i = 0; i < nsize; i++) data[fsize+i] = '\000';
    // initialize the memory by mapping
    mapdata (msize, data);
    setmode (mode);
    setbase (phdr->p_vaddr);
    setprot (get_seg_prot (phdr));
    // make the segment private
    d_type = PRIVATE;
  }
}
