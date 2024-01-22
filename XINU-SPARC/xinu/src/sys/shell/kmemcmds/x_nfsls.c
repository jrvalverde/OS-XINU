/* x_nfsls.c - x_nfsls */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <name.h>

#ifndef Nnfs
COMMAND	x_nfsls(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    kprintf("** Error: No NFS support.\n");
    return(SYSERR);
}

#else

#include <network.h>
#include <rpc.h>
#include <nfs.h>

/*#define DEBUG*/

#define MAXENTRIES 20


/*------------------------------------------------------------------------
 *  x_nfsls  -  get a listing of an NFS director
 *------------------------------------------------------------------------
 */
COMMAND	x_nfsls(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    struct nfs_readdir_entry ent[MAXENTRIES];
    struct nfs_readdir_req req;
    char newname[NAMLEN];
    int j;
    int dev;
    char *dir;

    /* check the args */
    if (nargs != 2) {
	fprintf(stderr, "usage: %s dir\n", args[0]);
	return(SYSERR);
    }

    /* read the directory */
    dir = args[1];
    if (nammap(dir, newname) != NFS) {
	fprintf(stderr,"%s: not an NFS file\n", dir);
	return(SYSERR);
    }
    if ((dev = open(NAMESPACE,dir,"ro")) == SYSERR) {
	fprintf(stderr,"%s: file not found\n", dir);
	return(SYSERR);
    }
	
    req.pent = ent;
    req.cookie = 0;
    req.eof = FALSE;
    while (!req.eof) {
	req.nentries = MAXENTRIES;
	if (control(dev, NFS_READDIR, &req) == SYSERR) {
	    fprintf(stderr,"NFS_READDIR failed\n");
	    break;
	}
#ifdef DEBUG
	kprintf("RESP: nentries:%d eof:%d\n", req.nentries,req.eof);
#endif
	for (j=0; j < req.nentries; ++j)
	    fprintf(stdout, "%s\n", ent[j].filename);
    }
    close(dev);

    return(OK);
}

#endif
