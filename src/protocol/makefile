#|--------------------------------------------------------------------------|
#|						   TOPMake 2.0										|
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.		|
#|	All Rights Reserved														|
#|--------------------------------------------------------------------------|
#| FILE NAME	: sample.dynamiclib.mak										|
#| DESCRIPTIONS : Sample makefile for dynamic library						|
#|--------------------------------------------------------------------------|

include $(HOME)/mak/platform.mak
include $(HOME)/mak/database.mak
include $(HOME)/mak/tuxedo.mak

PRGOBJS = cJSON.o 
PRGTARG = protocol
PRGDEFS = 

# used for db2 database
PRGDBNM = $(DBLINK)
PRGLIBS = -lcts -ltvm -lDbsApi -lBatCom -liboxcrypt -lxml2 $(NETLIBS) $(UXLIBS) $(DBLIBS)

debug all: debugstatic
release: releasestatic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(HOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
