// ---------------------------------------------------------------------------
// - Option.cpp                                                              -
// - iato:iaio client - option class implementation                          -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#include "Sio.hpp"
#include "Utils.hpp"
#include "Option.hpp"

namespace iato {
  using namespace std;

  // this procedure print a usage message and exit
  static void usage (void) {
    cerr << "usage: iaio [option] name [args]                       " << endl;
    cerr << "  -h                     help message                  " << endl;
    cerr << "  -r                     report all resources          " << endl;
    cerr << endl;     
    cerr << "  options related to the tracer                        " << endl;
    cerr << "  -t:v                   enable tracer verbose         " << endl;
    cerr << "  -t:r rcd1,rcd2         record type to trace          " << endl;
    cerr << "  -t:s src1,src2         record source to trace        " << endl;
    cerr << "  -t:f name              set tracer file name          " << endl;
    cerr << "  -t:t threshold         set tracer threshold          " << endl;
    cerr << "  -t:h                   set tracer reschedule         " << endl;
    cerr << "  -t:i                   set branch target tracing     " << endl;
    cerr << "  -t:b #                 begin trace cycle number      " << endl;
    cerr << "  -t:e #                 end trace cycle number        " << endl;
    cerr << endl;     
    cerr << "  options related to the simulator                     " << endl;
    cerr << "  -c                     enable checker mode           " << endl;
    cerr << "  -s                     enable stat report            " << endl;
    cerr << "  -s:c #                 enable cycle stat report      " << endl;
    cerr << "  -m   #                 maximum cycle count           " << endl;
    cerr << "  -p:s name:type=value   set a context parameter       " << endl;
    cerr << "  -p:d                   dump the context to the output" << endl;
    cerr << endl;
    cerr << "  options related to the architecture                  " << endl;
    cerr << "  -W   #                 issue width in bundles        " << endl;
    cerr << "  -M   #                 number of M units             " << endl;
    cerr << "  -I   #                 number of I units             " << endl;
    cerr << "  -F   #                 number of F units             " << endl;
    cerr << "  -B   #                 number of B units             " << endl;
    cerr << "  -G   #                 number of gr (registers)      " << endl;
    cerr << "  -a:b bprd              branch predictor type         " << endl;
    exit (1);
  }

  Option::Option (int argc, char** argv) {
    // initialize the default options
    d_cdump = false;
    d_tflag = false;
    d_tbflg = false;
    d_thflg = false;
    d_vflag = false;
    d_rflag = false;
    d_cflag = false;
    d_sflag = false;
    d_sccnt = 0;
    d_tthrs = TR_THRS;
    d_winsz = BN_IWSZ;
    d_munit = MK_MUSZ;
    d_iunit = MK_IUSZ;
    d_funit = MK_FUSZ;
    d_bunit = MK_BUSZ;
    d_grnum = GR_LRSZ;
    d_maxcc = 0;
    d_btrcc = TR_BICC;
    d_etrcc = TR_EICC;
    d_trcty = "";
    d_trsrc = "";
    d_bprd  = BP_TYPE;
    // check for arguments
    string data;
    if (argc < 2) usage ();
    int count = 1;
    // loop with arguments
    while (count < argc) {
      char* arg = argv[count];
      // check for option
      if ((arg[0] == '-') && (d_name.empty () == true)) {
	switch (arg[1]) {
	case 'h':
	  usage ();
	  break;
	case 'a':
	  if ((arg[2] == ':') && (arg[3] == 'b') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_bprd = argv[count];
	    break;
	  }
	  usage ();
	  break;
	case 't':
	  if ((arg[2] == ':') && (arg[3] == 'v') && (arg[4] == '\0')) {
	    d_tflag = true;
	    d_vflag = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'r') && (arg[4] == '\0')) {
	    d_tflag = true;
	    if (++count == argc) usage ();
	    d_trcty = argv[count];
	    break;
	  }	  
	  if ((arg[2] == ':') && (arg[3] == 's') && (arg[4] == '\0')) {
	    d_tflag = true;
	    if (++count == argc) usage ();
	    d_trsrc = argv[count];
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'f') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_tflag = true;
	    d_tname = argv[count];
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 't') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_tthrs = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'i') && (arg[4] == '\0')) {
	    d_tflag = true;
	    d_tbflg = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_btrcc = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'e') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_etrcc = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'h') && (arg[4] == '\0')) {
	    d_tflag = true;
	    d_thflg = true;
	    break;
	  }
	  usage ();
	  break;
	case 'c':
	  d_cflag = true;
	  break;
	case 's':
	  if (arg[2] == '\0') {
	    d_sflag = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'c') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_sccnt = tolong (argv[count]);
	    if (d_sccnt < 0) {
	      cerr << "error: illegal stat count, # = " << d_sccnt << endl;
	      exit (1);
	    }
	    d_sflag = true;
	    break;
	  }
	  usage ();
	  break;
	case 'r':
	  d_rflag = true;
	  break;
	case 'p':
	  if ((arg[2] == ':') && (arg[3] == 's') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_cprm.push_back (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'd') && (arg[4] == '\0')) {
	    d_cdump = true;
	    break;
	  }
	  usage ();
	  break;
	case 'W':
	  if (++count == argc) usage ();
	  d_winsz = tolong (argv[count]);
	  if (d_winsz <= 0) {
	    cerr << "error: illegal instruction window size, P# = " << d_winsz;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'M':
	  if (++count == argc) usage ();
	  d_munit = tolong (argv[count]);
	  if (d_munit <= 0) {
	    cerr << "error: illegal number of M units, M# = " << d_munit;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'I':
	  if (++count == argc) usage ();
	  d_iunit = tolong (argv[count]);
	  if (d_iunit <= 0) {
	    cerr << "error: illegal number of I units, I# = " << d_iunit;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'F':
	  if (++count == argc) usage ();
	  d_funit = tolong (argv[count]);
	  if (d_funit <= 0) {
	    cerr << "error: illegal number of F units, F# = " << d_funit;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'B':
	  if (++count == argc) usage ();
	  d_bunit = tolong (argv[count]);
	  if (d_bunit <= 0) {
	    cerr << "error: illegal number of B units, B# = " << d_bunit;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'G':
	  if (++count == argc) usage ();
	  d_grnum = tolong (argv[count]);
	  if (d_grnum <= 0) {
	    cerr << "error: illegal number of gr, G# = " << d_grnum;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'm':
	  if (++count == argc) usage ();
	  d_maxcc = tolong (argv[count]);
	  if (d_maxcc < 0) {
	    cerr << "error: illegal maximum cycle, # = " << d_maxcc;
	    cerr << endl;
	    exit (1);
	  }
	  break;	  
	default:
	  usage ();
	  break;
	}
      } else {
	if (d_name.empty () == true)
	  d_name = arg;
	else
	  d_argv.push_back (arg);
      }
      count++;
    }
  }
}
