// ---------------------------------------------------------------------------
// - Rpm.cpp                                                                 -
// - iato:isa library - rid pair mapping class implementation                -
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

#include "Rpm.hpp"

namespace iato {

  // create a default rpm
  
  Rpm::Rpm (void) {
    p_fmap = 0;
    reset ();
  }

  // create a rpm with a source and destinatiob rid

  Rpm::Rpm (const Rid& src, const Rid& dst) {
    d_rsrc = src;
    d_rdst = dst;
    p_fmap = 0;
  }

  // copy construct this rpm

  Rpm::Rpm (const Rpm& that) {
    d_rsrc = that.d_rsrc;
    d_rdst = that.d_rdst;
    p_fmap = that.p_fmap;
  }

  // assign  rpm to this one

  Rpm& Rpm::operator = (const Rpm& that) {
    d_rsrc = that.d_rsrc;
    d_rdst = that.d_rdst;
    p_fmap = that.p_fmap;
    return *this;
  }

  // reset this rpm
  
  void Rpm::reset (void) {
    d_rsrc.reset ();
    d_rdst.reset ();
  }

  // return true if the rpm is valid

  bool Rpm::isvalid (void) const {
    return d_rsrc.isvalid () && d_rdst.isvalid ();
  }

  // set the mapping by source and destination

  void Rpm::setmap (const Rid& src, const Rid& dst) {
    d_rsrc = src;
    d_rdst = dst;
  }

  // set the mapping by source, destination and function mapping

  void Rpm::setmap (const Rid& src, const Rid& dst, t_rfm rfm) {
    d_rsrc = src;
    d_rdst = dst;
    p_fmap = rfm;
  }

  // return the source rpm

  Rid Rpm::getsrc (void) const {
    return d_rsrc;
  }

  // return the destination rpm

  Rid Rpm::getdst (void) const {
    return d_rdst;
  }

  // return the function map
  
  Rpm::t_rfm Rpm::getrfm (void) const {
    return p_fmap;
  }
}
