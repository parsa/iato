// ---------------------------------------------------------------------------
// - t_cdirect.cpp                                                           -
// - iato:isa testers - direct cache class tester                            -
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

#include "CacheDirect.hpp"

namespace iato {
  // cache size in bytes (16Kib)
  const long   CACHE_SIZE = 16 * 1024;
  // cache line size in bytes
  const long   LINE_SIZE  = 32;
  // the test byte
  const t_byte TEST_BYTE  = 0xFF;
}

int main (int, char**) {
  using namespace iato;

  // create a new cache memory
  CacheDirect* cache = new CacheDirect (CACHE_SIZE, LINE_SIZE);
  // set the cache parameters
  cache->setparam (1, Cache::UNIFIED, Cache::WRITETH, Cache::WRALLOC);
  // check invalid line
  for (long i = 0; i < LINE_SIZE; i++) {
    t_octa addr = i;
    assert (cache->isvalid  (addr) == false);
  }
  // create a buffer line that is used to update the cache line
  t_byte* line = new t_byte[LINE_SIZE];
  for (long i = 0; i < LINE_SIZE; i++) line[i] = TEST_BYTE;
  // update the cache at address 0
  cache->update (OCTA_0, line);
  // check byte line
  for (long i = 0; i < LINE_SIZE; i++) {
    t_octa addr = i;
    assert (cache->isvalid  (addr) == true);
    assert (cache->readbyte (addr) == TEST_BYTE);
  }
  // success
  delete [] line;
  delete cache;
  return 0;
}
