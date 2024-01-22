#	{ConC-dir}/src/gen/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/gen

INCLUDE= ../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	SYS_.o \
	abort.o alarm.o atoi.o atol.o \
	calloc.o closedir.o crypt.o ctime.o ctype_.o \
	errlst.o execvp.o \
	fakcu.o frexp.o fstab.o \
	gcvt.o getenv.o getgrent.o getgrgid.o getgrnam.o getlogin.o \
	getpass.o getpwent.o getpwnamuid.o getttyent.o getttynam.o \
	getusershell.o getwd.o \
	index.o initgroups.o isatty.o \
	malloc.o mkstemp.o mktemp.o \
	ndbm.o nlist.o \
	opendir.o \
	perror.o popen.o psignal.o \
	qsort.o \
	random.o readdir.o regex.o rindex.o \
	scandir.o seekdir.o setegid.o seteuid.o setgid.o setrgid.o \
	setruid.o setuid.o siginterrupt.o siglist.o signal.o \
	sleep.o strcat.o strcmp.o strcpy.o strlen.o strncat.o \
	strncmp.o strncpy.o swab.o syslog.o system.o \
	telldir.o time.o timezone.o ttyname.o ttyslot.o \
	ualarm.o usleep.o \
	valloc.o

CFILES=	SYS_.c \
	abort.c alarm.c atoi.c atol.c \
	calloc.c closedir.c crypt.c ctime.c ctype_.c \
	errlst.c execvp.c \
	fakcu.c frexp.c fstab.c \
	gcvt.c getenv.c getgrent.c getgrgid.c getgrnam.c getlogin.c \
	getpass.c getpwent.c getpwnamuid.c getttyent.c getttynam.c \
	getusershell.c getwd.c \
	index.c initgroups.c isatty.c \
	malloc.c mkstemp.c mktemp.c \
	ndbm.c nlist.c \
	opendir.c \
	perror.c popen.c psignal.c \
	qsort.c \
	random.c readdir.c regex.c rindex.c \
	scandir.c seekdir.c setegid.c seteuid.c setgid.c setrgid.c \
	setruid.c setuid.c siginterrupt.c siglist.c signal.c \
	sleep.c strcat.c strcmp.c strcpy.c strlen.c strncat.c \
	strncmp.c strncpy.c swab.c syslog.c system.c \
	telldir.c time.c timezone.c ttyname.c ttyslot.c \
	ualarm.c usleep.c \
	valloc.c

SFILES= 

.s.o:
	/lib/cpp ${CPP_FLAGS} $*.s | ${AS} -o $*.o

.c.o:
	cc ${CFLAGS} $*.c

lib_flag:	${OBJS}
	touch lib_flag

errlst.o: errlst.c
	cc -S -I${INCLUDE} errlst.c
	ed - <:errfix errlst.s
	as -o errlst.o errlst.s
	/bin/rm -f errlst.s

clean:
	@echo "***  Cleaning ${DIR} ..."
	@/bin/rm -f ${OBJS} lib_flag

depend:
		@echo "***  Making ${DIR} depend  ..."
		@echo '^#include[ 	]*(<(DUMMY.x' > .ils
		@/bin/ls ${INCLUDE} >> .ils
		@echo 'DUMMY.x)>|")' >> .ils
		@(egrep -f .ils ${CFILES} ${SFILES} ${INCLUDE}/*.h	\
		| sed	-e 's/</"$${INCLUDE}\//;s/>/"/'			\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'		\
		| sed	-e 's/\.c/.o/'					\
			-e 's/\.s/.o/' >makedep)
		@echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		@echo '$$r makedep' >>eddep
		@echo 'w' >>eddep
		@cp Makefile .,oldmake
		@cp Makefile ,Makefile
		@ed - ,Makefile < eddep
		@/bin/rm eddep makedep .ils
		@echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		@echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		@echo '#	see make depend	above'>>,Makefile
		@/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
SYS_.o: ${INCLUDE}/stdio.h
alarm.o: ${INCLUDE}/kernel.h
alarm.o: ${INCLUDE}/errno.h
crypt.o: ${INCLUDE}/kernel.h
ctime.o: ${INCLUDE}/kernel.h
errlst.o: ${INCLUDE}/kernel.h
execvp.o: ${INCLUDE}/kernel.h
execvp.o: ${INCLUDE}/errno.h
fstab.o: ${INCLUDE}/kernel.h
fstab.o: ${INCLUDE}/stdio.h
getgrent.o: ${INCLUDE}/kernel.h
getgrent.o: ${INCLUDE}/stdio.h
getgrgid.o: ${INCLUDE}/kernel.h
getgrnam.o: ${INCLUDE}/kernel.h
getlogin.o: ${INCLUDE}/kernel.h
getpass.o: ${INCLUDE}/kernel.h
getpass.o: ${INCLUDE}/stdio.h
getpwent.o: ${INCLUDE}/kernel.h
getpwent.o: ${INCLUDE}/stdio.h
getpwnamuid.o: ${INCLUDE}/kernel.h
getpwnamuid.o: ${INCLUDE}/stdio.h
getttyent.o: ${INCLUDE}/kernel.h
getttyent.o: ${INCLUDE}/stdio.h
getttynam.o: ${INCLUDE}/kernel.h
getusershell.o: ${INCLUDE}/kernel.h
getusershell.o: ${INCLUDE}/stdio.h
initgroups.o: ${INCLUDE}/kernel.h
initgroups.o: ${INCLUDE}/stdio.h
malloc.o: ${INCLUDE}/kernel.h
malloc.o: ${INCLUDE}/stdio.h
malloc.o: ${INCLUDE}/stdio.h
ndbm.o: ${INCLUDE}/stdio.h
ndbm.o: ${INCLUDE}/errno.h
nlist.o: ${INCLUDE}/stdio.h
popen.o: ${INCLUDE}/kernel.h
popen.o: ${INCLUDE}/errno.h
popen.o: ${INCLUDE}/stdio.h
qsort.o: ${INCLUDE}/kernel.h
random.o: ${INCLUDE}/kernel.h
random.o: ${INCLUDE}/stdio.h
regex.o: ${INCLUDE}/kernel.h
signal.o: ${INCLUDE}/kernel.h
syslog.o: ${INCLUDE}/kernel.h
syslog.o: ${INCLUDE}/netdb.h
timezone.o: ${INCLUDE}/kernel.h
ttyname.o: ${INCLUDE}/kernel.h
ttyslot.o: ${INCLUDE}/kernel.h
ualarm.o: ${INCLUDE}/kernel.h
ualarm.o: ${INCLUDE}/errno.h
usleep.o: ${INCLUDE}/kernel.h
usleep.o: ${INCLUDE}/errno.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
