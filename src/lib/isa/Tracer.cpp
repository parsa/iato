// ---------------------------------------------------------------------------
// - Tracer.cpp                                                              -
// - iato:isa library - cycle based record tracing class implementation      -
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

#include "Lrn.hpp"
#include "Utils.hpp"
#include "Tracer.hpp"
#include "Tracer.hxx"
#include "Exception.hpp"

namespace iato {

  // -------------------------------------------------------------------------
  // - tracer header section                                                 -
  // -------------------------------------------------------------------------

  // the magic header number of character
  const long   MAGIC_SIZE = 5;
  // the programm traced name size
  const long   HEADER_NAME_SIZE = 128;
  // the magic value
  const t_byte MAGIC[MAGIC_SIZE] = {'\177', 'I', 'A', 'O', 'O'};
  // the header major number
  const t_byte HEADER_MAJOR = RMAJOR;
  // the header minor number
  const t_byte HEADER_MINOR = RMINOR;
      
  // the tracer header structure
  struct t_theader {
    // the magic number
    t_byte d_magic[MAGIC_SIZE];
    // the major number
    t_byte d_major;
    // the minor number
    t_byte d_minor;
    // reserved
    t_byte d_resvd;
    // the start trace cycle value
    t_long d_bicc;
    // the end trace cycle value
    t_long d_eicc;
    // the program traced name
    t_byte d_ptnm[HEADER_NAME_SIZE];
    // the default constructor
    t_theader (void) {
      for (long i = 0; i < MAGIC_SIZE; i++) d_magic[i] = MAGIC[i];
      d_major = HEADER_MAJOR;
      d_minor = HEADER_MINOR;
      d_resvd = BYTE_0;
      d_bicc  = TR_BICC;
      d_eicc  = TR_EICC;
      for (long i = 0; i < HEADER_NAME_SIZE; i++) d_ptnm[i] = BYTE_0;
    }
    // set the header name
    void setname (const string& name) {
      long size = name.size ();
      if (size >= HEADER_NAME_SIZE) size = HEADER_NAME_SIZE-1;
      for (long i = 0; i < size; i++) d_ptnm[i] = name[i];
    }
  };

  // -------------------------------------------------------------------------
  // - tracer section                                                        -
  // -------------------------------------------------------------------------

  // this procecure check a record source name against the valid name
  // if the vector is empty, the name is valid
  static bool is_valid_name (const Record& rcd, const vector<string>& rnam) {
    long size = rnam.size ();
    if (size  == 0) return true;
    string rsrc = rcd.getname ();
    for (long i = 0; i < size; i++) {
      if (rnam[i] == rsrc) return true;
    }
    return false;
  }

  // this procecedure cleans a trace vector
  static void delete_trace_vector (Tracer::t_trcd* vtrc) {
    if (!vtrc) return;
    long size = vtrc->size ();
    for (long i = 0; i < size; i++) delete (*vtrc)[i];
    delete vtrc;
  }

  // this procedure writes a trace header
  static void write_trace_header (int fd, const t_theader& hd) {
    if (fd < 0) return;
    assert (write (fd, &hd, sizeof (hd)) == sizeof (hd));
  }

  // this procedure write a record vector
  static void write_trace_record (int fd, Tracer::t_vrcd* vrcd) {
    // check for arguments
    if ((fd == -1) || (!vrcd)) return;
    // get the vector size
    t_long size = vrcd->size ();
    t_octa data = size;
    if (write (fd, &data , 8) != 8)
      throw Exception ("tracer-error", "cannot write trace record");
    // write each record
    for (t_long i = 0; i < size; i++) {
      const Record& rcd = (*vrcd)[i];
      rcd.rcdwr (fd);
    }
  }

  // this procedure write a trace vector
  static void write_trace_vector (int fd, Tracer::t_trcd* vtrc) {
    // check for arguments
    if ((fd == -1) || (!vtrc)) {
      delete_trace_vector (vtrc);
      return;
    }
    // loop for traces
    long size = vtrc->size ();
    for (long i = 0; i < size; i++) {
      Tracer::t_vrcd* vrcd = (*vtrc)[i];
      write_trace_record (fd, vrcd);
    }
    delete_trace_vector (vtrc);
  }

  // create a default tracer

  Tracer::Tracer (void) {
    d_vflg = false;
    d_bicc = TR_BICC;
    d_eicc = TR_EICC;
    d_rmax = TR_RMAX;
    d_ptnm = "";
    d_tcnt = 0;
    p_ctrc = 0;
    p_vtrc = new t_trcd;
    d_tthr = TR_THRS;
    d_fd   = -1;
    for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = true;
    d_rnam.clear ();
  }

  // create a new tracer with a verbose flag

  Tracer::Tracer (const bool vflg) {
    d_vflg = vflg;
    d_bicc = TR_BICC;
    d_eicc = TR_EICC;
    d_rmax = TR_RMAX;
    d_ptnm = "";
    d_tcnt = 0;
    p_ctrc = 0;
    p_vtrc = new t_trcd;
    d_tthr = TR_THRS;
    d_fd   = -1;
    for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = true;
    d_rnam.clear ();
  }
  
  // create a tracer with a context

  Tracer::Tracer (Ctx* ctx) {
    d_vflg = ctx->getbool  ("TRACER-VERBOSE-FLAG");
    d_bicc = ctx->getllong ("TRACER-BEGIN-INDEX");
    d_eicc = ctx->getllong ("TRACER-END-INDEX");
    d_rmax = ctx->getlong  ("TRACER-MAX-RECORD");
    d_ptnm = ctx->getstr   ("TRACER-PROGRAM-NAME"),
    d_tcnt = 0;
    p_ctrc = 0;
    p_vtrc = new t_trcd;
    d_tthr = ctx->getlong ("TRACER-THRESHOLD");
    d_rnam.clear ();
    // reset file name
    d_fd   = -1;
    setname (ctx->getstr ("TRACER-FILE-NAME"));
    // mark the record type we trace
    string rcd = ctx->getstr ("TRACER-RECORD-TYPE");
    // if the string is empty, all records are traced
    // else selective tracing is done
    if (rcd.length () == 0) {
      for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = true;
    } else {
      for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = false;
      vector<string> vec = split (rcd, ",");
      long size = vec.size();
      for (long i = 0; i < size; i++) {
	d_rctp[Record::totype (vec[i])] = true;
      }
    }
    // initialize record name
    string rnam = ctx->getstr ("TRACER-RECORD-NAME");
    d_rnam = split (rnam, ",");
  }

  // destroy this tracer

  Tracer::~Tracer (void) {
    // save the trace and write it
    if (p_ctrc) addtrc ();
    trcwr ();
    // delete the trace and close
    delete_trace_vector (p_vtrc);
    if (d_fd != -1) close (d_fd);
  }

  // reset this tracer

  void Tracer::reset (void) {
    // eventually save the trace and write it
    if (p_ctrc && (d_fd != -1)) {
      p_vtrc->push_back (p_ctrc);
      p_ctrc = 0;
      trcwr ();
    }
    // loop in the trace vector to reset
    long size = p_vtrc->size ();
    for (long i = 0; i < size; i++) delete (*p_vtrc)[i];
    p_vtrc->clear ();
    delete p_ctrc;
    p_ctrc = 0;
    // reset counter
    d_tcnt = 0;
  }

  // set the trace file and open it

  void Tracer::setname (const string& name) {
    // close the old file
    if ((d_name.empty () == false) || (d_fd != -1)) {
      close (d_fd);
      d_name = "";
    }
    // check for empty file name
    if (name.empty () == true) {
      d_name = "";
      d_fd = -1;
      return;
    }
    // open the new one
    d_fd = open (name.c_str (), TRACER_OPEN_MODE, TRACER_FILE_MODE);
    if (d_fd == -1) {
      string msg = "cannot open file ";
      throw Exception ("tracer-error", msg + name);
    }
    d_name = name;
    // initialize the trace header
    t_theader hd;
    hd.d_bicc = d_bicc;
    hd.d_eicc = d_eicc;
    hd.setname (d_ptnm);
    // write the trace header
    write_trace_header (d_fd, hd);
  }

  // add a record type to the trace type vector

  void Tracer::addtype (Record::t_rctp val) {
    d_rctp[val] = true;
  }
  
  // start a new trace

  void Tracer::newtrace (void) {
    // save the old trace
    if (p_ctrc) addtrc ();
    p_ctrc = new t_vrcd;  
    d_tcnt++;
    // check if we have reach the threshold to write the trace vector
    if (p_vtrc->size () >= (unsigned long) d_tthr) trcwr ();
  }

  // add several new traces

  void Tracer::newtraces (const long count) {
    for (long i = 0; i < count; i++) {
      newtrace ();
      print    ();
    }
  }

  // add a record to the current cycle
  
  void Tracer::add (const Record& rcd) {
    // check for cycle range
    if (inrange () == false) return;
    // check for valid record
    if (rcd.isignore () == true) return;
    if (d_rctp[rcd.gettype ()] == false) return;
    if (is_valid_name (rcd.getname (), d_rnam) == false) return;
    // check for current trace
    if (!p_ctrc) newtrace ();
    // check if the max record is reached
    if ((d_rmax != 0) && (p_ctrc->size () >= (unsigned long) d_rmax)) 
      newtrace ();
    // push back record
    p_ctrc->push_back (rcd);
  }

  // add a record vector to the current cycle

  void Tracer::add (t_vrcd* vrcd) {
    // save the old trace if any
    if (p_ctrc) addtrc ();
    p_ctrc = vrcd;
    vrcd = 0;
    d_tcnt++;
    addtrc ();
    // check if we have reach the threshold to write the trace vector
    if (p_vtrc->size () >= (unsigned long) d_tthr) trcwr ();
  }

  // print the current record vector

  void Tracer::print (void) const {
    // check for cycle range
    if (inrange () == false) return;
    if ((!p_ctrc) || (d_vflg == false)) return;
    cout << "trace " << d_tcnt << " {"   << endl;
    long size = p_ctrc->size ();
    for (long i = 0; i < size; i++) {
      cout << " " << (*p_ctrc)[i].repr () << endl;
    }
    cout << '}' << endl;
  }

  // write the trace vector if any file is open

  void Tracer::trcwr (void) {
    // grab the old trace and create a new one
    t_trcd* vtrc = p_vtrc;
    p_vtrc = new t_trcd;
    // write the trace
    write_trace_vector (d_fd, vtrc);
  }

  // add a trace to the trace vector if needed

  void Tracer::addtrc (void) {
    // if no file descriptor or out of range delete vector
    if ((d_fd == -1) || (inrange () == false)) {
      delete p_ctrc;
      p_ctrc = 0;
      return;
    }
    p_vtrc->push_back (p_ctrc);
    p_ctrc = 0;
  }

  // return true if the cycle count is in the desired range

  bool Tracer::inrange (void) const {
    if ((d_bicc != TR_BICC) && (d_tcnt < d_bicc)) return false;
    if ((d_eicc != TR_EICC) && (d_tcnt > d_eicc)) return false;
    return true;
  }

  // -------------------------------------------------------------------------
  // - tracer reader section                                                 -
  // -------------------------------------------------------------------------

  // this procedure reads and checks the trace header
  static bool check_trace_header (t_theader* hd, int fd) {
    // try to read the header
    if (fd < 0) return false;
    if (readn (fd, hd, sizeof (*hd)) != sizeof (*hd)) return false;
    // check for valid magic number
    for (long i = 0; i < MAGIC_SIZE; i++) {
      if (hd->d_magic[i] != MAGIC[i]) return false;
    }
    return true;
  }

  // create a new tracer reader by name

  Tracer::Reader::Reader (const string& name) {
    // try to open the file
    if (name.empty () == true) {
      d_fd = dup (STDIN_FILENO);
    } else {
      d_fd = open (name.c_str (), READER_OPEN_MODE);
    }
    if (d_fd == -1) {
      string msg = "cannot open trace file ";
      throw Exception ("tracer-error", msg + name);
    }
    // check the header
    p_hd = new t_theader ();
    if (check_trace_header (p_hd, d_fd) == false) {
      string msg = "invalid trace header from file ";
      throw Exception ("tracer-error", msg + name);
    }
    // save trace name
    d_name = name;
  }

  // destroy this trace reader

  Tracer::Reader::~Reader (void) {
    if (d_fd != -1) close (d_fd);
    delete p_hd;
 }

  // read a trace vector

  Tracer::t_vrcd* Tracer::Reader::trcrd (void) {
    // get the size
    t_octa data = OCTA_0;
    int status = readn (d_fd, &data, 8);
    if (status == 0) return 0;
    if (status != 8) {
      string msg = "cannot read trace size from file ";
      throw Exception ("tracer-error", msg + d_name);
    }
    t_long size = data;
    // create a new vector
    t_vrcd* vrcd = new t_vrcd;
    // now read the record
    Record rcd;
    for (t_long i = 0; i < size; i++) {
      rcd.rcdrd (d_fd);
      vrcd->push_back (rcd);
    }
    // here we are
    return vrcd;
  }

  // get the next record

  Record* Tracer::Reader::getrcd (void) {
    // check if the queue is not empty
    if (d_rqueue.empty () == false) {
      Record* rcd = new Record (d_rqueue.front ());
      d_rqueue.pop ();
      return rcd;
    }
    // here the queue is empty, try to get a new trace
    Tracer::t_vrcd* vrcd = trcrd ();
    if (!vrcd) return 0;
    // fill in the queue with the trace
    long size = vrcd->size ();
    for (long i=1; i < size; i++) {
      d_rqueue.push ((*vrcd)[i]);
    }
    // get the next record
    Record* rcd = new Record ((*vrcd)[0]);
    // clean the record vector and get first available record
    delete vrcd;
    return rcd;
  }

  // print header info

  void Tracer::Reader::hdinfo (void) {
    // the file descriptor should be ready
    assert (d_fd != -1);
    // header version
    cout << "trace version                  : ";
    cout << (long) p_hd->d_major << "." << (long) p_hd->d_minor << endl;
    // traced program name
    cout << "program name                   : ";
    if (p_hd->d_ptnm[0] == BYTE_0)
      cout << "unknown name" << endl;
    else 
      cout << p_hd->d_ptnm << endl;
    // cycle start index
    if (p_hd->d_bicc == TR_BICC) 
      cout << "cycle start                    : begining of program " << endl;
    else
      cout << "cycle start                    : " << p_hd->d_bicc << endl;;
    // cycle end index
    if (p_hd->d_eicc == TR_EICC) 
      cout << "cycle end                      : end of program " << endl;
    else
      cout << "cycle end                      : " << p_hd->d_eicc << endl;
  }

  // get the begin trace cycle index

  t_long Tracer::Reader::getbicc (void) const {
    return p_hd->d_bicc;
  }  

  // get the end trace cycle index

  t_long Tracer::Reader::geteicc (void) const {
    return p_hd->d_eicc;
  }


}
