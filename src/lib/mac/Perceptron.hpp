// ---------------------------------------------------------------------------
// - Perceptron.hpp                                                          -
// - iato:mac library - perceptron element class definition                  -
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

#ifndef  IATO_PERCEPTRON_HPP
#define  IATO_PERCEPTRON_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

namespace iato {

  /// The Perceptron class is a base class used to build a perceptron
  /// predictor. The perceptron operates a set of weights. The weights
  /// are used to compute the perceptron value that is later mapped to
  /// a boolean value. The weights can be bound by a value. The number
  /// of weights is called the perceptron size.

  class Perceptron {
  private:
    /// the perceptron size
    long  d_size;
    /// the perceptron weights
    long* p_whts;
    /// the weight bound
    long  d_wbnd;
    /// the perceptron threshold
    long  d_pthr;
    /// the confidence bound
    long  d_pcbn;

  public:
    /// create a default perceptron
    Perceptron (void);

    /// create a perceptron with a context
    /// @param mtx the architectural context
    Perceptron (Mtx* mtx);

    /// destroy this perceptron
    ~Perceptron (void);

    /// reset this perceptron
    void reset (void);

    /// @return the perceptron size
    long getsize (void) const;

    /// @return the perceptron weight bound
    long getwbnd (void) const;

    /// @return the perceptron threshold
    long getpthr (void) const;

    /// @return the perceptron confidence bound
    long getpcbn (void) const;

    /// @return true if the perceptron is within the confidence bound
    bool isvalid (const t_octa hist) const;

    /// @return true if the perceptron is ok
    bool istrue (const t_octa hist) const;

    /// @return the perceptron value
    long compute (const t_octa hist) const;

    /// update a perceptron with a history
    /// @param hist the history used for update
    void update (const t_octa hist);

  private:
    // make the copy constructor private
    Perceptron (const Perceptron&);
    // make the assignement operator private
    Perceptron& operator = (const Perceptron&);
  };
}

#endif
