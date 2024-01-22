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
ttyinit, ionull, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
037764020004, 0160, 0160,
ioerr, ioerr, NULLPTR, 0,

/*  ETHER  is eth  */

1, "ETHER",
ethinit, ioerr, ioerr,
ethread, ethwrite, ioerr,
ioerr, ioerr, ethcntl,
037764200000, 0124, 0124,
ethinter, ethinter, NULLPTR, 0,

/*  NAMESPACE  is nam  */

2, "NAMESPACE",
naminit, namopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  NULLDEV  is nulldev  */

3, "NULLDEV",
ionull, nullopen, ionull,
nullread, nullwrite, ioerr,
nullgetc, ionull, nullcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  TCP  is tcpm  */

4, "TCP",
ionull, tcpmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, tcpmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  TCP1  is tcp  */

5, "TCP1",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  TCP2  is tcp  */

6, "TCP2",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  TCP3  is tcp  */

7, "TCP3",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  TCP4  is tcp  */

8, "TCP4",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  TCP5  is tcp  */

9, "TCP5",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  TCP6  is tcp  */

10, "TCP6",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  TCP7  is tcp  */

11, "TCP7",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  TCP8  is tcp  */

12, "TCP8",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  TCP9  is tcp  */

13, "TCP9",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  TCP10  is tcp  */

14, "TCP10",
tcpinit, ioerr, tcpclose,
tcpread, tcpwrite, ioerr,
tcpgetc, tcpputc, tcpcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  RFILSYS  is rfm  */

15, "RFILSYS",
ioerr, rfopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE1  is rf  */

16, "RFILE1",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE2  is rf  */

17, "RFILE2",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  RFILE3  is rf  */

18, "RFILE3",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  RFILE4  is rf  */

19, "RFILE4",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  UDP  is dgm  */

20, "UDP",
ionull, dgmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, dgmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM1  is dg  */

21, "DGRAM1",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM2  is dg  */

22, "DGRAM2",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  DGRAM3  is dg  */

23, "DGRAM3",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  DGRAM4  is dg  */

24, "DGRAM4",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  DGRAM5  is dg  */

25, "DGRAM5",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  DGRAM6  is dg  */

26, "DGRAM6",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  DGRAM7  is dg  */

27, "DGRAM7",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  DGRAM8  is dg  */

28, "DGRAM8",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  DGRAM9  is dg  */

29, "DGRAM9",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  DGRAM10  is dg  */

30, "DGRAM10",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  DGRAM11  is dg  */

31, "DGRAM11",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 10,

/*  DGRAM12  is dg  */

32, "DGRAM12",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 11,

/*  DGRAM13  is dg  */

33, "DGRAM13",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 12,

/*  DGRAM14  is dg  */

34, "DGRAM14",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 13,

/*  DGRAM15  is dg  */

35, "DGRAM15",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 14,

/*  DGRAM16  is dg  */

36, "DGRAM16",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 15,

/*  DGRAM17  is dg  */

37, "DGRAM17",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 16,

/*  DGRAM18  is dg  */

38, "DGRAM18",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 17,

/*  DGRAM19  is dg  */

39, "DGRAM19",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 18,

/*  DGRAM20  is dg  */

40, "DGRAM20",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 19,

/*  DGRAM21  is dg  */

41, "DGRAM21",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 20,

/*  DGRAM22  is dg  */

42, "DGRAM22",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 21,

/*  DGRAM23  is dg  */

43, "DGRAM23",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 22,

/*  DGRAM24  is dg  */

44, "DGRAM24",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 23,

/*  DGRAM25  is dg  */

45, "DGRAM25",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 24,

/*  DGRAM26  is dg  */

46, "DGRAM26",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 25,

/*  DGRAM27  is dg  */

47, "DGRAM27",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 26,

/*  DGRAM28  is dg  */

48, "DGRAM28",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 27,

/*  DGRAM29  is dg  */

49, "DGRAM29",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 28,

/*  DGRAM30  is dg  */

50, "DGRAM30",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 29,

/*  RPC  is rpcm  */

51, "RPC",
rpcminit, rpcmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, rpcmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RPC1  is rpc  */

52, "RPC1",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RPC2  is rpc  */

53, "RPC2",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  RPC3  is rpc  */

54, "RPC3",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  RPC4  is rpc  */

55, "RPC4",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  RPC5  is rpc  */

56, "RPC5",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  RPC6  is rpc  */

57, "RPC6",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  RPC7  is rpc  */

58, "RPC7",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  RPC8  is rpc  */

59, "RPC8",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  RPC9  is rpc  */

60, "RPC9",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  RPC10  is rpc  */

61, "RPC10",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  RPC11  is rpc  */

62, "RPC11",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 10,

/*  RPC12  is rpc  */

63, "RPC12",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 11,

/*  RPC13  is rpc  */

64, "RPC13",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 12,

/*  RPC14  is rpc  */

65, "RPC14",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 13,

/*  RPC15  is rpc  */

66, "RPC15",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 14,

/*  RPC16  is rpc  */

67, "RPC16",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 15,

/*  RPC17  is rpc  */

68, "RPC17",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 16,

/*  RPC18  is rpc  */

69, "RPC18",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 17,

/*  RPC19  is rpc  */

70, "RPC19",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 18,

/*  RPC20  is rpc  */

71, "RPC20",
rpcinit, ioerr, rpcclose,
rpcread, rpcwrite, ioerr,
ioerr, ioerr, rpccntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 19,

/*  NFS  is nfsm  */

72, "NFS",
nfsminit, nfsmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, nfsmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  NFS1  is nfs  */

73, "NFS1",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  NFS2  is nfs  */

74, "NFS2",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  NFS3  is nfs  */

75, "NFS3",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  NFS4  is nfs  */

76, "NFS4",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  NFS5  is nfs  */

77, "NFS5",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 4,

/*  NFS6  is nfs  */

78, "NFS6",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 5,

/*  NFS7  is nfs  */

79, "NFS7",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 6,

/*  NFS8  is nfs  */

80, "NFS8",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 7,

/*  NFS9  is nfs  */

81, "NFS9",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 8,

/*  NFS10  is nfs  */

82, "NFS10",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 9,

/*  NFS11  is nfs  */

83, "NFS11",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 10,

/*  NFS12  is nfs  */

84, "NFS12",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 11,

/*  NFS13  is nfs  */

85, "NFS13",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 12,

/*  NFS14  is nfs  */

86, "NFS14",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 13,

/*  NFS15  is nfs  */

87, "NFS15",
nfsinit, ioerr, nfsclose,
nfsread, nfswrite, nfsseek,
nfsgetc, nfsputc, nfscntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 14
	};
