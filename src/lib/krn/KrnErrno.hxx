// ---------------------------------------------------------------------------
// - KrnErrno.hxx                                                            -
// - iato:krn library - kernel error number definition                       -
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

#ifndef  IATO_KRNERRNO_HXX
#define  IATO_KRNERRNO_HXX

namespace iato {

  // this file contains the Gnu/Linux system errno number as defined by the
  // kernel 2.4. Hopefully these numbers are not going to change very much.

  const long KRN_EPERM           = 1;      // Operation not permitted 
  const long KRN_ENOENT          = 2;      // No such file or directory 
  const long KRN_ESRCH           = 3;      // No such process 
  const long KRN_EINTR           = 4;      // Interrupted system call 
  const long KRN_EIO             = 5;      // I/O error 
  const long KRN_ENXIO           = 6;      // No such device or address 
  const long KRN_E2BIG           = 7;      // Arg list too long 
  const long KRN_ENOEXEC         = 8;      // Exec format error 
  const long KRN_EBADF           = 9;      // Bad file number 
  const long KRN_ECHILD          = 10;      // No child processes 
  const long KRN_EAGAIN          = 11;      // Try again 
  const long KRN_ENOMEM          = 12;      // Out of memory 
  const long KRN_EACCES          = 13;      // Permission denied 
  const long KRN_EFAULT          = 14;      // Bad address 
  const long KRN_ENOTBLK         = 15;      // Block device required 
  const long KRN_EBUSY           = 16;      // Device or resource busy 
  const long KRN_EEXIST          = 17;      // File exists 
  const long KRN_EXDEV           = 18;      // Cross-device link 
  const long KRN_ENODEV          = 19;      // No such device 
  const long KRN_ENOTDIR         = 20;      // Not a directory 
  const long KRN_EISDIR          = 21;      // Is a directory 
  const long KRN_EINVAL          = 22;      // Invalid argument 
  const long KRN_ENFILE          = 23;      // File table overflow 
  const long KRN_EMFILE          = 24;      // Too many open files 
  const long KRN_ENOTTY          = 25;      // Not a typewriter 
  const long KRN_ETXTBSY         = 26;      // Text file busy 
  const long KRN_EFBIG           = 27;      // File too large 
  const long KRN_ENOSPC          = 28;      // No space left on device 
  const long KRN_ESPIPE          = 29;      // Illegal seek 
  const long KRN_EROFS           = 30;      // Read-only file system 
  const long KRN_EMLINK          = 31;      // Too many links 
  const long KRN_EPIPE           = 32;      // Broken pipe 
  const long KRN_EDOM            = 33;      // Math argument out of domain 
  const long KRN_ERANGE          = 34;      // Math result not representable 
  const long KRN_EDEADLK         = 35;      // Resource deadlock would occur 
  const long KRN_ENAMETOOLONG    = 36;      // File name too long 
  const long KRN_ENOLCK          = 37;      // No record locks available 
  const long KRN_ENOSYS          = 38;      // Function not implemented 
  const long KRN_ENOTEMPTY       = 39;      // Directory not empty 
  const long KRN_ELOOP           = 40;      // Too many symbolic links
  const long KRN_ENOMSG          = 42;      // No message of desired type 
  const long KRN_EIDRM           = 43;      // Identifier removed 
  const long KRN_ECHRNG          = 44;      // Channel number out of range 
  const long KRN_EL2NSYNC        = 45;      // Level 2 not synchronized 
  const long KRN_EL3HLT          = 46;      // Level 3 halted 
  const long KRN_EL3RST          = 47;      // Level 3 reset 
  const long KRN_ELNRNG          = 48;      // Link number out of range 
  const long KRN_EUNATCH         = 49;      // Protocol driver not attached 
  const long KRN_ENOCSI          = 50;      // No CSI structure available 
  const long KRN_EL2HLT          = 51;      // Level 2 halted 
  const long KRN_EBADE           = 52;      // Invalid exchange 
  const long KRN_EBADR           = 53;      // Invalid request descriptor 
  const long KRN_EXFULL          = 54;      // Exchange full 
  const long KRN_ENOANO          = 55;      // No anode 
  const long KRN_EBADRQC         = 56;      // Invalid request code 
  const long KRN_EBADSLT         = 57;      // Invalid slot 
  const long KRN_EBFONT          = 59;      // Bad font file format 
  const long KRN_ENOSTR          = 60;      // Device not a stream 
  const long KRN_ENODATA         = 61;      // No data available 
  const long KRN_ETIME           = 62;      // Timer expired 
  const long KRN_ENOSR           = 63;      // Out of streams resources 
  const long KRN_ENONET          = 64;      // Machine is not on the network 
  const long KRN_ENOPKG          = 65;      // Package not installed 
  const long KRN_EREMOTE         = 66;      // Object is remote 
  const long KRN_ENOLINK         = 67;      // Link has been severed 
  const long KRN_EADV            = 68;      // Advertise error 
  const long KRN_ESRMNT          = 69;      // Srmount error 
  const long KRN_ECOMM           = 70;      // Communication error on send 
  const long KRN_EPROTO          = 71;      // Protocol error 
  const long KRN_EMULTIHOP       = 72;      // Multihop attempted 
  const long KRN_EDOTDOT         = 73;      // RFS specific error 
  const long KRN_EBADMSG         = 74;      // Not a data message 
  const long KRN_EOVERFLOW       = 75;      // Value too large
  const long KRN_ENOTUNIQ        = 76;      // Name not unique on network 
  const long KRN_EBADFD          = 77;      // File descriptor in bad state 
  const long KRN_EREMCHG         = 78;      // Remote address changed 
}

#endif
