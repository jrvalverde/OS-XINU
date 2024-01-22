/* x_mem.c - x_mem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>

extern char *etext, *edata;

/*------------------------------------------------------------------------
 *  x_mem  -  (command mem) print memory use and free list information
 *------------------------------------------------------------------------
 */
COMMAND	x_mem(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    STATWORD ps;    
    struct	mblock	*mptr;
    char	str[80];
    int alen, mlen, rlen, locked, total, free;

    sprintf(str, "Physical Memory Size: %d = 0x%08x\n",
	    (unsigned long) mem_size,
	    (unsigned long) mem_size);
    write(stdout, str, strlen(str));

    sprintf(str, "Kernel size = %d\n", (unsigned long) xkernsize);
    write(stdout, str, strlen(str));
	
    sprintf(str, "    Kernel: text %d, data %d, bss %d, heap %d\n",
	    (unsigned) &etext - (unsigned) vmaddrsp.xinu,
	    (unsigned) &edata - (unsigned) &etext,
	    (unsigned) &end - (unsigned) &edata,
	    (unsigned long) ((unsigned long) xkernsize -
			     ((unsigned long)&end - (unsigned long) vmaddrsp.xinu)));
    write(stdout, str, strlen(str));

    sprintf(str,"    Kernel free heap list:\n");
    write(stdout, str, strlen(str));
    
    disable(ps);
    for( mptr=memlist.mnext ; mptr!=(struct mblock *)NULL ;
	mptr=mptr->mnext) {
	sprintf(str,"        block at 0x%08x, length %ld (0x%x)\n",
		mptr, mptr->mlen, mptr->mlen);
	write(stdout, str, strlen(str));
    }

    total = ftinfo.frameavail;
    if ((free = scount(ftfreesem)) < 0)
	free = 0;
    alen = ftinfo.alen;
    rlen = ftinfo.rlen;
    mlen = ftinfo.mlen;
    locked = ftinfo.locked;
    restore(ps);

    sprintf(str, "Page Size: %d = 0x%x,  Frame Size: %d = 0x%0x\n",
	    PGSIZ, PGSIZ, PGSIZ, PGSIZ);
    write(stdout, str, strlen(str));

    sprintf(str, "Total # of frames: %d\n", total);
    write(stdout, str, strlen(str));

    sprintf(str, "Number of Free Frames: %d\n", free);
    write(stdout, str, strlen(str));

    sprintf(str, "Length of the active list  : %d\n", alen);
    write(stdout, str, strlen(str));

    sprintf(str, "Length of the reclaim list : %d\n", rlen);
    write(stdout, str, strlen(str));

    sprintf(str, "Length of the modified list: %d\n", mlen);
    write(stdout, str, strlen(str));

    sprintf(str, "Length of the locked list  : %d\n", locked);
    write(stdout, str, strlen(str));

    return(OK);
}

