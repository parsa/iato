// ---------------------------------------------------------------------------
// - Printing.cpp                                                            -
// - iato:iata client - trace printing class implementation                  -
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

#include "Printing.hpp"

namespace iato {
  using namespace std;

  // create a new trace printing object

  Printing::Printing (const string& name, Atx* atx) {
    p_reader = new Tracer::Reader (name);
    p_filter = new Filtering (atx);
  }

  // destroy this reader

  Printing::~Printing (void) {
    delete p_reader;
    delete p_filter;
  }

  // print the traces
  
  void Printing::print (void) const {
    t_long bicc = p_reader->getbicc ();
    if (bicc == TR_BICC) bicc = 1;
    Tracer::t_vrcd* src = p_reader->trcrd ();
    Tracer::t_vrcd* dst = new Tracer::t_vrcd;
    while (src) {
      p_filter->filter (dst, src);
      long size = dst->size ();
      cout << "trace " << bicc++ << " {"   << endl;
      for (long i = 0; i < size; i++) {
	cout << " " << (*dst)[i].repr () << endl;
      }
      cout << '}' << endl;
      delete src;
      src = p_reader->trcrd ();
    }
    delete dst;
  }
}
