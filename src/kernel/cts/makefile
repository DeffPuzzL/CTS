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

PRGOBJS = 
        
PRGTARG = cts 
PRGLIBS = -lstvm -lcomm -ldbsapi -lprotocol -lxml2 -lcurl $(NETLIBS) $(UXLIBS) $(DBLIBS) `xml2-config --libs`
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f make.lib
	@make -f make.cts
	@make -f make.core
	@make -f make.mon
	@make -f make.msgex
	@make -f make.tool
	@make -f make.alarm
