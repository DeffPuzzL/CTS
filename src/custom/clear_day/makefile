#|----------------------------------------------------------------------------|

include $(HOME)/mak/platform.mak
include $(HOME)/mak/database.mak
include $(HOME)/mak/tuxedo.mak

PRGOBJS = clear_day.o main.o 
        
PRGTARG = clear_day
PRGLIBS = -lcts -lstvm -ldbsapi -lcomm -lxml2 -lprotocol -lcurl $(NETLIBS) $(UXLIBS) $(DBLIBS)
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(HOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)" $(HOME)/include
