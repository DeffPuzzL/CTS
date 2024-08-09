
include $(BATHOME)/mak/platform.mak
include $(BATHOME)/mak/database.mak
include $(BATHOME)/mak/tuxedo.mak

PRGOBJS = http_client.o
        
PRGTARG = http_client
PRGLIBS = -lcts -lstvm -lcomm -ldbsapi -lprotocol -lpthread -lxml2 -lcurl $(NETLIBS) $(UXLIBS) $(DBLIBS)
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)" $(HOME)/include
