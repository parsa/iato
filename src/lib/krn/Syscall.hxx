// ---------------------------------------------------------------------------
// - Syscall.hxx                                                             -
// - iato:krn library - system call definition                               -
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

#ifndef  IATO_SYSCALL_HXX
#define  IATO_SYSCALL_HXX

#ifndef  IATO_SYSCALL_HPP
#error   "this file cannot be used outside the system call implementation"
#endif

namespace iato {

  // this file contains the Gnu/Linux system call number as defined by the
  // kernel 2.4. Hopefully these numbers are not going to change very much.
  // Although, the numbers are defined, not all system calls are mapped here,
  // nor they are implemented.

  const long NR_EXIT           = 1025;
  const long NR_READ           = 1026;
  const long NR_WRITE          = 1027;
  const long NR_OPEN           = 1028;
  const long NR_CLOSE	       = 1029;
  const long NR_LINK           = 1031;
  const long NR_UNLINK         = 1032;
  const long NR_LSEEK          = 1040;
  const long NR_GETPID         = 1041;
  const long NR_GETPPID        = 1042;
  const long NR_SETUID         = 1045;
  const long NR_GETUID         = 1046;
  const long NR_GETEUID        = 1047;
  const long NR_ACCESS         = 1049;
  const long NR_KILL	       = 1053;
  const long NR_RENAME         = 1054;
  const long NR_MKDIR          = 1055;
  const long NR_RMDIR          = 1056;
  const long NR_DUP            = 1057;
  const long NR_PIPE           = 1058;
  const long NR_TIMES          = 1059;
  const long NR_BRK            = 1060;
  const long NR_SETGID         = 1061;
  const long NR_GETGID         = 1062;
  const long NR_GETEGID        = 1063;
  const long NR_IOCTL          = 1065;
  const long NR_FCNTL          = 1066;
  const long NR_UMASK          = 1067;
  const long NR_DUP2           = 1070;
  const long NR_SETREUID       = 1071;
  const long NR_SETREGID       = 1072;
  const long NR_GETRESUID      = 1073;
  const long NR_SETRESUID      = 1074;
  const long NR_GETRESGID      = 1075;
  const long NR_SETRESGID      = 1076;
  const long NR_GETGROUPS      = 1077;
  const long NR_SETGROUPS      = 1078;
  const long NR_GETPGID        = 1079;
  const long NR_SETPGID        = 1080;
  const long NR_SETSID         = 1081;
  const long NR_GETSID         = 1082;
  const long NR_GETTIMEOFDAY   = 1087;
  const long NR_SETTIMEOFDAY   = 1088;
  const long NR_UNAME          = 1130;
  const long NR_SYSCTL         = 1150;
  const long NR_MMAP           = 1151;
  const long NR_MUNMAP         = 1152;
  const long NR_RT_SIGACTION   = 1177;
  const long NR_RT_SIGPROCMASK = 1179;
  const long NR_STAT           = 1210;
  const long NR_LSTAT          = 1211;
  const long NR_FSTAT          = 1212;
  const long NR_EXIT_GROUP     = 1236;
}

#endif
