/* x_rpcinfo.c - x_rpcinfo */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <portmapper.h>
#include <rpc.h>
#include <rpcprogs.h>

static dump();
static call();

#define DEBUG

static g_stdout;
static g_stderr;

/*------------------------------------------------------------------------
 *  x_rpcinfo  -  run the Unix rpcinfo command
 *------------------------------------------------------------------------
 */
COMMAND	x_rpcinfo(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	g_stdout = stdout;
	g_stderr = stderr;

	if (((nargs == 3) || (nargs == 2)) && (strcmp(args[1],"-p") == 0)) {
		if (nargs == 3)
		    return dump(args[2]);
		else
		    return dump("localhost");
	} else if ((nargs == 5) && (strcmp(args[1],"-u") == 0)) {
		return call(args[2],atoi(args[3]),atoi(args[4]));
	} else {
		fprintf(g_stderr, "use: rpcinfo -p host\n");
		fprintf(g_stderr, "     rpcinfo -u host program version\n");
		return(SYSERR);
	}
}


static dump(host)
     char *host;
{
	int ret;
	char pmapstr[200];
	int dev;
	unsigned program;
	unsigned version;
	unsigned proto;
	unsigned port;
	unsigned more;
	
	
	sprintf(pmapstr,"%s:%d:%d:%d", host,
		RPCPROG_PMAP,PMAP_VERS,PMAP_PROC_DUMP);

	dev = open(RPC,pmapstr,RPCT_CLIENT);

	ret = control(dev,RPC_CALL);

	kprintf("RPCCALL return value: %d\n", ret);

	fprintf(g_stdout,"    program     vers  proto  port\n");
	fprintf(g_stdout,"    ==========  ====  =====  ======\n");
	control(dev,XGET_UINT,&more);
	while (more) {
		control(dev,XGET_UINT,&program);
		control(dev,XGET_UINT,&version);
		control(dev,XGET_UINT,&proto);
		control(dev,XGET_UINT,&port);
		control(dev,XGET_UINT,&more);

		fprintf(g_stdout,"    %-10d  %-4d  %-5s  %-6d\n",
			program, version,
			proto==IPT_UDP?"UDP":proto==IPT_TCP?"TCP":"??",
			port);
	}
	
	close(dev);
}

static call(host,program,version)
     char *host;
     unsigned program;
     unsigned version;
{
	char pmapstr[200];
	int dev;
	int ret;
	
	
	kprintf("Calling program %d:%d:%d on host '%s'\n",
		program,version,0,host);

	sprintf(pmapstr,"%s:%d:%d:%d", host,program,version,0);

	dev = open(RPC,pmapstr,RPCT_CLIENT);

	ret = control(dev,RPC_CALL);

	kprintf("RPCCALL return value: %d\n", ret);

	close(dev);

	return(OK);
}
