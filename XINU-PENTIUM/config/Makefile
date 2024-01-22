#
# Make the Xinu configuration program
#
CC	= /usr/local/gnu/gcc	
LEX	= /usr/ccs/bin/lex
YACC	= /usr/ccs/bin/yacc
DEFS	=
CFLAGS	= -traditional ${DEFS}

all:		config

config:		lex.yy.c y.tab.c
		${CC} ${CFLAGS} -o config y.tab.c -ll

lex.yy.c:	config.l
		${LEX} config.l

y.tab.c:	config.y
		${YACC} config.y

clean:
		/bin/rm -f lex.yy.c y.tab.c core
