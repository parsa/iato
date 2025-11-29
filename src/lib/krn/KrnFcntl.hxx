// ---------------------------------------------------------------------------
// - KrnFcntl.hxx                                                            -
// - iato:krn library - fcntl related system call argument definition        -
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

#ifndef  IATO_KRNFCNTL_HXX
#define  IATO_KRNFCNTL_HXX

#ifndef  IATO_KRNTYPES_HXX
#include "KrnTypes.hxx"
#endif

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  // fcntl commands
  const t_octa KRN_F_DUPFD  = 0;       // duplicate file descriptor.
  const t_octa KRN_F_GETFD  = 1;       // get file descriptor flags.
  const t_octa KRN_F_SETFD  = 2;       // set file descriptor flags.
  const t_octa KRN_F_GETFL  = 3;       // get file status flags.
  const t_octa KRN_F_SETFL  = 4;       // set file status flags.
  const t_octa KRN_F_GETLK  = 5;       // get record locking info.
  const t_octa KRN_F_SETLK  = 6;       // set record locking info (non-block).
  const t_octa KRN_F_SETLKW = 7;       // set record locking info (blocking).

  // fcntl/open arguments - carefull it's octal base code
  const t_octa KRN_O_RDONLY     = 00;
  const t_octa KRN_O_WRONLY     = 01;      
  const t_octa KRN_O_RDWR       = 02;
  const t_octa KRN_O_CREAT      = 0100;
  const t_octa KRN_O_EXCL       = 0200;
  const t_octa KRN_O_NOCTTY     = 0400;
  const t_octa KRN_O_TRUNC      = 01000;
  const t_octa KRN_O_APPEND     = 02000;
  const t_octa KRN_O_NONBLOCK   = 04000;
  const t_octa KRN_O_SYNC       = 010000;
  const t_octa KRN_O_LARGEFILE  = 0100000; 
  const t_octa KRN_O_DIRECTORY  = 0200000;
  const t_octa KRN_O_NOFOLLOW   = 0400000;

  // lseek command arguments
  const t_octa KRN_SEEK_SET     = 0;   // seek from beginning of file.
  const t_octa KRN_SEEK_CUR     = 1;   // seek from current position.
  const t_octa KRN_SEEK_END     = 2;   // seek to the end of file

  // access mode
  const t_octa KRN_R_OK         = 4;   // test for read permission.
  const t_octa KRN_W_OK         = 2;   // test for write permission.
  const t_octa KRN_X_OK         = 1;   // test for execute permission.
  const t_octa KRN_F_OK         = 0;   // test for existence.

  // permission mode
  const t_octa KRN_S_ISUID      = 04000; // test user ID on execution
  const t_octa KRN_S_ISGID      = 02000; // test group ID on execution
  const t_octa KRN_S_ISVTX      = 01000; // test sticky bit
  const t_octa KRN_S_IRUSR      = 00400; // test read by owner
  const t_octa KRN_S_IWUSR      = 00200; // test write by owner
  const t_octa KRN_S_IXUSR      = 00100; // test execute/search by owner
  const t_octa KRN_S_IRGRP      = 00040; // test read by group
  const t_octa KRN_S_IWGRP      = 00020; // test write by group
  const t_octa KRN_S_IXGRP      = 00010; // test execute/search by group
  const t_octa KRN_S_IROTH      = 00004; // test read by others
  const t_octa KRN_S_IWOTH      = 00002; // test write by others
  const t_octa KRN_S_IXOTH      = 00001; // test execute/search by others



  // the kernel stat structure
  struct krn_sstat {
    t_krn_dev     d_dev;      // device
    t_krn_ino     d_ino;      // file serial number
    t_krn_nlink   d_nlink;    // link count
    t_krn_mode    d_mode;     // file mode
    t_krn_uid     d_uid;      // user id of the file's owner
    t_krn_gid     d_gid;      // group id of the file's group
    t_quad        pad0;
    t_krn_dev     d_rdev;     // device number, if device
    t_krn_off     d_size;     // size of file, in bytes
    t_krn_time    d_atime;    // time of last access
    t_long        reserved0;  // reserved for atime.nanoseconds
    t_krn_time    d_mtime;    // time of last modification
    t_long        reserved1;  // reserved for mtime.nanoseconds
    t_krn_time    d_ctime;    // time of last status change
    t_long        reserved2;  // reserved for ctime.nanoseconds
    t_long        d_blksize;  // optimal block size for I/O
    t_long        d_blocks;   // 512-byte blocks allocated
    t_long        unused[3];
  };
}

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

namespace iato {
  // the fcntl command mapping is identical under linux
  static t_octa map_fcntl_cmd (const t_octa cmd) {
    return cmd;
  }
  // the open flags mapping is identical under linux
  static t_octa map_open_flags (const t_octa flags) {
    return flags;
  }
  // the lseek command flag mapping
  static t_octa map_lseek_whence (const t_octa whence) {
    return whence;
  }
  // the access mode mapping
  static t_octa map_access_mod (const t_octa arg) {
    return arg;
  }
  // the permission mode mapping
  static t_octa map_perm_mod (const t_octa arg) {
    return arg;
  }
}

#endif

#ifdef   OS_DARWIN
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

namespace iato {
  static t_octa map_fcntl_cmd (const t_octa cmd) {
      if (cmd == KRN_F_DUPFD) return F_DUPFD;
      if (cmd == KRN_F_GETFD) return F_GETFD;
      if (cmd == KRN_F_SETFD) return F_SETFD;
      if (cmd == KRN_F_GETFL) return F_GETFL;
      if (cmd == KRN_F_SETFL) return F_SETFL;
      if (cmd == KRN_F_GETLK) return F_GETLK;
      if (cmd == KRN_F_SETLK) return F_SETLK;
      if (cmd == KRN_F_SETLKW) return F_SETLKW;
      return cmd;
  }
  static t_octa map_open_flags (const t_octa flags) {
    t_octa res = 0;
    t_octa acc = flags & 3;
    if (acc == KRN_O_RDONLY) res |= O_RDONLY;
    else if (acc == KRN_O_WRONLY) res |= O_WRONLY;
    else if (acc == KRN_O_RDWR) res |= O_RDWR;

    if (flags & KRN_O_CREAT) res |= O_CREAT;
    if (flags & KRN_O_EXCL) res |= O_EXCL;
    if (flags & KRN_O_NOCTTY) res |= O_NOCTTY;
    if (flags & KRN_O_TRUNC) res |= O_TRUNC;
    if (flags & KRN_O_APPEND) res |= O_APPEND;
    if (flags & KRN_O_NONBLOCK) res |= O_NONBLOCK;
    // O_SYNC might be O_FSYNC on mac?
    if (flags & KRN_O_SYNC) res |= O_SYNC;
#ifdef O_DIRECTORY
    if (flags & KRN_O_DIRECTORY) res |= O_DIRECTORY;
#endif
#ifdef O_NOFOLLOW
    if (flags & KRN_O_NOFOLLOW) res |= O_NOFOLLOW;
#endif
#ifdef O_LARGEFILE
    if (flags & KRN_O_LARGEFILE) res |= O_LARGEFILE;
#endif
    
    return res;
  }
  static t_octa map_lseek_whence (const t_octa whence) {
    return whence;
  }
  static t_octa map_access_mod (const t_octa arg) {
    return arg;
  }
  static t_octa map_perm_mod (const t_octa arg) {
    return arg;
  }
}
#endif

#endif
