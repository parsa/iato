// ---------------------------------------------------------------------------
// - ElfImage.cpp                                                            -
// - iato:elf library - elf image class implementation                       -
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
#include "Elf.hxx"
#include "ElfImage.hpp"
#include "Exception.hpp"
#include "ElfSegment.hpp"
#include "ElfChecker.hpp"

namespace iato {

  // this function find an elf interpreter by name
  static ElfInterp* find_interp (const string& interp) {
    if (ElfInterp::isvalid (interp) == true) return new ElfInterp (interp);
    return NULL;
  }

  // this function returns an elf section by name
  static Elf_Scn* get_section (const string& sect, void* eptr, void* hptr) {
    if ((eptr == NULL) || (hptr == NULL)) return NULL;
    // remap to elf data
    Elf*         elf = reinterpret_cast <Elf*> (eptr);
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (hptr);
    // get the shared index section
    Elf_Scn*    scn  = elf_getscn  (elf, ehdr->e_shstrndx);
    Elf_Data*   data = elf_getdata (scn, NULL);
    if ((scn == NULL) || (ehdr == NULL)) return NULL;    
    // reset section and loop
    scn = NULL;
    Elf64_Shdr* shdr = NULL;
    while ((scn = elf_nextscn ((Elf*) elf, scn)) != NULL) {
      if ((shdr = elf64_getshdr (scn)) == NULL) return NULL;
      char* name = (char*) data->d_buf + shdr->sh_name;
      if (sect == name) return scn;
    }
    return NULL;
  }

  // this function fill an elf etxt array with text sections
  static void fill_text_scns (ElfText* text, void* eptr, void* hptr, 
			      bool mode) {
    if ((!text) || (!eptr) || (!hptr)) return;
    // remap to elf data
    Elf*         elf = reinterpret_cast <Elf*> (eptr);
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (hptr);
    // get the shared index section
    Elf_Scn*    scn  = elf_getscn  (elf, ehdr->e_shstrndx);
    Elf_Data*   data = elf_getdata (scn, NULL);
    if ((scn == NULL) || (ehdr == NULL)) return;
    // reset section and loop
    scn = NULL;
    while ((scn = elf_nextscn (elf, scn)) != NULL) {
      // get the section header
      Elf64_Shdr* shdr = elf64_getshdr (scn);
      if (shdr->sh_type == SHT_PROGBITS) {
        if ((shdr->sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR) {
          char* name = (char*) data->d_buf + shdr->sh_name;
          text->add (new ElfSection (name, scn, mode));
        }
      }
    }
  }

  // this function fill an elf load array with load segment
  static void fill_load_segs (ElfLoad* lseg, ElfInterp* interp,
			      int fd, void* eptr, void* hptr, bool mode) {
    if ((!lseg) || (!eptr)) return;
    // remap to elf data and get number of headers
    Elf*         elf = reinterpret_cast <Elf*> (eptr);
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (hptr);
    long   phnum = ehdr->e_phnum;
    t_octa phoff = ehdr->e_phoff;
    t_octa phent = ehdr->e_phentsize;
    // get the segment array and mark the interpreter
    Elf64_Phdr* phdr = elf64_getphdr (elf);
    interp->setph (phdr->p_vaddr + phoff, phent, phnum);
    // loop in the segment array
    for (long i = 0; i < phnum; i++) {
      Elf64_Phdr* sh = &(phdr[i]);
      // check for loadable segment
      if (sh && (sh->p_type == PT_LOAD)) {
	lseg->add (new ElfSegment (fd, sh, mode));
      }
    }
  }

  // create a new elf image by name
  
  ElfImage::ElfImage (const string& name) {
    // set base elf parameters
    d_name   = name;
    p_args   = new ElfArgs (name);
    p_envp   = new ElfEnvp;
    p_kernel = new ElfKernel;
    // reset data
    d_fd     = -1;
    p_elf    = NULL;
    p_hdr    = NULL;
    p_interp = NULL;
    d_static = false;
    reset ();
    // initialize the image
    initialize ();
  }

  // create a new elf image by context and name
  
  ElfImage::ElfImage (Etx* etx, const string& name) {
    // set base elf parameters
    d_name   = name;
    p_args   = new ElfArgs (name);
    p_envp   = new ElfEnvp;
    p_kernel = new ElfKernel (etx);
    // reset data
    d_fd     = -1;
    p_elf    = NULL;
    p_hdr    = NULL;
    p_interp = NULL;
    d_static = false;
    reset ();
    // initialize the image
    initialize ();
  }

  // create a new elf image by context, program name and arguments
  
  ElfImage::ElfImage (Etx* etx, const string& name, 
		      const vector<string>& argv) {
    // set base elf parameters
    d_name   = name;
    p_args   = new ElfArgs (name, argv);
    p_envp   = new ElfEnvp;
    p_kernel = new ElfKernel (etx);
    // reset data
    d_fd     = -1;
    p_elf    = NULL;
    p_hdr    = NULL;
    p_interp = NULL;
    d_static = false;
    reset ();
    // initialize the image
    initialize ();
  }

  // create a new elf image by context and arguments
  
  ElfImage::ElfImage (Etx* etx, const long argc, const char** argv) {
    // set base elf parameters
    p_args   = new ElfArgs (argc, argv);
    p_envp   = new ElfEnvp (argc, argv);
    p_kernel = new ElfKernel (etx);
    d_name   = p_args->getname ();
    // reset data
    d_fd     = -1;
    p_elf    = NULL;
    p_hdr    = NULL;
    p_interp = NULL;
    reset ();
    // initialize the image
    initialize ();
  }

  // destroy this elf image

  ElfImage::~ElfImage (void) {
    if (p_elf != NULL) elf_end ((Elf*) p_elf);
    if (d_fd  != -1)   close (d_fd);
    delete p_interp;
    delete p_args;
    delete p_envp;
  }

  // reset this elf image

  void ElfImage::reset (void) {
    if (p_elf != NULL)    elf_end ((Elf*) p_elf);
    if (d_fd  != -1)      close (d_fd);
    if (p_interp != NULL) delete p_interp;
    d_fd     = -1;
    p_elf    = NULL;
    p_hdr    = NULL;
    p_interp = NULL;
    d_static = false;
  }

  // initialize the elf image

  void ElfImage::initialize (void) {
    // try to open the elf file
    if ((d_fd = open (d_name.c_str(), O_RDONLY)) == -1) {
      string mesg = "cannot open file ";
      throw Exception ("open-error", mesg + d_name);
    }

    // synchronize elf version
    elf_version (EV_CURRENT);

    // start with elf descriptor
    if ((p_elf = elf_begin (d_fd, ELF_C_READ, NULL)) == NULL) {
      close (d_fd);
      string mesg = "cannot read elf file ";
      throw Exception ("elf-error", mesg + d_name);
    }

    // get the header
    if ((p_hdr = elf64_getehdr ((Elf*) p_elf)) == NULL) {
      elf_end ((Elf*) p_elf);
      close (d_fd);
      string mesg = "cannot read elf header from file ";
      throw Exception ("elf-error", mesg + d_name);
    }
    // bind the interpreter - if no interpreter can be found
    // but the image is an executable, we can assume that the file
    /// is statically linked and the flag is set accordingly
    p_interp = find_interp (getinterpname ());
    if ((p_interp == NULL) && (isexec () == true)) {
      p_interp = new ElfInterp;
      d_static = true;
    }
    // set the kernel parameters mode
    p_kernel->setmode (ismsb ());
    // bind kernel parameters
    if (p_interp) p_interp->bind (p_kernel);
  }

  // return the elf image name

  string ElfImage::getname (void) const {
    return d_name;
  }

  // return a string representation of this file

  string ElfImage::repr (void) const {
    if (isvalid () == false) return "invalid";
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    // build image status string
    ostringstream os;
    if (ehdr->e_type == ET_REL)  os << "elf relocatable file";
    if (ehdr->e_type == ET_EXEC) {
      if (isstatic () == true)
	os << "elf static executable file";
      else
	os << "elf dynamic executable file";
    }
    if (iscls64  () == true)  os << ", 64 bits class";
    if (ismsb    () == true)  os << ", msb encoding";
    if (ismsb    () == false) os << ", lsb encoding";
    return os.str ();
  }

  // return true if the elf image is valid

  bool ElfImage::isvalid (void) const {
    // check that we have a good descriptor
    if ((d_fd == -1) || (p_elf == NULL) || (p_hdr == NULL)) return false;
    // check that we have the right type
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    if (ehdr->e_type == ET_REL)  return true;
    if (ehdr->e_type == ET_EXEC) return true;
    return false;
  }

  // return true if the elf file is an executable

  bool ElfImage::isexec (void) const {
    if (isvalid () == false) return false;
    // check that we have the right type
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    if (ehdr->e_type == ET_EXEC) return true;
    return false;
  }

  // return true if the file is static

  bool ElfImage::isstatic (void) const {
    return d_static;
  }

  // return true if the image is of class 64

  bool ElfImage::iscls64 (void) const {
    if (isvalid () == false) return false;
    // get the header and check type
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr); 
    if (ehdr && (ehdr->e_ident[EI_CLASS] == ELFCLASS64)) return true;
    return false;
  }

   // return true if the image is an IA64 machine

  bool ElfImage::isia64 (void) const {
    if (isvalid () == false) return false;
    // get the header and check type
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr); 
    if (ehdr && (ehdr->e_machine == EM_IA_64)) return true;
    return false;
  }
 
  // return true if the image has msb encoding

  bool ElfImage::ismsb (void) const {
    if (isvalid () == false) return false;
    // get the header and check type
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr); 
    if (ehdr && (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)) return true;
    return false;
  }

  // return the number of program headers

  long ElfImage::getphnum (void) const {
    if (isvalid () == false) return 0;
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    return ehdr->e_phnum;
  }

  // return the number of section headers

  long ElfImage::getshnum (void) const {
    if (isvalid () == false) return 0;
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    return ehdr->e_shnum;
  }

  // return the elf entry point

  t_octa ElfImage::getentry (void) const {
    if (isvalid () == false) return 0;
    Elf64_Ehdr* ehdr = reinterpret_cast <Elf64_Ehdr*> (p_hdr);
    return ehdr->e_entry;
  }

  // return the elf interpreter name

  string ElfImage::getinterpname (void) const {
    string interp;
    if (isvalid () == false) return interp;
    Elf_Scn*   scn = get_section (".interp", p_elf, p_hdr);
    Elf_Data* data = (scn == NULL) ? NULL : elf_getdata (scn, NULL);
    if (data == NULL) return interp;
    interp = (const char*) data->d_buf;
    return interp;
  }

  // return the associated interpreter

  ElfInterp* ElfImage::getinterp (void) const {
    return p_interp;
  }

  // get an elf section by name

  ElfSection* ElfImage::getscn (const string& name) const {
    if (isvalid () == false) return NULL;
    Elf_Scn* scn = get_section (name, p_elf, p_hdr);
    return (scn == NULL) ? NULL : new ElfSection (name, scn, ismsb ());
  }

  // return all elf text sections

  ElfText* ElfImage::gettxt (void) const {
    // create an elf text
    ElfText* text = new ElfText;
    // fill in text sections
    fill_text_scns (text, p_elf, p_hdr, ismsb ());
    return text;
  }

  // return all elf load segments

  ElfLoad* ElfImage::getload (void) const {
    // create an elf load array
    ElfLoad* lseg = new ElfLoad;
    // fill in segments
    fill_load_segs (lseg, p_interp, d_fd, p_elf, p_hdr, ismsb ());
    return lseg;  
  }

  // return the elf breakable array

  ElfBrk* ElfImage::getbrkm (void) const {
    // create an elf break array
    ElfBrk* brk = new ElfBrk (p_kernel);
    // fill in segments
    fill_load_segs (brk, p_interp, d_fd, p_elf, p_hdr, ismsb ());
    return brk;
  }

  // return an elf exec segment image

  ElfExec* ElfImage::getexec (void) const {
    // check that we have something valid
    if (isexec () == false) return 0;
    // check that we have an elf interpreter
    if (!p_interp) {
      string msg = "cannot find elf interpreter ";
      throw Exception ("elf-error", msg + getinterpname ());
    }
    // create an elf exec image
    ElfExec* exim = new ElfExec (p_kernel);
    // add the break array
    exim->setbrkm (getbrkm ());
    // bind the arguments
    p_interp->bind (p_args, p_envp, exim->getstkm ());
    return exim;  
  }

  // return an elf image in the form of memory - throw an execption
  // if the image is invalid

  ElfExec* ElfImage::getimage (void) const {
    ElfExec* exim = getexec ();
    if (!exim) {
      string msg = "invalid elf image (not an executable) ";
      throw Exception ("exec-error", msg + d_name);
    }
    return exim;
  }

  // return a checker object if it exists

  Checker* ElfImage::getchecker (void) const {
    // get a checker section
    if (isvalid () == false) return NULL;
    // get the checker section
    string  name = IATO_SECTION_CHECKER;
    Elf_Scn* scn = get_section (name, p_elf, p_hdr);
    ElfChecker* chk = (scn == NULL) ? NULL : 
                      new ElfChecker (name, scn, ismsb ());
    // build the checker and clean
    Checker* result = (chk == NULL) ? NULL : chk->getchecker ();
    delete chk;
    return result;
  }
}
