// ---------------------------------------------------------------------------
// - Utils.cpp                                                               -
// - iato:isa library - utility function implementation                      -
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

#include "Utils.hpp"
#include "Exception.hpp"
#include <cstring>
#include <unistd.h>

namespace iato {
  using namespace std;

  // convert a string to an integer

  long tolong (const char* s) {
    // check for pointer
    if (!s) {
      throw Exception ("convert-error", "connot convert null pointer to long");
    }
    char* ep = 0;
    long result = strtol (s, &ep, 0);
    if (ep[0] != '\0') {
      string msg = "connot convert to long ";
      throw Exception ("convert-error", msg + s);
    }
    return result;
  }

  // convert a string to a double

  double todouble (const char* s) {
    // check for pointer
    if (!s) {
      throw Exception ("convert-error", "null pointer conversion");
    }
    char* ep = 0;
    double result = strtod (s, &ep);
    if (ep[0] != '\0') {
      string msg = "connot convert to double ";
      throw Exception ("convert-error", msg + s);
    }
    return result;
  }

  // copy one string into another

  void cpystr (t_byte* d, const t_byte* s) {
    if (!d) return;
    if (!s) *d = '\0';
    strcpy ((char*) d, (const char*) s);
  }

  // left trim a string

  string ltrim (const string& s) {
    string str = s;
    return str.erase (0, s.find_first_not_of (" \t"));
  }

  // right trim a string

  string rtrim (const string& s) {
    string str = s;
    return str.erase (str.find_last_not_of (" \t") + 1);
  }

  // trim a string

  string trim (const string& s) {
    return ltrim (rtrim (s));
  }

  // fill right a string with blank characters

  string rfill (const string& s, const long size) {
    // build result string and check
    string result = s;
    long len = result.length ();
    if (len >= size) return result;
    // add remaining blank
    long delta = size - len;
    for (long i = 0; i < delta; i++) result += ' ';
    return result;
  }

  // split a string

  vector<string> split (const string& str, const string& delimiters) {
    vector<string> result;
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);    
    while (string::npos != pos || string::npos != lastPos)
      {
        // Found a string, add it to the vector.
        result.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
      }
    return result;
  }

  // get the significand bit position

  long getmsbi (const t_octa val) {
    // check position 0
    if ((val & 0x0000000000000001ULL) != OCTA_0) return 1;
    // loop for upper bits
    for (long i = 1; i < 64; i++) {
      t_octa data = (val >> i) & 0x0000000000000001ULL;
      if (data != OCTA_0) return i;
    }
    // not one bit found
    return 0;
  }

  // get a mask by size

  t_octa getsmsk (const long size) {
    // get the most significand bit
    long msbi = getmsbi (size);
    // compute mask
    t_octa result = OCTA_A;
    for (long i = 0; i < msbi; i++) result = result << 1;
    return result;
  }

  // get a mask by bit size

  t_octa getbmsk (const long size) {
    // compute mask
    long len = size % (8 * sizeof (t_octa));
    t_octa result = OCTA_A << len;
    return result;
  }

  // read 'n' bytes from a descpriptor

  t_size readn (int fd, void* buf, const t_size n) {
    t_size nleft = n;
    t_size nread = 0;
    t_byte*  ptr = (t_byte*) buf;
    // loop until all data are read or reach the eof
    while (nleft > 0) {
      if ((nread = read (fd, ptr, nleft)) < 0)
	// error in blocking mode
	return nread;
      else if (nread == 0) 
	// eof reached
	break;
      nleft -= nread;
      ptr   += nread;
    }
    // return the number read even if eof reached
    return (n - nleft);
  }
}
