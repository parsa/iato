// ---------------------------------------------------------------------------
// - ElfMemory.hpp                                                           -
// - iato:elf library - elf base memory class definition                     -
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

#ifndef  IATO_ELFMEMORY_HPP
#define  IATO_ELFMEMORY_HPP

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

namespace iato {

  /// The ElfMemory is a container for other elements. The class
  /// implements the memory interface and is responsible to select
  /// the rigth memory block for any operation, like read or write. The
  /// class works also with imbricated memory block, that is an elf memory
  /// can also contain other memory block.

  class ElfMemory : public Memory {
  protected:
    /// the vector of memory
    vector<Memory*> d_vmem;

  public:
    /// create a default elf memory
    ElfMemory (void);

    /// destroy this elf memory
    ~ElfMemory (void);

    /// reset this elf memory
    void reset (void);

    /// @return true if the address is valid
    bool isvalid (const t_octa addr) const;

    /// read a byte from this memory
    /// @param addr the address to read
    t_byte readbyte (const t_octa addr) const;

    /// read a byte from this memory and check for execute
    /// @param addr the address to read
    t_byte readexec (const t_octa addr) const;

    /// write a byte at a certain address
    /// @param addr the address to write the byte
    /// @param byte the byte to write
    void writebyte (const t_octa addr, const t_byte byte);

    /// @return the number of memories
    virtual long length (void) const;

    /// @return a memory index by address
    virtual long find (const t_octa addr) const;
    
    /// add a memory to this memory array
    /// @param mem the memory to add
    virtual void add (Memory* mem);

    /// @return a memory by index
    virtual Memory* getmem (const long index) const;

    /// remove a memory by index
    virtual bool remove (const long index);

  private:
    // make the copy constructor private
    ElfMemory (const ElfMemory&);
    // make the assignment operator private
    ElfMemory& operator = (const ElfMemory&);
  };
}

#endif
