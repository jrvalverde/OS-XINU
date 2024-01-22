/* nfswalkpath.c -- contact an nfs server and parse a path */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>


/*#define DEBUG*/
#define PRINTERRORS


/* routines used */
char *index();
char *getmem();
static dowalk();


/*
 * ====================================================================
 * nfswalkpath - lookup the file whose path is given given as 'rname' in
 *            the pnfs structure.
 *	      N.B.  this routine uses the NFS cache, which may be
 *                  invalid.  You should stat the file before you assume
 *                  that it really exists.
 * ====================================================================
 */
nfswalkpath(pnfs, roothandle, links_left, followlink)
     struct nfsblk *pnfs;
     fhandle roothandle;
     int links_left;
     Bool followlink;
{
	int retval;
	char *workpath;
	char *pwork;
	char *currpart;
	fhandle oldhandle;
	fhandle newhandle;
	struct fattr newattr;
	char *slashptr;
	unsigned ftype;
	int len;
	Bool usecache;

#ifdef DEBUG
	kprintf("nfswalkpath('%s') called\n", pnfs->nfs_fpath);
#endif

	ftype = NFDIR;

	/* handles are invalid at this point */
	pnfs->nfs_fhandle_valid = FALSE;
	pnfs->nfs_phandle_valid = FALSE;


	/* make a copy of the path and handle */
	workpath = getmem(NFS_PATHLEN);
	strcpy(workpath, pnfs->nfs_fpath);
	pwork = workpath;
	blkcopy(oldhandle, roothandle, NFS_FHSIZE);

	/* names ending in '/' get a '.' appended */
	len = strlen(workpath);
	if (workpath[len-1] == '/') {
		workpath[len] = '.';
		workpath[len+1] = NULLCH;
	}


	while (1) {
		/* eat up any '/'s at the front */
		while (*pwork == NFS_SEPARATOR)
		    ++pwork;
		currpart = pwork;

		
		/* look for the '/' that delimites the next piece */
		slashptr = index(pwork, NFS_SEPARATOR);
		if (slashptr == NULL) {
			/* last component - everything else is a name */
			pwork = "";
		} else {
			/* isolate and skip over this component */
			*slashptr = NULLCH;
			pwork = slashptr + 1;
		}


#ifdef DEBUG
		kprintf("currpart is '%s', workpath is '%s'\n",
			currpart, *pwork?pwork:"<NULL>");
#endif
		/* if this is the last component, don't trust the cache */
		usecache = (*pwork != NULLCH);
		   
		/* find the handle for 'currpart' */
		if (nfslookupname(pnfs, oldhandle, currpart,
				  newhandle, &newattr,
				  usecache) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("nfswalkpath: SYSERR from lookupname\n");
#endif
			retval = SYSERR;
			break;
		}

		if (pnfs->nfs_status != NFS_OK) {
			if ((*pwork == NULLCH) &&
			    (pnfs->nfs_status) == NFSERR_NOENT) {
#ifdef DEBUG
				kprintf("nfswalkpath: file didn't exist\n");
#endif
				pnfs->nfs_phandle_valid = TRUE;
				blkcopy(pnfs->nfs_phandle, oldhandle,
					NFS_FHSIZE);
			}
#if defined(DEBUG)
			kprintf("nfswalkpath, error status %s for '%s'\n",
				nfsStatString(pnfs->nfs_status), currpart);
#endif
			retval = SYSERR;
			break;
		}

		ftype = newattr.fa_type;
		if (ftype == NFDIR) {
#ifdef DEBUG
			kprintf("%s: directory\n", currpart);
#endif
		} else if ((ftype == NFLNK) &&
			   (*pwork == NULLCH) && (!followlink)) {
			/* if this is the last component, we stop if asked */
		} else if (ftype == NFLNK) {
			--links_left;
			if (links_left <= 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
				kprintf("nfswalkpath: too many links\n");
#endif
				retval = SYSERR;
				break;
			}

			/* expand the symbolic link, put in workpath */
			if (nfssymlink(pnfs, newhandle, pwork, workpath) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
				kprintf("nfswalkpath: symlink error\n");
#endif
				retval = SYSERR;
				break;
			}

			pwork = workpath;
			if (*pwork == NFS_SEPARATOR) {
				/* absolute path, must back up ... */
				strcpy(pnfs->nfs_fpath,workpath);
				pnfs->nfs_status = NFSERR_ABSSYMLINK;
				retval = SYSERR;
				break;
			}
		} else {
			/************************************************/
			/*						*/
			/* any other file type, just continue and see	*/
			/* what happens...				*/
			/*						*/
			/************************************************/
#ifdef DEBUG
			if (*pwork != NULLCH) {
				kprintf("%s not a directory: type %d\n",
					currpart, newattr.fa_type);
			}
#endif
		}


		/* see if we're done */
		if (*pwork == NULLCH) {
			/* no more. take what we have */
			pnfs->nfs_phandle_valid = TRUE;
			blkcopy(pnfs->nfs_phandle, oldhandle, NFS_FHSIZE);
			pnfs->nfs_fhandle_valid = TRUE;
			blkcopy(pnfs->nfs_fhandle, newhandle, NFS_FHSIZE);
			blkcopy(&pnfs->nfs_attributes,&newattr,
				sizeof(struct fattr));
			pnfs->nfs_ftype = ftype;
			retval = OK;
			break;
		}

		/* unless symlink, walk to next directory */
		if (ftype != NFLNK)
		    blkcopy(oldhandle, newhandle, NFS_FHSIZE);
	}

	freemem(workpath,NFS_PATHLEN);
	return(retval);
}
