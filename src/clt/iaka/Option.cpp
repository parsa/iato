// ---------------------------------------------------------------------------
// - Option.cpp                                                              -
// - iato:iaka client - option class implementation                          -
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

#include "Eio.hpp"
#include "Etx.hpp"
#include "Utils.hpp"
#include "Option.hpp"

namespace iato {
  using namespace std;

  // this procedure print a usage message and exit
  static void usage (void) {
    cerr << "usage: iaka [option] name [args]                       " << endl;
    cerr << "  -h                     help message                  " << endl;
    cerr << endl;
    cerr << "  options related to the tracer                        " << endl;
    cerr << "  -t:v                   verbose tracer                " << endl;
    cerr << "  -t:r rcd1,rcd2         record type to trace          " << endl;
    cerr << "  -t:s src1,src2         record source to trace        " << endl;
    cerr << "  -t:f name              set tracer file name          " << endl;
    cerr << "  -t:t threshold         set tracer threshold          " << endl;
    cerr << "  -t:i                   set branch target tracing     " << endl;
    cerr << "  -t:o                   set branch tracing only       " << endl;
    cerr << "  -t:p                   set predicate tracing only    " << endl;
    cerr << "  -t:g                   set predicate targets only    " << endl;
    cerr << "  -t:b #                 begin trace cycle number      " << endl;
    cerr << "  -t:e #                 end trace cycle number        " << endl;
    cerr << endl;
    cerr << "  options related to the emulator                      " << endl;
    cerr << "  -c                     enable checker mode           " << endl;
    cerr << "  -s                     enable stat report            " << endl;
    cerr << "  -m #                   maximum bundle count          " << endl;
    cerr << "  -p:s name:type=value   set a context parameter       " << endl;
    cerr << "  -p:d                   dump the context to the output" << endl;
    cerr << endl;
    cerr << "  options related to the architecture                  " << endl;
    cerr << "  -W #                   issue width in bundles        " << endl;
    cerr << "  -G #                   number of general registers   " << endl;
    cerr << "  -a:d                   enable disperse mode          " << endl;
    cerr << "  -a:p                   branch pipeline refill        " << endl;
    cerr << "  -a:b bprd              branch predictor type         " << endl;
    cerr << endl;
    exit (1);
  }

  Option::Option (int argc, char** argv) {
    // initialize the default options
    d_cdmp = false;
    d_tflg = false;
    d_tbfl = false;
    d_tvfl = false;
    d_tbof = EM_BOFL;
    d_tpof = EM_POFL;
    d_tpgf = EM_PGFL;
    d_dflg = false;
    d_cflg = false;
    d_sflg = false;
    d_tthr = TR_THRS;
    d_btrc = TR_BICC;
    d_etrc = TR_EICC;
    d_mbnd = EM_MXCC;
    d_iwsz = BN_IWSZ;
    d_gnum = GR_LRSZ;
    d_pfil = EM_PFIL;
    d_rcty = "";
    d_rsrc = "";
    d_tnam = "";
    d_bprd = BP_TYPE;

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
	case 't':
	  if ((arg[2] == ':') && (arg[3] == 'v') && (arg[4] == '\0')) {
	    d_tflg = true;
	    d_tvfl = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'r') && (arg[4] == '\0')) {
	    d_tflg = true;
	    if (++count == argc) usage ();
	    d_rcty = argv[count];
	    break;
	  }	  
	  if ((arg[2] == ':') && (arg[3] == 's') && (arg[4] == '\0')) {
	    d_tflg = true;
	    if (++count == argc) usage ();
	    d_rsrc = argv[count];
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'f') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_tflg = true;
	    d_tnam = argv[count];
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 't') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_tthr = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'i') && (arg[4] == '\0')) {
	    d_tflg = true;
	    d_tbfl = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'o') && (arg[4] == '\0')) {
	    d_tflg = true;
	    d_tbof = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'p') && (arg[4] == '\0')) {
	    d_tflg = true;
	    d_tpof = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'g') && (arg[4] == '\0')) {
	    d_tflg = true;
	    d_tpgf = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_btrc = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'e') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_etrc = tolong (argv[count]);
	    break;
	  }
	  usage ();
	  break;
	case 'c':
	  d_cflg = true;
	  break;
	case 's':
	  d_sflg = true;
	  break;  
	case 'm':
	  if (++count == argc) usage ();
	  d_mbnd = tolong (argv[count]);
	  if (d_mbnd < 0) {
	    cerr << "error: illegal maximum bundle count, # = " << d_mbnd;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'p':
	  if ((arg[2] == ':') && (arg[3] == 's') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_cprm.push_back (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'd') && (arg[4] == '\0')) {
	    d_cdmp = true;
	    break;
	  }
	  usage ();
	  break;
	case 'a':
	  if ((arg[2] == ':') && (arg[3] == 'd') && (arg[4] == '\0')) {
	    d_dflg = true;
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'p') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_pfil = tolong (argv[count]);
	    break;
	  }
	  if ((arg[2] == ':') && (arg[3] == 'b') && (arg[4] == '\0')) {
	    if (++count == argc) usage ();
	    d_bprd = argv[count];
	    break;
	  }
	  usage ();
	  break;
	case 'W':
	  if (++count == argc) usage ();
	  d_iwsz = tolong (argv[count]);
	  if (d_iwsz <= 0) {
	    cerr << "error: illegal issue width, W# = " << d_iwsz;
	    cerr << endl;
	    exit (1);
	  }
	  break;
	case 'G':
	  if (++count == argc) usage ();
	  d_gnum = tolong (argv[count]);
	  if (d_gnum < GR_LRSZ) {
	    cerr << "error: illegal general register size, # = " << d_gnum;
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
    // fix the record source according to flag
    if (d_tbof == true) {
      if (d_rsrc.size () == 0) d_rsrc = "EBO"; else d_rsrc += ",EBO";
    }
    if (d_tpof == true) {
      if (d_rsrc.size () == 0) d_rsrc = "EPO"; else d_rsrc += ",EPO";
    }
    if (d_tpgf == true) {
      if (d_rsrc.size () == 0) d_rsrc = "EPG"; else d_rsrc += ",EPG";
    }
  }
}
