// ---------------------------------------------------------------------------
// - Plugin.hpp                                                              -
// - iato:isa library - plugin class definition                              -
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

#ifndef  IATO_PLUGIN_HPP
#define  IATO_PLUGIN_HPP

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Plugin class is a pseudo-abstract class that implements the plugin
  /// interface. A plugin acts as a substitute to the processor, in order
  /// to perform certain task. A typical plugin is a system call handler.
  /// When properly installed, an emulator or a simulator can trap to the 
  /// plugin and perform the required task, before returning to normal mode.
  /// The plugin can operate with any environment resources. It is the 
  /// plugin constructor that is responsible to grab all necessary resources
  /// needed to operate correctly. Since the plugin is a resource, the tracer
  /// can be activated  to record its operation. The plugin can operate in
  /// three mode. The first takes no parameter. The second mode operates
  /// with an instruction. The third mode operates with an interrupt.

  class Plugin : public Resource {
  public:
    /// create a plugin by name
    /// @param name the plugin name
    Plugin (const string& name);

    /// reset this plugin
    void reset (void);

    /// apply this plugin
    virtual void apply (void);

    /// apply this plugin with an interrupt
    /// @param vi the virtul interrupt
    virtual void apply (const Interrupt& vi);
  };
}

#endif
