// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iaos client - option class implementation                          -
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

namespace iato {
  using namespace std;

  // this procedure print a usage message and exit
  static void usage (void) {
    cerr << "usage: iaos [option] name                        " << endl;
    cerr << "  -h        help message                         " << endl;
    cerr << endl;
    cerr << "  options related to elf image                   " << endl;
    cerr << "  -e        report elf information               " << endl;
    cerr << "  -e:h      report elf header                    " << endl;
    cerr << "  -e:e      report elf executable                " << endl;
    cerr << "  -e:t      report elf text section              " << endl;
    cerr << endl;
    cerr << "  options related to verification                " << endl;
    cerr << "  -v        verify instructions                  " << endl;
    cerr << "  -v:b      verify bundles only                  " << endl;
    cerr << endl;
    cerr << "  options related to disassembly                 " << endl;
    cerr << "  -d        disassemble                          " << endl;
    cerr << "  -d:a      disassemble address                  " << endl;
    cerr << "  -d:b      disassemble bundles                  " << endl;
    cerr << "  -d:i      disassemble instructions             " << endl;
    cerr << "  -d:f      ignore disassembly error             " << endl;
    cerr << endl;
    cerr << "  options related to statistic operation         " << endl;
    cerr << "  -s        compute statisitics                  " << endl;
    cerr << "  -s:b      compute bundle distibution           " << endl;
    cerr << "  -s:i      compute instruction distribution     " << endl;
    cerr << "  -s:n      compute percentage of nops           " << endl;
    exit (1);
  }

  Option::Option (int argc, char** argv) {
    // set default options
    d_ehdr = false;
    d_eexc = false;
    d_etxt = false;
    d_vbnd = false;
    d_vins = false;
    d_dadr = false;
    d_dbnd = false;
    d_dins = false;
    d_dfrc = false;
    d_sflg = false;
    d_bflg = false;
    d_iflg = false;
    d_nflg = false;

    // the sub option
    string data;
    // check for arguments
    if (argc < 2) usage ();
    int count = 1;
    // loop with arguments
    while (count < argc) {
      char* arg = argv[count];
      // check for option
      if (arg[0] == '-') {
	switch (arg[1]) {
	case 'h':
	  usage ();
	  break;
	case 'e':
	  if (arg[2] == '\0') {
	    d_ehdr = true;
	    d_eexc = true;
	    d_etxt = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'h')) {
	    d_ehdr = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'e')) {
	    d_eexc = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 't')) {
	    d_etxt = true;
	    break;
	  }
	  usage ();
	  break;
	case 'v':
	  if (arg[2] == '\0') {
	    d_vbnd = true;
	    d_vins = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b')) {
	    d_vbnd = true;
	    break;
	  }
	  usage ();
	  break;
	case 'd':
	  if (arg[2] == '\0') {
	    d_dadr = true;
	    d_dbnd = true;
	    d_dins = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'a')) {
	    d_dadr = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b')) {
	    d_dbnd = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'i')) {
	    d_dins = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'f')) {
	    d_dfrc = true;
	    break;
	  }
	  usage ();
	  break;
	case 's':
	  if (arg[2] == '\0'){
	    d_sflg = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b')) {
	    d_sflg = true;
	    d_bflg = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'i')) {
	    d_sflg = true;
	    d_iflg = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'n')) {
	    d_sflg = true;
	    d_nflg = true;
	    break;
	  }
	  usage ();
	  break;
	default:
	  usage ();
	  break;
	}
      } else {
	if (d_name.length () != 0) usage ();
	d_name = arg;
      }
      count++;
    }
  }
}
