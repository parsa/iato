// ---------------------------------------------------------------------------
// - Exception.hpp                                                           -
// - iato:isa library - exception class definition                           -
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

#ifndef  IATO_EXCEPTION_HPP
#define  IATO_EXCEPTION_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Exception class is a basic class that is used exceptional runtime
  /// event, like error or priviledge violation. An exception has a type
  /// and a reason.

  class Exception {
  protected:
    /// the exception type
    string d_type;
    /// the exception reason
    string d_reason;

  public:
    /// create a default exception
    Exception (void);

    /// create a new exception
    /// @param type the exception type
    Exception (const string& type);
    
    /// create a new exception
    /// @param type the exception type
    /// @param reason the exception reason
    Exception (const string& type, const string& reason);

    /// copy construct this exception
    /// @param that the exception to copy
    Exception (const Exception& that);

    /// destroy this exception
    virtual ~Exception (void);

    /// assign an exception to this one
    /// @param that the exception to assign
    Exception& operator = (const Exception& that);

    /// @return the exception type
    virtual string gettype (void) const;
    
    /// @return the exception reason
    virtual string getreason (void) const;

    /// print the exception message
    virtual void print (void) const;

    /// print the exception and abort
    virtual void abort (void) const;
  };
}

#endif
