// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - iato:iaos client - main program implementation                          -
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

#include "Option.hpp"
#include "Binary.hpp"
#include "Verify.hpp"
#include "Disass.hpp"
#include "ElfImage.hpp"
#include "Exception.hpp"
#include "Statistic.hpp"

namespace iato {
  // this procedure report binary information
  static void report_binary (const Option& opt, ElfImage* elf) {
    if (!elf) return;
    // get elf option flags
    bool ehdr = opt.getehdr ();
    bool eexc = opt.geteexc ();
    bool etxt = opt.getetxt ();
    if (!ehdr && ! eexc && !etxt) return;
    // create elf binary object
    Binary es  (elf);
    es.setflag (ehdr, eexc, etxt);
    es.report  ();
  }

  // this procedure verify a binary text
  static void verify_binary (const Option& opt, ElfImage* elf) {
    if (!elf) return;
    // get verify option flags
    bool vbnd = opt.getvbnd ();
    bool vins = opt.getvins ();
    if (!vbnd && !vins) return;
    // create elf verify object
    Verify vo  (elf);
    vo.setflag (vbnd, vins);
    vo.verify  ();
  }

  // this procedure disaseemble a binary text
  static void disass_binary (const Option& opt, ElfImage* elf) {
    if (!elf) return;
    // get disassemble option flags
    bool dadr = opt.getdadr ();
    bool dbnd = opt.getdbnd ();
    bool dins = opt.getdins ();
    bool dfrc = opt.getdfrc ();
    if (!dadr && !dbnd && !dins) return;
    // create disassemble object
    Disass ds  (elf);
    ds.setflag (dadr, dbnd, dins, dfrc);
    ds.report  ();
  }

  // this procedure computes statistic info from a binary text
  static void stat_binary (const Option& opt, ElfImage* elf) {
    if (!elf) return;
    // get statistic option flags
    bool sflg = opt.getsflg ();
    bool dbnd = opt.getbflg ();
    bool dins = opt.getiflg ();
    bool dnop = opt.getnflg ();
    if (!sflg) return;
    // create a statistic object
    Statistic stat (elf);
    stat.setflg    (dbnd, dins, dnop);
    stat.compute   ();
    stat.print     ();
  }
}

// and everything start here

int main (int argc, char** argv) {
  using namespace iato;
  // process the arguments
  Option opt (argc, argv);
  // get the program option
  string name = opt.getname ();

  // get the elf image
  try {
    ElfImage* elf = new ElfImage (name);
    if (!elf || elf->isvalid () == false) {
      cerr << "error : invalid elf file " << name << endl;
      exit (1);
    }
    // check for report
    report_binary (opt, elf);
    // check for verify
    verify_binary (opt, elf);
    // check for disassembly
    disass_binary (opt, elf);
    // check for statistic
    stat_binary (opt, elf);
    // clean eveything
    delete elf;
  } catch (Exception& e) {
    e.abort ();
  }
  // done
  return 0;
}
