// ---------------------------------------------------------------------------
// - KrnFcntl.cpp                                                            -
// - iato:krn library - fcntl related system call implementation             -
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

#include "Bits.hpp"
#include "KrnFcntl.hpp"
#include "KrnFcntl.hxx"

// declare this function to handle the messy situation between the stat
// structure and the C function stat. As far as gcc is concerned, between
// 2.95 and 3.0 there is a namespace conflict. This wrapper outside the
// iato namespace provides a satisfactory fix (Uhhh...)
static int kstat (const char* file, struct stat* buf) {
  return stat (file, buf);
}

namespace iato {

  // this procedure maps the machine stat structure into the kernel one
  static inline void map_stat (krn_sstat& dst, const struct stat& src) {
    dst.d_dev     = lfixocta (src.st_dev);
    dst.d_ino     = lfixocta (src.st_ino);
    dst.d_mode    = lfixquad (src.st_mode);
    dst.d_nlink   = lfixocta (src.st_nlink);
    dst.d_uid     = lfixquad (src.st_uid);
    dst.d_gid     = lfixquad (src.st_gid);
    dst.d_rdev    = lfixocta (src.st_rdev);
    dst.d_size    = lfixocta (src.st_size);
    dst.d_blksize = lfixocta (src.st_blksize);
    dst.d_blocks  = lfixocta (src.st_blocks);
    dst.d_atime   = lfixocta (src.st_atime);
    dst.d_mtime   = lfixocta (src.st_mtime);
    dst.d_ctime   = lfixocta (src.st_ctime);
  }

  // fcntl system call
  
  void krn_fcntl (Rse* rse, Register* rbk) {
    // initialize result and arguments
    int result = -1;
    // get the first argument
    int fd  = (int) sys_args_getoval (0, rse, rbk);
     // get the command
    t_octa cmd = sys_args_getoval (1, rse, rbk);
    t_octa ctl = map_fcntl_cmd (cmd);
    // process the command
    switch (cmd) {
    case KRN_F_GETFD:
      result = fcntl (fd, ctl);
      break;
    default:
      throw Exception ("syscall-error", "unimplemented fcntl function");
      break;
    }
    // set syscall result
    if (result == 0) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // open system call

  void krn_open (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // get the flags and map them
    t_octa data = sys_args_getoval (1, rse, rbk);
    t_octa flag = map_open_flags   (data);
    // check if we have a third argument
    bool   mflg = false;
    t_octa mode = OCTA_0;
    if (((flag & KRN_O_WRONLY) == KRN_O_WRONLY) || 
	((flag & KRN_O_RDWR)   == KRN_O_RDWR)) {
      mflg = true;
      mode = sys_args_getoval (2, rse, rbk);
    }
    // process system call
    int result = -1;
    result = mflg ? open ((const char*) name, flag, mode) : 
                    open ((const char*) name, flag);
    delete [] name;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // read system call

  void krn_read (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the file descriptor argument
    int fd  = (int) sys_args_getoval (0, rse, rbk);
    // get the buffer size
    size_t size = sys_args_getoval (2, rse, rbk);
    if (size  == 0) {
      sys_args_setretn (OCTA_0, rbk);
      return;
    }
    // allocate a buffer and read
    t_byte*   buf = new t_byte[size];
    t_long result = read (fd, buf, size);
    // check result before copy
    if (result == 0) {
      sys_args_setretn (OCTA_0, rbk);
      return;
    }
    if (result == -1) {
      sys_args_seterrn (result, errno, rbk);
      return;
    }
    // get the address to copy buffer
    t_octa addr = sys_args_getoval (1, rse, rbk);
    sys_args_setdata (addr, buf, result, mem);
    // set result
    sys_args_setretn (result, rbk);
  }

  // write system call

  void krn_write (Rse* rse, Register* rbk, ElfExec* mem) {
    // collect argument to copy buffer
    int    fd = (int) sys_args_getoval (0, rse, rbk);
    t_octa bp = sys_args_getoval (1, rse, rbk);
    t_octa sz = sys_args_getoval (2, rse, rbk);
    // copy buffer if needed
    if (sz == OCTA_0) {
      sys_args_setretn (OCTA_0, rbk);
      return;
    }
    t_long size = sz;
    t_byte* buf = new t_byte[size];
    for (t_long i = 0; i < size; i++) {
      buf[i] = mem->readbyte (bp++);
    }
    // call local system call
    t_long result = write (fd, buf, size);
    delete [] buf;
    // set result
    if (result == -1) {
      sys_args_seterrn (result, errno, rbk);
    } else {
      sys_args_setretn (result, rbk);
    }
  }

  // close system call

  void krn_close (Rse* rse, Register* rbk) {
    // get the file descriptor argument
    int fd  = (int) sys_args_getoval (0, rse, rbk);
    // get status for close
    t_long result = close (fd);
    if (result == -1) {
      sys_args_seterrn (result, errno, rbk);
    } else {
      sys_args_setretn (OCTA_0, rbk);
    }
  }

  // lseek system call

  void krn_lseek (Rse* rse, Register* rbk) {
    // get the file descriptor argument
    int fd  = (int) sys_args_getoval (0, rse, rbk);
    // get the offset and command
    off_t offset  = (off_t) sys_args_getoval (1, rse, rbk);
    t_octa  data  = sys_args_getoval (2, rse, rbk);
    t_octa whence = map_lseek_whence (data);
    // get status for close
    t_long result = lseek (fd, offset, whence);
    if (result == -1) {
      sys_args_seterrn (result, errno, rbk);
    } else {
      sys_args_setretn (result, rbk);
    }
  }

  // unlink system call
  void krn_unlink (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // process system call
    t_long result = unlink ((const char*) name);
    delete [] name;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // link system call
  void krn_link (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the src argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* src  = sys_args_getstr  (addr, mem);
    // get the dst argument
    addr         = sys_args_getoval (1, rse, rbk);
    t_byte* dst  = sys_args_getstr  (addr, mem);
    // process system call
    t_long result = link ((const char*) src, (const char*) dst);
    delete [] src;
    delete [] dst;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // rename system call
  void krn_rename (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the dest string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* src  = sys_args_getstr  (addr, mem);
    // get the src string argument
    addr         = sys_args_getoval (1, rse, rbk);
    t_byte* dst  = sys_args_getstr  (addr, mem);
    // process system call
    t_long result = rename ((const char*) src, (const char*) dst);
    delete [] dst;
    delete [] src;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // mkdir system call
  void krn_mkdir (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the name string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // get the mode unsigned int argument
    t_octa  mod  = map_perm_mod (sys_args_getoval (1, rse, rbk));
    // process system call
    t_long result = mkdir ((const char*) name, (mode_t) mod);
    delete [] name;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // rmdir system call
  void krn_rmdir (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the name string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // process system call
    t_long result = rmdir ((const char*) name);
    delete [] name;
    // set syscall result
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // access system call
  void krn_access (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the dest string argument
    t_octa  addr = sys_args_getoval (0, rse, rbk);
    t_byte* file = sys_args_getstr  (addr, mem);
    // get the mode argument
    t_octa mod = map_access_mod (sys_args_getoval (1, rse, rbk));
    // process system call
    t_long result = access ((const char *) file, mod);
    delete [] file;
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }

  // --------------------------------------------------------------------------
  // - linux 2.6 new system calls                                             -
  // --------------------------------------------------------------------------

  // this section incorporates system call that requires some tweaks with
  // the argument mapping as they were changed between 2.4 and 2.6. If the
  // arguments are replicated then we have a new system call.

  // stat system call

  void krn_stat (Rse* rse, Register* rbk, ElfExec* mem) {
    // check for new system call by comparing 1 with 3 and 2 with 4
    bool nsys = sys_args_compare (1, 3, rse, rbk) && 
                sys_args_compare (2, 4, rse, rbk);
    // grab the file name, so we can request the value
    long    aidx = nsys ? 1 : 0;
    t_octa  addr = sys_args_getoval (aidx, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // make local stat
    struct stat mst; 
    int status = kstat ((const char*) name, &mst);
    if (status == -1) {
      sys_args_seterrn (status, errno, rbk);
      return;
    }
    // the kernel result structue
    union {
      t_byte    bst[sizeof(krn_sstat)];
      krn_sstat kst;
    };
    // map to the kernel result structure
    map_stat (kst, mst);
    // get result address structure
    long   sidx = nsys ? 2 : 1;
    t_octa sptr = sys_args_getoval (sidx, rse, rbk);
    // copy structure, endianess is set before
    sys_args_setdata (sptr, bst, sizeof (kst), mem);
    // set syscall result
    sys_args_setretn (OCTA_0, rbk);
  }

  void krn_lstat (Rse* rse, Register* rbk, ElfExec* mem) {
    // check for new system call by comparing 1 with 3 and 2 with 4
    bool nsys = sys_args_compare (1, 3, rse, rbk) && 
                sys_args_compare (2, 4, rse, rbk);
    // grab the file name, so we can request the value
    long    aidx = nsys ? 1 : 0;
    t_octa  addr = sys_args_getoval (aidx, rse, rbk);
    t_byte* name = sys_args_getstr  (addr, mem);
    // the local stat
    struct stat mst; 
    int status = lstat ((const char*) name, &mst);
    if (status == -1) {
      sys_args_seterrn (status, errno, rbk);
      return;
    }
    // the kernel result structue
    union {
      t_byte   bst[sizeof(krn_sstat)];
      krn_sstat kst;
    };
    // change to the kernel result structue
    map_stat (kst, mst);
    // get result address structure
    long   sidx = nsys ? 2 : 1;
    t_octa sptr = sys_args_getoval (sidx, rse, rbk);
    // copy structure, endianess is set before
    sys_args_setdata (sptr, bst, sizeof (kst), mem);
    // set syscall result
    sys_args_setretn (OCTA_0, rbk);
  }

  // fstat system call
  
  void krn_fstat (Rse* rse, Register* rbk, ElfExec* mem) {
    // check for new system call by comparing 1 with 3 and 2 with 4
    bool nsys = sys_args_compare (1, 3, rse, rbk) && 
                sys_args_compare (2, 4, rse, rbk);
    // grab the file id, so we can request the value
    long fi = nsys ? 1 : 0;
    int  fd = (int) sys_args_getoval (fi, rse, rbk);
    // the local call stat
    struct stat mst; 
    int status = fstat (fd, &mst);
    if (status == -1) {
      sys_args_seterrn (status, errno, rbk);
      return;
    }
    // the kernel result structue
    union {
      t_byte    bst[sizeof(krn_sstat)];
      krn_sstat kst;
    };
    // map to the kernel result structure
    map_stat (kst, mst);
    // get result address structure
    long   sidx = nsys ? 2 : 1;
    t_octa sptr = sys_args_getoval (sidx, rse, rbk);
    // copy structure, endianess is set before
    sys_args_setdata (sptr, bst, sizeof (kst), mem);
    // set syscall result
    sys_args_setretn (OCTA_0, rbk);
  }
}
