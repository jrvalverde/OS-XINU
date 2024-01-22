/* rpcinit.c - rpcinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>

rpcinit(pdev)
     struct	devsw	*pdev;
{
    struct	rpcblk	*prpc;

    prpc = &rpctab[pdev->dvminor];
    pdev->dvioblk = (char *) prpc;
    prpc->rpc_dnum = pdev->dvnum;
    prpc->rpc_state = RPCS_FREE;
    prpc->rpc_reqxbuf = NULLPTR;
    prpc->rpc_reqxbufsize = 0;
    prpc->rpc_reqxix = 0;
    prpc->rpc_reqxlen = 0;
    return OK;
}
