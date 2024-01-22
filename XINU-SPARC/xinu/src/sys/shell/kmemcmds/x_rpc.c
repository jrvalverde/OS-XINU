/* x_rpc.c - x_rpc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#ifdef Nrpc
#include <network.h>
#include <portmapper.h>
#include <rpc.h>
#endif


#define DEBUG



/*------------------------------------------------------------------------
 *  x_rpc  -  dump rpc information
 *------------------------------------------------------------------------
 */
COMMAND	x_rpc(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
#ifdef Nrpc
	int i;
	struct rpcblk *prpc;
	char hostbuf[80];

	fprintf(stdout,"rpcmutex: %d\n", rpcmutex);
	
	fprintf(stdout,"\nRPC table:\n");
	fprintf(stdout, "\
dev sem RTO   sent  recv  resent badrcv prog   vers proc lport rhost\n");
	fprintf(stdout, "\
=== === ===== ===== ===== ====== ====== ====== ==== ==== ===== =============\n");
	for (i=0; i < Nrpc; ++i) {
		prpc = &rpctab[i];
		if (prpc->rpc_state == RPCS_FREE)
		    continue;
		fprintf(stdout,"%3d ", prpc->rpc_dnum);
		fprintf(stdout,"%3d ", prpc->rpc_mutex);
		fprintf(stdout,"%5d ", prpc->rpc_RTO);
		fprintf(stdout,"%5d ", prpc->rpc_sends);
		fprintf(stdout,"%5d ", prpc->rpc_recvs);
		fprintf(stdout,"%6d ", prpc->rpc_resends);
		fprintf(stdout,"%6d ", prpc->rpc_badrecvs);
		fprintf(stdout,"%6d ", prpc->rpc_program);
		fprintf(stdout,"%4d ", prpc->rpc_version);
		fprintf(stdout,"%4d ", prpc->rpc_procedure);
		fprintf(stdout,"%5d ", prpc->rpc_lport);
		ip2name(prpc->rpc_hostIP,hostbuf);
		fprintf(stdout,"%s:%d  ", hostbuf, prpc->rpc_rport);

		fprintf(stdout,"\n");
	}
#endif
}
