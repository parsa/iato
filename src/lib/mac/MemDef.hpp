// ---------------------------------------------------------------------------
// - MemDef.hpp                                                              -
// - iato:mac library - memory request type definition                       -
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

#ifndef  IATO_MEMDEF_HPP
#define  IATO_MEMDEF_HPP

namespace iato {

  /// The MemDef enumeration defines the supported memory request operations.
  /// Those operation are implemented by the memory request, memory
  /// acknowledge and store buffer. Not all request types are supported across
  /// all units. For instance, read request are not supported in the store
  /// buffer.

  enum t_mreq {
    REQ_NUL, // no request
    REQ_BND, // request bundle
    REQ_LD1, // request load byte
    REQ_LD2, // request load word
    REQ_LD4, // request load quad
    REQ_LD8, // request load octa
    REQ_LDS, // request load single real
    REQ_LDD, // request load double real
    REQ_LDE, // request load extended real
    REQ_LDF, // request load fill real
    REQ_LDI, // request load integer real
    REQ_ST1, // request store byte
    REQ_ST2, // request store word
    REQ_ST4, // request store quad
    REQ_ST8, // request store octa
    REQ_STS, // request store single real
    REQ_STD, // request store double real
    REQ_STE, // request store extended real
    REQ_STF, // request store spill real
    REQ_STI  // request store integer real
  };
}

#endif
