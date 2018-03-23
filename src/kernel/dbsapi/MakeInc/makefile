#******************************************************************************#
#* 说明::本工程文件适应于如下配置:                                            *#
#*       操作系统  : AIX5                                                     *#
#*       数据库系统: ORACLE9i                                                 *#
#*       编译器    : gcc                                                      *#
#*----------------------------------------------------------------------------*#
#* 编者：Li JunXu    编写日期:20030112   修改者:           修改日期:          *#
#******************************************************************************#

PROCINCLUDE=include=$(ORACLE_HOME)/precomp/public include=$(SWHOME)/include include=$(HOME)/include include=$(HOME)/incl_c
APPINCLUDE= -I../incl -I$(ORACLE_HOME)/precomp/public -I$(HOME)/include -I$(HOME)/include/tvminc -I$(HOME)/incl_c -I/usr/include -I/usr/include/libxml2 
SYS_INCLUDE='sys_include=(/usr/include)'
ORALIBDIR= -L$(ORACLE_HOME)/lib
APPLIBDIR= -L$(HOME)/lib 
DBSDIR= $(HOME)/dbsvr
BINDIR= $(DBSDIR)/bin
LIBDIR= $(HOME)/lib
USERID=scott/tiger
PROC=proc 
CC=gcc 
ORALIB= -lclntsh -ldl -lm -lpthread -lnsl -lirc -lipgo -lpthread  `cat $(ORACLE_HOME)/lib/sysliblist`
CFLAGS= -lxml2 -DLINUX -D__DEBUG__ -DNOWHAT=1 -D_THREAD_SAFE

#############################################
##           USER DEFINE PART			   ##
#############################################
TUSERLIB =
TARGET= MakeDbInc

#############################################
#############################################

#############################################
##           PUBLIC PART				   ##
#############################################
all: $(TARGET) 
$(TARGET): gendbch.o
	$(CC) -g $(ORALIBDIR) $(APPLIBDIR) $(CFLAGS) $(APPINCLUDE) $< -o $@ $(ORALIB) $(TUSERLIB) -L$(HOME)/lib -lcomm -lxml2 -lm -lc

	@echo "$@ is OK!"
.SUFFIXES: .pc .c .o

.pc.o:
	$(PROC) lines=yes define=_DB_ORA $(PROCINCLUDE) code=ANSI_C parse=none MODE=ORACLE DBMS=V8 unsafe_null=yes iname=$*.pc
	$(CC) -g -rdynamic -o $@ $(CFLAGS) -w $(APPINCLUDE) -c $*.c
	@rm -f $*.c
	@rm -f $*.lis

.c.o:
	$(CC)  $(CFLAGS) -w $(APPINCLUDE) -c $*.c

fall:
	@touch *c
	@make all
clean:
	rm -f *.o *.so *.c *~ *.lis
