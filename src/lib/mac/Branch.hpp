// ---------------------------------------------------------------------------
// - Branch.hpp                                                              -
// - iato:mac library - base branch prediction class definition              -
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

#ifndef  IATO_BRANCH_HPP
#define  IATO_BRANCH_HPP

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

  /// The Branch class is a class that models a branch prediction
  /// system. Given an instruction pointer (IP) and an instruction window
  /// size, the branch prediction system computes the next instruction
  /// pointer. The default implementation is to add the instruction window
  /// size to the current ip. Complex sstem can also be devised by derivation
  /// of this class.

  class Branch : public Resource {
  public:
    /// return a new branch predictor by context
    /// @param mtx  the architectural context
    static Branch* mkbr (Mtx* mtx);

  public:
    /// create a default branch predictor
    Branch (void);

    /// create a branch predictor by name
    /// @param name the branch resource name
    Branch (const string& name);

    /// create a new branch system by context
    /// @param mtx  the architectural context
    Branch (Mtx* mtx);

    /// create a new branch system by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Branch (Mtx* mtx, const string& name);

    /// reset this branch system
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// @return true if the branch is predicted taken
    virtual bool istaken (const t_octa addr, const long slot) const;

    /// @return true if the branch can be predicted
    virtual bool ispredict (const t_octa addr, const long slot) const;

    /// compute the next ip from the current ip and window size
    /// @param cip the current instruction pointer
    /// @param ws  the window size
    virtual t_octa nextip (const t_octa cip, const long ws);

    /// predict the next ip from the current ip and slot
    /// @param cip the current instruction pointer
    /// @param slt the instruction slot
    virtual t_octa predict (const t_octa cip, const long slt);

    /// update the branch prediction with a current and next ip
    /// @param cip the current ip to update
    /// @param slt the current slot to update
    /// @param btk the branch taken flag
    /// @param nip the next ip to use
    virtual void update (const t_octa cip, const long slt,
			 const bool btk, const t_octa nip);

    /// update the branch system with an instruction, result and cancel flag
    /// @param inst the branch instruction
    /// @param resl the instruction result
    /// @param btk  the branch taken flag
    virtual void markbr (const Instr& inst, const Result& resl,const bool btk);

  private:
    // make the copy constructor private
    Branch (const Branch&);
    // make the assignment oerator private
    Branch& operator = (const Branch&);
  };
}

#endif
