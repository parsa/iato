// ---------------------------------------------------------------------------
// - Interrupt.hpp                                                           -
// - iato:isa library - base interrupt class definition                      -
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

#ifndef  IATO_INTERRUPT_HPP
#define  IATO_INTERRUPT_HPP

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

#ifndef  IATO_INTCODE_HPP
#include "Intcode.hpp"
#endif

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Interrupt class is a special class of exception that is used to model
  /// the IA64 interruption. By itself, an interrupt is not different than an
  /// exception, except that an instruction (the offending one) can be
  /// recorded. There are four classes of interrupts, namely, aborts,
  /// interrupt, faults and traps. The interruption priority is given by its
  /// code as recordered in the enumeration type. Note that the IA32 interrupt
  /// are not defined here. This class performs also the interrupt decoding
  /// by computing (on demand) the interrupt vector, the IIP, the IFA, the IIM,
  /// and the ISR. Note that some bit in the ISR are not necessarely set here
  /// since they depends on the execution context (ni bit for example). All
  /// bits that are context dependent are set to 0. In the extreme case that
  /// the interrupt comes from a place where no instruction is available,
  /// some extra field are provided to help in resolving the source or error.
  /// The base line information is the ip. In this case, the instruction
  /// record is invalid.

  class Interrupt : public Exception {
  private:
    /// the valid bit
    bool d_valid;
    /// the interrupt code
    t_icode d_code;
    /// the offending instruction
    Instr  d_inst;
    /// the offending ip
    t_octa d_ip;

  public:
    /// create a default interrupt
    Interrupt (void);

    /// create a new interrupt by type and reason
    /// @param code the interrupt code
    /// @param reason the interrupt reason
    Interrupt (t_icode code, const string& reason);

    /// create a new interrupt by type, reason and ip
    /// @param code the interrupt code
    /// @param reason the interrupt reason
    /// @param ip the offending ip
    Interrupt (t_icode code, const string& reason, const t_octa ip);

    /// create a new interrupt by type and instruction
    /// @param code the interrupt code
    /// @param inst the offending instruction
    Interrupt (t_icode code, const Instr& inst);

    /// create a new interrupt by type, instruction and reason
    /// @param code the interrupt code
    /// @param inst the offending instruction
    /// @param reason the interrupt reason
    Interrupt (t_icode code, const Instr& inst, const string& reason);
 
    /// copy construct this exception
    /// @param that the interrupt to copy
    Interrupt (const Interrupt& that);

    /// assign an interrupt to this one
    /// @param that the interrupt to assign
    Interrupt& operator = (const Interrupt& that);

    /// reset this interrupt
    void reset (void);

    /// @return true if the interrupt is valid
    bool isvalid (void) const;

    /// @return true if the interrupt is abort
    bool isabort (void) const;

    /// @return true if the interrupt is inter
    bool isinter (void) const;

    /// @return true if the interrupt is a fault
    bool isfault (void) const;

    /// @return true if the interrupt is a trap
    bool istrap (void) const;

    /// @return the interruption code
    t_icode getcode (void) const;

    /// set the interrupt instruction
    /// @param inst the instruction to set
    void setinst (const Instr& inst);

    /// @return the interrupt instruction
    Instr getinst (void) const;

    /// set the interrupt offending ip
    /// @param ip the offending ip
    void setip (const t_octa ip);

    /// @return the offending ip
    t_octa getip (void) const;

    /// @return the offending slot
    long getslot (void) const;

    /// @return the interrupt immediate value
    t_octa getiim (void) const;

    /// print the interrupt message
    void print (void) const;
  };
}

#endif
