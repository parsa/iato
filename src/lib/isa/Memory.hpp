// ---------------------------------------------------------------------------
// - Memory.hpp                                                              -
// - iato:isa library - abstract memory class definition                     -
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

#ifndef  IATO_MEMORY_HPP
#define  IATO_MEMORY_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {

  /// The Memory class is an abstract class that defines the basic interface
  /// for any memory subsystem. Resetting the memory clear all bytes. A
  /// memory line can be copied into a buffer. The memory interface also
  /// defined the read and write endian mode. By default, the memory
  /// sub systsem operates in little endian mode. The 'setmode' method can 
  /// alter this behavior. when set to true, the target memory is working
  /// in big endian mode. Note however that the memory operation with word,
  /// quad and octa are done in the host mode and the necessary conversion
  /// are made automatically. Memory protection is also defined here. By 
  /// default a memory is initialized in read/write mode. Note also that
  /// a flag controls whether or not memory alignement check is done.

  class Memory {
  public:
    /// everything protected
    static const t_byte PROT_NO = 0x00;
    /// read protection
    static const t_byte PROT_RD = 0x01;
    /// write protection
    static const t_byte PROT_WR = 0x02;
    /// execute protection
    static const t_byte PROT_EX = 0x04;
    /// read, write protection
    static const t_byte PROT_RW = PROT_RD | PROT_WR;
    /// read, write and execute protection
    static const t_byte PROT_FU = PROT_RD | PROT_WR | PROT_EX;

  protected:
    /// the endian mode
    bool d_emode;
    /// the swap mode
    bool d_smode;
    /// the align mode
    bool d_align;
    /// the protection mode
    t_byte d_prot;

  public:
    /// create a default memory core in little endian mode
    Memory (void);

    /// destoy this memory
    virtual ~Memory (void);

    /// reset this memory
    virtual void reset (void) =0;

    /// set the memory endian mode
    /// @param mode the memory endian mode (true = msb)
    void setmode (const bool mode);
    
    /// set the memory alignement mode
    /// @param mode the alignment mode (true = check)
    void setalign (const bool mode);

    /// set the memory protection mode
    /// @param mode the mode to set
    void setprot (const t_byte mode);

    /// @return the memory protection mode
    t_byte getprot (void) const;

    /// @return true if the address is valid
    virtual bool isvalid (const t_octa addr) const =0;

    /// read a byte from this memory
    /// @param addr the address to read
    virtual t_byte readbyte (const t_octa addr) const =0;

    /// @return a byte and check for exec mode
    virtual t_byte readexec (const t_octa addr) const;

    /// read a word from this memory
    /// @param addr the address to read
    virtual t_word readword (const t_octa addr) const;

    /// read a quad from this memory
    /// @param addr the address to read
    virtual t_quad readquad (const t_octa addr) const;

    /// read an octa from this memory
    /// @param addr the address to read
    virtual t_octa readocta (const t_octa addr) const;

    /// read an extended from this memory
    /// @param addr the address to read
    virtual t_real readsing (const t_octa addr) const;

    /// read an extended from this memory
    /// @param addr the address to read
    virtual t_real readdoub (const t_octa addr) const;

    /// read an extended from this memory
    /// @param addr the address to read
    virtual t_real readxten (const t_octa addr) const;

    /// read a fill value from this memory
    /// @param addr the address to read
    virtual t_real readfill (const t_octa addr) const;

    /// read a integer value from this memory
    /// @param addr the address to read
    virtual t_real readint (const t_octa addr) const;

    /// write a byte at a certain address
    /// @param addr the address to write the byte
    /// @param byte the byte to write
    virtual void writebyte (const t_octa addr, const t_byte byte) =0;

    /// write a word starting at a certain address
    /// @param addr the address to write the byte
    /// @param src  the 2 bytes to write
    virtual void writeword (const t_octa addr, const t_word src);

    /// write a quad starting at a certain address
    /// @param addr the address to write the byte
    /// @param src  the 4 bytes to write
    virtual void writequad (const t_octa addr, const t_quad src);

    /// write an octa starting at a certain address
    /// @param addr the address to write the byte
    /// @param src  the 8 bytes to write
    virtual void writeocta (const t_octa addr, const t_octa src);

    /// write a single starting at a certain address
    /// @param addr the address to write the byte
    /// @param src the real to write
    virtual void writesing (const t_octa addr, const t_real& src);

    /// write a double starting at a certain address
    /// @param addr the address to write the byte
    /// @param src the real to write
    virtual void writedoub (const t_octa addr, const t_real& src);

    /// write an extended starting at a certain address
    /// @param addr the address to write the byte
    /// @param src the real to write
    virtual void writexten (const t_octa addr, const t_real& src);
    
    /// write a spill val starting at a certain address
    /// @param addr the address to write the byte
    /// @param src the real to write
    virtual void writespill (const t_octa addr, const t_real& src);

    /// write an integer val starting at a certain address
    /// @param addr the address to write the byte
    /// @param src the real to write
    virtual void writeint (const t_octa addr, const t_real& src);

    /// read a memory line to a buffer
    /// @param addr the start address
    /// @param blen the buffer length
    /// @param buf the buffer to copy into
    virtual long rdbuf (const t_octa addr, const long blen, 
			t_byte* buf) const;

    /// read a memory line to a buffer in execute mode
    /// @param addr the start address
    /// @param blen the buffer length
    /// @param buf the buffer to copy into
    virtual long exbuf (const t_octa addr, const long blen, 
			t_byte* buf) const;

    /// write a memory line to a buffer
    /// @param addr the start address
    /// @param blen the buffer length
    /// @param buf the buffer to copy into
    virtual long wrbuf (const t_octa addr, const long blen,
			const t_byte* buf);
  };
}

#endif
