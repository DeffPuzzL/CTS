
PROCINCLUDE=include=$(ORACLE_HOME)/precomp/public include=$(HOME)/include include=$(HOME)/include include=$(HOME)/incl_c
INCLUDE= -I/usr/include/libxml2 -I$(ORACLE_HOME)/precomp/public -I$(HOME)/include -I$(HOME)/incl_c -I./ -I./include 
APPIND=-I$(HOME)/include/tvminc -I$(HOME)/include/adainc
LIBDIR= -L$(HOME)/lib -lcts -lstvm -lcomm 
ORALIB= -L$(ORACLE_HOME)/lib -lpthread -lxml2 -lcurl -lm -ldl -lc -lnsl 
PROC=proc
CC=cc -fPIC
CFLAGS= -shared -fPIC -g 

OBJFILE=userpack.o
TARGET=libuser.so

all: $(TARGET) clean
$(TARGET): $(OBJFILE)
	$(CC) $(CFLAGS) -o $@ $(OBJFILE) $(LIBDIR) $(ORALIB)
	mv $@ $(HOME)/lib

.SUFFIXES: .pc .c .o

.pc.o:
	$(PROC) $(PROCINCLUDE) dbms=v7 unsafe_null=yes lines=yes parse=none iname=$*.pc
	$(CC) -g $(CFLAGS) -w $(INCLUDE) w$(APPIND) -c $*.c $(LIBDIR) $(ORALIB)
	@rm -f $*.c
	@rm -f $*.lis

.c.o:
	$(CC) -g $(INCLUDE) $(APPIND) -c $*.c 

fall:
	@touch *c
	@make all
clean:
	rm -f *.o 
