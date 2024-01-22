# Makefile for the xsh dynamic program

VMROOT	=	../../../..
VMLIB	=	$(VMROOT)/lib
SLIB	=	$(VMLIB)/unixsyscall.a

UNIXCLIB = 	$(VMLIB)/libxc.a
LIB_DEFINE=	-DSIMPLE_LIB

# have a builtin echo command
BUILTIN_ECHO=	-DBUILTIN_ECHO

# to keep from includeing alloca.h, define _alloca_h
DEFINES	=	$(LIB_DEFINE) $(BUILTIN_ECHO) -D_alloca_h

ROOTDIR	=	../../../..

MYLIB	= 	$(ROOTDIR)/lib/readline.a
#OTHERLIB=	-lcurses -ltermlib /u3/sdo/dynamic/lib/xtralib.a
OTHERLIB=	-lcurses -ltermlib 

BINDIR  =	$(ROOTDIR)/bin
NAME	=	xsh
INCLUDE	= 	-I../h -I${VMROOT}/include
CFLAGS 	=	-g $(INCLUDE) $(DEFINES)

CC	=	cc -fsoft   $(CFLAGS) 
LD	=	ld -T 2020 -e _unixstartup -Bstatic
#MAKETD	=	/usr/local/bin/maketd
MAKETD	=	../../../../../xinu/src/tools/maketd

SRC 	= 	scanner.l main.c parser.y hash.c processcmd.c 	\
		processbultin.c processdynamic.c varsub.c 	\
		parse.c utils.c reporterror.c fileexist.c	\
		bultincmds.c xinu.c io.c g_hash.c mem.c		\
		doit.c lexio.c 

SRCg 	= 	lex.yy.c y.tab.c

OBJ 	= 	y.tab.o lex.yy.o 				\
	 	main.o hash.o processcmd.o 			\
		processbultin.o processdynamic.o varsub.o 	\
		parse.o utils.o reporterror.o fileexist.o	\
		bultincmds.o xinu.o io.o g_hash.o mem.o		\
		doit.o lexio.o setjmp.o

HDR 	= 	main.h tn.h hash.h command.h parse.h g_hash.h xsys.h

HDRg	=	y.tab.h

DEP 	=	$(SRC) $(SRCg)

xsh:		$(OBJ)
		$(LD) -o xsh $(OBJ) $(MYLIB) $(OTHERLIB) $(UNIXCLIB)	\
			 $(SLIB)

install:	$(BINDIR)/$(NAME)

$(BINDIR)/$(NAME):	xsh
		cp xsh $(BINDIR)/$(NAME)

.c.o:
		$(CC) -c $<
.s.o:
	/lib/cpp  $< > .tmp.s
	${CC} -c  -o $@ .tmp.s

lex.yy.c: scanner.l
	lex scanner.l

y.tab.c: parser.y 
	yacc -dv parser.y
	-(cmp -s y.tab.h ../h/tn.h || cp y.tab.h ../h/tn.h)

print: $(SRC) Makefile README ../h/$(HDR)
	lpr -Pmalory $(SRC) Makefile README ../h/$(HDR)

clean:
	rm -f $(OBJ) $(SRCg) $(HDRg) a.out xsh y.output

depend:
	$(MAKETD) $(INCLUDE) $(DEFINES) $(DEP)
	
# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
# Dependencies generated at: Fri Aug 17 14:24:58 EST 1990

bultincmds.o: ../../../../include/ctype.h
bultincmds.o: ../../../../include/ip.h
bultincmds.o: ../../../../include/sys/xinuconf.h
bultincmds.o: ../../../../include/tty.h
bultincmds.o: ../h/command.h
bultincmds.o: ../h/hash.h
bultincmds.o: ../h/main.h
bultincmds.o: ../h/utils.h
bultincmds.o: ../h/xsys.h
bultincmds.o: bultincmds.c
doit.o: ../h/hash.h
doit.o: ../h/main.h
doit.o: ../h/parse.h
doit.o: ../h/tn.h
doit.o: ../h/utils.h
doit.o: doit.c
fileexist.o: ../../../../include/ctype.h
fileexist.o: ../h/hash.h
fileexist.o: ../h/utils.h
fileexist.o: fileexist.c
g_hash.o: ../h/hash.h
g_hash.o: ../h/main.h
g_hash.o: ../h/utils.h
g_hash.o: g_hash.c
hash.o: ../h/hash.h
hash.o: hash.c
io.o: ../h/hash.h
io.o: ../h/main.h
io.o: ../h/utils.h
io.o: io.c
lex.yy.o: ../h/hash.h
lex.yy.o: ../h/main.h
lex.yy.o: ../h/tn.h
lex.yy.o: ../h/utils.h
lex.yy.o: lex.yy.c
lexio.o: ../h/hash.h
lexio.o: ../h/main.h
lexio.o: ../h/utils.h
lexio.o: lexio.c
main.o: ../../../../include/ip.h
main.o: ../../../../include/sys/xinuconf.h
main.o: ../../../../include/tty.h
main.o: ../h/hash.h
main.o: ../h/main.h
main.o: ../h/parse.h
main.o: ../h/utils.h
main.o: ../h/xsys.h
main.o: main.c
mem.o: mem.c
parse.o: ../h/hash.h
parse.o: ../h/main.h
parse.o: ../h/parse.h
parse.o: ../h/utils.h
parse.o: parse.c
processbultin.o: ../h/command.h
processbultin.o: ../h/parse.h
processbultin.o: ../h/utils.h
processbultin.o: processbultin.c
processcmd.o: ../h/main.h
processcmd.o: ../h/parse.h
processcmd.o: processcmd.c
processdynamic.o: ../../../../include/ip.h
processdynamic.o: ../../../../include/sys/xinuconf.h
processdynamic.o: ../../../../include/tty.h
processdynamic.o: ../h/main.h
processdynamic.o: ../h/parse.h
processdynamic.o: ../h/xsys.h
processdynamic.o: processdynamic.c
reporterror.o: ../h/main.h
reporterror.o: reporterror.c
utils.o: ../h/command.h
utils.o: ../h/parse.h
utils.o: ../h/utils.h
utils.o: utils.c
varsub.o: ../../../../include/ctype.h
varsub.o: ../h/hash.h
varsub.o: ../h/main.h
varsub.o: ../h/parse.h
varsub.o: ../h/utils.h
varsub.o: varsub.c
xinu.o: ../../../../include/ctype.h
xinu.o: ../h/hash.h
xinu.o: ../h/main.h
xinu.o: ../h/utils.h
xinu.o: xinu.c
y.tab.o: ../h/hash.h
y.tab.o: ../h/main.h
y.tab.o: ../h/parse.h
y.tab.o: ../h/utils.h
y.tab.o: y.tab.c

# DO NOT ADD ANYTHING HERE - WILL GO AWAY
