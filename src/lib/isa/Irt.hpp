// ---------------------------------------------------------------------------
// - Irt.hpp                                                                 -
// - iato:isa library - interrupt routing table class definition             -
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

#ifndef  IATO_IRT_HPP
#define  IATO_IRT_HPP

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

#ifndef  IATO_PLUGIN_HPP
#include "Plugin.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  
  /// The Irt class is the interrupt routing table. The IRT is not part of
  /// the IA64 ISA but is defined here as a convenient way to manage 
  /// interrupt in the context of emaultation or simulation. The key function
  /// of the IRT is to decide whether a particular interrupt will be serviced
  /// by the processor or by a plugin. In the case of an emulation, it is
  /// somtimes desirable to emulate some portion of the processor activity
  /// without reproducing all internal operation. For example, a system call
  /// that is the result of an intruction break, might be serviced directly
  /// instead or recomputing the whole processor activity. This is where IRT
  /// comes into play. For each interrupt vector, a configuration code
  /// indicates what operation needs to be done. The interrupt might be
  /// ignored, rethrown (like an exception), processed (using the processor 
  /// ISA) or emulated (using a plugin). When the vector is set to operate
  /// in plugin mode, control is transfered to the appropriate plugin that
  /// will handle the interrupt. When the control is set in processed mode,
  /// normal processor operation occurs as defined by the ISA.
  
  class Irt : public Resource {
  public:
    /// the irt mode
    enum t_imode {
      THROWN, // the interrupt is rethrown
      IGNORE, // the interrupt is ignored
      PLUGIN  // transfer to the plugin
    };

  private:
    /// the ir table
    struct t_irt* p_irt;

  public:
    /// create a default irt
    Irt (void);

    /// create an irt with a context
    /// @param ctx the current context
    Irt (Ctx* ctx);

    /// create an irt with a context and name
    /// @param ctx the current context
    /// @param name the resource name
    Irt (Ctx* ctx, const string& name);

    /// destroy this irt
    ~Irt (void);

    /// reset this irt
    void reset (void);

    /// report this irt
    void report (void) const;

    /// clear this irt
    void clear (void);

    /// set the irt mode by interrupt code
    /// @param code the interrupt code
    /// @param mode the irt mode
    void setmode (const t_icode code, const t_imode mode);

    /// @return the irt mode by interrupt code
    t_imode getmode (const t_icode icode) const;

    /// bind a plugin by interrupt code
    /// @param code the interrupt code
    /// @param plug the plugin to bind
    void bind (const t_icode code, Plugin* plug);

    /// @return the irt plugin pointer
    Plugin* getplug (const t_icode code) const;

    /// route an interrupt and return a boolean status
    /// @param vi the virtual interrupt
    void route (const Interrupt& vi) const;

  private:
    // make the copy constructor private
    Irt (const Irt&);
    // make the assignment operator private
    Irt& operator = (const Irt&);
  };
}

#endif
