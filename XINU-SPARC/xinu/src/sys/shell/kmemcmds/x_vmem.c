/* x_vmem.c - x_vmem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>


/*------------------------------------------------------------------------
 *  x_vmem  -  print virtual memory layout
 *------------------------------------------------------------------------
 */
COMMAND	x_vmem(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	sb[80];

	sprintf(sb, "Virtual memory facts:\n");
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    Page size = %d bytes\n", PGSIZ);
	write(stdout, sb, strlen(sb));

	/* print virtual memory layout */
	sprintf(sb, "\nVirtual address space layout of a thread:\n");
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    User heap area:          %08x - %08x\n",
		(unsigned)vmaddrsp.minaddr, (unsigned)vmaddrsp.maxheap);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    User stack space:        %08x - %08x\n",
		(unsigned)vmaddrsp.minstk, (unsigned)vmaddrsp.stkstrt);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    Per thread kernel stack: %08x - %08x\n",
		(unsigned) (vmaddrsp.stkstrt+(sizeof(int))),
		(unsigned)vmaddrsp.kernstk);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    Per thread RSA space:    %08x - %08x\n",
		(unsigned) (vmaddrsp.kernstk+(sizeof(int))),
		(unsigned)vmaddrsp.rsaio);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    Shared private area:     %08x - %08x\n",
		(unsigned) (vmaddrsp.rsaio+(sizeof(int))),
		(unsigned)vmaddrsp.maxsp);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "    Kernel space:            %08x - %08x\n",
		(unsigned)vmaddrsp.xinu, (unsigned)vmaddrsp.xheapmax);
	write(stdout, sb, strlen(sb));

	return(OK);
}

