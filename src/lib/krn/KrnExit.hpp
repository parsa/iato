// ---------------------------------------------------------------------------
// - KrnExit.hpp                                                             -
// - iato:isa library - exit exception class definition                      -
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

#ifndef  IATO_KRNEXIT_HPP
#define  IATO_KRNEXIT_HPP

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace iato {
  using namespace std;

  /// The krnExit exception is a special exception that is thrown by the
  /// interrupt engine when an exit system call has been received. The
  /// exception holds the exit status value

  class KrnExit : public Exception {
  private:
    /// the exit status
    t_octa d_status;

  public:
    /// create a new exit exception with a status
    /// @param status the exception status
    KrnExit (const t_octa status);
 
    /// copy construct this exception
    /// @param that the exception to copy
    KrnExit (const KrnExit& that);

    /// assign an exception to this one
    /// @param that the exception to assign
    KrnExit& operator = (const KrnExit& that);

    /// @return the exit status
    t_octa getstatus (void) const;

    /// print the exception message
    void print (void) const;
  };
}

#endif
