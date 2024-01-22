/* ee_read.c - ee_read */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <q.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 *  ee_read - read a frame from ethernet
 *------------------------------------------------------------------------
 */
ee_read(pdev, ppep, len)
     struct	devsw	*pdev;
     struct	ep	**ppep;
     int	len;
{
    struct	ep	*pep;
    struct	etdev	*ped;

    ped = (struct etdev *)pdev->dvioblk;
    
    /* wait for exclusive read access */
    wait(ped->ed_rsem);
    
    ped->ed_rpid = currpid;
    
    while ((pep = (struct ep *) deq(ped->ed_inq)) == (struct ep *) 0) {
	suspend(currpid);
    }
    
    ped->ed_rpid = BADPID;
    if (pep->ep_len > len) {
	len = SYSERR;
	*ppep = (struct ep *) NULL;
    } else {
	len = pep->ep_len;
	*ppep = pep;
    }
    
    signal(ped->ed_rsem);
    return(len);
}
