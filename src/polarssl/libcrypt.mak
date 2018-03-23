#
# $Id: libcrypt.mak 2012-10-09 17:43:18Z dodoworm $
#
TOPDIR  = $(shell pwd)/../..

APPNAME = crypt

APPOBJS = \
		bignum.o \
		md.o \
		md_wrap.o \
		md2.o \
		md4.o \
		md5.o \
		sha1.o \
		sha2.o \
		sha4.o \
		rsa.o \
		asn1parse.o \
		pem.o \
		x509parse.o \
		base64.o \
		aes.o \
		des.o \
		padlock.o \



APP_CMPFLGS = -I./ -D_FILE_OFFSET_BITS=64 -Wall -W -Wdeclaration-after-statement

APP_LDDFLGS = 

# app build type: bldexe blddll bldslb bldexe_c blddll_c bldslb_c
APP_BUILDTYPE = blddll_c
all: $(APP_BUILDTYPE)
clean: cleanup

bldexe blddll bldslb bldexe_c blddll_c bldslb_c cleanup:
	@TOPDIR="$(TOPDIR)"; \
	APPNAME="$(APPNAME)"; \
	APPOBJS="$(APPOBJS)"; \
	APP_CMPFLGS="$(APP_CMPFLGS)"; \
	APP_LDDFLGS="$(APP_LDDFLGS)"; \
	APP_BUILDTYPE="$(APP_BUILDTYPE)"; \
	export TOPDIR APPNAME APPOBJS APP_CMPFLGS APP_LDDFLGS APP_BUILDTYPE; \
	$(MAKE) -f $(TOPDIR)/makstand $@
