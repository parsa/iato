// ---------------------------------------------------------------------------
// - Cache.cpp                                                               -
// - iato:isa library - abstract cache memory class implementation           -
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

#include "Cache.hpp"

namespace iato {

  // create a default cache
  
  Cache::Cache (void) {
    d_type  = UNIFIED;
    d_cupd  = WRITETH;
    d_cwrt  = WRNOALC;
    d_level = 0;
  }

  // set the cache parameters

  void Cache::setparam (const long    level, const t_cmtype type, 
			const t_cmupd cupd,  const t_cmwrt  cwrt) {
    d_type  = type;
    d_cupd  = cupd;
    d_cwrt  = cwrt;
    d_level = level; assert (d_level > 0);
  }

  // return the cache type

  Cache::t_cmtype Cache::gettype (void) const {
    return d_type;
  }

  // return the cache level

  long Cache::getlevel (void) const {
    return d_level;
  }
}
