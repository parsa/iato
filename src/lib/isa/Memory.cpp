// ---------------------------------------------------------------------------
// - Memory.cpp                                                              -
// - iato:isa library - abstract memory class implementation                 -
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

#include "Bits.hpp"
#include "Memory.hpp"
#include "Interrupt.hpp"

namespace iato {
  
  // the word alignement mask
  const t_octa WORD_ALIGN_MASK = 0x0000000000000001LL;
  const t_octa QUAD_ALIGN_MASK = 0x0000000000000003LL;
  const t_octa OCTA_ALIGN_MASK = 0x0000000000000007LL;
  const t_octa HUGE_ALIGN_MASK = 0x000000000000000FLL;

  // this procedure check that a memory address is aligned according
  // to a mask defined as a constant above
  static void check_align (const t_octa addr, const t_octa mask) {
    using namespace std;

    if ((addr & mask) != OCTA_0) {
      ostringstream os;
      os << "unaligned data reference fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_ALIGN, os.str ());
    }
  }
  
  // this procedure compute the swap flag according to the memory endian mode
  // and the host endian mode. If the memory and host differ, some swaping
  // might be needed to get the host representation for word, quad and octa.
  static bool get_swap_mode (const bool emode) {
    if ((emode == false) && (MA_MODE == true))  return true;
    if ((emode == true)  && (MA_MODE == false)) return true;
    return false;
  }
  
  // create a defaul memory core in little endian mode

  Memory::Memory (void) {
    d_emode = false;
    d_smode = get_swap_mode (d_emode);
    d_align = true;
    d_prot  = PROT_RW;
  }

  // destroy this memory

  Memory::~Memory (void) {
  }

  // set the memory endian mode

  void Memory::setmode (const bool mode) {
    d_emode = mode;
    d_smode = get_swap_mode (d_emode);
  }

  // set the memory alignment mode

  void Memory::setalign (const bool mode) {
    d_align = mode;
  }

  // set the memory protection mode

  void Memory::setprot (const t_byte mode) {
    d_prot = mode;
  }

  // get the memory protection

  t_byte Memory::getprot (void) const {
    return d_prot;
  }

  // read a byte and check for exception

  t_byte Memory::readexec (const t_octa addr) const {
    if ((d_prot & PROT_EX) != PROT_EX) {
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
    return readbyte (addr);
  }

  // read a word in this memory according to the mode

  t_word Memory::readword (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, WORD_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[2];
      t_word wval;
    } data;
    t_octa ma = addr;
    if (d_smode == true) {
      data.bval[1] = readbyte (ma);
      data.bval[0] = readbyte (++ma);
    } else {
      data.bval[0] = readbyte (ma);
      data.bval[1] = readbyte (++ma);
    }
    return data.wval;
  }

  // read a quad in this memory according to the mode

  t_quad Memory::readquad (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, QUAD_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[4];
      t_quad qval;
    } data;
    t_octa ma = addr;
    if (d_smode == true) {
      data.bval[3] = readbyte (ma);
      data.bval[2] = readbyte (++ma);
      data.bval[1] = readbyte (++ma);
      data.bval[0] = readbyte (++ma);
    } else {
      data.bval[0] = readbyte (ma);
      data.bval[1] = readbyte (++ma);
      data.bval[2] = readbyte (++ma);
      data.bval[3] = readbyte (++ma);
    }
    return data.qval;
  }

  // read an octa in this memory according to the mode

  t_octa Memory::readocta (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[8];
      t_octa oval;
    } data;
    t_octa ma = addr;
    if (d_smode == true) {
      data.bval[7] = readbyte (ma);
      data.bval[6] = readbyte (++ma);
      data.bval[5] = readbyte (++ma);
      data.bval[4] = readbyte (++ma);
      data.bval[3] = readbyte (++ma);
      data.bval[2] = readbyte (++ma);
      data.bval[1] = readbyte (++ma);
      data.bval[0] = readbyte (++ma);
    } else {
      data.bval[0] = readbyte (ma);
      data.bval[1] = readbyte (++ma);
      data.bval[2] = readbyte (++ma);
      data.bval[3] = readbyte (++ma);
      data.bval[4] = readbyte (++ma);
      data.bval[5] = readbyte (++ma);
      data.bval[6] = readbyte (++ma);
      data.bval[7] = readbyte (++ma);
    }
    return data.oval;
  }

  // read a single in this memory according to the e_mode mot s_mode

  t_real Memory::readsing (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, QUAD_ALIGN_MASK);
    t_byte bval[t_real::TR_SISZ];
    t_octa ma = addr;
    if (d_emode == true){
      for (long i = t_real::TR_SISZ-1; i>=0; i--)
	bval[i] = readbyte (ma++);
    } else {
      for (long i = 0 ; i < t_real::TR_SISZ; i++)
	bval[i] = readbyte (ma++);
    }
    t_real rval;
    rval.singleld (bval);
    return rval;
  }

 // read a double in this memory according to the e_mode not s_mode

  t_real Memory::readdoub (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // adjust to endian mode
    t_byte bval[t_real::TR_DOSZ];
    t_octa ma = addr;
    if (d_emode == true)
      for (long i = t_real::TR_DOSZ-1; i>=0; i--)
	bval[i] = readbyte (ma++);
    else
      for (long i = 0 ; i < t_real::TR_DOSZ; i++)
	bval[i] = readbyte (ma++);
    t_real rval;
    rval.doubleld (bval);
    return rval;
  }

  // read an extended in this memory according to the e_mode

  t_real Memory::readxten (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, HUGE_ALIGN_MASK);
    // adjust to e_mode not s_mode
    t_byte bval[t_real::TR_DESZ];
    t_octa ma = addr;
    if (d_emode == true)
      for (long i = t_real::TR_DESZ-1; i>=0; i--)
	bval[i] = readbyte (ma++);
    else
      for (long i = 0 ; i < t_real::TR_DESZ; i++)
	bval[i] = readbyte (ma++);
    t_real rval;
    rval.extendedld (bval);
    return rval;
  }
  
  // read a fill value in this memory according to the mode
  
  t_real Memory::readfill (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, HUGE_ALIGN_MASK);
    // adjust to e_mode not s_mode
    t_byte bval[t_real::TR_SFSZ];
    t_octa ma = addr;
    if (d_emode == true)
      for (long i = t_real::TR_SFSZ-1; i>=0; i--)
	bval[i] = readbyte (ma++);
    else
      for (long i = 0 ; i < t_real::TR_SFSZ; i++)
	bval[i] = readbyte (ma++);
    t_real rval;
    rval.fill (bval);
    return rval;
  }

 // read a integer in this memory according to the e_mode not s_mode

  t_real Memory::readint (const t_octa addr) const {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // adjust to endian mode
    t_byte bval[t_real::TR_DOSZ];
    t_octa ma = addr;
    if (d_emode == true)
      for (long i = t_real::TR_DOSZ-1; i>=0; i--)
	bval[i] = readbyte (ma++);
    else
      for (long i = 0 ; i < t_real::TR_DOSZ; i++)
	bval[i] = readbyte (ma++);
    t_real rval;
    rval.integerld (bval);
    return rval;
  }

  // write a word to this memory according to the mode

  void Memory::writeword (const t_octa addr, const t_word src) {
    // check for align
    if (d_align == true) check_align (addr, WORD_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[2];
      t_word wval;
    };
    t_octa ma = addr;
    wval      = src;
    if (d_smode == true) {
      writebyte (ma,   bval[1]);
      writebyte (++ma, bval[0]);
    } else {
      writebyte (ma,   bval[0]);
      writebyte (++ma, bval[1]);
    }
  }

  // write a quad to this memory according to the mode

  void Memory::writequad (const t_octa addr, const t_quad src) {
    // check for align
    if (d_align == true) check_align (addr, QUAD_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[4];
      t_quad qval;
    };
    t_octa ma = addr;
    qval      = src;
    if (d_smode == true) {
      writebyte (ma,   bval[3]);
      writebyte (++ma, bval[2]);
      writebyte (++ma, bval[1]);
      writebyte (++ma, bval[0]);
    } else {
      writebyte (ma,   bval[0]);
      writebyte (++ma, bval[1]);
      writebyte (++ma, bval[2]);
      writebyte (++ma, bval[3]);
    }
  }

  // write an octa to this memory according to the mode

  void Memory::writeocta (const t_octa addr, const t_octa src) {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // adjust to endian mode
    union {
      t_byte bval[8];
      t_octa oval;
    };
    t_octa ma = addr;
    oval      = src;
    if (d_smode == true) {
      writebyte (ma,   bval[7]);
      writebyte (++ma, bval[6]);
      writebyte (++ma, bval[5]);
      writebyte (++ma, bval[4]);
      writebyte (++ma, bval[3]);
      writebyte (++ma, bval[2]);
      writebyte (++ma, bval[1]);
      writebyte (++ma, bval[0]);
    } else {
      writebyte (ma,   bval[0]);
      writebyte (++ma, bval[1]);
      writebyte (++ma, bval[2]);
      writebyte (++ma, bval[3]);
      writebyte (++ma, bval[4]);
      writebyte (++ma, bval[5]);
      writebyte (++ma, bval[6]);
      writebyte (++ma, bval[7]);
    }
  }

  // write an extend to this memory according to the e_mode

  void Memory::writesing (const t_octa addr, const t_real& src) {
    // check for align
    if (d_align == true) check_align (addr, QUAD_ALIGN_MASK);
    // fill in the memory buffer (little endian coding)
    t_byte buf[t_real::TR_SISZ];
    src.singlest (buf);
    // adjust to memory coding (based on emode, not swap mode)
    t_octa ma = addr;
    if (d_emode == true) {
      for (long i = t_real::TR_SISZ-1; i >= 0; i--) writebyte (ma++, buf[i]);
    } else {
      for (long i = 0; i < t_real::TR_SISZ; i++) writebyte (ma++, buf[i]);
    }
  }

  // write an double to this memory according to the e_mode

  void Memory::writedoub (const t_octa addr, const t_real& src) {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // fill in the memory buffer (little endian coding)
    t_byte buf[t_real::TR_DOSZ];
    src.doublest (buf);
    // adjust to memory coding (based on emode, not swap mode)
    t_octa ma = addr;
    if (d_emode == true) {
      for (long i = t_real::TR_DOSZ-1; i >= 0; i--) writebyte (ma++, buf[i]);
    } else {
      for (long i = 0; i < t_real::TR_DOSZ; i++) writebyte (ma++, buf[i]);
    }
  }

  // write an extend to this memory according to the e_mode

  void Memory::writexten (const t_octa addr, const t_real& src) {
    // check for align
    if (d_align == true) check_align (addr, HUGE_ALIGN_MASK);
    // fill in the memory buffer (little endian coding)
    t_byte buf[t_real::TR_DESZ];
    src.extendedst (buf);
    // adjust to memory coding (based on emode, not swap mode)
    t_octa ma = addr;
    if (d_emode == true) {
      for (long i = t_real::TR_DESZ-1; i >= 0; i--) writebyte (ma++, buf[i]);
    } else {
      for (long i = 0; i < t_real::TR_DESZ; i++) writebyte (ma++, buf[i]);
    }
  }

  // write a spill value to this memory according to the e_mode

  void Memory::writespill (const t_octa addr, const t_real& src) {
    // check for align
    if (d_align == true) check_align (addr, HUGE_ALIGN_MASK);
    // fill in the memory buffer (little endian coding)
    t_byte buf[t_real::TR_SFSZ];
    src.spill (buf);
    // adjust to memory coding (based on emode, not swap mode)
    t_octa ma = addr;
    if (d_emode == true) {
      for (long i = t_real::TR_SFSZ-1; i >= 0; i--) writebyte (ma++, buf[i]);
    } else {
      for (long i = 0; i < t_real::TR_SFSZ; i++) writebyte (ma++, buf[i]);
    }
  }

  // write an integer to this memory according to the e_mode

  void Memory::writeint (const t_octa addr, const t_real& src) {
    // check for align
    if (d_align == true) check_align (addr, OCTA_ALIGN_MASK);
    // fill in the memory buffer (little endian coding)
    t_byte buf[t_real::TR_DOSZ];
    src.integerst (buf);
    // adjust to memory coding (based on emode, not swap mode)
    t_octa ma = addr;
    if (d_emode == true) {
      for (long i = t_real::TR_DOSZ-1; i >= 0; i--) writebyte (ma++, buf[i]);
    } else {
      for (long i = 0; i < t_real::TR_DOSZ; i++) writebyte (ma++, buf[i]);
    }
  }

  // read a memory line into a buffer

  long Memory::rdbuf (const t_octa addr, const long blen, t_byte* buf) const {
    long result = 0;
    for (long i = 0; i < blen; i++) {
      t_octa ma = addr + (t_octa) i;
      buf[i] = readbyte (ma);
      result++;
    }
    return result;
  }

  // read a memory line into a buffer in execute mode

  long Memory::exbuf (const t_octa addr, const long blen, t_byte* buf) const {
    long result = 0;
    for (long i = 0; i < blen; i++) {
      t_octa ma = addr + (t_octa) i;
      buf[i] = readexec (ma);
      result++;
    }
    return result;
  }

  // write a memory line into a buffer

  long Memory::wrbuf (const t_octa addr, const long blen, const t_byte* buf) {
    long result = 0;
    for (long i = 0; i < blen; i++) {
      t_octa ma = addr + (t_octa) i;
      writebyte (ma, buf[i]);
      result++;
    }
    return result;
  }
}
