// ---------------------------------------------------------------------------
// - Register.hpp                                                            -
// - iato:isa library - register class definition                            -
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

#ifndef  IATO_REGISTER_HPP
#define  IATO_REGISTER_HPP

#ifndef  IATO_IP_HPP
#include "Ip.hpp"
#endif

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

#ifndef  IATO_CFM_HPP
#include "Cfm.hpp"
#endif

#ifndef  IATO_PSR_HPP
#include "Psr.hpp"
#endif

#ifndef  IATO_OPERAND_HPP
#include "Operand.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Register class is a complete IA64 ISA register bank implementation.
  /// The bank holds the general purpose (and nat), the floating, the 
  /// predicate, the branch, the application and machine specific registers.
  /// A generic read and write method can be used to mute these registers.
  /// This register bank is configured by default with the IA64 isa. However,
  /// for specific implementation, the size of each bank can be redefined.
  /// Note that all operation that uses an operand or a rid operates with
  /// the physical register number. Such number is assumed to be built
  /// from the logical number, the rse and any other specific implementation,
  /// like a RAT with an out-of-order machine.

  class Register : public Resource {
  private:
    /// number of general register (and nat)
    long d_gnum;
    /// number of floating registers
    long d_fnum;
    /// number of predicate registers
    long d_pnum;
    /// number of branch registers
    long d_bnum;
    /// number of application registers
    long d_anum;
    /// number of control registers
    long d_cnum;
    /// the bank 0 GR
    t_octa* p_gbnk;
    /// the bank 0 nat
    bool*   p_nbnk;
    /// the GR bank 
    t_octa* p_greg;
    /// the NR bank
    bool*   p_nreg;
    /// the FR bank
    t_real* p_freg;
    /// the PR bank
    bool*   p_preg;
    /// the BR bank
    t_octa* p_breg;
    /// the AR bank
    t_octa* p_areg;
    /// the CR bank
    t_octa* p_creg;
    /// the instruction pointer
    Ip  d_ip;
    /// the current frame marker
    Cfm d_cfm;
    /// the processor status register
    Psr d_psr;

  public:
    /// create a default register bank
    Register (void);

    /// create a register bank with a context
    /// @param ctx the current context
    Register (Ctx* ctx);

    /// create a register bank with a context and name
    /// @param ctx the current context
    /// @param name the resource name
    Register (Ctx* ctx, const string& name);

    /// destroy this register bank
    ~Register (void);

    /// reset the register
    void reset (void);

    /// report this register bank
    void report (void) const;

    /// write a register by type, index and value
    /// @param lreg the register type
    /// @param index the register index
    /// @param value the register value
    void write (t_lreg lreg, const long index, const t_octa value);

    /// write a register by type, index and value
    /// @param lreg the register type
    /// @param index the register index
    /// @param value the register value
    void write (t_lreg lreg, const long index, const bool value);

    /// write a register by type, index and value
    /// @param lreg the register type
    /// @param index the register index
    /// @param value the register value
    void write (t_lreg lreg, const long index, const t_real& value);

    /// write a result to the register file
    /// @param resl the result to write
    void write (const Result& resl);

    /// read a register value by type
    /// @param lreg the register type
    /// @param index the register index
    t_octa getoval (t_lreg lreg, const long index) const;

    /// read a register value by type
    /// @param lreg the register type
    /// @param index the register index
    bool getbval (t_lreg lreg, const long index) const;

    /// read a register value by type
    /// @param lreg the register type
    /// @param index the register index
    t_real getrval (t_lreg lreg, const long index) const;

    /// evaluate a register with a rid
    /// @param rid the register id
    t_octa getoval (const Rid& rid) const;

    /// evaluate a register with a rid
    /// @param rid the register id
    bool getbval (const Rid& rid) const;

    /// evaluate a register with a rid
    /// @param rid the register id
    t_real getrval (const Rid& rid) const;

    /// evaluate an operand from the bank
    /// @param oprd the operand to evaluate
    void eval (Operand& oprd) const;

    /// check one record against the register file content
    /// @param rcd the record to check
    bool check (const Record& rcd) const;

  private:
    // make the copy constructor private
    Register (const Register&);
    // make the assignment operator private
    Register& operator = (const Register&);
  };
}

#endif
