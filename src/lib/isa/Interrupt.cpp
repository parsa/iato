// ---------------------------------------------------------------------------
// - Interrupt.cpp                                                           -
// - iato:isa library - interrupt class implementation                       -
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

#include "Utils.hpp"
#include "Interrupt.hpp"

namespace iato {
  using namespace std;

  // this procedure map a interruption type to a string representation
  static string to_reason (t_icode code) {
    string result = "unknown interrupt";
    switch (code) {
    case ABORT_RESET:
      result = "machine reset";
      break;
    case ABORT_MCA:
      result = "machine check abort";
      break;
    case FAULT_IT_RNAT_CONS:
      result = "register nat consumption fault";
      break;
    case FAULT_IT_INST_BREAK:
      result = "instruction break fault";
      break;
    case FAULT_IT_INST_PAGE:
      result = "instruction page not present fault";
      break;
    case FAULT_IT_INST_ACRGT:
      result = "instruction access rights fault";
      break;
    case FAULT_IT_DATA_PAGE:
      result = "data page not present fault";
      break;
    case FAULT_IT_DATA_ACRGT:
      result = "data access rights fault";
      break;
    case FAULT_IT_DATA_ALIGN:
      result = "unaligned data reference fault";
      break;
    case FAULT_IT_RESV_RGFD:
      result = "reserved register/field fault";
      break;
    default:
      {
	ostringstream os;
	os << "unknown interrupt code " << (long) code;
	result = os.str ();
      }
      break;
    }
    return result;
  }
  
  // create a default interrupt

  Interrupt::Interrupt (void) {
    d_valid = false;
  }

  // create an interrupt with a type and a reason

  Interrupt::Interrupt (t_icode code, const string& reason) {
    d_valid  = true;
    d_code   = code;
    d_type   = "interrupt";
    d_reason = reason;
    d_ip     = OCTA_0;
  }

  // create an interrupt with a type, reason and ip

  Interrupt::Interrupt (t_icode code, const string& reason, const t_octa ip) {
    d_valid  = true;
    d_code   = code;
    d_type   = "interrupt";
    d_reason = reason;
    d_ip     = ip;
  }
 
  // create an interrupt with a type and an instruction

  Interrupt::Interrupt (t_icode code, const Instr& inst) {
    d_valid  = true;
    d_code   = code;
    d_inst   = inst;
    d_type   = "interrupt";
    d_reason = to_reason (code);
    d_ip     = inst.isvalid () ? inst.getiip () : OCTA_0;
  }

 
  // create an interrupt with a type, an instruction and a reason

  Interrupt::Interrupt (t_icode code, const Instr& inst, 
			const string& reason) {
    d_valid  = true;
    d_code   = code;
    d_inst   = inst;
    d_type   = "interrupt";
    d_reason = reason;
    d_ip     = inst.isvalid () ? inst.getiip () : OCTA_0;
  }

  // copy construct this interrupt

  Interrupt::Interrupt (const Interrupt& that) : Exception (that) {
    d_valid = that.d_valid;
    d_code  = that.d_code;
    d_inst  = that.d_inst;
    d_ip    = that.d_ip;
  }

  // assign an interrupt to this one

  Interrupt& Interrupt::operator = (const Interrupt& that) {
    this->Exception::operator = (that);
    d_valid = that.d_valid;
    d_code  = that.d_code;
    d_inst  = that.d_inst;
    d_ip    = that.d_ip;
    return *this;
  }

  // reset an interrupt

  void Interrupt::reset (void) {
    d_valid = false;
    d_inst.reset ();
    d_ip     = OCTA_0;
    d_type   = "interrupt";
    d_reason = "";
  }

  // return true if the interrupt is valid

  bool Interrupt::isvalid (void) const {
    return d_valid;
  }

  // return true if the interrupt is abort

  bool Interrupt::isabort (void) const {
    if (d_valid == false) return false;
    if ((d_code > ABORT_FLOOR) || (d_code <= ABORT_CEIL)) return true;
    return false;
  }

  // return true if the interrupt is inter

  bool Interrupt::isinter (void) const {
    if (d_valid == false) return false;
    if ((d_code > INTER_FLOOR) || (d_code <= INTER_CEIL)) return true;
    return false;
  }

  // return true if the interrupt is a fault

  bool Interrupt::isfault (void) const {
    if (d_valid == false) return false;
    if ((d_code > FAULT_FLOOR) || (d_code <= FAULT_CEIL)) return true;
    return false;
  }

  // return true if the interrupt is a trap

  bool Interrupt::istrap (void) const {
    if (d_valid == false) return false;
    if ((d_code > TRAPS_FLOOR) || (d_code <= TRAPS_CEIL)) return true;
    return false;
  }

  // get the interrupt code

  t_icode Interrupt::getcode (void) const {
    assert (d_valid == true);
    return d_code;
  }

  // set the interrupt instruction

  void Interrupt::setinst (const Instr& inst) {
    assert (d_valid == true);
    d_inst = inst;
  }

  // get the interrupt instruction

  Instr Interrupt::getinst (void) const {
    assert (d_valid == true);
    return d_inst;
  }

  // set the offending ip

  void Interrupt::setip (const t_octa ip) {
    assert (d_valid == true);
    d_ip = ip;
  }

  // return the offending ip

  t_octa Interrupt::getip (void) const {
    assert (d_valid == true);
    return d_ip;
  }

  // return the offending slot

  long Interrupt::getslot (void) const {
    assert (d_valid == true);
    if (d_inst.isvalid () == true) return d_inst.getslot ();
    return 0;
  }

  // return the interrupt immediate value

  t_octa Interrupt::getiim (void) const {
    assert (d_valid == true);
    t_octa result = OCTA_0;
    switch (d_code) {
    case FAULT_IT_INST_BREAK:
      result = d_inst.getimmv (0);
      break;
    default:
      break;
    }
    return result;
  }

  // print the interrupt message

  void Interrupt::print (void) const {
    if (d_valid == false) return;
    cerr << "error   : "   << d_type   << endl;
    cerr << "reason  : "   << d_reason;
    if (d_inst.isvalid () == true) {
      cerr << " at 0x";
      cerr << hex << setw (16) << setfill ('0') << d_inst.getiip ();
      cerr << dec << " slot " << d_inst.getslot () << endl;
      cerr << "opcode  : " << trim (d_inst.recode ()) << endl;
    } else {
      cerr << endl;
    }
  }
}
