// ---------------------------------------------------------------------------
// - Mrt.hpp                                                                 -
// - iato:isa library - memory request object class definition               -
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

#ifndef  IATO_MRT_HPP
#define  IATO_MRT_HPP

#ifndef  IATO_RID_HPP
#include "Rid.hpp"
#endif

namespace iato {

  /// The Prq class is a simple class used to define memory port request.
  /// A request is defined by a type, an address and eventually a data.
  /// Such request is generally processed by a port.

  class Mrt {
  public:
    /// the memory request type
    enum t_mrtt {
      REQ_NUL, // no request
      REQ_BND, // request bundle
      REQ_LD1, // request load byte
      REQ_LD2, // request load word
      REQ_LD4, // request load quad
      REQ_LD8, // request load octa
      REQ_LDS, // request load single real
      REQ_LDD, // request load double real
      REQ_LDE, // request load extended real
      REQ_LDF, // request load fill real
      REQ_LDI, // request load integer real
      REQ_LPS, // request load pair single
      REQ_LPD, // request load pair double
      REQ_LPI, // request load pair integer
      REQ_ST1, // request store byte
      REQ_ST2, // request store word
      REQ_ST4, // request store quad
      REQ_ST8, // request store octa
      REQ_STS, // request store single real
      REQ_STD, // request store double real
      REQ_STE, // request store extended real
      REQ_STF, // request store spill real
      REQ_STI  // request store integer real
    };

  private:
    /// the request type
    t_mrtt d_type;
    /// the request address
    t_octa d_addr;
    /// the ordering flag
    bool   d_mofl;
    /// the speculative bit
    bool   d_sbit;
    /// the nat value bit
    bool   d_nval;
    /// the integer buffer
    union {
      t_byte d_bval;
      t_word d_wval;
      t_quad d_qval;
      t_octa d_oval;
    };
    /// the low real buffer
    t_real d_lval;
    /// the high real buffer
    t_real d_hval;
    /// the load low register
    Rid d_lrid;
    /// the load high register
    Rid d_hrid;

  public:
    /// create a default mrt
    Mrt (void);

    /// create a mrt by type and address
    /// @param type the mrt type
    /// @param addr the mrt address
    Mrt (const t_mrtt type, const t_octa addr);

    /// create a mrt by type, address and speculative bit
    /// @param type the mrt type
    /// @param addr the mrt address
    /// @param sbit the speculative bit
    Mrt (const t_mrtt type, const t_octa addr, const bool sbit);

    /// copy construct this mrt
    /// @param that the mrt to copy
    Mrt (const Mrt& that);

    /// assign  mrt to this one
    /// @param that the mrt to assign
    Mrt& operator = (const Mrt& that);

    /// reset this mrt
    void reset (void);

    /// @return true if the mrt is valid
    bool isvalid (void) const;

    /// return true if the speculative bit is set
    bool issbit (void) const;

    /// return true if the nat bit is set
    bool isnval (void) const;

    /// @return true if the mrt is a load
    bool isload (void) const;

    /// @return true if the mrt is a store
    bool isstore (void) const;

    /// @return the mrt type
    t_mrtt gettype (void) const;

    /// @return the mrt address
    t_octa getaddr (void) const;

    /// @return the mrt mask
    t_octa getmask (void) const;

    /// @return the mrt ordering flag
    bool getmofl (void) const;

    /// set the nat value bit
    /// @param nval the nat value to set
    void setnval (const bool nval);

    /// set the mrt byte value
    /// @param bval the value to set
    void setbval (const t_byte bval);

    /// @return the mrt byte value
    t_byte getbval (void) const;

    /// set the mrt word value
    /// @param wval the value to set
    void setwval (const t_word wval);

    /// @return the mrt word value
    t_word getwval (void) const;
    
    /// set the mrt quad value
    /// @param qval the value to set
    void setqval (const t_quad qval);
    
    /// @return the mrt quad value
    t_quad getqval (void) const;
    
    /// set the mrt octa value
    /// @param oval the value to set
    void setoval (const t_octa oval);

    /// @return the mrt octa value
    t_octa getoval (void) const;

    /// set the mrt low real value
    /// @param rval the value to set
    void setlval (const t_real& rval);

    /// @return the mrt low real value
    t_real getlval (void) const;

    /// set the mrt high real value
    /// @param rval the value to set
    void sethval (const t_real& rval);

    /// @return the mrt high real value
    t_real gethval (void) const;

    /// set the mrt low rid 
    /// @param rid the rid to set
    void setlrid (const Rid& rid);

    /// @return the load low register
    Rid getlrid (void) const;
 
    /// set the mrt high rid 
    /// @param rid the rid to set
    void sethrid (const Rid& rid);

    /// @return the load high register
    Rid gethrid (void) const;

    /// set a bundle mrt by address
    /// @param addr the request address
    void setbnd (const t_octa addr);

    /// set the load information by type, address, speculative bit and rid
    /// @param type the load type to process
    /// @param addr the load address
    /// @param sbit the speculative bit
    /// @param rid  the register to load
    void setld (t_mrtt type, const t_octa addr, const bool sbit,
		const Rid& rid);

    /// set the load information by type, address and rid pair
    /// @param type the load type to process
    /// @param addr the load address
    /// @param sbit the speculative bit
    /// @param lrid the low register to load
    /// @param hrid the high register to load
    void setld (t_mrtt type, const t_octa addr, const bool sbit,
		const Rid& lrid, const Rid& hrid);

    /// set the store information by type, address and value
    /// @param type the load type to process
    /// @param addr the load address
    /// @param oval the octa value to store
    void setst (t_mrtt type, const t_octa addr, const t_octa oval);

    /// set the store information by type, address and value
    /// @param type the load type to process
    /// @param addr the load address
    /// @param rval the real value to store
    void setst (t_mrtt type, const t_octa addr, const t_real& rval);

    /// set the store information by type, address and pair value
    /// @param type the load type to process
    /// @param addr the load address
    /// @param lval the low real value to store
    /// @param hval the high real value to store
    void setst (t_mrtt type, const t_octa addr, const t_real& lval,
		const t_real& hval);

    /// set the mrt value from a mrt and return a ordering status
    /// @param mrt the mrt used for setting
    bool setmv (const Mrt& mrt);
  };
}

#endif
