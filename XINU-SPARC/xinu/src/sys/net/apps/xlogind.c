/* 
 * xlogind.c - Xinu xlogin daemon
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue May 22 22:27:34 1990
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>
#include <shell.h>
#include <cmd.h>
#include <tty.h>

#define DEBUG

#define XLOGINPORT 1000
#define SHELL_PATH "./xsh"

static void Service();
static void Log();
static void BeShell();
static int MungeClose();
static void RestoreClose();


/*------------------------------------------------------------------------
 *  xlogind - the NEW xinu xlogin daemon -- uses Andy's shell
 *------------------------------------------------------------------------
 */
PROCESS	xlogind()
{
    int	fd;

    fd = open(TCP, ANYFPORT, XLOGINPORT);	/* a stub LISTEN state TCP fd */
    control(fd, TCPC_LISTENQ, 5);		/* set the queue length */


    while (TRUE) {
	int	fdConnect;

	/* fdConnect is a CONNECTED TCP stream when control() returns */
	if ((fdConnect = control(fd, TCPC_ACCEPT)) == SYSERR) {
	    break;
	}

	resume(kcreate(Service, 4000, 20,"xlogind",1,fdConnect));
    }
}



static void
Service(fd)
    int fd;
{
    struct tcpstat tcpstat;
    char   fromString[128];


    /* see who the connection is from */
    control(fd, TCPC_STATUS, &tcpstat);

    ip2name(tcpstat.ts_faddr,fromString);
#ifdef DEBUG
    Log("Serving connection from %s:%d on dev:%d\n",
	fromString, tcpstat.ts_fport, fd);
#endif

    BeShell(fd);

#ifdef DEBUG
    Log("logging out of dev:%d\n",fd);
#endif
    close(fd);
}



static void BeShell(dev)
int	dev;
{
	int close_routine;
	int child;
	static char *args[] = {"xsh","-p",0};

	close_routine = MungeClose(dev);

	child = xinu_execv(SHELL_PATH,	/* load path	*/
			   20,		/* priority	*/
			   dev,		/* stdin	*/
			   dev,		/* stdout	*/
			   dev,		/* stderr	*/
			   2,		/* nargs	*/
			   args);	/* arguments	*/

	if (child == SYSERR) {
#ifdef DEBUG
		Log("Could not execute %s\n", SHELL_PATH);
#endif
		return;
	}

	resume(child);
	
	/* wait for the message that says the child is done */
	recvclr();
	receive();

	RestoreClose(dev,close_routine);
}




static int MungeClose(dev)
     int dev;
{
	int save;

	save = (int) devtab[dev].dvclose;
	devtab[dev].dvclose = ionull;
	return(save);
}


static void RestoreClose(dev,routine)
     int dev;
     int routine;
{
    devtab[dev].dvclose = (int (*)()) routine;
}


static void Log(m,a1,a2,a3,a4)
     char *m;
     int a1,a2,a3,a4;
{
    fprintf(CONSOLE,"xlogin: ");
    fprintf(CONSOLE,m,a1,a2,a3,a4);
}
