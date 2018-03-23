#|----------------------------------------------------------------------------|
#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.exec.mak                                             |
#| DESCRIPTIONS : Sample makefile for executable binary                       |
#|----------------------------------------------------------------------------|

include $(BATHOME)/mak/platform.mak
include $(BATHOME)/mak/database.mak
include $(BATHOME)/mak/tuxedo.mak

PRGOBJS = dfile.o gendb.o top_cfg_parser.o top_ipath_parser.o top_map_parser.o top_sql_parser.o
#PRGOBJS = settle_algo.o main.o
        
PRGTARG = gendb
PRGLIBS = $(NETLIBS) $(UXLIBS) $(DBLIBS)
PRGDEFS = -DDBD=_DB_ORA

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!
releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)" $(HOME)/include
