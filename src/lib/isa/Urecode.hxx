// ---------------------------------------------------------------------------
// - Urecode.hxx                                                             -
// - iato:isa library - utility function for instruction recode              -
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

#ifndef  IATO_URECODE_HXX
#define  IATO_URECODE_HXX

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// @return a string representation of a PR
  string to_pred (const long pred);

  /// @return a string representation of GR
  string to_greg (const long greg);

  /// @return a string representation of FR
  string to_freg (const long freg);

  /// @return a string representation of PR
  string to_preg (const long preg);

  /// @return a string representation of BR
  string to_breg (const long breg);

  /// @return a string representation of AR
  string to_areg (const long areg);

  /// @return a string representation of CR
  string to_creg (const long creg);

  /// @return a string representation of an immediate
  string to_immv (const t_octa immv, const bool sign, const bool hflg);  

  /// @return a string representation of a branch target
  string to_brtg (const t_octa immv, const t_octa ip);
}

#endif
