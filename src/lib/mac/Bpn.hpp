// ---------------------------------------------------------------------------
// - Bpn.hpp                                                                 -
// - iato:mac library - bypass network class definition                      -
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

#ifndef  IATO_BPN_HPP
#define  IATO_BPN_HPP

#ifndef  IATO_BPE_HPP
#include "Bpe.hpp"
#endif

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Bpn class is the bypass network. It is a collection of bypass
  /// element. Each element is allocated by a stage that needs result
  /// forwarding. The bypass network can be used directly to evaluate some
  /// operands. In case of match, such operand value is set automatically.

  class Bpn : public Resource {
  private:
    /// the vector of bpe
    vector<Bpe*> d_vbpe;

  public:
    /// create an empty network
    Bpn (void);

    /// create an empty network by context
    /// @param mtx the architectural context
    Bpn (Mtx* mtx);

    /// create an empty network by context and name
    /// @param mtx the architectural context
    /// @param name the resource name
    Bpn (Mtx* mtx, const string& name);

    /// reset this network
    void reset (void);

    /// report this resource
    void report (void) const;

    /// add a network element
    /// @param bpe the element to add
    void add (Bpe* bpe);

    /// evaluate an rid in the bypass network
    /// @param rid the rid to evaluate
    Uvr eval (const Rid& rid) const;

    /// evaluate an operand in the network
    /// @param oprd the operand to evaluate
    void eval (Operand& oprd) const;

    /// update an operand in the network
    /// @param oprd the operand to evaluate
    void update (Operand& oprd) const;

    /// update the result with rpm value
    /// @param ssi the instruction for update
    /// @param resl the result to update
    void update (const Ssi& ssi, Result& resl) const;

    /// clear a bypass element by rid
    /// @param rid the bpe rid to clear
    void clear (const Rid& rid);

    /// clear a bypass element by result
    /// @param resl the result used for clearing
    void clear (const Result& resl);

  private:
    // make the copy constructor private
    Bpn (const Bpn&);
    // make the assignment operator private
    Bpn& operator = (const Bpn&);
  };
}

#endif
