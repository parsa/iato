// ---------------------------------------------------------------------------
// - ElfMemory.cpp                                                           -
// - iato:elf library - elf base memory class implementation                 -
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

#include "ElfMemory.hpp"
#include "Interrupt.hpp"

namespace iato {

  // create a default elf memory

  ElfMemory::ElfMemory (void) {
    setprot (PROT_NO);
  }

  // destroy this elf memory

  ElfMemory::~ElfMemory (void) {
    typedef vector<Memory*>::iterator t_it;
    for (t_it it = d_vmem.begin (); it != d_vmem.end (); it++) {
      delete *it;;
    }
  }

  // reset this elf elf memory

  void ElfMemory::reset (void) {
    long size = d_vmem.size ();
    for (long i = 0; i < size; i++) d_vmem[i]->reset ();
  }

  // return true if the address is valid

  bool ElfMemory::isvalid (const t_octa addr) const {
    long size = d_vmem.size ();
    for (long i = 0; i < size; i++) {
      if (d_vmem[i]->isvalid (addr) == true) return true;
    }
    return false;
  }

  // read a byte at a certain address

  t_byte ElfMemory::readbyte (const t_octa addr) const {
    long index = find (addr);
    if (index == -1) {
      ostringstream os;
      os << "data access rights fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_ACRGT, os.str ());
    }
    return d_vmem[index]->readbyte (addr);
  }

  // read a byte at a certain address in execute mode

  t_byte ElfMemory::readexec (const t_octa addr) const {
    long index = find (addr);
    if (index == -1) {
      ostringstream os;
      os << "instruction access rights fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_INST_ACRGT, os.str (), addr);
    }
    if (isvalid (addr) == false) {
      ostringstream os;
      os << "instruction page not present at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_INST_PAGE, os.str (), addr);
    }
    return d_vmem[index]->readexec (addr);
  }

  // write a byte at a certain address

  void ElfMemory::writebyte (const t_octa addr, const t_byte byte) {
    long index = find (addr);
    if (index == -1) {
      ostringstream os;
      os << "data access rights fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_ACRGT, os.str ());
    }
    d_vmem[index]->writebyte (addr, byte);
  }

  // return the number of memories

  long ElfMemory::length (void) const {
    return d_vmem.size ();
  }

  // find a memory index by address

  long ElfMemory::find (const t_octa addr) const {
    long size = d_vmem.size ();
    for (long i = 0; i < size; i++) {
      if (d_vmem[i]->isvalid (addr) == true) return i;
    }
    return -1;
  }

  // add a memory to this elf exec image

  void ElfMemory::add (Memory* mem) {
    if (!mem) return;
    d_vmem.push_back (mem);
  }

  // return a memory by index

  Memory* ElfMemory::getmem (const long index) const {
    assert ((index >= 0) && (index < (long) d_vmem.size ()));
    return d_vmem[index];
  }

  // remove a memory by index

  bool ElfMemory::remove (const long index) {
    long size = d_vmem.size ();
    if ((index < 0) || (index >= size)) return false;
    // special case with only one element
    if (size == 1) {
      assert (index == 0);
      delete d_vmem[0];
      d_vmem.pop_back ();
    } else {
      // delete the memory element
      Memory* mem = d_vmem[index];
      delete mem;
      // swap the memory element with the last one so we can pop it
      d_vmem[index] = d_vmem[size-1];
      d_vmem.pop_back ();
    }
    return true;
  }
}
