// ---------------------------------------------------------------------------
// - Mapper.cpp                                                              -
// - iato:mac library - memory mapper class implementation                   -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#include "Urn.hpp"
#include "Ecu.hpp"
#include "Mapper.hpp"
#include "Interrupt.hpp"

namespace iato {

  // create a default mapper

  Mapper::Mapper (void) : Resource (RESOURCE_MMP) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // create a mapper with a context
  
  Mapper::Mapper (Mtx* mtx) : Resource (RESOURCE_MMP) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // create a mapper with a context by name
  
  Mapper::Mapper (Mtx* mtx, const string& name) : Resource (name) {
    p_dmem = 0; 
    p_fmem = 0; 
    p_bmem = 0; 
  }

  // reset this mapper
  
  void Mapper::reset (void) {
  }

  // bind a memory to this mapper

  void Mapper::bind (Memory* mem) {
    p_dmem = mem;
    p_fmem = mem;
    p_bmem = mem;
  }

  // bind a memory to this mapper by type

  void Mapper::bind (t_tmem type, Memory* mem) {
    switch (type) {
    case DMEM:
      p_dmem = mem;
      break;
    case FMEM:
      p_fmem = mem;
      break;
    case BMEM:
      p_bmem = mem;
      break;
    }
  }

  // process a memory request

  void Mapper::process (Mrt& mrt) {
    // check for valid request
    if (mrt.isvalid () == false) return;
    // dispatch request
    switch (mrt.gettype ()) {
    case Mrt::REQ_BND:
      assert (false);
      break;
    case Mrt::REQ_LD1:
      mrt.setbval (p_dmem->readbyte (mrt.getaddr ()));
      break;
    case Mrt::REQ_LD2:
      mrt.setwval (p_dmem->readword (mrt.getaddr ()));
      break;
    case Mrt::REQ_LD4:
      mrt.setqval (p_dmem->readquad (mrt.getaddr ()));
      break;
    case Mrt::REQ_LD8:
      mrt.setoval (p_dmem->readocta (mrt.getaddr ()));
      break;
    case Mrt::REQ_LDS:
      mrt.setlval (p_fmem->readsing (mrt.getaddr ()));
      break;
    case Mrt::REQ_LDD:
      mrt.setlval (p_fmem->readdoub (mrt.getaddr ()));
      break;
    case Mrt::REQ_LDE:
      mrt.setlval (p_fmem->readxten (mrt.getaddr ()));
      break;
    case Mrt::REQ_LDI:
      mrt.setlval (p_fmem->readint (mrt.getaddr ()));
      break;
    case Mrt::REQ_LDF:
      mrt.setlval (p_fmem->readfill (mrt.getaddr ()));
      break;
    case Mrt::REQ_LPS:
      mrt.setlval (p_fmem->readsing (mrt.getaddr ()));
      mrt.sethval (p_fmem->readsing (mrt.getaddr () + 4));
      break;
    case Mrt::REQ_LPD:
      mrt.setlval (p_fmem->readdoub (mrt.getaddr ()));
      mrt.sethval (p_fmem->readdoub (mrt.getaddr () + 8));
      break;
    case Mrt::REQ_LPI:
      mrt.setlval (p_fmem->readint (mrt.getaddr ()));
      mrt.sethval (p_fmem->readint (mrt.getaddr () + 8));
      break;
    case Mrt::REQ_ST1:
      p_dmem->writebyte (mrt.getaddr (), mrt.getbval ());
      break;
    case Mrt::REQ_ST2:
      p_dmem->writeword (mrt.getaddr (), mrt.getwval ());
      break;
    case Mrt::REQ_ST4:
      p_dmem->writequad (mrt.getaddr (), mrt.getqval ());
      break;
    case Mrt::REQ_ST8:
      p_dmem->writeocta (mrt.getaddr (), mrt.getoval ());
      break;
    case Mrt::REQ_STS:
      p_fmem->writesing (mrt.getaddr (), mrt.getlval ());
      break;
    case Mrt::REQ_STD:
      p_fmem->writedoub (mrt.getaddr (), mrt.getlval ());
      break;
    case Mrt::REQ_STI:
      p_fmem->writeint  (mrt.getaddr (), mrt.getlval ());
      break;
    case Mrt::REQ_STE:
      p_fmem->writexten (mrt.getaddr (), mrt.getlval ());
      break;
    case Mrt::REQ_STF:
      p_fmem->writespill (mrt.getaddr (), mrt.getlval ());
      break;
    default:
      break;
    }
  }

  // update a result with a memory request

  void Mapper::update (Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // get associated mrt
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // process the mrt
    process (mrt);
    // update the result
    resl.update (mrt);
  } 
}
