// ---------------------------------------------------------------------------
// - Resource.cpp                                                            -
// - iato:isa library - resource class implementation                        -
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

#include "Resource.hpp"

namespace iato {
  
  // create a default resource

  Resource::Resource (void) {
    d_name   = "UNKNOWN";
    p_stat   = 0;
    p_tracer = 0;
  }

  // create a new resource by name
  
  Resource::Resource (const string& name) {
    d_name   = name;
    p_stat   = 0;
    p_tracer = 0;
  }

  // copy construct this resource

  Resource::Resource (const Resource& that) {
    d_name   = that.d_name;
    p_stat   = that.p_stat;
    p_tracer = that.p_tracer;
  }

  // destroy this reource

  Resource::~Resource (void) {
  }

  // assign a resource to this one

  Resource& Resource::operator = (const Resource& that) {
    d_name   = that.d_name;
    p_stat   = that.p_stat;
    p_tracer = that.p_tracer;
    return *this;
  }

  // report some resource information
  
  void Resource::report (void) const {
    using namespace std;
    cout << "resource : " << d_name << endl;
  }

  // set the resource name

  void Resource::setname (const string& name) {
    d_name = name;
  }

  // return the resource name

  string Resource::getname (void) const {
    return d_name;
  }

  // set the resource stat collector

  void Resource::setstc (Stat* stc) {
    p_stat = stc;
  }

  // set the resource tracer

  void Resource::settrc (Tracer* tracer) {
    p_tracer = tracer;
  }
}
