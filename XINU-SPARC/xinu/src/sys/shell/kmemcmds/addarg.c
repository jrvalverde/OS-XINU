/* addarg.c - addarg */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <shell.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  addarg  -  copy arguments to area reserved on process stack
 *------------------------------------------------------------------------
 */
addarg(pid, nargs, len)
int	pid;				/* process to receive arguments	*/
int	nargs;				/* number of arguments to copy	*/
int	len;				/* size of arg. area (in bytes)	*/
{
	struct	pentry	*pptr;
	char	**fromarg;
	int	*toarg;
	char	*to;
	int	*saddr;
	int	temp;
	int	arglen;

	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate != PRSUSP)
		return(SYSERR);

	/* On the SPARC the first 6 parameters go into registers, and	*/
	/* the remaining args go onto the stack.  Consequently, we 	*/
	/* cannot just the args onto the stack.  We must know how many	*/
	/* args the Shell put in registers, and we get to use the	*/
	/* remaining registers and everything else goes on the stack.	*/
	/* SO, we assume the shell pushed stdin, stdout, stderr, and 	*/
	/* nargs into registers.  That leaves 2 registers and the stack	*/
	/* for the arguements we have to push on the stack.  So, we	*/
	/* push argv in reg %i4.  Instead of pushing argv[0] in %i5, we	*/
	/* put argv[0] in the save area for %i6, and set the argv 	*/
	/* stored in %i4 to point to the save area %i6 where argv[0] is	*/
	/* stored.  We assume the shell reserved some extra space on 	*/
	/* the stack so that we can put what create thought was going 	*/
	/* be the 6th argument (argv[0]) in the 7th position (%i6 in	*/
	/* the save area).						*/

	/* put the address where we will be depositing the argv[] array */
	/* and its strings into toarg and to respectively.		*/
	toarg = (int *) (pptr->pregs[FP] + SAV_IREG_6_OFFSET);
	to = (char *) (toarg + (nargs + 1));
	
	/* move the address of argv into %i4 on the stack */
	saddr = (int *) (pptr->pregs[FP] + IREG_0_OFFSET);
	saddr += 4;			/* move to %i4 location		*/
	temp = (int) toarg;
	vbcopy(&temp, currpid, saddr, pid, sizeof(int), TRUE);

	/* now store the rest of the arguements on the stack */
	for (fromarg=Shl.shtok ; nargs > 0 ; nargs--) {
		temp = (int) to;
		vbcopy(&temp, currpid, toarg++, pid, sizeof(int),TRUE);
		
		arglen = strlen(*fromarg) + 1;
		vbcopy(*fromarg++, currpid, to, pid, arglen,TRUE);
		to += arglen;
	}
	temp = 0;
	vbcopy(&temp, currpid, toarg, pid, sizeof(int),TRUE);
	return(OK);
}
