/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <conf.h>

/* device independent I/O switch */

struct	devsw	devtab[NDEVS] = {

/*  Format of entries is:
device-number, device-name,
init, open, close,
read, write, seek,
getc, putc, cntl,
device-csr-address, input-vector, output-vector,
iint-handler, oint-handler, control-block, minor-device,
*/

/*  CONSOLE  is tty  */

0, "CONSOLE",
ttyinit, ttyopen, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
0000000, 0000, 0000,
ttyiin, ttyoin, NULLPTR, 0,

/*  SERIAL0  is serial  */

1, "SERIAL0",
cominit, ionull, ionull,
ionull, comwrite, ioerr,
comgetc, computc, comcntl,
0001770, 0044, 0000,
comint, ioerr, NULLPTR, 0,

/*  SERIAL1  is serial  */

2, "SERIAL1",
cominit, ionull, ionull,
ionull, comwrite, ioerr,
comgetc, computc, comcntl,
0001370, 0043, 0000,
comint, ioerr, NULLPTR, 1,

/*  KBMON  is kbmon  */

3, "KBMON",
kbminit, ionull, ionull,
kbmread, kbmwrite, ioerr,
kbmgetc, kbmputc, kbmcntl,
0000000, 0041, 0000,
kbmint, ioerr, NULLPTR, 0,

/*  TTY0  is tty  */

4, "TTY0",
ttyinit, ttyopen, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
0000000, 0000, 0000,
ttyiin, ttyoin, NULLPTR, 1,

/*  TTY1  is tty  */

5, "TTY1",
ttyinit, ttyopen, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
0000000, 0000, 0000,
ttyiin, ttyoin, NULLPTR, 2,

/*  TTY2  is tty  */

6, "TTY2",
ttyinit, ttyopen, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
0000000, 0000, 0000,
ttyiin, ttyoin, NULLPTR, 3,

/*  ETHER  is eth  */

7, "ETHER",
ee_init, ioerr, ioerr,
ee_read, ee_write, ioerr,
ioerr, ioerr, ee_cntl,
0001120, 0045, 0000,
ee_intr, ee_intr, NULLPTR, 0,

/*  UDP  is dgm  */

8, "UDP",
ionull, dgmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, dgmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM0  is dg  */

9, "DGRAM0",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM1  is dg  */

10, "DGRAM1",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  DGRAM2  is dg  */

11, "DGRAM2",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  DGRAM3  is dg  */

12, "DGRAM3",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  DGRAM4  is dg  */

13, "DGRAM4",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  DGRAM5  is dg  */

14, "DGRAM5",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  DGRAM6  is dg  */

15, "DGRAM6",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  DGRAM7  is dg  */

16, "DGRAM7",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  DGRAM8  is dg  */

17, "DGRAM8",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  DGRAM9  is dg  */

18, "DGRAM9",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  DGRAMA  is dg  */

19, "DGRAMA",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 10,

/*  DGRAMB  is dg  */

20, "DGRAMB",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 11,

/*  DGRAMC  is dg  */

21, "DGRAMC",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 12,

/*  DGRAMD  is dg  */

22, "DGRAMD",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 13,

/*  DGRAME  is dg  */

23, "DGRAME",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 14,

/*  DGRAMF  is dg  */

24, "DGRAMF",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 15,

/*  TCP  is tcpm  */

25, "TCP",
ionull, tcpmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, tcpmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  TCP0  is tcp  */

26, "TCP0",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  TCP1  is tcp  */

27, "TCP1",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  TCP2  is tcp  */

28, "TCP2",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  TCP3  is tcp  */

29, "TCP3",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  TCP4  is tcp  */

30, "TCP4",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  TCP5  is tcp  */

31, "TCP5",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  TCP6  is tcp  */

32, "TCP6",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  TCP7  is tcp  */

33, "TCP7",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  TCP8  is tcp  */

34, "TCP8",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  TCP9  is tcp  */

35, "TCP9",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  TCPA  is tcp  */

36, "TCPA",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 10,

/*  TCPB  is tcp  */

37, "TCPB",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 11,

/*  TCPC  is tcp  */

38, "TCPC",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 12,

/*  TCPD  is tcp  */

39, "TCPD",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 13,

/*  TCPE  is tcp  */

40, "TCPE",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 14,

/*  TCPF  is tcp  */

41, "TCPF",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 15,

/*  RFILSYS  is rfm  */

42, "RFILSYS",
ioerr, rfopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE1  is rf  */

43, "RFILE1",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE2  is rf  */

44, "RFILE2",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  RFILE3  is rf  */

45, "RFILE3",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  RFILE4  is rf  */

46, "RFILE4",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  NAMESPACE  is nam  */

47, "NAMESPACE",
naminit, namopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0
	};
