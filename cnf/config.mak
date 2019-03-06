# ---------------------------------------------------------------------------
# - config.mak                                                              -
# - iato platform configuration file                                        -
# ---------------------------------------------------------------------------
# - (c) inria 2002-2004                                                     -
# ---------------------------------------------------------------------------
# - authors                                        Amaury Darsch    2002:2004 -
# -                                                Pierre Villalon  2002:2003 -
# -                                                Andre  Seznec    2002:2004 -
# -----------------------------------------------------------------------------
# - This program  is  free  software;  you can redistribute  it and/or modify -
# - it  under  the  terms  of  the  GNU  General Public License version 2, as -
# - published by the Free Software Foundation.                                -
# -                                                                           -
# - This  program  is  distributed  in  the  hope  that  it  will be  useful, -
# - but   without  any  warranty;  without  even  the   implied  warranty  of -
# - merchantability or fitness for a particular purpose.                      -
# -                                                                           -
# - See the GNU General Public License version 2 for more details             -
# -----------------------------------------------------------------------------

# find os and machine
OSNAME	       := $(shell uname -s)
MANAME         := $(shell uname -m)

# set install path
DIRINSTALL	= /usr/local
LIBINSTALL      = $(DIRINSTALL)/lib
EXEINSTALL      = $(DIRINSTALL)/exe
HDRINSTALL      = $(DIRINSTALL)/include/iato
MANINSTALL	= $(DIRINSTALL)/man/man1

# set eventually local elf path
ifeq ($(LELF),)
LELFHDR		= /usr/include/libelf
LELFLIB		= -lelf
else
LELFHDR		= $(LELF)/include
LELFLIB		= -L$(LELF)/lib -lelf
endif

# process solaris options
ifeq ($(OSNAME),SunOS)
CC     		= g++
XAS		= ia64-linux-gnu-as -x
XCC		= ia64-linux-gnu-gcc
XLD		= ia64-linux-gnu-ld
XLK		= ia64-linux-gnu-gcc -static
PLTDEFS		= -DIATO -DOS_SOLARIS -D_REENTRANT -DHAS_PTHREAD
XPLTDEFS	= -DIATO -DOS_SOLARIS
PLTINCS		=
XPLTINCS	=
PLTLIBS		= $(LELFLIB)
XPLTLIBS	=
endif

# process linux options
ifeq ($(OSNAME),Linux)
CC		= g++
ifeq ($(MANAME),ia64)
XCC		= gcc
XAS		= as -x
XLD		= ld
XLK		= gcc -static
endif
ifeq ($(MANAME),i686)
XCC		= ia64-linux-gnu-gcc
XAS		= ia64-linux-gnu-as -x
XLD		= ia64-linux-gnu-ld
XLK		= ia64-linux-gnu-gcc -static
endif
PLTDEFS		= -DIATO -DOS_LINUX -D_REENTRANT -DHAS_PTHREAD
XPLTDEFS	= -DIATO -DOS_LINUX
PLTINCS		= -I$(LELFHDR)
XPLTINCS	= 
PLTLIBS		= $(LELFLIB)
XPLTLIBS	=
endif

# add g++ options
ifeq ($(CC),g++)
ifeq ($(CCMODE),OPTIMIZED)
CCFLAGS	 	= -Wall -O2 -MMD -fPIC
XCCFLAGS	= -Wall -O2 -MMD
CCDEFINES	= $(PLTDEFS)
XCCDEFINES	= $(XPLTDEFS)
else
ifeq ($(CCMODE),PROFILED)
CCFLAGS	 	= -Wall -g -MMD -pg -fPIC
XCCFLAGS	= -Wall -O2 -MMD
LKFLAGS		= -pg
CCDEFINES	= $(PLTDEFS)
XCCDEFINES	= $(XPLTDEFS)
else
CCFLAGS	 	= -Wall -Wswitch -Woverloaded-virtual -Wno-error -g -MMD -fPIC
XCCFLAGS	= -Wall -Wswitch -Wno-error -g -MMD
CCDEFINES	= -DDEBUG $(PLTDEFS)
XCCDEFINES	= -DDEBUG $(XPLTDEFS)
endif
endif
endif

# add ecpc options
ifeq ($(CC),ecpc)
XAS		= as -x
XLD		= ld
XLK		= ecpc
ifeq ($(CCMODE),OPTIMIZED)
CCFLAGS	 	= -O2 -KPIC
XCCFLAGS	= -O2
CCDEFINES	= -DDEBUG $(PLTDEFS)
XCCDEFINES	= -DDEBUG $(XPLTDEFS)
else
CCFLAGS	 	= -g -KPIC
XCCFLAGS	= -g
CCDEFINES	= -DDEBUG $(PLTDEFS)
XCCDEFINES	= -DDEBUG $(XPLTDEFS)
endif
endif
