// ---------------------------------------------------------------------------
// - Result.hpp                                                              -
// - iato:isa library - exeecution result value class definition             -
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

#ifndef  IATO_RESULT_HPP
#define  IATO_RESULT_HPP

#ifndef  IATO_ISA_HPP
#include "Isa.hpp"
#endif

#ifndef  IATO_MRT_HPP
#include "Mrt.hpp"
#endif

#ifndef  IATO_UVR_HPP
#include "Uvr.hpp"
#endif

namespace iato {

  /// The Result class is a simple class that holds an execution unit result.
  /// For each result, a valid bit is associated to discriminate the result 
  /// validity. The result object is generated by an execution unit that
  /// uses input result objects. The result class differs from the result
  /// class by the number of result and by the existence of a valid bit
  /// that indicates the execution validity, even if no result is produced.

  class Result {
  public:
    /// the result operation type
    enum t_rop {
      ROP_NOP, // no operation
      REG_UPD, // register update
      REG_RRT, // register reroute
      REG_LD1, // register load 1 byte
      REG_LD2, // register load 2 bytes
      REG_LD4, // register load 4 bytes
      REG_LD8, // register load 8 bytes
      REG_LDS, // register load single float
      REG_LDD, // register load double float
      REG_LDE, // register load extended float
      REG_LDF, // register load fill float
      REG_LDI, // register load integer
      RPL_LDS, // register load pair single  (low)
      RPH_LDS, // register load pair single  (high)
      RPL_LDD, // register load pair double  (low)
      RPH_LDD, // register load pair double  (high)
      RPL_LDI, // register load pair integer (low)
      RPH_LDI, // register load pair intger  (high)
      REG_ST1, // register store 1 bytes
      REG_ST2, // register store 2 bytes
      REG_ST4, // register store 4 bytes
      REG_ST8, // register store 8 bytes
      REG_STS, // register store single float
      REG_STD, // register store double float
      REG_STE, // register store extended float
      REG_STF, // register store spill float
      REG_STI, // register store integer
      REG_CHK, // register check
      REG_CX1, // register compare and exchange 1 byte
      REG_CX2, // register compare and exchange 2 bytes
      REG_CX4, // register compare and exchange 4 bytes
      REG_CX8, // register compare and exchange 8 bytes
      RSE_ALC, // rse alloc update
      RSE_CAL, // rse call update
      RSE_RRB, // rse clrrrb update
      RSE_RET, // rse return update
      RSE_LOP, // rse loop update
      ALT_INV  // invalidate alat
    };

  private:
    /// the valid flag
    bool   d_valid;
    /// the load/store address
    t_octa d_addr[IA_MDST];
    /// the destination result type
    t_rop  d_drop[IA_MDST];
    /// the destination register
    Rid    d_drid[IA_MDST];
    /// the boolean value or nat bit
    bool   d_bval[IA_MDST];
    /// the octa  values
    t_octa d_oval[IA_MDST];
    /// the real values
    t_real d_rval[IA_MDST];
    /// the alat set bit
    bool   d_aset[IA_MDST];
    /// the alat check bit
    bool   d_achk[IA_MDST];
    /// the alat clear bit
    bool   d_aclr[IA_MDST];
    /// the speculative bit
    bool   d_spec[IA_MDST];

  public:
    /// create  a default result
    Result (void);

    /// create a result by index and value
    /// @param index the result index
    /// @param value the result value
    Result (const long index, const bool value);

    /// create a result by index and value
    /// @param index the result index
    /// @param value the result value
    Result (const long index, const t_octa value);

    /// create a result by index and value
    /// @param index the result index
    /// @param value the result value
    Result (const long index, const t_real& value);

    /// copy construct an result
    /// @param that the result to copy
    Result (const Result& that);

    /// assign an result to this one
    /// @param that the result to assign
    Result& operator = (const Result& that);

    /// reset this result
    void reset (void);

    /// set the result register id
    /// @param index the register index
    /// @param rid the register id
    void setrid (const long index, const Rid& rid) {
      assert ((index >= 0) && (index < IA_MDST));
      d_drid[index]  = rid;
    }

    /// @return the result register by index
    Rid getrid (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      return d_drid[index];
    }

    /// set the valid bit
    /// @param flag the valid bit to set
    void setvalid (const bool flag) {
      d_valid = flag;
    }

    /// @return true if the result is valid
    bool isvalid (void) const {
      return d_valid;
    }

    /// @return true if the result rop is none
    bool isnone (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      return (d_drop[index] == ROP_NOP);
    }

    /// return true if a rid as the right type
    /// @param index the register index
    /// @param lreg the register type
    bool isreg (const long index, t_lreg lreg) const;

    /// @return true if one rid has the register type
    bool isreg (t_lreg lreg) const;

    /// @return the result rop by index
    t_rop getrop (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      return d_drop[index];
    }

    /// @return the result ip
    t_octa getrip (void) const;

    /// set the reroute flag for a predicate register
    /// @param index the register index
    /// @param flag  the reroute flag to set
    void setrrt (const long index, const bool flag);

    /// set the load/store address by index
    /// @param index the index to set
    /// @param rop  the operation to set
    /// @param addr the address to set
    void setaddr (const long index, t_rop rop, const t_octa addr);

    /// @return the load/store address
    t_octa getaddr (const long index) const;

    /// set an immediate value for store
    /// @param index the result index
    /// @param value the result value
    void setimmv (const long index, const t_octa value);

    /// set a real value for store
    /// @param index the result index
    /// @param value the result value
    void setrimv (const long index, const t_real value);

    /// @return the immediate store value by index
    t_octa getimmv (const long index) const;

    /// @return the real store value by index
    t_real getrimv (const long index) const;

    /// @return the result uvr value by index
    Uvr getuvr (const long index) const;

    /// set the result by index and value
    /// @param index the result index
    /// @param value the result value
    void setbval (const long index, const bool value);

    /// set the result by rid and value
    /// @param rid   the result rid
    /// @param value the result value
    void setbval (const Rid& rid, const bool value);

    /// @return the result value by index
    bool getbval (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_bval[index];
    }

    /// set the result by index and value
    /// @param index the result index
    /// @param value the result value
    void setoval (const long index, const t_octa value);

    /// set the result by index rop and and value
    /// @param index the result index
    /// @param rop   the rop to set
    /// @param value the result value
    void setoval (const long index, t_rop rop, const t_octa value);

    /// set the result by rid and value
    /// @param rid   the result rid
    /// @param value the result value
    void setoval (const Rid& rid, const t_octa value);

    /// @return the result value by index
    t_octa getoval (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_oval[index];
    }

    /// set the result by index and value
    /// @param index the result index
    /// @param value the result value
    void setrval (const long index, const t_real& value);

    /// set the result by rid and value
    /// @param rid   the result rid
    /// @param value the result value
    void setrval (const Rid& rid, const t_real& value);

    /// @return the result value by index
    /// @param index the result index
    t_real getrval (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_rval[index];
    }

    /// set the result by rid and value
    /// @param rid   the result rid
    /// @param value the result value
    void setuval (const Rid& rid, const Uvr& value);

    /// set the alat flag by index
    /// @param index the result index
    /// @param value the value to set
    void setaset (const long index, const bool value) {
      assert ((index >= 0) && (index < IA_MDST));
      assert (d_drid[index].isvalid () == true);
      d_aset[index] = value;
    }

    /// set the alat check by index
    /// @param index the result index
    /// @param value the value to set
    void setachk (const long index, const bool value) {
      assert ((index >= 0) && (index < IA_MDST));
      assert (d_drid[index].isvalid () == true);
      d_achk[index] = value;
    }

    /// set the alat clear by index
    /// @param index the result index
    /// @param value the value to set
    void setaclr (const long index, const bool value) {
      assert ((index >= 0) && (index < IA_MDST));
      assert (d_drid[index].isvalid () == true);
      d_aclr[index] = value;
    }
    
    /// set the speculation bit
    /// @param index the result index
    /// @param value the value to set
    void setspec (const long index, const bool value) {
      assert ((index >= 0) && (index < IA_MDST));
      assert (d_drid[index].isvalid () == true);
      d_spec[index] = value;
    }

    /// @return the alat bit boolean value
    bool getaset (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_aset[index];
    }

    /// @return the alat check bit boolean value
    bool getachk (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_achk[index];
    }

    /// @return the alat clear bit boolean value
    bool getaclr (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_aclr[index];
    }
    
    /// @return the speculative bit boolean value
    bool getspec (const long index) const {
      assert ((index >= 0) && (index < IA_MDST));
      assert ((d_valid == true) && (d_drop[index] != ROP_NOP));
      assert (d_drid[index].isvalid () == true);
      return d_spec[index];
    }
    
    /// set the alat index to invalidate
    /// @param index the result rid index to invalidate
    void setinv (const long index) {
       assert ((index >= 0) && (index < IA_MDST));
       d_drid[index].reset (); 
    }

    /// @return the result associated mrt
    Mrt getmrt (void) const;

    /// update a result by mrt
    /// @param mrt the mrt used for update
    void update (const Mrt& mrt);
  };
}

#endif
