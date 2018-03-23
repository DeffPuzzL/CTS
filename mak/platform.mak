#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix.mak                                           |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc -g -O0 -Werror
//CC    = cc -g
CXX   = g++ -g
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -fpic
CXXFLG=
EXFLG =
DBFLG = -g
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm -lc -ldl -lpthread -lssl
DLLIBS = -ldl
NETLIBS = 
CURLIBS = -lcurses
CRYLIBS = -lcrypt
THREADLIBS = -lpthread
X11LIBS = -lXext -lX11
X11SMLIBS= -lICE -lSM
XLIBS   = $(X11SMLIBS) $(DLLIBS) $(X11LIBS)
XLIBP   = /usr/X11R6/lib
OSDEF = -DREDHAT_LINUX
OSTYPE = rhlux
