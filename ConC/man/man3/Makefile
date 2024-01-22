#
#  Make man3 ConcurrenC Manual Pages
#


MACROS	=	../tmac.an

FILES	=	$(MACROS)	\
		intro.3ccc	\
		buffer.3ccc \
		eis.3ccc \
		fgetc.3ccc fputc.3ccc \
		gets.3ccc \
		printf.3ccc puts.3ccc \
		qsort.3ccc \
		rand.3ccc \
		scanf.3ccc string.3ccc swab.3ccc \
		alarm.3 \
		crypt.3 \
		execl.3 \
		getfsent.3 getgrent.3 gethostent.3 getnetent.3 \
		getopt.3 getprotoent.3 getpwent.3 getservent.3 \
		getusershell.3 \
		initgroups.3 \
		pause.3 \
		random.3 regex.3 ruserpass.3 \
		setexit.3 setjmp.3 signal.3 syslog.3 \
		ualarm.3 usleep.3

all:		$(FILES)

buffer.3ccc :	raw/buffer.3ccc.raw
		nroff ../tmac.an raw/buffer.3ccc.raw > buffer.3ccc
  
eis.3ccc :	raw/eis.3ccc.raw
		nroff ../tmac.an raw/eis.3ccc.raw > eis.3ccc
  
fgetc.3ccc :	raw/fgetc.3ccc.raw
		nroff ../tmac.an raw/fgetc.3ccc.raw > fgetc.3ccc
  
fputc.3ccc :	raw/fputc.3ccc.raw
		nroff ../tmac.an raw/fputc.3ccc.raw > fputc.3ccc
  
gets.3ccc :	raw/gets.3ccc.raw
		nroff ../tmac.an raw/gets.3ccc.raw > gets.3ccc
  
intro.3ccc :	raw/intro.3ccc.raw
		nroff ../tmac.an raw/intro.3ccc.raw > intro.3ccc
  
printf.3ccc :	raw/printf.3ccc.raw
		nroff ../tmac.an raw/printf.3ccc.raw > printf.3ccc
  
puts.3ccc :	raw/puts.3ccc.raw
		nroff ../tmac.an raw/puts.3ccc.raw > puts.3ccc
  
qsort.3ccc :	raw/qsort.3ccc.raw
		nroff ../tmac.an raw/qsort.3ccc.raw > qsort.3ccc
  
rand.3ccc :	raw/rand.3ccc.raw
		nroff ../tmac.an raw/rand.3ccc.raw > rand.3ccc
  
scanf.3ccc :	raw/scanf.3ccc.raw
		nroff ../tmac.an raw/scanf.3ccc.raw > scanf.3ccc
  
string.3ccc :	raw/string.3ccc.raw
		nroff ../tmac.an raw/string.3ccc.raw > string.3ccc
  
swab.3ccc :	raw/swab.3ccc.raw
		nroff ../tmac.an raw/swab.3ccc.raw > swab.3ccc
  
alarm.3 :	raw/alarm.3.raw
		nroff ../tmac.an raw/alarm.3.raw > alarm.3
  
crypt.3 :	raw/crypt.3.raw
		nroff ../tmac.an raw/crypt.3.raw > crypt.3
  
execl.3 :	raw/execl.3.raw
		nroff ../tmac.an raw/execl.3.raw > execl.3
  
getfsent.3 :	raw/getfsent.3.raw
		nroff ../tmac.an raw/getfsent.3.raw > getfsent.3
  
getgrent.3 :	raw/getgrent.3.raw
		nroff ../tmac.an raw/getgrent.3.raw > getgrent.3
  
gethostent.3 :	raw/gethostent.3.raw
		nroff ../tmac.an raw/gethostent.3.raw > gethostent.3
  
getnetent.3 :	raw/getnetent.3.raw
		nroff ../tmac.an raw/getnetent.3.raw > getnetent.3
  
getopt.3 :	raw/getopt.3.raw
		nroff ../tmac.an raw/getopt.3.raw > getopt.3
  
getprotoent.3 :	raw/getprotoent.3.raw
		nroff ../tmac.an raw/getprotoent.3.raw > getprotoent.3
  
getpwent.3 :	raw/getpwent.3.raw
		nroff ../tmac.an raw/getpwent.3.raw > getpwent.3
  
getservent.3 :	raw/getservent.3.raw
		nroff ../tmac.an raw/getservent.3.raw > getservent.3
  
getusershell.3:	raw/getusershell.3.raw
		nroff ../tmac.an raw/getusershell.3.raw > getusershell.3
  
initgroups.3 :	raw/initgroups.3.raw
		nroff ../tmac.an raw/initgroups.3.raw > initgroups.3
  
pause.3 :	raw/pause.3.raw
		nroff ../tmac.an raw/pause.3.raw > pause.3
  
random.3 :	raw/random.3.raw
		nroff ../tmac.an raw/random.3.raw > random.3
  
regex.3 :	raw/regex.3.raw
		nroff ../tmac.an raw/regex.3.raw > regex.3
  
ruserpass.3 :	raw/ruserpass.3.raw
		nroff ../tmac.an raw/ruserpass.3.raw > ruserpass.3
  
setexit.3 :	raw/setexit.3.raw
		nroff ../tmac.an raw/setexit.3.raw > setexit.3
  
setjmp.3 :	raw/setjmp.3.raw
		nroff ../tmac.an raw/setjmp.3.raw > setjmp.3
  
signal.3 :	raw/signal.3.raw
		nroff ../tmac.an raw/signal.3.raw > signal.3
  
syslog.3 :	raw/syslog.3.raw
		nroff ../tmac.an raw/syslog.3.raw > syslog.3
  
ualarm.3 :	raw/ualarm.3.raw
		nroff ../tmac.an raw/ualarm.3.raw > ualarm.3
  
usleep.3 :	raw/usleep.3.raw
		nroff ../tmac.an raw/usleep.3.raw > usleep.3
  
install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.3ccc *.3

print:		listings

listings:	$(FILES)
		cat *.3ccc *.3 | sed '1,2d;s/^/	/' | lpr
