// ---------------------------------------------------------------------------
// - Predicate.hpp                                                           -
// - iato:mac library - base predicate prediction class definition           -
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

#ifndef  IATO_PREDICATE_HPP
#define  IATO_PREDICATE_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Predicate class is a class that models a predicate prediction
  /// system. This class is a base class that provides three types of
  /// services, namely prediction confidence (i.e avaibility), predicate
  /// prediction and system update. A complex prediction system can be built
  /// by derivation. This system, as a base system never provide prediction.

  class Predicate : public Resource {
  public:
    /// @return a new predicate predictor by name
    static Predicate* mkpr (const string& name);    
    /// @return a new predicate predictor by context
    static Predicate* mkpr (Mtx* mtx);
    /// @return a new predicate predictor by name and context
    static Predicate* mkpr (const string& name, Mtx* mtx);

  protected:
    /// the predictor type
    string d_type;
    /// the predictor history
    t_octa d_phst;

  public:
    /// create a default predictor
    Predicate (void);

    /// create a predictor by name
    /// @param name the resource name
    Predicate (const string& name);

    /// create a new predicate system by context
    /// @param mtx  the architectural context
    Predicate (Mtx* mtx);

    /// create a new predicate system by context and name
    /// @param mtx  the architectural context
    /// @param name the predicate resource name
    Predicate (Mtx* mtx, const string& name);

    /// reset this predictor
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// @return the predictor type
    virtual string gettype (void) const;

    /// @return true if the predicate can be predicted
    virtual bool isvalid (const t_octa ip, const long slot, 
			  const long pred) const;

    /// @return true if the instruction predicate can be predicted
    virtual bool ispredict (const Instr& inst) const;

    /// @return the predictor history
    virtual t_octa getphst (void) const;

    /// set the predictor history
    /// @param phst the history to set
    virtual void setphst (const t_octa phst);

    /// compute the predicate value
    /// @param ip   the instruction ip
    /// @param slot the instruction slot
    /// @param pred the predicate number
    virtual bool compute (const t_octa ip, const long slot, 
			  const long pred) const;

    /// predict the instruction predicate value
    /// @param inst the instruction used for prediction
    virtual bool predict (const Instr& inst) const;

    /// update the predicate prediction by index and value
    /// @param ip   the instruction ip
    /// @param slot the instruction slot
    /// @param pred the predicate index
    /// @param pval the predicate value
    /// @param phst the predictor history
    virtual void update (const t_octa ip, const long slot, const long pred, 
			 const bool pval, const t_octa phst); 

    /// update the predicate prediction by instruction and value
    /// @param inst the instruction used for update
    /// @param pval the predicate value
    virtual void markpp (const Instr& inst, const bool pval);

    /// update the predicate prediction by instruction and result
    /// @param inst the instruction used for update
    /// @param resl the result used for update
    virtual void markpp (const Instr& inst, const Result& resl);

  private:
    // make the copy constructor private
    Predicate (const Predicate&);
    // make the assignment oerator private
    Predicate& operator = (const Predicate&);
  };
}

#endif
