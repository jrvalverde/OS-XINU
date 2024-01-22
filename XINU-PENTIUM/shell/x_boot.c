/* x_boot.c - boot */
#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <bootp.h>

/*------------------------------------------------------------------------
 *  x_boot  -  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */

COMMAND	x_boot(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	fprintf(stdin, "ATTEMPTING TFTP of ->%s<- from %s\n", Bootrecord.bootfile,
		Bootrecord.tftpserver);
	tftp(Bootrecord.tftpserver, Bootrecord.bootfile);
	return(SYSERR);
}
