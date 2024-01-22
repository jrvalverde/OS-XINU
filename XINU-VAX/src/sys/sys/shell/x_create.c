/* x_create.c - x_creat, closret */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <shell.h>
#include "/usr/include/a.out.h"

LOCAL	char	symfile[] = "a.out";	/* name of object file to search*/

#define	SYMBLK	42*sizeof(struct nlist)

/*------------------------------------------------------------------------
 *  x_creat  -  (command create) create a process given a starting address
 *------------------------------------------------------------------------
 */
COMMAND	x_creat(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	ssize, prio;
	struct	exec   *aoutptr;
	int	dev,   symdev,   len,    pid;
	char	*loc,  *symbuf,  *last,  *buf;
	Bool	found;
	struct	nlist  *symptr;
	unsigned stroff, symoff;
	
	if (nargs < 4 || nargs > 5) {
	    fprintf(stderr,
		"usage: create procedure stack-size prio [name]\n");
	    return(SYSERR);
	}
	ssize = atoi(args[2]);
	prio  = atoi(args[3]);
	symbuf = (char *)(symdev = SYSERR);
	if ( (dev=open(NAMESPACE, symfile, "ro")) == SYSERR ||
	     (symdev=open(NAMESPACE, symfile, "ro")) == SYSERR ) {
		fprintf(stderr, "Cannot open %s\n", symfile);
		close(dev);
		close(symdev);
		return(SYSERR);
	}
	if ( (buf=(char *)getmem(SHCMDSTK)) == (char *)SYSERR ||
	     (symbuf=(char *)getmem(SHBUFLEN)) == (char *)SYSERR) {
		fprintf(stderr, "no memory\n");
		closret(dev, symdev, buf, symbuf);
		return(SYSERR);
	}
	strcpy(args[0], "_");
	strcat(args[0], args[1]);
	printf("Looking up %s\n", args[0]);
	read(dev, buf, sizeof(struct exec));
	aoutptr = (struct exec *) buf;
	symoff = N_SYMOFF(*aoutptr);
	stroff = N_STROFF(*aoutptr);
	if (N_BADMAG(*aoutptr) || stroff < N_TXTOFF(*aoutptr)) {
		fprintf(stderr, "bad a.out format\n");
		closret(dev, symdev, buf, symbuf);
		return(SYSERR);
	}
	seek(dev, symoff);
	for (found=FALSE ; !found && symoff < stroff ; symoff += SYMBLK) {
		len = read(dev, buf,
			(symoff+SYMBLK <= stroff) ? SYMBLK : stroff-symoff);
		if (len == SYSERR) {
			fprintf(stderr, "error reading %s\n", symfile);
			break;
		}
		last = &buf[len]; 
		for (symptr=(struct nlist *)buf;(char *)symptr<last;symptr++){
			if ((symptr->n_type == (N_TEXT|N_EXT)) &&
			     symptr->n_un.n_strx) {
				seek(symdev, stroff + symptr->n_un.n_strx);
				if (read(symdev, symbuf, SHBUFLEN) <= 0) {
					fprintf(stderr,"can't read strtab\n");
					closret(dev, symdev, buf, symbuf);
					return(SYSERR);
				}
				if (strcmp(symbuf,args[0]) == 0) {
					loc = (char *)symptr->n_value;
					found = TRUE;
					break;
				}
			}
		}
	}
	closret(dev, symdev, buf, symbuf);
	if (found) {
		pid=create(loc, ssize, prio, nargs==5?args[4]:&args[0][1], 0);
		fprintf(stderr, "[%d]\n", pid );
		return(resume(pid));
	} else {
		fprintf(stderr, "%s not found\n", args[0]);
		return (SYSERR);
	}
}

/*------------------------------------------------------------------------
 *  closret  -  close devices, free memory before returning
 *------------------------------------------------------------------------
*/
LOCAL	closret(dev, symdev, buf, symbuf)
int	dev, symdev;
char	*buf, *symbuf;
{
	close(dev);
	close(symdev);
	freemem(buf, SHCMDSTK);
	freemem(symbuf, SHBUFLEN);
}
