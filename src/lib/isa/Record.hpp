// ---------------------------------------------------------------------------
// - Record.hpp                                                              -
// - iato:isa library - information record class definition                  -
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

#ifndef  IATO_RECORD_HPP
#define  IATO_RECORD_HPP

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Record class is the base class of the trace system. A record is
  /// a piece of information that needs to be recorded for future analysis.
  /// A record has a name, a source and a value.

  class Record {
  public:
    /// the record type
    enum t_rctp {
      IGNORE = 0x00, // ignore this record
      HALTED = 0x01, // resource has been halted
      RDTACK = 0x02, // read transaction acknowledge
      BUNDLE = 0x03, // bundle byte stream
      RINSTR = 0x04, // raw instruction
      REGUPD = 0x05, // register update
      REGCHK = 0x06, // register checking
      RALLOC = 0x07, // record allocation
      RGREAD = 0x08, // register read
      SMEMRD = 0x09, // system memory read
      SMEMWR = 0x0a, // system memory write
      ALTUPD = 0x0b, // alat operations
      TYPMAX = 0x0c  // this have to be a the end with max value
    };

    /// a pair of record type and name
    struct t_tynm{
      t_rctp d_rcdt;
      string d_rcdn;
    };

    /// convert a string to a record type
    /// @param s the type string to convert
    static t_rctp totype (const string& s);

  private:
    /// the record type
    t_rctp d_type;
    /// the record source
    string d_rsrc;
    /// global record info
    union {
      /// read transaction info
      struct t_rdta {
	t_octa d_addr;          // transaction address
	t_sint d_rlen;          // transaction length
      } d_rdta;
      /// bundle information
      struct t_bndl {
	t_byte d_data[BN_BYSZ]; // byte bundle encoding
	t_octa d_ip;            // bundle ip
      } d_bndl;
      /// raw instruction
      struct t_inst {
	t_byte d_unit;          // unit type
	t_sint d_slot;          // slot number
	bool   d_stop;          // stop bit
	t_octa d_data;          // instruction data
	t_octa d_extd;          // extension data
	t_octa d_ip;            // instruction ip
	bool   d_br;            // branch instruction (the target valid bit)
	t_octa d_tip;           // the target ip
	bool   d_can;           // cancel flag
	bool   d_rsd;           // reschedule flag
      } d_inst;
      /// register result
      struct t_rdat {
	t_byte d_lreg;          // register type
	t_sint d_lnum;          // logical number
	t_sint d_pnum;          // physical number
	bool   d_bval;          // pred value or nat bit
	union {                 // octa or real value
	  t_octa d_oval;        
	  t_byte d_rval[t_real::TR_SFSZ];
	};
      } d_rdat;
      /// register check
      struct t_rchk {
	t_byte d_lreg;
	t_sint d_pnum;
	bool   d_bval;
	union {
	  t_octa d_oval;
	  t_byte d_rval[t_real::TR_SFSZ];
	};
      } d_rchk;
      /// system memory access
      struct t_mema {
	t_octa d_addr;
	t_octa d_oval;
	t_byte d_size;
      } d_mema;
      /// record allocation
      struct t_rcda {
	t_octa d_ip;
	t_sint d_ridx;
      } d_rcda;
      /// alat info
      struct t_alat {
	t_word d_tag;
	t_octa d_addr;
	t_byte d_ldsz;
	bool   d_add;  // add if true else remove
      } d_alat;
    } d_info;

  public:
    /// create a default record
    Record (void);

    /// create a default record by source
    /// @param rsrc the record source
    Record (const string& rsrc);

    /// create a record by source and bundle
    /// @param rsrc the record source
    /// @param bndl the bundle object
    Record (const string& rsrc, const Bundle& bndl);

    /// create a record by source and instruction
    /// @param rsrc the record source
    /// @param inst the instruction object
    Record (const string& rsrc, const Instr& inst);

    /// create a record by source, instruction and exec flag
    /// @param rsrc the record source
    /// @param inst the instruction object
    /// @param flag the execute flag (as ooposed to cancel)
    Record (const string& rsrc, const Instr& inst, const bool flag);

    /// create a record by source, instruction, cancel flag and target ip
    /// @param rsrc the record source
    /// @param inst the instruction object
    /// @param flag the cancel flag
    /// @param tg   the target value
    Record (const string& rsrc, const Instr& inst, const bool flag, 
	    const t_octa tg);

    /// create a record by source, result and index
    /// @param rsrc the record source
    /// @param resl the result object
    /// @param index the result index
    Record (const string& rsrc, const Result& resl, const long index);

    /// create a record by source, operand and index
    /// @param rsrc the record source
    /// @param oprd the operand object
    /// @param index the operand index
    Record (const string& rsrc, const Operand& oprd, const long index);

    /// create a record by source, ip and allocated index
    /// @param rsrc the record source
    /// @param ip   the record ip
    /// @param ridx the record index
    Record (const string& rsrc, const t_octa ip, const long ridx);

    /// copy construct this record
    /// @param that the record to copy
    Record (const Record& that);

    /// assign a record to this one
    /// @param that the record to assign
    Record& operator = (const Record& that);

    /// set the record source name
    /// @param rsrc the record source name
    void setname (const string& rsrc);

    /// @return the record source name
    string getname (void) const;

    /// set the record type
    /// @param type the record type to set
    void settype (t_rctp type);

    /// @return the record type
    t_rctp gettype (void) const;

    /// @return true if the record is of type ignore
    bool isignore (void) const;

    /// set a record read transaction ackowldge
    /// @param addr the request address
    /// @param rlen the request length
    void setrdta (const t_octa addr, const long rlen);

    /// set the record with a bundle
    /// @param bndl the bundle to set
    void setbndl (const Bundle& bndl);

    /// set the record with an instruction
    /// @param inst the instruction to set
    void setinst (const Instr& inst);

    /// set the record with a cancel instruction
    /// @param inst the instruction to set
    void setcanc (const Instr& inst);

    /// set the record with a branch instruction
    /// @param inst the instruction to set
    /// @param flg the cancel flag
    /// @param tg the branch target value
    void setbr (const Instr& inst, const bool flg, const t_octa tg);

    /// set the record with a rescheduled instruction
    /// @param inst the instruction to set
    void setrsch (const Instr& inst);

    /// set the record with a result and index
    /// @param resl the result to set
    /// @param index the result index
    void setresl (const Result& resl, const long index);

    /// set the record with an operand and index
    /// @param oprd the operand to set
    /// @param index the operand index
    void setoprd (const Operand& oprd, const long index);

    /// set a record with checking data
    /// @param lreg the register type
    /// @param pnum the register number
    /// @param data the register value
    void setrchk (const t_byte lreg, const t_byte pnum, const t_octa data);

    /// set a record with checking data for floatting point
    /// @param lreg the register type
    /// @param pnum the register number
    /// @param data1 the high part floating register value
    /// @param data2 the low part floatting register value
    void setrchk (const t_byte lreg, const t_byte pnum, const t_octa data1,
		  const t_octa data2);

    /// set the record with an ip and index
    /// @param ip   the record ip
    /// @param ridx the record index
    void setrcda (const t_octa ip, const long ridx);

    /// set the record with a memory read access information
    /// @param addr the memory address
    /// @param oval the octa value
    /// @param size the access size
    void setrmem (const t_octa addr, const t_octa oval, const t_byte size);

    /// set the record with a memory write access information
    /// @param addr the memory address
    /// @param oval the octa value
    /// @param size the access size
    void setwmem (const t_octa addr, const t_octa oval, const t_byte size);

    /// set the record with an alat action
    /// @param tag the tag
    /// @param addr the memory address
    /// @param size the access size
    /// @param add  the add flag bit
    void setalat (const t_word tag, const t_octa addr, const t_byte size,
		  const bool add);

    /// @return a string representation of this record
    string repr (void) const;

    /// print the record information
    void print (void) const;

    /// @return the record check register type
    t_lreg chklreg (void) const;

    /// @return the record check register number
    long chkpnum (void) const;

    /// @return the record check boolean value
    bool chkbval (void) const;

    /// @return the record check octa value
    t_octa chkoval (void) const;

    /// @return the record check real value
    t_real chkrval (void) const;

    /// write the record info a file
    /// @param fd the file descriptor
    void rcdwr (int fd) const;
    
    /// read and update a record from a file
    /// @param fd the file descriptor
    void rcdrd (int fd);

    /// @return an instruction from an instruction record
    Instr getinst (void) const;

    /// @return true if the instruction is canceled
    bool iscancel (void) const;

    /// @return true if the instruction has the tip set
    bool istip (void) const;

    /// @return true if the instruction is rescheduled
    bool isresched (void) const;

    /// @return a bundle from a bundle record
    Bundle getbnd (void) const;

    /// @return the target ip
    t_octa gettip (void) const;
  };
}

#endif
