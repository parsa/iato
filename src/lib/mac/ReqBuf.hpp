// ---------------------------------------------------------------------------
// - ReqBuf.hpp                                                              -
// - iato:mac library - port request buffer class definition                 -
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

#ifndef  IATO_REQBUF_HPP
#define  IATO_REQBUF_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_PORTREQ_HPP
#include "PortReq.hpp"
#endif

namespace iato {
  
  /// The ReqBuf class is the port request buffer class. It acts as an array
  /// of request ports. The request port buffer is used by the memory 
  /// architecture to bind each request port with the equivalent port 
  /// acknowledger in bypass mode.

  class ReqBuf : public Resource {
  private:
    /// the vector of requestors
    vector<PortReq*> d_vprq;

  public:
    /// create an empty buffer
    ReqBuf (void);

    /// create an empty buffer with a context
    /// @param mtx the architectural context
    ReqBuf (Mtx* mtx);

    /// reset this buffer
    void reset (void);

    /// flush this buffer
    void flush (void);

    /// report this resource
    void report (void) const;

    /// @return the buffer size
    long length (void) const;

    /// @return a port request by index
    PortReq* get (const long index) const;

    /// add a port request to this buffer
    /// @param preq the port request to add
    void add (PortReq* preq);

    /// bind a port acknowledger to all port request
    /// @param pack the port acknowledger to bind
    void bind (Weakable* pack);

  private:
    // make the copy constructor private
    ReqBuf (const ReqBuf&);
    // make the assignment operator private
    ReqBuf& operator = (const ReqBuf&);
  };
}

#endif
