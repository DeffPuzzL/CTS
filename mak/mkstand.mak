#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : mksstand.mak                                                |
#| DESCRIPTIONS : Makefile for all kind of platform, for c and pc file,       |
#|                for lib(dynamic & static) and execuatable object            |
#|----------------------------------------------------------------------------|

include $(BATHOME)/mak/platform.mak
include $(BATHOME)/mak/database.mak
include $(BATHOME)/mak/tuxedo.mak

# environment variables must be define outside of makefile
#
# $FEHOME : home directory of application 
# $CCFLG  : compiler options for c source code
# $CXXFLG : compiler options for c++ source code
# $DLFLG  : link options for dynamic library
# $SLFLG  : link options for static library
# $EXFLG  : link options for excecuteable object
# $DBFLG  : debug mode options
# $OPFLG  : optimize mode options
# $DLFIX  : suffix of dynamic library
# $SLFIX  : suffix of static library

# $OBJS   : c object files list
# $LIBS   : libs required for executable object
# $DEFS   : define flag list
# $TARGET : target objetc name

# $TUXSVCS: tuxedo services name
# $TUXCLTTARG: tuxedo client name
# $TUXRSMN: tuxedo resource manager name

# $DBNM   : db name (for db2)

# commands
 
PC		=	proc
EC		=	esql
DB2		=	db2

JAVAC   =   javac
JAVAH   =   javah
RMIC    =   rmic

RM		=	rm -f
CP		=	cp
MV		=   mv
CD		=   cd
ECHO	=	@echo

# paths and flags

SYSINCP	=	/usr/include
XMLINCP	=	/usr/include/libxml2
SYSLIBP	=	
JINCP	=	$(JAVA_HOME)/include
JMDINCP	=	$(JAVA_HOME)/include/$(JAVAMD)

SRCP	=	$(BATHOME)/src
BINP	= 	$(BATHOME)/bin
LIBP	=	$(BATHOME)/lib
INCP	=	$(BATHOME)/include 
TVMINC	=	$(BATHOME)/include/tvminc
ADAINC	=	$(BATHOME)/include/adainc
DBSINC	=	$(BATHOME)/include/dbsinc
CLSP    =   $(BINP)/classes
JLIBP   =   $(JAVA_HOME)/lib
BNDDIR  =   $(HOME)/bnd

INCPS	=   -I$(SYSINCP) -I$(XMLINCP) -I$(INCP) -I$(DBINCP) -I$(TVMINC) -I$(ADAINC) -I$(DBSINC) -I$(TUXINCP) -I$(JINCP) -I$(JMDINCP)
LIBPS	=   -L$(SYSLIBP) -L$(XLIBP) -L$(LIBP) $(DBLIBP) -L$(TUXLIBP)

CCFLGS	=   $(RLFLG) $(CCFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS)
CXXFLGS =   $(RLFLG) $(CXXFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS)
DLFLGS	=   $(DLFLG) $(LIBPS)
EXFLGS	=   $(EXFLG) $(LIBPS)
 
DLTARG	= $(LIBP)/lib$(TARGET)$(DLFIX)
SLTARG	= $(LIBP)/lib$(TARGET)$(SLFIX)
EXTARG	= $(BINP)/$(TARGET)
#EXTARG	= $(TARGET)
TUXTARG	= $(TUXBINP)/$(TARGET)
TUXCLTTARG	= $(CLIBINP)/$(TARGET)

debugdynamic: setdebug $(DLTARG)
releasedynamic: setrelease $(DLTARG)
debugstatic: setdebug $(SLTARG)
releasestatic: setrelease $(SLTARG)
debugexec: setdebug $(EXTARG)
releaseexec : setrelease $(EXTARG)
debugtuxsvrexec: setdebug $(TUXTARG)
releasetuxsvrexec : setrelease $(TUXTARG)
debugtuxcltexec: setdebug $(TUXCLTTARG)
releasetuxcltexec : setrelease $(TUXCLTTARG)

javaclasses : $(OBJS)
	$(ECHO) "Finish java package [$(TARGET)]"
rmi :
	$(ECHO) "rmic [$(TARGET)] ..."
	@$(CD) `dirname $(TARGET)`; \
	$(RMIC) `basename $(TARGET)`

setdebug:
	$(ECHO) $(DBFLG) > $(BATHOME)/mak/.mak.tmp

setrelease:
	$(ECHO) $(OPFLG) > $(BATHOME)/mak/.mak.tmp

$(SLTARG): $(OBJS) 
	@$(RM) $@
	@$(SLINK) $(SLFLG) $@ $(OBJS)
	@$(RM) $(BATHOME)/mak/.mak.tmp
	@$(RM) -rf *.o

$(DLTARG): $(OBJS) 
	@if [ $(OSTYPE) = aix ]; \
	then \
		$(DLINK) -g -o $@ $(DLFLGS) $(OBJS) $(LIBS) $(UXLIBS); \
	else \
		$(DLINK) -g -o `basename $@` $(DLFLGS) $(OBJS) $(LIBS); \
		$(MV) `basename $@` `dirname $@`; \
	fi
	@$(RM) $(FEHOME)/online/mak/.mak.tmp

$(EXTARG): $(OBJS)
	$(ECHO) "Linking   [$(EXTARG)] ..."
	$(CXX) -Wall -Wno-write-strings -Wno-unused -o $@ $(EXFLGS) $(OBJS) $(LIBS)
	$(RM) $(BATHOME)/mak/.mak.tmp

#$(TUXTARG): $(OBJS)
#	$(ECHO) "buildserver [$(TUXTARG)] ..."
#	TUXDIR="$(TUXDIR)" \
#	CFLAGS="$(EXFLGS)" \
#	$(TUXBS) -v -t -o $@  -f "$(TUXLIBPS)"  -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)"

#$(TUXCLTTARG): $(OBJS)
#	$(ECHO) "buildclient [$(TUXCLTTARG)] ..."
#	$(TUXBC)  -o $@ -f "$(OBJS)" -f "$(EXFLGS)" -f "$(LIBS)

.SUFFIXES:
.SUFFIXES: .java .class .xc .ec .pc .sqc .c .cpp .pcpp .o
 
.xc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*$(DBFIX)
	@$(CP) $*.xc $*$(DBFIX)
	@if [ $(DBTYPE) = ora ]; \
	then \
		$(PC) define=$(DBD) include=$(TUXINCP) include=$(INCP) include=$(JINCP) include=$(JMDINCP) code=ANSI_C iname=$*$(DBFIX); \
		$(CC) -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c; \
		$(RM) $*.lis; \
	else \
		$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
		$(CC) -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c; \
		$(RM) `basename $*`.c; \
	fi
	@$(RM) $*$(DBFIX) $*.c

.pcpp.o:
#	@$(PC) lines=yes define=$(DBD) include=$(TUXINCP) include=$(INCP) include=$(DBINCP) code=ANSI_C iname=$*$(DBFIX)
#	$(PC) lines=yes define=$(DBD) include=$(INCP) include=$(TVMINC) include=$(DBINCP) include=$(TUXINCP) code=ANSI_C parse=none MODE=ORACLE DBMS=V8 UNSAFE_NULL=yes iname=$*$(DBFIX)
	$(PC) lines=yes define=$(DBD) include=$(INCP) include=$(TVMINC) include=$(ADAINC) include=$(DBSINC) include=$(DBINCP) include=$(TUXINCP) code=CPP parse=none char_map=string MODE=ORACLE DBMS=V8 UNSAFE_NULL=yes iname=$*$(DBFIXCPP)
#	$(CXX) -rdynamic -Wall -Wno-write-strings -Wunused  -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c
	$(CXX) -rdynamic -Wno-write-strings -Wno-unused  -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c
	$(RM) $*.lis $*.c

.pc.o:
	$(ECHO) "Compiling [$@] ..."
#	@$(PC) lines=yes define=$(DBD) include=$(TUXINCP) include=$(INCP) include=$(DBINCP) code=ANSI_C iname=$*$(DBFIX)
	$(PC) lines=yes define=$(DBD) include=$(INCP) include=$(TVMINC) include=$(ADAINC) include=$(DBSINC) include=$(DBINCP) include=$(TUXINCP) code=ANSI_C parse=none char_map=string MODE=ORACLE DBMS=V8 UNSAFE_NULL=yes iname=$*$(DBFIX)
	$(CC) -rdynamic -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c
	@$(RM) $*.lis $*.c

.ec.o:
	$(ECHO) "Compiling [$@] ..."
	$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
	$(CC) -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c; \
	$(RM) `basename $*`.c; \

.sqc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*.bnd
	@$(DB2) connect to $(DBNM)
	-$(DB2) prep $< bindfile USING $(BNDDIR)/$*.bnd VERSION LDCARD
	$(DB2) bind $(BNDDIR)/$*.bnd DATETIME iso dynamicrules bind 
	@$(DB2) connect reset
	@$(DB2) terminate
	$(CD) `dirname $@`; \
        $(CC) -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c
	
.cpp.o:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@` ;
	$(CXX) -Wall -Wno-write-strings -Wno-unused -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CXXFLGS) -c `basename $<`

.c.o:
	$(ECHO) "Compiling [$@] ..."
	$(CD) `dirname $@`; \
	$(CC) -o $@ `cat $(BATHOME)/mak/.mak.tmp` $(CCFLGS) -c $(INCPS) `basename $<`
 
.java.class:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@`; \
	$(JAVAC) `basename $<`

# clearing object codes
 
clean: cleanup
	@$(RM) $(SLTARG) $(DLTARG) $(EXTARG) $(TUXTARG)
 
cleanup:
	@$(RM) $(OBJS)
	@$(RM) $(patsubst %.pcpp, %.c, $(shell ls *pcpp 2>/dev/null))
	@$(RM) $(patsubst %.pcpp, %.lis, $(shell ls *pcpp 2>/dev/null))
	@$(RM) $(patsubst %.pc, %.c, $(shell ls *pc 2>/dev/null))
	@$(RM) $(patsubst %.pc, %.lis, $(shell ls *pc 2>/dev/null))
