// ---------------------------------------------------------------------------
// - Stb.cpp                                                                 -
// - iato:mac library - store buffer class implementation                    -
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

#include "Prn.hpp"
#include "Mac.hpp"
#include "Stb.hpp"

namespace iato {

  // store buffer element
  struct t_stbe {
    // the valid bit
    bool d_valid;
    // the store request
    t_mreq d_type;
    // the store address
    t_octa d_addr;
    // the data value
    t_octa d_oval;
    // the real value
    t_real d_rval;
    // initialize a store buffer element
    t_stbe (void) {
      reset ();
    }
    // reset a store buffer element
    void reset (void) {
      d_valid = false;
      d_addr  = OCTA_0;
      d_type  = REQ_NUL;
      d_oval  = OCTA_0;
      d_rval  = 0.0L;
    }
    // allocate a new entry
    void alloc (t_mreq type, const t_octa addr, const t_octa data) {
      assert (d_valid == false);
      d_valid = true;
      d_type  = type;
      d_addr  = addr;
      d_oval  = data;
    }
    // allocate a new real entry
    void alloc (t_mreq type, const t_octa addr, const t_real& data) {
      assert (d_valid == false);
      d_valid = true;
      d_type  = type;
      d_addr  = addr;
      d_rval  = data;
    }
  };

  // create a new stb
  
  Stb::Stb (void) : Resource (RESOURCE_STB) {
    d_size = SB_SIZE;
    p_stbe = new t_stbe[d_size];
    p_bmem = 0;
    reset ();
  }

  // create a new stb with a context
  
  Stb::Stb (Mtx* mtx) : Resource (RESOURCE_STB) {
    d_size = mtx->getlong ("STB-SIZE"); assert (d_size > 0);
    p_stbe = new t_stbe[d_size];
    p_bmem = 0;
    reset ();
  }

  // destroy this stb

  Stb::~Stb (void) {
    delete [] p_stbe;
  }

  // reset this stb

  void Stb::reset (void) {
    d_iptr = 0;
    d_optr = 0;
    for (long i = 0; i < d_size; i++) p_stbe[i].reset ();
  }

  // report this resource

  void Stb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "store buffer" << endl;
    cout << "\tbuffer size  \t\t: " << d_size << endl;
  }

  // bind the bypass memory to this  store buffer

  void Stb::bind (Memory* mem) {
    p_bmem = mem;
  }

  // return true if the stb is empty

  bool Stb::isempty (void) const {
    for (long i = 0; i < d_size; i++) {
      if (p_stbe[i].d_valid == true) return false;
    }
    return true;
  }

  // push a request in the store buffer

  void Stb::push (t_mreq type, const t_octa addr, const t_octa data) {
    // check for bypass
    if (p_bmem) {
      switch (type) {
      case REQ_ST1:
	p_bmem->writebyte (addr, data);
	break;
      case REQ_ST2:
	p_bmem->writeword (addr, data);
	break;
      case REQ_ST4:
	p_bmem->writequad (addr, data);
	break;
      case REQ_ST8:
	p_bmem->writeocta (addr, data);
	break;
      default:
	assert (false);
	break;
      }
    } else {
      // allocate a new entry
      p_stbe[d_iptr].alloc (type, addr, data);
      // update index
      d_iptr = ++d_iptr % d_size;
    }
  }

  // push a request in the store buffer

  void Stb::push (t_mreq type, const t_octa addr, const t_real& data) {
    // check for bypass
    if (p_bmem) {
      switch (type) {
      case REQ_STS:
	p_bmem->writesing(addr, data);
	break;
      case REQ_STD:
	p_bmem->writedoub (addr, data);
	break;
      case REQ_STE:
	p_bmem->writexten (addr, data);
	break;
      case REQ_STF:
	p_bmem->writespill (addr, data);
	break;
      case REQ_STI:
	p_bmem->writeint (addr, data);
	break;
      default:
	assert (false);
	break;
      }
    } else {
      // allocate a new entry
      p_stbe[d_iptr].alloc (type, addr, data);
      // update index
      d_iptr = ++d_iptr % d_size;
    }
  }
}
