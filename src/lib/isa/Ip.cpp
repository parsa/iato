// ---------------------------------------------------------------------------
// - Ip.cpp                                                                  -
// - iato:isa library - instruction pointer resource class implementation    -
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

#include "Ip.hpp"
#include "Isa.hpp"

namespace iato {

  // create a default ip

  Ip::Ip (void) {
    d_ip = 0;
  }

  // create an ip with a value

  Ip::Ip (const t_octa value) {
    d_ip = value;
  }

  // create an ip with a value and displacement

  Ip::Ip (const t_octa value, const t_octa disp) {
    d_ip = value + disp;
  }

  // copy construct this ip

  Ip::Ip (const Ip& that) {
    d_ip = that.d_ip;
  }

  // assign an ip to this one

  Ip& Ip::operator = (const Ip& that) {
    d_ip = that.d_ip;
    return *this;
  }

  // increase this ip by one bundle (prefix)

  Ip& Ip::operator ++ (void) {
    d_ip += BN_BYSZ;
    return *this;
  }

  // increase this ip by one bundle (postfix)

  Ip Ip::operator ++ (int) {
    Ip result = *this;
    d_ip += BN_BYSZ;
    return result;
  }

  // reset this ip

  void Ip::reset (void) {
    d_ip = 0;
  }

  // set the ip value

  void Ip::setip (const t_octa ip) {
    d_ip = ip;
  }

  // return the current ip

  t_octa Ip::getip (void) const {
    return d_ip;
  }
}
