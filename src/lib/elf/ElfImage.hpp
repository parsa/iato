// ---------------------------------------------------------------------------
// - ElfImage.hpp                                                            -
// - iato:elf library - elf image class definition                           -
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

#ifndef  IATO_ELFIMAGE_HPP
#define  IATO_ELFIMAGE_HPP

#ifndef  IATO_ETX_HPP
#include "Etx.hpp"
#endif

#ifndef  IATO_ELFTEXT_HPP
#include "ElfText.hpp"
#endif

#ifndef  IATO_ELFEXEC_HPP
#include "ElfExec.hpp"
#endif

#ifndef  IATO_ELFINTERP_HPP
#include "ElfInterp.hpp"
#endif

#ifndef  IATO_CHECKER_HPP
#include "Checker.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Elf class is a complete representation of an elf object. At
  /// initialization, the elf file is opened and the header is read. A
  /// determination of weather the file is a relocatable file (.o) or
  /// an executable file. For a relocatable file, the linking view of
  /// the elf file is used (aka using section). For an executable file
  /// the executable view is used (aka segments). Various methods are
  /// provided to get an idea of the file nataure. The 'isvalid' method
  /// is particulary useful to determined the object state. The 'isexec'
  /// method tells if the elf image is an executable object. The 'getimage'
  /// method is the prefered way to get an execuatble image since it prepares
  /// the complete memory representation.
  
  class ElfImage {
  private:
    /// the file name
    string d_name;
    /// the file descriptor
    int d_fd;
    /// the elf descriptor
    void* p_elf;
    /// the elf header
    void* p_hdr;
    /// the static flag
    bool d_static;
    /// the elf arguments
    ElfArgs* p_args;
    /// the elf environment
    ElfEnvp* p_envp;
    /// the elf interpreter
    ElfInterp* p_interp;
    /// the elf kernel parameters
    ElfKernel* p_kernel;

  public:
    /// create a new elf image by name
    /// @param name the file name
    ElfImage (const string& name);

    /// create a new elf image by context and name
    /// @param etx the elf context
    /// @param name the file name
    ElfImage (Etx* etx, const string& name);

    /// create a new elf image by context, name and arguments
    /// @param etx the elf context
    /// @param name the program name
    /// @param argv the program arguments
    ElfImage (Etx* etx, const string& name, const vector<string>& argv);

    /// create a new elf image by context and arguments
    /// @param etx  the elf context
    /// @param argc the number of arguments
    /// @param argv the argument vector
    ElfImage (Etx* etx, const long argc, const char** argv);

    /// destroy this elf image
    ~ElfImage (void);

    /// reset this elf image
    void reset (void);

    /// @return the elf image name
    string getname (void) const;

    /// @return a string representation
    string repr (void) const;

    /// @return true if the elf image is valid
    bool isvalid (void) const;

    /// @return true if the elf image is an executable
    bool isexec (void) const;

    /// @return true if the elf image is static
    bool isstatic (void) const;

    /// @return true if the image is of class 64
    bool iscls64 (void) const;

    /// @return true if the image is an ia64 image
    bool isia64 (void) const;

    /// @return true if the image has msb encoding
    bool ismsb (void) const;

    /// @return the number of program headers
    long getphnum (void) const;

    /// @return the number of section headers
    long getshnum (void) const;

    /// @return the elf entry point
    t_octa getentry (void) const;

    /// @return the elf interpreter name
    string getinterpname (void) const;

    /// @return the associated elf interpreter
    ElfInterp* getinterp (void) const;

    /// @return a section by name
    ElfSection* getscn (const string& name) const;

    /// @return all elf text sections
    ElfText* gettxt (void) const;

    /// @return all elf load segments
    ElfLoad* getload (void) const;

    /// @return the elf breakable memory
    ElfBrk* getbrkm (void) const;

    /// @return the elf executable image
    ElfExec* getexec (void) const;

    /// @return an elf executable image or throw an exception
    ElfExec* getimage (void) const;

    /// @return a checker object if it exists
    Checker* getchecker (void) const;

  private:
    // make the copy constructor private
    ElfImage (const ElfImage&);
    // make the assignment operator private
    ElfImage& operator = (const ElfImage&);
    // initialize the elf image
    void initialize (void);
  };
}

#endif
