// ---------------------------------------------------------------------------
// - Exception.cpp                                                           -
// - iato:isa library - exception class implementation                       -
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

#include "Exception.hpp"

namespace iato {
  
  // create a default exception

  Exception::Exception (void) {
    d_type   = "unknown";
    d_reason = "unknown";
    d_cycle  = 0;
  }

  // create an exception by type

  Exception::Exception (const string& type) {
    d_type   = type;
    d_reason = "unknown";
    d_cycle  = 0;
  }

  // create an exception by type and reason

  Exception::Exception (const string& type, const string& reason) {
    d_type   = type;
    d_reason = reason;
    d_cycle  = 0;
  }

  // copy construct this exception

  Exception::Exception (const Exception& that) {
    d_type   = that.d_type;
    d_reason = that.d_reason;
    d_cycle  = that.d_cycle;
  }

  // destroy this exception

  Exception::~Exception (void) {
  }

  // assign an exception to this one

  Exception& Exception::operator = (const Exception& that) {
    d_type   = that.d_type;
    d_reason = that.d_reason;
    d_cycle  = that.d_cycle;
    return *this;
  }

  // get the exception type

  string Exception::gettype (void) const {
    return d_type;
  }

  // get the exception reason

  string Exception::getreason (void) const {
    return d_reason;
  }

  // set the exception cycle

  void Exception::setcycle (const t_long cycle) {
    d_cycle = cycle;
  }

  // return the exception cycle

  t_long Exception::getcycle (void) const {
    return d_cycle;
  }

  // print the exception message

  void Exception::print (void) const {
    cerr << "error  : " << d_type   << endl;
    cerr << "reason : " << d_reason << endl;
    if (d_cycle != 0) {
      cerr << "cycle  : " << d_cycle  << endl;
    }
  }

  // print the exception and abort

  void Exception::abort (void) const {
    print ();
    exit  (1);
  }
}
