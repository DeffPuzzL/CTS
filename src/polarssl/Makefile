# #####################################################################
#                              Makefile
# #####################################################################

# Library's name
SONAME =	libiboxcrypt.so

# Name of source files
SRC =	\
		bignum.c \
		md.c \
		md_wrap.c \
		md2.c \
		md4.c \
		md5.c \
		sha1.c \
		sha2.c \
		sha4.c \
		rsa.c \
		asn1parse.c \
		pem.c \
		x509parse.c \
		base64.c \
		aes.c \
		des.c \
		padlock.c \

# Name of header files
INCLUDES =  \
		bignum.h \
		md.h \
		md_wrap.h \
		md2.h \
		md4.h \
		md5.h \
		sha1.h \
		sha2.h \
		sha4.h \
		rsa.h \
		pem.h \
		base64.h \
		aes.h \
		des.h \
		padlock.h \
		asn1.h \
		bn_mul.h \
		cipher.h \
		config.h \
		x509.h

# #####################################################################

# Paths to header files
IPATH   =       -I./include  -I${HOME}/include -I${HOME}/include/polarssl
# Path to libraries and lib's names
LDPATH  =       -L.
# Compiler options
OPTIMIZATION?=-O2
WARNINGS= -Wall -W -Wstrict-prototypes -Wwrite-strings
WARNINGS+= -W -Wdeclaration-after-statement
DEBUG?= -g -ggdb

# #####################################################################

CC      =	gcc
RM      =	/bin/rm -f
OBJS    =	$(SRC:.c=.o)

# Objects compilation options
REAL_CFLAGS=$(OPTIMIZATION) $(CFLAGS) $(WARNINGS) $(DEBUG) $(IPATH)
REAL_CFLAGS+= -fPIC -pthread
REAL_CFLAGS+= -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_THREAD_SAFE
REAL_CFLAGS+= -D_FILE_OFFSET_BITS=64
# Link options
REAL_LDFLAGS=$(LDFLAGS) $(DEBUG) 
REAL_LDFLAGS+=-shared -Wl,-soname,$(SONAME)
# #####################################################################

.PHONY: clean all

$(SONAME): $(OBJS) $(SRC)
	$(CC) $(OBJS) $(REAL_LDFLAGS) -o $(SONAME)
	mv $(SONAME) $(HOME)/lib

clean:
	$(RM) $(OBJS) $(SONAME) *~

all: clean $(NAME)

.c.o:
	$(CC) $(REAL_CFLAGS) -c $<
