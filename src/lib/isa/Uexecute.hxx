// ---------------------------------------------------------------------------
// - Uexecute.hxx                                                            -
// - iato:isa library - utility function for instruction execution           -
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

#ifndef IATO_UEXECUTE_HXX
#define IATO_UEXECUTE_HXX

#ifndef  IATO_CONFIG_HXX
#include "Config.hxx"
#endif

namespace iato {

  static inline t_long sign_extend (const t_byte value) {
    t_long result = value;
    if ((result & SIGN_1) == SIGN_1)  result |= ~MASK_1;
    return result;
  }
  
  static inline t_long sign_extend (const t_word value) {
    t_long result = value;
    if ((result & SIGN_2) == SIGN_2)  result |= ~MASK_2;
    return result;
  }
  
  static inline t_long sign_extend (const t_quad value) {
    t_long result = value;
    if ((result & SIGN_4) == SIGN_4)  result |= ~MASK_4;
    return result;
  }

  // return true if negatif false otherwise
  static inline bool get_sign (const t_octa value) {
    return ((value & SIGN_8) == SIGN_8) ? true : false;
  }
}
#endif
