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
vcuinit, ttyopen, ionull,
ttyread, vcuwrite, ioerr,
ttygetc, vcuputc, vcucntl,
0000000, 0370, 0374,
vcuiin, vcuoin, NULLPTR, 0,

/*  ETHER  is eth  */

1, "ETHER",
ethinit, ioerr, ioerr,
ethread, ethwrite, ioerr,
ioerr, ioerr, ioerr,
017774440, 01400, 01400,
ethinter, ethinter, NULLPTR, 0,

/*  INTERNET  is dgm  */

2, "INTERNET",
ionull, dgmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, dgmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM1  is dg  */

3, "DGRAM1",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM2  is dg  */

4, "DGRAM2",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  DGRAM3  is dg  */

5, "DGRAM3",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  DGRAM4  is dg  */

6, "DGRAM4",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  DGRAM5  is dg  */

7, "DGRAM5",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  DGRAM6  is dg  */

8, "DGRAM6",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  RFILSYS  is rfm  */

9, "RFILSYS",
ioerr, rfopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE1  is rf  */

10, "RFILE1",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE2  is rf  */

11, "RFILE2",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  RFILE3  is rf  */

12, "RFILE3",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  RFILE4  is rf  */

13, "RFILE4",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  RFILE5  is rf  */

14, "RFILE5",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  RFILE6  is rf  */

15, "RFILE6",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  RFILE7  is rf  */

16, "RFILE7",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  RFILE8  is rf  */

17, "RFILE8",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  RFILE9  is rf  */

18, "RFILE9",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  RFILE10  is rf  */

19, "RFILE10",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  NAMESPACE  is nsys  */

20, "NAMESPACE",
naminit, namopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0
	};
