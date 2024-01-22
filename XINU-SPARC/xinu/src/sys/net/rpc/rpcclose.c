/* rpcclose.c - rpcclose */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>

int rpcclose(pdev)
     struct	devsw	*pdev;
{
	struct	rpcblk	*prpc;
	int		freebuf();

	prpc = (struct rpcblk *) pdev->dvioblk;

	freemem(prpc->rpc_reqxbuf,prpc->rpc_reqxbufsize);

	/* save rto estimates for next time */
	if (prpc->rpc_type == RPCT_CLIENT) {
		rpcrtosave(prpc);
	}

	/* deallocate the UDP device */
	close(prpc->rpc_udpdev);

	/* deallocate it's semaphore */
	sdelete(prpc->rpc_mutex);

	/* free any pep hanging around */
	if (prpc->rpc_resppep) {
		freebuf(prpc->rpc_resppep);
	}

	/* deallocate the pseudo device */
	prpc->rpc_state = RPCS_FREE;

	return(OK);
}
