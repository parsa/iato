// ---------------------------------------------------------------------------
// - Eib.hpp                                                                 -
// - iato:mac library - external interrupt buffer class definition           -
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

#ifndef  IATO_EIB_HPP
#define  IATO_EIB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Eib class is the external interrupt buffer  that is implemented
  /// as a queue. External interrupts are supported by storing them in a queue
  /// and processing them as soon as possible. An external interrupt is by 
  /// nature asynchronous to the underlying hardware and should not be confused
  /// with the synchronous one (aka instruction interrupt).

  class Eib : public Resource {
  private:
    /// the interrupt buffer
    queue<Interrupt> d_ibuf;

  public:
    /// create a new interrupt buffer
    Eib (void);

    /// create a new interrupt buffer with a context
    /// @param mtx the architectural context
    Eib (Mtx* mtx);

    /// reset this interrupt buffer
    void reset (void);
    
    /// report this resource
    void report (void) const;

    /// @return true if the interrupt buffer is empty
    bool isempty (void) const;

    /// push an interrupt in the buffer
    /// @param vi the virtual interrupt to push
    void push (const Interrupt& vi);

    /// pop an interrupt from this buffer
    Interrupt pop (void);

  private:
    // make the copy constructor private
    Eib (const Eib&);
    // make the assignment operator private
    Eib& operator = (const Eib&);
  };
}

#endif
