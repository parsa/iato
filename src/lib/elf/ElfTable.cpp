// ---------------------------------------------------------------------------
// - ElfTable.cpp                                                            -
// - iato:elf library - elf character table class implementation             -
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

#include "Abi.hpp"
#include "ElfTable.hpp"

namespace iato {
  using namespace std;

  // this procedure return the argument block padding
  static long get_argv_pads (const long size) {
    long pad = ABI_ADDR_ALIGN - (size % ABI_ADDR_ALIGN);
    if (pad == ABI_ADDR_ALIGN) pad = 0;
    return pad;
  }

  // this procedure compute the string vector total size
  static long get_argv_size (const vector<string>& argv) {
    long argc = argv.size ();
    long args = 0;
    for (long  i = 0; i < argc; i++) {
      string str = argv[i];
      args += str.length () + 1;
    }
    return args;
  }
  
  // this procedure creates the argument block array from a string vector
  // the block array is padded with null character.
  static t_byte* get_argv_blok (const vector<string>& argv) {
    // compute the total block size - that is the vector total size
    // plus the padding
    long args = get_argv_size (argv);
    long pads = get_argv_pads (args);
    long size = args + pads;
    // allocate the block array with the total size and nullify
    t_byte* argb = new t_byte[size];
    for (long i = 0; i < size; i++) argb[i] = '\0';
    // fill in the block with the character strings
    long argc = argv.size ();
    long indx = 0;
    for (long i = 0; i < argc; i++) {
      string str = argv[i];
      long   len = str.length ();
      for (long j = 0; j < len; j++) argb[indx++] = str[j];
      argb[indx++] = '\0';
    }
    return argb;
  }

  // create a empty character table

  ElfTable::ElfTable (void) {
    reset ();
  }

  // destroy this character table

  ElfTable::~ElfTable (void) {
  }

  // reset this character table

  void ElfTable::reset (void) {
    d_svec.clear ();
  }

  // add a new entry to the character table

  void ElfTable::add (const string& s) {
    d_svec.push_back (s);
  }

  // return the number of arguments

  long ElfTable::getargc (void) const {
    return d_svec.size ();
  }

  // return the argument block size

  long ElfTable::getargs (void) const {
    long args = get_argv_size (d_svec);
    long pads = get_argv_pads (args);
    return args + pads;
  }

  // return the argument block

  t_byte* ElfTable::getargb (void) const {
    return get_argv_blok (d_svec);
  }

  // relocate a block at a certain address

  t_octa* ElfTable::getargv (const t_octa addr) const {
    long    argc = d_svec.size ();
    if (argc == 0) return 0;
    t_octa* argv = new t_octa[argc + 1];
    t_octa  disp = addr;
    for (long i = 0; i < argc; i++) {
      // set index to displacement
      argv[i] = disp;
      // compute new displacement
      string str = d_svec[i];
      disp += str.length () + 1;
    }
    // set null pointer
    argv[argc] = OCTA_0;
    return argv;
  }
}
