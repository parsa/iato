// ---------------------------------------------------------------------------
// - ElfSection.cpp                                                          -
// - iato:elf library - elf section class implementation                     -
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
#include "ElfSection.hpp"

namespace iato {

  // -------------------------------------------------------------------------
  // - elf section based methods                                             -
  // -------------------------------------------------------------------------
 
  // this procedure return the section protection mask
  static t_byte get_scn_prot (Elf64_Shdr* shdr) {
    t_byte result = Memory::PROT_RD;
    if (shdr->sh_type == SHT_PROGBITS) {
      if ((shdr->sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR) {
	result |= Memory::PROT_WR | Memory::PROT_EX;
      }
    }
    return result;
  }

  // create a new elf section by name and elf section descriptor
  
  ElfSection::ElfSection (const string& name, void* scn, const bool mode){
    Elf_Scn*    sptr = reinterpret_cast <Elf_Scn*> (scn);
    Elf64_Shdr* shdr = sptr ? elf64_getshdr (sptr) : 0;
    Elf_Data*   data = (sptr == NULL) ? NULL : elf_getdata (sptr, NULL);
    // set ther memory attributes
    setmode (mode);
    setbase (shdr->sh_addr);
    setprot (get_scn_prot (shdr));
    // initialize the memory segment
    if (data != NULL) {
      long    blen = data->d_size;
      t_byte* buf  = reinterpret_cast <t_byte*> (data->d_buf);
      setdata (blen, buf);
    }
    // save section info
    d_name = name;
  }

  // return the section name

  string ElfSection::getname (void) const {
    return d_name;
  }

  // -------------------------------------------------------------------------
  // - elf section iterator based methods                                    -
  // -------------------------------------------------------------------------

  // create a default const iterator

  ElfSection::const_iterator::const_iterator (void) {
    p_sect  = 0;
    d_addr  = 0x0000000000000000LL;
  }

  // create a const iterator with section and address

  ElfSection::const_iterator::const_iterator (const ElfSection* scn,
					      const t_octa addr) {
    p_sect  = scn;
    d_addr  = addr;
  }

  // copy construct a const iterator

  ElfSection::const_iterator::const_iterator (const const_iterator& that) {
    p_sect = that.p_sect;
    d_addr = that.d_addr;
  }

  // assign a const iterator to this one

  ElfSection::const_iterator& ElfSection::const_iterator::operator = 
                              (const const_iterator& it) {
    p_sect = it.p_sect;
    d_addr = it.d_addr;
    return *this;
  }

  // move the iterator one step (prefix)

  ElfSection::const_iterator& ElfSection::const_iterator::operator ++ (void) {
    if (p_sect) d_addr += BN_BYSZ;
    return *this;
  }

  // move the iterator one step (postfix)

  ElfSection::const_iterator ElfSection::const_iterator::operator ++ (int) {
    const_iterator self = *this;
    if (p_sect) d_addr += BN_BYSZ;
    return self;
  }

  // return true if the iterator is equal to the other

  bool ElfSection::const_iterator::operator == 
                   (const ElfSection::const_iterator& it) const {
    return (d_addr == it.d_addr);
  }

  // return true if the iterator is less than the other

  bool ElfSection::const_iterator::operator < 
                   (const ElfSection::const_iterator& it) const {
    return (d_addr < it.d_addr);
  }

  // return true if the iterator is less equal than the other

  bool ElfSection::const_iterator::operator <= 
                   (const ElfSection::const_iterator& it) const {
    return (d_addr <= it.d_addr);
  }

  // return true if the iterator is greater than the other

  bool ElfSection::const_iterator::operator > 
                   (const ElfSection::const_iterator& it) const {
    return (d_addr > it.d_addr);
  }

  // return true if the iterator is greater equal than the other

  bool ElfSection::const_iterator::operator >= 
                   (const ElfSection::const_iterator& it) const {
    return (d_addr >= it.d_addr);
  }

  // get a bundle from this iterator

  Bundle ElfSection::const_iterator::operator * (void) const {
    Bundle bnd;
    if (p_sect) {
      t_octa ip = d_addr;
      bnd.setbip (ip);
      for (long j = 0; j < BN_BYSZ; j++) bnd.push (p_sect->readbyte (ip++));
    }
    return bnd;
  }

  // return the current iterator ip

  t_octa ElfSection::const_iterator::getip (void) const {
    return d_addr;
  }

  // return an iterator pointing to the beginning of the section

  ElfSection::const_iterator ElfSection::begin (void) const {
    return const_iterator (this, this->getbase ());
  }

  // return an iterator pointing to the end of the section

  ElfSection::const_iterator ElfSection::end (void) const {
    return const_iterator (this, this->getbase () + this->getsize ());
  }
}
