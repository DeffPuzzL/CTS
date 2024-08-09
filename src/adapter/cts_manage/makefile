
include $(BATHOME)/mak/platform.mak
include $(BATHOME)/mak/database.mak
include $(BATHOME)/mak/tuxedo.mak

#PRGOBJS = posp_ctsin.o epoll.o shthread.o
PRGOBJS = cts_manage.o 
        
PRGTARG = cts_manage
PRGLIBS = -lcts -lstvm -lcomm -ldbsapi -lprotocol -lxml2 -lcurl $(NETLIBS) $(UXLIBS) $(DBLIBS) `xml2-config --libs`
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)" $(HOME)/include
