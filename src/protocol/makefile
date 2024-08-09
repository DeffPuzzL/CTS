
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
