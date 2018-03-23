include $(HOME)/mak/platform.mak
include $(HOME)/mak/database.mak
include $(HOME)/mak/tuxedo.mak

PRGOBJS = \
          dbsvr_tbl_r_cha_con.o \
          dbsvr_tbl_r_cha_mod.o \
          dbsvr_tbl_r_cha_param.o \
          dbsvr_tbl_r_cmer_con.o \
          dbsvr_tbl_r_cmer_mod.o \
          dbsvr_tbl_r_cmer_record.o \
          dbsvr_tbl_r_lmer_con.o \
          dbsvr_tbl_r_lmer_mod.o \
          dbsvr_tbl_r_special_mcht.o \
          dbsvr_tbl_r_sync_mcht.o \
          dbsvr_tbl_r_txn.o \

PRGTARG = Dbs
PRGLIBS = $(DBLIBS) $(UXLIBS)
PRGDEFS =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(HOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
