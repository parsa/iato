// ---------------------------------------------------------------------------
// - Segment.hpp                                                             -
// - iato:isa library - memory segment class definition                      -
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

#ifndef  IATO_SEGMENT_HPP
#define  IATO_SEGMENT_HPP

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

namespace iato {

  /// The Segment class is an implementation of the memory core interface.
  /// The segment is defined as an array of bytes that can be accessed with a 
  /// particular address. A base is used to offset the memory address.

  class Segment : public Memory {
  protected:
    /// the segment size
    t_long  d_size;
    /// the segment array
    t_byte* p_data;
    /// the base address
    t_octa  d_base;

  public:
    /// create an empty segment
    Segment (void);

    /// create a new segment instance by size
    /// @param size the segment size
    Segment (const t_long size);

    /// destroy this segment
    ~Segment (void);

    /// reset this segment
    void reset (void);

    /// @return the segment size
    virtual t_long getsize (void) const;

    /// set the segment base address
    /// @param addr the base segment address
    virtual void setbase (const t_octa addr);

    /// @return the base address
    virtual t_octa getbase (void) const;

    /// initialize the segment data by copy
    /// @param blen the buffer length
    /// @param buf  the buffer to set
    virtual void setdata (const long blen, t_byte* buf);

    /// initialize the segment data by install
    /// @param blen the buffer length
    /// @param buf  the buffer to set
    virtual void mapdata (const long blen, t_byte* buf);
   
    /// @return true if the address is valid
    bool isvalid (const t_octa addr) const;

    /// read a byte from this segment
    /// @param addr the address to read
    t_byte readbyte (const t_octa addr) const;

    /// write a byte at a certain address
    /// @param addr the address to write the byte
    /// @param byte the byte to write
    void writebyte (const t_octa addr, const t_byte byte);

  private:
    // make the copy constructor private
    Segment (const Segment&);
    // make the assignment operator private
    Segment& operator = (const Segment&);
  };
}

#endif
