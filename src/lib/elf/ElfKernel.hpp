// ---------------------------------------------------------------------------
// - ElfKernel.hpp                                                           -
// - iato:elf library - elf kernel parameters class definitionn              -
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

#ifndef  IATO_ELFKERNEL_HPP
#define  IATO_ELFKERNEL_HPP

#ifndef  IATO_ETX_HPP
#include "Etx.hpp"
#endif

namespace iato {

  /// The ElfKernel class is a parameters class that is used to represent
  /// the process image according to some kernel parameters. Such parameters
  /// include the page size, process stack definition as well as backing
  /// store area information. The class is used to construct other object
  /// like the stack or the bsa.

  class ElfKernel {
  private:
    /// the endian mode
    bool d_mode;
    /// the page shift
    long   d_pshft;
    /// the page size
    t_octa d_psize;
    /// the mappable base address
    t_octa d_mbase;
    /// the stack top address
    t_octa d_stkva;
    /// the stack size
    long d_stksz;
    /// the bsa base address
    t_octa d_bsava;
    /// the bsa size
    long d_bsasz; 
    /// the clock times ticks
    long d_clktk;

  public:
    /// create a default elf kernel parameters
    ElfKernel (void);

    /// create a kernel parameters by context
    /// @param etx the elf context
    ElfKernel (Etx* etx);

    /// set the endian mode
    /// @param mode the mode to set
    void setmode (const bool mode);

    /// @return the endian mode
    bool getmode (void) const;

    /// @return the mappable base address
    t_octa getmapb (void) const;

    /// @return the page size
    t_octa getpgsz (void) const;

    /// @return the top stack address
    t_octa getstkva (void) const;

    /// @return the stack size
    long getstksz (void) const;

    /// @return the bsa base address
    t_octa getbsava (void) const;

    /// @return the bsa size
    long getbsasz (void) const;

    /// @return the times clock ticks
    long getclktk (void) const;

  private:
    // make the copy constructor private
    ElfKernel (const ElfKernel&);
    // make the assignment operator private
    ElfKernel& operator = (const ElfKernel&);
    // recompute all parameters
    void initialize (void);
  };
}

#endif
