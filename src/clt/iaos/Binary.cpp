// ---------------------------------------------------------------------------
// - Binary.cpp                                                              -
// - iato:iaos client - elf binary statistic class implementation            -
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

#include "Binary.hpp"

namespace iato {
  using namespace std;

  // this procedure report some elf header information
  static void report_elf_header (ElfImage* elf) {
    if (!elf) return;
    // compute interpreter name
    string interp = elf->getinterpname ();
    if (interp.length () == 0) interp = "not specified";
    cout << "file name       : " << elf->getname ()  << endl;
    cout << "file type       : " << elf->repr ()     << endl;
    cout << "interpreter     : " << interp           << endl;
    cout << "entry point     : " << hex << "0x" << elf->getentry () << dec;
    cout << endl;
    cout << "program headers : " << elf->getphnum () << endl;
    cout << "section headers : " << elf->getshnum () << endl;
  }

  // this procedure reports some executable contents
  static void report_exec_content (ElfImage* elf) {
    // check for an executable
    if (elf->isexec () == false) return;
    ElfLoad* eld = elf->getload ();
    long ldnum = eld->length ();
    cout << "load headers    : " << ldnum << endl;
    cout << endl;
    for (long i = 0; i < ldnum; i++) { 
      // get a segment image
      Segment* seg = eld->getseg (i);
      t_octa size = seg->getsize ();
      t_octa base = seg->getbase ();
      t_octa send = base + size;
      cout << "segment index   : " << i << endl;
      cout << "segment size    : " << size << endl;
      cout << "segment base    : " << hex << "0x" << base << dec << endl;
      cout << "segment end     : " << hex << "0x" << send << dec << endl;
      cout << endl;    
    }
  }

  // this procedure reports some text section contents
  static void report_text_content (ElfImage* elf) {
    ElfText* etx = elf->gettxt ();
    long stnum = etx->length ();
    cout << "sections        : " << stnum << endl;
    cout << endl;
    for (long i = 0; i < stnum; i++) { 
      // get a section image
      ElfSection* scn = etx->getscn (i);
      cout << "section index   : " << i << endl;
      cout << "section name    : " << scn->getname () << endl;
      cout << "section size    : " << scn->getsize () << endl;
      cout << "section base    : " << hex << "0x" << scn->getbase () << dec;
      cout << endl << endl;    
    }
  }

  // create a new elf stat with an image
  
  Binary::Binary (ElfImage* elf) {
    p_elf  = elf;
    d_ehdr = false;
    d_eexc = false;
    d_etxt = false;
  }

  // set the report flag

  void Binary::setflag (const bool ehdr, const bool eexc, const bool etxt) {
    d_ehdr = ehdr;
    d_eexc = eexc;
    d_etxt = etxt;
  }

  // report this class
  
  void Binary::report (void) const {
    if (d_ehdr == true) report_elf_header   (p_elf);
    if (d_eexc == true) report_exec_content (p_elf);
    if (d_etxt == true) report_text_content (p_elf);
  }
}
