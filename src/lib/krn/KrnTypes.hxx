// ---------------------------------------------------------------------------
// - KrnTypes.hxx                                                            -
// - iato:krn library - kernel based types definition                        -
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

#ifndef  IATO_KRNTYPES_HXX
#define  IATO_KRNTYPES_HXX

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  // base types definition
  typedef t_quad t_krn_mode;  // file mode
  typedef t_octa t_krn_ino;   // inode number
  typedef t_octa t_krn_dev;   // device id
  typedef t_octa t_krn_nlink; // number of links
  typedef t_quad t_krn_uid;   // user id
  typedef t_quad t_krn_gid;   // group id
  typedef t_octa t_krn_off;   // file size or offset
  typedef t_octa t_krn_time;  // file size or offset
  typedef t_octa t_krn_size;  // another size
}

#endif
