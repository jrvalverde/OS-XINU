#
# Makefile for Mg 2a
#

PROGNAME =	mg

.SUFFIXES:.o .c .s

#
# Make variables
#
ROOTDIR	=	../../..
BIN	=	$(ROOTDIR)/bin
LIB	=	$(ROOTDIR)/lib
INC	=	$(ROOTDIR)/include
SLIB	=	$(LIB)/unixsyscall.a
CLIB	=	$(LIB)/libxc.a
LD	=	/bin/ld -T 2020 -e _unixstartup
CC	=	/bin/cc
AS	=	/bin/as
SYS	= bsd
LIBS	= -ltermcap

# CDEFS gets defines, and gets passed to lint. CFLAGS gets flags, and doesn't
# get passed to lint.
#
# Compile-time options that MUST be defined for Xinu v8:
#
#	XINU8
#
# Compile-time options that (seem to) work with Xinu v8:
#
#	BSMAP		-- enable swapping of Delete and BS
#	REGEX		-- create regular expression functions
#	GOSREC		-- do Gosling style recenters
#	GOSMACS		-- enable a few Gosling functions
#	DO_METAKEY	-- if bit 7 is set for a key, treat like a META key
#	PREFIXREGION	-- enable function "prefix-region"
#	CVMVAS		-- argument to scroll-(up, down) is screens, not lines
#	NO_BACKUP	-- enable "make-backup-files"
#	NO_STARTUP	-- do not look for and handle initialization file
#	NO_MACRO	-- do not handle keyboard macros (see docs)
#	NO_HELP		-- disable the help functions
#	NO_SPAWN	-- disable the suspend-emacs function
#	NO_DPROMPT	-- disable delayed, multi-key prompts
#	NO_DIR		-- disable directory operations
#	NO_DIRED	-- disable directory edit operations
#
# This one is untested:
#
#	XKEYS		-- use termcap function key definitions. Warning -
#				XKEYS and bsmap mode do _not_ get along.
#
# NOTE: There are other options in the source code.  Most do not apply to
#       this system so should not be defined.  A few may apply and are
#	automatically set for this system.  The remaining options are unknown
#	and may or may not function under Xinu v8.
#
CDEFS=	-DXINU8 -DBSMAP=1 -DREGEX -DGOSREC -DGOSMACS -DDO_METAKEY -DPREFIXREGION -DCVMVAS
CFLAGS=	-g -I$(INC) $(CDEFS)

# Objects which only depend on the "standard" includes
OBJS=	basic.o dir.o dired.o file.o line.o match.o paragraph.o 	\
	random.o region.o search.o version.o window.o word.o

# Those with unique requirements
IND=	buffer.o display.o echo.o extend.o help.o kbd.o keymap.o 	\
	macro.o modes.o regex.o re_search.o

# System dependent objects
OOBJS = alloca.o chdir.o cinfo.o fastgetc.o fastputc.o getwd.o ls.o	\
	spawn.o ttyio.o tty.o ttykbd.o xmalloc.o

XOFILES= $(OBJS) $(IND) $(OOBJS) fileio.o

OSRCS=	alloca.c chdir.c cinfo.c fastgetc.c fastputc.c fileio.c getwd.c	\
	ls.c spawn.c ttyio.c tty.c ttykbd.c xmalloc.c
SRCS= 	basic.c dir.c dired.c file.c line.c match.c paragraph.c 	\
	random.c region.c search.c version.c window.c word.c buffer.c	\
	display.c echo.c extend.c help.c kbd.c keymap.c macro.c main.c	\
	modes.c regex.c re_search.c

OINCS=	ttydef.h sysdef.h chrdef.h
INCS=	def.h

CFILES= $(OSRCS) $(SRCS)

.c.o:
		$(CC) -c $(CFLAGS) $<
.s.o:
		$(AS) $<

$(PROGNAME):	main.o $(XOFILES)
		$(LD) main.o $(XOFILES) $(LIBS) $(CLIB) $(SLIB) -o $(PROGNAME)

install:	$(PROGNAME)
		cp $(PROGNAME) $(BIN)/$(PROGNAME)
		ln -s $(BIN)/mg $(BIN)/emacs

clean:
		rm -f $(PROGNAME) *.o ,* .,* core

lint: $(SRCS) $(OSRCS) $(INCS) $(OINCS)
	lint -ahbz $(CDEFS) $(SRCS) $(OSRCS)

# strip mg once you're satisfied it'll run -- makes it much smaller
strip:
	strip mg

$(XOFILES):	$(INCS) $(OINCS)

dir.o search.o:	$(INCS) $(OINCS)

regex.o re_search.o:	$(INCS) $(OINCS) regex.h

kbd.o:	$(INCS) $(OINCS) macro.h kbd.h key.h

macro.o main.o:	$(INCS) $(OINCS) macro.h

buffer.o display.o keymap.o help.o modes.o dired.o fileio.o: \
	$(INCS) $(OINCS) kbd.h

extend.o:	$(INCS) $(OINCS) kbd.h macro.h key.h

help.o:	$(INCS) $(OINCS) kbd.h key.h macro.h

echo.o:	$(INCS) $(OINCS) key.h macro.h

$(OOBJS):	$(INCS) $(OINCS)

