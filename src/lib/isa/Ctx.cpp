// ---------------------------------------------------------------------------
// - Ctx.cpp                                                                 -
// - iato:isa library - base context class definition                        -
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

#include "Ctx.hpp"
#include "Isa.hpp"
#include "Utils.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new context

  Ctx::Ctx (void) {
    reset ();
  }

  // destroy this context

  Ctx::~Ctx (void) {
  }

  // reset this context

  void Ctx::reset (void) {
    update (DEF_ARCH);
  }

  // parse a string and update the context

  void Ctx::parse (const string& s) {
    // separate the name/type with value
    vector<string> data = split (s, "=");
    if (data.size () != 2) {
      string msg = "cannot parse ";
      throw Exception ("context-error",  msg + s);
    }
    // separate name and type
    const string qual = data[0];
    vector<string> param = split (qual, ":");
    if (param.size () != 2) {
      string msg = "cannot parse ";
      throw Exception ("context-error",  msg + s);
    }
    // extract elements
    const string name = trim (param[0]);
    const string type = trim (param[1]);
    const string elem = trim (data[1]);
    // dispatch boolean
    if (type == "bool") {
      if (elem == "true") {
	setbool (name, true);
	return;
      }
      if (elem == "false") {
	setbool (name, false);
	return;
      }
      string msg = "cannot parse ";
      throw Exception ("context-error",  msg + s);
    }
    // dispatch long integer
    if (type == "long") {
      long lval = tolong (elem.c_str ());
      setlong (name, lval);
      return;
    }
    // dispatch double
    if (type == "real") {
      double rval = todouble (elem.c_str ());
      setreal (name, rval);
      return;
    }
    string emsg = "cannot parse ";
    throw Exception ("context-error",  emsg + s);
  }

  // update this context with a vector of parameters

  void Ctx::parse (const vector<string>& cprm) {
    long size = cprm.size ();
    for (long  i = 0; i < size; i++) {
      const string& param = cprm[i];
      parse (param);
    }
  }

  // dump the context database into the standard output

  void Ctx::dump (void) const {
    // dump the boolean parameters
    typedef map<string, bool>::const_iterator t_bit;
    for (t_bit bit = d_bmap.begin (); bit != d_bmap.end (); bit++) {
      cout << bit->first << ':' << "bool" << '=';
      if (bit->second == true)  cout << "true";
      if (bit->second == false) cout << "false";
      cout << endl;
    }
    // dump the long parameters
    typedef map<string, long>::const_iterator t_lit;
    for (t_lit lit = d_lmap.begin (); lit != d_lmap.end (); lit++) {
      cout << lit->first << ':' << "long" << '=' << lit->second << endl;
    }
    // dump the real parameters
    typedef map<string, long double>::const_iterator t_rit;
    for (t_rit rit = d_rmap.begin (); rit != d_rmap.end (); rit++) {
      cout << rit->first << ':' << "real" << '=' << rit->second << endl;
    }
   }

  // update this context with a particular architecture

  void Ctx::update (const t_arch arch) {
    // set tracer default parameters
    d_bmap["TRACER-VERBOSE-FLAG"]      = false;
    d_lmap["TRACER-THRESHOLD"]         = TR_THRS;
    d_hmap["TRACER-BEGIN-INDEX"]       = TR_BICC;
    d_hmap["TRACER-END-INDEX"]         = TR_EICC;
    d_lmap["TRACER-MAX-RECORD"]        = TR_RMAX;

    // set checker parameters
    d_bmap["CHECKER-VERBOSE-FLAG"]     = false;

    // set architectural parameters
    d_lmap["ISSUE-WIDTH"]              = BN_IWSZ;
    d_lmap["LR-GR-SIZE"]               = GR_LRSZ;
    d_lmap["LR-FR-SIZE"]               = FR_LRSZ;
    d_lmap["LR-PR-SIZE"]               = PR_LRSZ;
    d_lmap["LR-BR-SIZE"]               = BR_LRSZ;
    d_lmap["LR-AR-SIZE"]               = AR_LRSZ;
    d_lmap["LR-CR-SIZE"]               = CR_LRSZ;

    // set resource parameters
    d_lmap["ALAT-SIZE"]                = AL_SIZE;

    // set archiecture specific parameters
    switch (arch) {
    case MKINLEY:
      // architecture info
      d_smap["ARCHITECTURE-CODE-NAME"] = "McKinley";
      d_smap["ARCHITECTURE-FULL-NAME"] = "Itanium II";
      d_lmap["ARCHITECTURE-REVISION"]  = 2;
      break;
    case ITANIUM:
      // architecture info
      d_smap["ARCHITECTURE-CODE-NAME"] = "Merced";
      d_smap["ARCHITECTURE-FULL-NAME"] = "Itanium I";
      d_lmap["ARCHITECTURE-REVISION"]  = 1;
      break;
    }
  }

  // get a boolean parameter

  bool Ctx::getbool (const string& what) const {
    map<string, bool>::const_iterator it = d_bmap.find (what);
    if (it == d_bmap.end ()) return false;
    return (*it).second;
  }

  // set a boolean parameter

  void Ctx::setbool (const string& what, const bool value) {
    d_bmap[what] = value;
  }

  // set a long parameter

  void Ctx::setlong (const string& what, const long value) {
    d_lmap[what] = value;
  }

  // get a long parameter

  long Ctx::getlong (const string& what) const {
    map<string, long>::const_iterator it = d_lmap.find (what);
    if (it == d_lmap.end ()) return 0;
    return (*it).second;
  }

  // set a long long parameter

  void Ctx::setllong (const string& what, const t_long value) {
    d_hmap[what] = value;
  }

  // get a long long parameter

  t_long Ctx::getllong (const string& what) const {
    map<string, t_long>::const_iterator it = d_hmap.find (what);
    if (it == d_hmap.end ()) return 0;
    return (*it).second;
  }

  // get a real parameter

  long double Ctx::getreal (const string& what) const {
    map<string, long double>::const_iterator it = d_rmap.find (what);
    if (it == d_rmap.end ()) return 0.0L;
    return (*it).second;
  }

  // set a real parameter

  void Ctx::setreal (const string& what, const long double value) {
    d_rmap[what] = value;
  }

  // get a string parameter

  string Ctx::getstr (const string& what) const {
    map<string, string>::const_iterator it = d_smap.find (what);
    if (it == d_smap.end ()) return "";
    return (*it).second;
  }

  // set a string parameter

  void Ctx::setstr (const string& what, const string& value) {
    d_smap[what] = value;
  }

  // return the issue width

  long Ctx::getiwsz (void) const {
    return getlong ("ISSUE-WIDTH");
  }

  // return the instruction window size in slots

  long Ctx::getswsz (void) const {
    return getiwsz () * BN_SLSZ;
  }

  // return the instruction window size in bytes

  long Ctx::getbwsz (void) const {
    return getiwsz () * BN_BYSZ;
  }
}
