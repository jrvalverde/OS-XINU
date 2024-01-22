## -*- text -*- ####################################################
#								   #
# Makefile for readline and history libraries.			   #
#								   #
####################################################################

ROOTDIR	    = ../../../..
LIBDIR	    = $(ROOTDIR)/lib
HDIR	    = ../h
DOCDIR	    = ../texinfo
INCLUDES    = -I. -I../h 

#MAKETD	    = /usr/local/bin/maketd
MAKETD	    = ../../../../../xinu/src/tools/maketd

# A good alternative is gcc -traditional.
# CC = gcc -traditional
CC	    = cc

# HP-UX compilation requires the BSD library.
#LOCAL_LIBS  = -lBSD

# Xenix compilation requires -ldir -lx
#LOCAL_LIBS  = -ldir -lx

# Define SIG_TYPE as -DVOID_SIGHANDLER if your operating system uses
# a return type of "void" for signal handlers.
#SIG_TYPE    = -DVOID_SIGHANDLER

# Define SYSV as -DSYSV if you are using a System V operating system.
#SYSV	    = -DSYSV

# Request the vi line editing mode and features.
#VI_DEFINE = -DVI_MODE

# Make malloc static ( so you may redefine xmalloc, xrealloc )
STATIC_MALLOC	=   -DSTATIC_MALLOC

# Make xinu compatable - don't use sparc builtin_alloca
XINU_DEFINE = -DXINU -D_alloca_h

DEFINES	    = $(VI_DEFINE) $(XINU_DEFINE) $(SIG_TYPE) $(SYSV) $(STATIC_MALLOC)

CFLAGS	    = -g $(DEFINES) $(INCLUDES) 

# Here is a rule for making .o files from .c files that doesn't force
# the type of the machine (like -sun3) into the flags.
.c.o:
	    $(CC) -c $(CFLAGS) $(CPPFLAGS) $*.c


RANLIB = /usr/bin/ranlib
AR = ar
RM = rm
CP = cp

CSOURCES    = readline.c history.c funmap.c keymaps.c emacs_keymap.c \
		keymaps.c xinu.c alloca.c vi_mode.c vi_keymap.c

OBJS	    = readline.o history.o funmap.o keymaps.o emacs_keymap.o \
		keymaps.o xinu.o alloca.o vi_mode.o vi_keymap.o

HSOURCES    = $(HDIR)/readline.h $(HDIR)/chardefs.h $(HDIR)/history.h \
		$(HDIR)/keymaps.h

SOURCES	    = $(CSOURCES) $(HSOURCES)

DOCUMENTATION = $(DOCDIR)/readline.texinfo $(DOCDIR)/inc-readline.texinfo \
		$(DOCDIR)/history.texinfo $(DOCDIR)/inc-history.texinfo

SUPPORT	    = COPYING Makefile $(DOCUMENTATION) ChangeLog

THINGS_TO_TAR = $(SOURCES) $(SUPPORT)

##########################################################################

readline.a:	$(OBJS)	$(HSOURCES)
		$(RM) -f readline.a
		$(AR) clq readline.a $(OBJS)
		-if [ -f $(RANLIB) ]; then $(RANLIB) readline.a; fi

readline.tar:	$(THINGS_TO_TAR)
		tar -cf readline.tar $(THINGS_TO_TAR)

readline.tar.Z:	readline.tar
		compress -f readline.tar

install:	$(LIBDIR)/readline.a 

$(LIBDIR)/readline.a: readline.a
#		-mv $(LIBDIR)/readline.a $(LIBDIR)/readline.old
		cp readline.a $(LIBDIR)/readline.a
		$(RANLIB) -t $(LIBDIR)/readline.a

clean:
		rm -f *.o *.a *.log *.cp *.tp *.vr *.fn *.aux *.pg *.toc

depend:
		$(MAKETD) $(DEFINES) $(INCLUDES) $(CSOURCES)
	
# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
# Dependencies generated at: Fri Aug 17 14:11:31 EST 1990

alloca.o: static_malloc.inc
alloca.o: alloca.c
emacs_keymap.o: ../h/chardefs.h
emacs_keymap.o: ../h/keymaps.h
emacs_keymap.o: ../h/readline.h
emacs_keymap.o: emacs_keymap.c
funmap.o: ../h/chardefs.h
funmap.o: ../h/keymaps.h
funmap.o: ../h/readline.h
funmap.o: static_malloc.inc
funmap.o: funmap.c
history.o: ../h/history.h
history.o: static_malloc.inc
history.o: history.c
keymaps.o: ../h/chardefs.h
keymaps.o: ../h/keymaps.h
keymaps.o: ../h/readline.h
keymaps.o: emacs_keymap.c
keymaps.o: static_malloc.inc
keymaps.o: keymaps.c
readline.o: ../h/chardefs.h
readline.o: ../h/history.h
readline.o: ../h/keymaps.h
readline.o: ../h/readline.h
readline.o: static_malloc.inc
readline.o: readline.c
vi_keymap.o: vi_keymap.c
vi_mode.o: vi_mode.c
xinu.o: ../../../../include/tty.h
xinu.o: xinu.c

# DO NOT ADD ANYTHING HERE - WILL GO AWAY
