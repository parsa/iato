// ---------------------------------------------------------------------------
// - System.hpp                                                              -
// - iato:mac library - system interface class definition                    -
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

#ifndef  IATO_SYSTEM_HPP
#define  IATO_SYSTEM_HPP

#ifndef  IATO_HMA_HPP
#include "Hma.hpp"
#endif

#ifndef  IATO_SYSCALL_HPP
#include "Syscall.hpp"
#endif

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {
  using namespace std;

  /// The System class is the memory system interface. The class is
  /// used to bind the system memory as well as other interface that might
  /// needed during the course of the simulation. With a program name,
  /// a program image is created and associated with various memory
  /// unit. The binding between an instruction memory and a program image
  /// is done during the simulation binding process.

  class System : public Runnable {
  private:
    /// the program memory image
    ElfImage* p_elf;
    /// the executable image
    ElfExec*  p_mem;
    /// the system call interface
    Syscall*  p_sci;
    /// the memory model architecture
    Hma*   p_hma;

  public:
    /// create a new system interface by context and program name
    /// @param mtx the architectural context
    /// @param name the program name
    System (Mtx* mtx, const string& name);

    /// create a new system interface by context, program name and arguments
    /// @param mtx the architectural context
    /// @param name the program name
    /// @param argv the program arguments
    System (Mtx* mtx, const string& name, const vector<string>& argv);

    /// destroy this system interface
    ~System (void);

    /// reset this system interface
    void reset (void);

    /// flsuh this system interface
    void flush (void);

    /// run this system interface
    void run (void);

    /// report some system information
    void report (void) const;

    /// @return the system call plugin
    Syscall* getsci (void) const;

    /// @return the memory model architecture
    Hma* gethma (void) const;

    /// @return the entry point
    t_octa getentry (void) const;

    /// @return the top of stack
    t_octa getstkva (void) const;

    /// @return the backing store base
    t_octa getbspva (void) const;

    /// @return the elf image checker
    Checker* getchecker (void) const;

  private:
    // make the copy constructor private
    System (const System&);
    // make the assignment operator private
    System& operator = (const System&);
  };
}

#endif

