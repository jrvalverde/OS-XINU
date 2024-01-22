/* x_rusers.c - x_rusers */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <portmapper.h>
#include <rpc.h>
#include <rpcprogs.h>

/*#define DEBUG*/

/* lengths of the fields we want */
#define MAX_HOST	10
#define MAX_FROMHOST	20
#define MAX_NAME	8
#define MAX_LINE	8

/*------------------------------------------------------------------------
 *  x_rusers  -  run the Unix rusers command
 *------------------------------------------------------------------------
 */
COMMAND	x_rusers(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	pmapstr[200];
	char	timbuf[200];
	IPaddr	hostIP;
	char	host[200];
	char	line[MAX_LINE+1];
	char	name[MAX_NAME+1];
	char	fromhost[MAX_FROMHOST+1];
	int	time;
	int	dev;
	int	ret;
	int	respcount;
	int	i;


	if (nargs > 2) {
		fprintf(stderr, "use: %s [host]\n", args[0]);
		return(SYSERR);
	}
	

	sprintf(pmapstr,"%s:%d:%d:%d",
		(nargs==2)?args[1]:RPC_EVERYHOST,
		RPCPROG_RUSERS,1,3);

	if ((dev = open(RPC,pmapstr,RPCT_CLIENT)) < 0) {
		fprintf(stdout,"%s: couldn't open device, return = %d\n",
			args[0], dev);
		return(SYSERR);
	}

	ret = control(dev,RPC_CALL);

	if (ret < 0) {
#ifdef DEBUG
		fprintf(stdout,"%s: RPCCALL failed, return value: %d\n",
			args[0], ret);
#endif
		return(SYSERR);
	}

	fprintf(stdout,"host        name      line     from host             login time\n");
	fprintf(stdout,"==========  ========  =======  ====================  =================\n");

	control(dev,RPC_SETNEXTTO,3000);
	while (1) {
		/* determine which host responded */
		control(dev,RPC_GETHOST,hostIP);
		ip2name(hostIP,host);
		host[MAX_HOST] = NULLCH;

		/* see how many responses there were */
		control(dev,XGET_UINT,&respcount);

#ifdef DEBUG
		fprintf(stdout,"%s: %d\n", host, respcount);
#endif

		for (i=0; i < respcount; ++i) {
			control(dev,XGET_STRING,line,sizeof(line));
			control(dev,XGET_STRING,name,sizeof(name));
			control(dev,XGET_STRING,fromhost,sizeof(fromhost));
			control(dev,XGET_UINT,&time);

			/* convert the date */
			ascdate(time,timbuf);
			timbuf[17] = NULLCH; /* discard the seconds */

			fprintf(stdout,"%-10s  %-8s  %-7s  %-20s  %-17s\n",
				host, name, line, fromhost, timbuf);
		}

		/* if a host was specified, we're done */
		if (nargs == 2)
		    break;

		/* wait for another response */
		if (control(dev,RPC_NEXTRESP) != OK) {
#ifdef DEBUG
			fprintf(stdout,"no more\n");
#endif
			break;
		}
	}

	close(dev);

	return(OK);
}
