// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iata client - option class implementation                          -
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
#include "Option.hpp"
#include "Generate.hpp"

namespace iato {
  using namespace std;

  // this procedure print a usage message and exit
  static void usage (void) {
    cerr << "usage: iata [option] name                               " << endl;
    cerr << "  -h               help message                         " << endl;
    cerr << endl;
    cerr << "  options related to filter                             " << endl;
    cerr << "  -f:t rcd1,rcd2         record type selection filter   " << endl;
    cerr << "  -f:n src1,src2         record source selection filter " << endl;
    cerr << "  -f:p src1:rcd1         pair selection filter          " << endl;
    cerr << "  -f:g igr1:igr2         instruction group selection    " << endl;
    cerr << endl;
    cerr << "  options related to trace file                         " << endl;
    cerr << "  -t:h                   write trace header             " << endl;
    cerr << "  -t:b #                 starting trace number          " << endl;
    cerr << "  -t:e #                 ending trace number            " << endl;
    cerr << endl;
    cerr << "  options related to statistics                         " << endl;
    cerr << "  -s                     compute all statisitic         " << endl;
    cerr << "  -s:b                   add bundle distibution         " << endl;
    cerr << "  -s:i                   add instruction distribution   " << endl;
    cerr << "  -s:n                   add nop distribution           " << endl;
    cerr << "  -s:e #                 ignore # empty traces          " << endl;
    cerr << endl;
    cerr << "  option related to trace generation                    " << endl;
    cerr << "  -g                     generate default trace         " << endl;
    cerr << "  -g:o                   trace output                   " << endl;
    cerr << "  -g:w #                 the instruction window size    " << endl;
    cerr << "  -g:p #                 the branch penalty             " << endl;
    cerr << "  -g:f #                 the f-unit latencie            " << endl;
    cerr << endl;
    cerr << "  option related to the analyzer                        " << endl;
    cerr << "  -a:p name              analyze a predicate predictor  " << endl;
    cerr << "  -a:t                   build an analyze table         " << endl;
    cerr << endl;
    cerr << "  options related to the analyzer                       " << endl;
    cerr << "  -d                     dump all traces                " << endl;
    cerr << "  -p:s name:type=value   set a context parameter        " << endl;
    exit (1);
  }

  // special stdin usage message
  static void printerr (void) {
    cerr << endl;
    cerr << "With the standard input, it is only possible to either" << endl;
    cerr << "  - print the trace"        << endl;
    cerr << "  - print the trace header" << endl;
    cerr << "  - generate a new trace"   << endl;
    cerr << "  - compute statistics"     << endl;
    exit (1);
  }

  Option::Option (int argc, char** argv) {
    // set default options
    d_dflg   = false;
    d_sflg   = false;
    d_bflg   = false;
    d_iflg   = false;
    d_nflg   = false;
    d_hflg   = false;
    d_neti   = 0;
    d_gflg   = false;
    d_tbgn   = OCTA_0;
    d_tend   = OCTA_A;
    d_gwin   = DEF_WINSIZE;
    d_gbrp   = DEF_BRPTY;
    d_gflat  = DEF_FULAT;
    d_gname  = "";
    d_rcty   = "";
    d_rsrc   = "";
    d_rpair  = "";
    d_pflg   = false;
    d_mdat   = false;
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
	case 'd':
	  if (arg[2] == '\0') {
	    d_dflg = true;
	    break;
	  }
	  usage ();
	  break;
	case 'a':
	  if ((arg[2] == ':') && (arg[3] == 'p') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_pflg = true;
	    d_pnam = argv[count];
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 't') && (arg[4] == '\0')) {
	    d_mdat = true;
	    break;
	  }
	  usage ();
	  break;
	case 'p':
	  if ((arg[2] == ':') && (arg[3] == 's') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_cprm.push_back (argv[count]);
	    break;
	  }
	  usage ();
	  break;
	case 'f':
	  if (arg[2] == ':') {
	    if ((arg[3] == 't') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_rcty = argv[count];
	      break;
	    }
	    if ((arg[3] == 'n') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_rsrc = argv[count];
	      break;
	    }
	    if ((arg[3] == 'p') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_rpair = argv[count];
	      break;
	    }
	    if ((arg[3] == 'g') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_group = argv[count];
	      break;
	    }
	  }
	  usage ();
	  break;
	case 't':
	  if (arg[2] == ':') {
	    if ((arg[3] == 'h') && (arg[4] == '\0')) {
	      d_hflg = true;
	      break;
	    }
	    if ((arg[3] == 'b') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_tbgn = tolong (argv[count]);
	      if (d_tbgn < 0) usage ();
	      break;
	    }
	    if ((arg[3] == 'e') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_tend = tolong (argv[count]);
	      if (d_tend < 0) usage ();
	      break;
	    }
	  }
	  usage ();
	  break;
	case 's':
	  if (arg[2] == '\0'){
	    d_sflg = true;
	    break;
	  }
	  if (arg[2] == ':') {
	    if ((arg[3] == 'b') && (arg[4] == '\0')) {
	      d_sflg = true;
	      d_bflg = true;
	      break;
	    }
	    if ((arg[3] == 'i') && (arg[4] == '\0')) {
	      d_sflg = true;
	      d_iflg = true;
	      break;
	    }
	    if ((arg[3] == 'n') && (arg[4] == '\0')) {
	      d_sflg = true;
	      d_nflg = true;
	      break;
	    }
	    if ((arg[3] == 'e') && (arg[4] == '\0')) {
	      d_sflg = true;
	      if (++count == argc) usage ();
	      d_neti = tolong (argv[count]);
	      if (d_neti < 0) usage ();
	      break;
	    }
	  }
	  usage ();
	  break;
	case 'g':
	  if (arg[2] == '\0') {
	    d_gflg = true;
	    break;
	  }
	  if (arg[2] == ':') {
	    d_gflg = true;
	    if ((arg[3] == 'o')  && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      d_gname = argv[count];
	    }
	    else if ((arg[3] == 'w') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      long value = tolong (argv[count]);
	      if (value > 0) {
		d_gwin = value;
		break;
	      } else usage ();
	    }
	    else if ((arg[3] == 'p') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      long value = tolong (argv[count]);
	      if (value >= 0) {
		d_gbrp = value;
		break;
	      } else usage ();
	    }
	    else if ((arg[3] == 'f') && (arg[4] == '\0')) {
	      if (++count == argc) usage ();
	      long value = tolong (argv[count]);
	      if (value > 0) {
		d_gflat = value;
		break;
	      } else usage ();
	    }
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
    // special verification when using the standard input
    if (d_name.length () != 0) return;
    // reset counter
    count = 0;
    if (d_bflg || d_iflg || d_nflg) count++;
    if (d_dflg) count++;
    if (d_hflg) count++;
    if (d_gflg) count++;
    if (d_pflg) count++;
    if (count < 2) return;
    printerr ();
  }
}

