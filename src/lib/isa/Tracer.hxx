// ---------------------------------------------------------------------------
// - Tracer.hxx                                                              -
// - iato:isa library - tracer system dependant configuration                -
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

#ifndef IATO_TRACER_HXX
#define IATO_TRACER_HXX

// ---------------------------------------------------------------------------
// - linux configuration                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#ifndef  __USE_LARGEFILE64
#define  __USE_LARGEFILE64
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// ---------------------------------------------------------------------------
// - sparc/solaris configuration                                             -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// ---------------------------------------------------------------------------
// - local tracer configuration                                              -
// ---------------------------------------------------------------------------

namespace iato {
  // default file open mode
  const long TRACER_OPEN_MODE = O_WRONLY|O_CREAT|O_LARGEFILE|O_TRUNC;
  const long TRACER_FILE_MODE = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
  // the reader open mode have to be coherent with the tracer one.
  const long READER_OPEN_MODE = O_RDONLY|O_LARGEFILE;
}

#endif
