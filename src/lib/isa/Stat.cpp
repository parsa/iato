// ---------------------------------------------------------------------------
// - Stat.cpp                                                                -
// - iato:isa library - isa stat collection class implementation             -
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

#include "Stat.hpp"
#include "Utils.hpp"

namespace iato {
  // the string filling size
  static const long FILL_SIZE = 30;

  // create a default stat collection

  Stat::Stat (void) {
    d_bflg = false;
    d_iflg = false;
    d_nflg = false; 
    reset ();
  }

  // destroy this stat collection

  Stat::~Stat (void) {
  }

  // reset this stat collection

  void Stat::reset (void) {
    d_stim = time (NULL);
    d_etim = d_stim;
    d_ncyc = 0;
    d_nbnd = 0;
    d_nins = 0;
    d_nuis = 0;
    d_nprd = 0;
    d_nbpd = 0;
    d_ncan = 0;
    d_nbcn = 0;
    d_ntpf = 0;
    d_nbpf = 0;
    d_nopf = 0;
    d_npbr = 0;
    d_npbs = 0;
    d_nppr = 0;
    d_npps = 0;
    d_nxmi = 0;
    for (long i = 0; i < Bundle::BN_MAXTPL; i++) d_bndl[i] = 0;
    for (long i = 0; i < OPCODE_MAX; i++)        d_inst[i] = 0;
  }

  // set report flags

  void Stat::setflg (const bool bflg, const bool iflg, const bool nflg) {
    d_bflg = bflg;
    d_iflg = iflg;
    d_nflg = nflg;
  }

  // mark a simulation cycle

  void Stat::marksc (void) {
    d_ncyc++;
  }

  // mark a simulation cycle by value

  void Stat::marksc (const long count) {
    d_ncyc+= count;
  }

  // mark the end of the simulation

  void Stat::markes (void) {
    d_etim = time (NULL);
  }

  // mark a pipeline flush

  void Stat::markpf (const bool bflg) {
    d_ntpf++;
    if (bflg == true)
      d_nbpf++;
    else
      d_nopf++;
  }

  // mark branch prediction

  void Stat::markbp (const bool bflg) {
    d_npbr++;
    if (bflg == true) d_npbs++;
  }

  // mark predicate prediction

  void Stat::markpp (const bool pflg) {
    d_nppr++;
    if (pflg == true) d_npps++;
  }

  // mark the extra flag

  void Stat::markxf (const bool xflg) {
    if (xflg == true) d_nxmi++;
  }

  // add a bundle to this collection

  void Stat::addbndl (const Bundle& bndl) {
    if (bndl.isvalid () == false) return;
    d_nbnd++;
    d_bndl[bndl.gettmpl ()]++;
  }

  // add an instruction to this collection

  void Stat::addinst (const Instr& inst) {
    if (inst.isvalid () == false) return;
    // update instruction info
    d_nins++;
    if (inst.isnop () == false) d_nuis++;
    d_inst[inst.getiopc ()]++;
    // update predicate info
    if (inst.ispred () == true) {
      d_nprd++;
      if (inst.isbr () == false) d_nbpd++;
    }
  }

  // add an instruction to this collection with cancel flag

  void Stat::addinst (const Instr& inst, const bool cnlf) {
    // check and update instruction
    if (inst.isvalid () == false) return;
    addinst (inst);
    // update cancel status
    if (cnlf == true) {
      d_ncan++;
      if (inst.isbr () == false) d_nbcn++;
    }
  }

  // add an instruction to this collection with cancel and rescheule flags

  void Stat::addinst (const Instr& inst, const bool cnlf, const bool xflg) {
    // check and update instruction
    if (inst.isvalid () == false) return;
    // update instruction
    addinst (inst,cnlf);
    // update extra flag
    if (xflg == true) d_nxmi++;
  }

  // add a nop instruction

  void Stat::addnop (t_unit unit) {
    switch (unit) {
    case MUNIT:
      d_inst[M_NOP]++;
      d_nins++;
      break;
    case IUNIT:
      d_inst[I_NOP]++;
      d_nins++;
      break;
    case FUNIT:
      d_inst[F_NOP]++;
      d_nins++;
      break;
    case BUNIT:
      d_inst[B_NOP]++;
      d_nins++;
      break;
    case XUNIT:
      d_inst[X_NOP]++;
      d_nins++;
      break;
    default:
      break;
    }
  }

  // get number of nop instruction

  t_long Stat::getnnop (void) const {
    return (d_inst[I_NOP] + d_inst[M_NOP] + 
	    d_inst[F_NOP] + d_inst[B_NOP] + d_inst[X_NOP]);
  }

  // print the bundle information

  void Stat::printb (void) const {
    if (d_nbnd == 0) return;
    for (t_byte i = 0; i < Bundle::BN_MAXTPL; i++) {
      double prct = 100 * (double) d_bndl[i] / (double) d_nbnd;
      string bndl = rfill (Bundle::tostrws (i), FILL_SIZE);
      cout << bndl << " : " << d_bndl[i];
      cout << "\t(" << setprecision (3) << prct << "%)" << endl;
    }
    cout << endl;
  }

  // print the instruction information

  void Stat::printi (void) const {
    if (d_nins == 0) return;
    for (long i = 0; i < OPCODE_MAX; i++) {
      double prct = 100.0 * (double) d_inst[i] / (double) d_nins;
      string inst = rfill (Instr::tostr (i), FILL_SIZE);
      cout << inst << " : " << d_inst[i];
      cout << "\t(" << setprecision (3) << prct << "%)" << endl;
    }
    cout << endl;
  } 

  // print the nop information

  void Stat::printn (void) const {
    if (d_nins == 0) return;
    cout << "number of nop instructions [M] : " << d_inst[M_NOP] << endl;
    cout << "number of nop instructions [I] : " << d_inst[I_NOP] << endl;
    cout << "number of nop instructions [F] : " << d_inst[F_NOP] << endl;
    cout << "number of nop instructions [B] : " << d_inst[B_NOP] << endl;
    cout << "number of nop instructions [X] : " << d_inst[X_NOP] << endl;
    cout << "number of nop instructions     : " << getnnop ()    << endl;
    cout << endl;
  } 

  // report the stat collection summary

  void Stat::summary (void) const {
    // compute simulation time
    t_long et = d_etim - d_stim;
    if (et != 0) {
      double rate = (double) d_nins / ((double) et * 1000.0);
      cout << "simulation time (in s)         : " << et;
      cout << "\t(" << setprecision (3) << rate << " Ki/s)" << endl;
    }
    // report bundles
    if (d_nbnd != 0) {
      cout << "number of bundles              : " << d_nbnd << endl;
    }
    // report cycle and ipc
    if (d_ncyc != 0) {
      cout << "number of cycles               : " << d_ncyc << endl;
      if (d_nins != 0) {
	double ipc = (double) d_nins / (double) d_ncyc;
	cout << "instruction per cycle (ipc)    : ";
	cout << setprecision (3) << ipc << endl;
      }
    }
    // report instructions
    if (d_nins != 0) {
      double pnop = 100.0 * (double) getnnop () / (double) d_nins;
      double pprd = 100.0 * (double) d_nprd / (double) d_nins;
      double pbpd = 100.0 * (double) d_nbpd / (double) d_nins;
      cout << "number of instructions         : " << d_nins << endl;
      cout << "number of usefull instructions : " << d_nuis << endl;
      cout << "nop instructions               : " << getnnop ();
      cout << "\t(" << setprecision (3) << pnop << "%)" << endl;
      cout << "predicated instructions        : " << d_nprd;
      cout << "\t(" << setprecision (3) << pprd << "%)" << endl;
      cout << "predicated instructions non br : " << d_nbpd;
      cout << "\t(" << setprecision (3) << pbpd << "%)" << endl;
      if (d_ncan != 0) {
	double pcan = 100.0 * (double) d_ncan / (double) d_nins;
	double pbcn = 100.0 * (double) d_nbcn / (double) d_nins;
	cout << "cancelled instructions         : " << d_ncan;
	cout << "\t(" << setprecision (3) << pcan << "%)" << endl;
	cout << "cancelled instructions non br  : " << d_nbcn;
	cout << "\t(" << setprecision (3) << pbcn << "%)" << endl;
      }
      if (d_nxmi != 0) {
	double pxmi = 100.0 * (double) d_nxmi / (double) d_nins;
	cout << "extra marked instructions      : " << d_nxmi;
	cout << "\t(" << setprecision (3) << pxmi << "%)" << endl;
      }
    }
    // report pipeline flushes
    if (d_ntpf != 0) {
      double pbpf = 100.0 * (double) d_nbpf / (double) d_ntpf;
      double popf = 100.0 * (double) d_nopf / (double) d_ntpf;
      cout << "number of pipeline flushes     : " << d_ntpf << endl;
      cout << "number of branch   flushes     : " << d_nbpf;
      cout << "\t(" << setprecision (3) << pbpf << "%)" << endl;
      cout << "number of other    flushes     : " << d_nopf;
      cout << "\t(" << setprecision (3) << popf << "%)" << endl;
    }
    // report branch prediction
    if (d_npbr != 0) {
      double ppbs = 100.0 * (double) d_npbs / (double) d_npbr;
      cout << "number of branch predictions   : " << d_npbr << endl;
      cout << "successfull branches           : " << d_npbs;      
      cout << "\t(" << setprecision (3) << ppbs << "%)" << endl;
    }
    // report predicate prediction
    if (d_nppr != 0) {
      double ppps = 100.0 * (double) d_npps / (double) d_nppr;
      cout << "number of predicate prediction : " << d_nppr << endl;
      cout << "successfull predicates         : " << d_npps;
      cout << "\t(" << setprecision (3) << ppps << "%)" << endl;
    }
  }

  // print all statistics

  void Stat::print (void) const { 
    // print bundle stat
    if (d_bflg == true) printb ();
    // print instruction stat
    if (d_iflg == true) printi ();
    // print nop stat
    if (d_nflg == true) printn ();
    // print stat summary
    summary ();
  }
}
