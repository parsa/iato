// ---------------------------------------------------------------------------
// - Operand.hpp                                                             -
// - iato:isa library - instruction operand value class definition           -
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

#ifndef  IATO_OPERAND_HPP
#define  IATO_OPERAND_HPP

#ifndef  IATO_ISA_HPP
#include "Isa.hpp"
#endif

#ifndef  IATO_RID_HPP
#include "Rid.hpp"
#endif

#ifndef  IATO_UVR_HPP
#include "Uvr.hpp"
#endif

namespace iato {

  /// The Operand class is a simple class that holds an instruction source
  /// operand value. The operand is associated with a register number. This
  /// means that an operand object only holds value associated with a
  /// register. During execution, immediate value that are need by a unit
  /// are taken from the decoded instruction. The operand object is primarily
  /// used during the evaluation stage.

  class Operand {
  private:
    /// security valid bit
    bool d_valid[IA_MSRC];
    /// the register id
    Rid d_orid[IA_MSRC];
    /// the boolean value or nat bit
    bool d_bval[IA_MSRC];
    /// the octa operands
    t_octa d_oval[IA_MSRC];
    /// the real operands
    t_real d_rval[IA_MSRC];

  public:
    /// create  a default operand
    Operand (void);

    /// copy construct an operand
    /// @param that the operand to copy
    Operand (const Operand& that);

    /// assign an operand to this one
    /// @param that the operand to assign
    Operand& operator = (const Operand& that);

    /// reset this operand
    void reset (void);

    /// set the operand register id
    /// @param index the register index
    /// @param rid the register id
    void setrid (const long index, const Rid& rid) {
      assert ((index >= 0) && (index < IA_MSRC));
      d_orid[index]  = rid;
    }

    /// @return the operand register by index
    Rid getrid (const long index) const {
      assert ((index >= 0) && (index < IA_MSRC));
      return d_orid[index];
    }

    /// @return true if the operand is valid
    bool isvalid (void) const;

    /// @return the operand valid bit
    bool isvalid (const long index) const {
      assert ((index >= 0) && (index < IA_MSRC));
      return d_valid [index];
    }

    /// set the operand by index and uvr
    /// @param index the operand index
    /// @param uvr the uvr operand value
    void setuvr (const long index, const Uvr& uvr);

    /// set the operand by index and value
    /// @param index the operand index
    /// @param value the operand value
    void setbval (const long index, const bool value) {
      assert ((index >= 0) && (index < IA_MSRC));
      d_valid[index] = true;
      d_bval[index] = value;
    }

    /// @return the operand value by index
    bool getbval (const long index) const  {
      assert ((index >= 0) && (index < IA_MSRC));
      assert (d_valid[index] == true);
      return d_bval[index];
    }

    /// set the operand by index and value
    /// @param index the operand index
    /// @param value the operand value
    void setoval (const long index, const t_octa value) {
      assert ((index >= 0) && (index < IA_MSRC));
      d_valid[index] = true;
      d_oval[index]  = value;
    }

    /// @return the operand value by index
    t_octa getoval (const long index) const {
      assert ((index >= 0) && (index < IA_MSRC));
      assert (d_valid[index] == true);
      return d_oval[index];
    } 

    /// set the operand by index and value
    /// @param index the operand index
    /// @param value the operand value
    void setrval (const long index, const t_real& value) {
      assert ((index >= 0) && (index < IA_MSRC));
      d_valid[index] = true;
      d_rval[index] = value;
    }

    /// @return the operand value by index
    t_real getrval (const long index) const {
      assert ((index >= 0) && (index < IA_MSRC));
      assert (d_valid[index] == true);
      return d_rval[index];
    }
  };
}

#endif
