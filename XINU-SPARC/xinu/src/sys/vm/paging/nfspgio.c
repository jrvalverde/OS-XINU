/*----------------------------------------------------------------------
 * nfspgio.c - NFS paging threads
 *----------------------------------------------------------------------
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <frame.h>
#include <network.h>
#include <nfs.h>
#include <q.h>
#include <mark.h>
#include <pgio.h>
#include <nfspgio.h>

/*#define DEBUG*/
#define PRINTERRORS
#define INFO

LOCAL int saved_frames[MAX_PAGEOUT];
#define MAX_SWAP_FILE_LENGTH 	256

/* N.B. make sure the following structures are WORD aligned for SPARC */
LOCAL struct idinfo {
	int factive;
	unsigned writes;
	unsigned reads;
	fhandle handle;
};

LOCAL struct swapinfo {
	struct idinfo	pid[NPROC];
	struct idinfo	asid[NADDR];
	char dir[MAX_SWAP_FILE_LENGTH];
};

#define IDTOPINFO(id) ((ispid(id))?\
	 &swapinfo.pid[id] : &swapinfo.asid[asidindex(id)])


int	nfspgin_sem;
int	nfspgout_sem;
int	nfspginit_sem;

/* locally global variables */
LOCAL int	devlog;
LOCAL MARKER	pgmark;
LOCAL int	nfsprocpids;
LOCAL struct swapinfo swapinfo;
LOCAL int	gpq_create;
LOCAL int	gpq_page;
LOCAL int	pgpool;

/* external references */
extern char *index();

/*-------------------------------------------------------------------------
 * nfspgioproc - nfs paging process
 *-------------------------------------------------------------------------
 */
void nfspgioproc(who)
     int who;	/* 0: page in, 1: page out */
{
    int dev;

    if (who == 0) {
	init();
#ifdef INFO
	kprintf("NFS swap directory = %s\n", swapinfo.dir);
#endif
    }

    /* everyone waits here until things are initialized */
    wait(nfspginit_sem);
    

    /* open the paging device for this process */
    if ((dev = open(NAMESPACE, swapinfo.dir, "r")) == SYSERR) {
	panic("nfspgioproc: Couldn't open process swap file '%s'",
	      swapinfo.dir);
    }

    /* page in thread */
    if (who == 0) {
	struct pgreq *rqptr;
	
    	while (TRUE) {
	    wait(nfspgin_sem);
	    /* handle process creation/termination requests */
	    while (TRUE) {
		/* take the next node from the create request list. */
		/* If that's empty, try the page fetch list.        */
		rqptr = (struct pgreq *) deq(gpq_create);
	    
		if (rqptr == (struct pgreq *) NULL)
		    rqptr = (struct pgreq *) deq(gpq_page);
	    
		if (rqptr == (struct pgreq *) NULL)
		    break;
	    
		switch(rqptr->pm_type) {
		  case CREATE_REQ:
		    docreate(rqptr->pm_id);
		    break;

		  case TERMIN_REQ:
		    doterminate(rqptr->pm_id);
		    break;

		  case FETCH_REQ:
		    fetchpage(dev, rqptr->pm_id, rqptr->pm_wakepid,
			      rqptr->pm_vaddr, rqptr->pm_fn);
		    break;
		    
		  case FETCH_AOUT:
		    load_aout(rqptr->pm_id, rqptr->pm_wakepid,
			      rqptr->pm_vaddr, rqptr->pm_fn);
		    break;

		  default:
 		    panic("nfspg: illegal request (%d)", rqptr->pm_type);
		}
		freebuf(rqptr);
	    } /* while (TRUE) */
	} /* while */
    } /* if */

    /* page out thread */
    if (who == 1) {
	int findex;
	int count, morework = FALSE;
    
	while (TRUE) {			/* loop forever */
	    if (morework == FALSE)
		wait(nfspgout_sem);
	    
	    /* start with the first frame after the head of the	*/
	    /* modified list (might just be the tail)		*/
	    /* scan the modified list for pages to write out	*/
	    count = 0;
	    morework = TRUE;
	    while (morework) {
		/* remove one frame from the modified list */
		/* assume ftrmhd() calls disable()	       */
		if ((findex = ftrmhd(FTMHD)) == SYSERR) {
		    morework = FALSE;
		    break;		/* done */
		}

		if (ftobsolete(findex))	/* owner died */
		    saved_frames[count++] = findex;
		else
		    /* very possible to context switch in storepage() */
		    storepage(dev, &findex);
		
		/* check if pfault() may have reclaimed it */
		if ((findex >= 0) && ftlocked(findex))
		    saved_frames[count++] = findex;
	    
		if (count == MAX_PAGEOUT) {
		    morework = FALSE;
		    break;
		}
	    } /* while (morework) */
	    
	    if (count)
		freenframe(count, saved_frames);
	} /* while (TRUE) */
    } /* if */
}

/*-------------------------------------------------------------------------
 * nfspgiostart - 
 *-------------------------------------------------------------------------
 */
void nfspgiostart()
{
    kprintf("Starting NFS paging daemons...\n");
    nfspgioinit();
    nfspgin_sem = screate(0);		/* create pgingsem	*/
    nfspgout_sem = screate(0);		/* create pgingsem	*/
    nfspginit_sem = screate(0);		/* create pgingsem	*/

    /* create page in thread */
    resume(kcreate(NFSPGPROC, NFSPGSTK, NFSPGPRI, NFSPGIN, 1, 0));

    /* create page out thread */
    resume(kcreate(NFSPGPROC, NFSPGSTK, NFSPGPRI - 1, NFSPGOUT, 1, 1));
}

/*-------------------------------------------------------------------------
 * nfspgioterminate - 
 *-------------------------------------------------------------------------
 */
int nfspgioterminate(id)
     int id;
{
    insertreq(TERMIN_REQ, id, 0, 0, 0);
    return(OK);
}


/*-------------------------------------------------------------------------
 * nfspgiogetpage - fetch a page
 *-------------------------------------------------------------------------
 */
int nfspgiogetpage(id, findex, virt_addr)
     int      id, findex;
     unsigned virt_addr;
{
#ifdef DEBUG
    kprintf("nfspgiogetpage: id=%d, findex=%d, vaddr=%08x\n", id, findex, virt_addr);
#endif
    insertreq(FETCH_REQ, id, currpid, findex, virt_addr);

    /* wait for the page to arrive */
    wait(proctab[currpid].pginsem);
    return(OK);
}

/*-------------------------------------------------------------------------
 * load_iamge - send a reauest to page-in server to load a.out
 *-------------------------------------------------------------------------
 */
int load_image(asid, findex, virt_addr)
     int      asid, findex;
     unsigned virt_addr;
{
    insertreq(FETCH_AOUT, asid, currpid, findex, virt_addr);

    /* wait for the page to arrive */
    wait(proctab[currpid].pginsem);
    return(OK);
}


/*-------------------------------------------------------------------------
 * nfspgiocreate - create a process or address space
 *-------------------------------------------------------------------------
 */
int nfspgiocreate(id)
     int id;
{
    insertreq(CREATE_REQ, id, 0, 0, 0);
    return(OK);
}

/*-------------------------------------------------------------------------
 * nfspgpstohost - convert the netconf.pgserver field to a host name.
 *                 Intended for use when configuring the name of the
 *                 NFS pageserver host.
 *-------------------------------------------------------------------------
 */
char *nfspgpstohost()
{
    char buf[100];
    char *nam;
    char *pch;
    IPaddr ip;

    strcpy(buf, netconf.pgserver);

    /* remove the trailing colon for the port number */
    if ((pch = index(buf, ':')) != NULLPTR) {
	*pch = NULLCH;
    }

    dot2ip(ip, buf);
    ip2name(ip, buf);

    /* domain name qualifiers */
    if ((pch = index(buf, '.')) != NULLPTR) {
	*pch = NULLCH;
    }

    nam = (char *) getmem(strlen(buf)+1);
    strcpy(nam, buf);

    return(nam);
}


/*-------------------------------------------------------------------------
 * nfspgdump - dump the info on the nfs paging system
 *-------------------------------------------------------------------------
 */
void nfspgdump(dev)
     int dev;
{
    int id;

    fprintf(dev,"NFS Paging Information and Statistics\n");

    fprintf(dev,"\tswapping directory: %s\n", swapinfo.dir);

    fprintf(dev,"\n  Current swap usage:\n");
    fprintf(dev,"\tid   writes  reads\n");
    fprintf(dev,"\t===  ======  =====\n");
    for (id = 0; id < NPROC; ++id) {
	if (!swapinfo.pid[id].factive) continue;
	fprintf(dev,"\t%3d  %6d  %5d\n",
		id,
		swapinfo.pid[id].writes,
		swapinfo.pid[id].reads);
    }
    for (id = 0; id < NADDR; ++id) {
	if (!swapinfo.asid[id].factive) continue;
	fprintf(dev,"\t%3d %6d  %5d\n",
		id + ASIDBASE,
		swapinfo.asid[id].writes,
		swapinfo.asid[id].reads);
    }
}

/*
 * ====================================================================
 *
 *                       LOCAL ROUTINES
 *
 * ====================================================================
 */

/*-------------------------------------------------------------------------
 * nfspgioinit - initialize the NFS page server subsystem
 *-------------------------------------------------------------------------
 */
LOCAL nfspgioinit()
{
    devlog = SYSERR;

    /* create the queues for passing info */
    gpq_create = newq(RQNUM, QF_WAIT);
    gpq_page   = newq(RQNUM, QF_WAIT);

    /* create a pool to use for the messages */
    pgpool= mkpool(sizeof(struct pgreq), RQNUM);
    return(OK);
}

/*-------------------------------------------------------------------------
 * init - initialize the NFS page server subsystem
 *-------------------------------------------------------------------------
 */
LOCAL init()
{
    struct fattr fattr;
    char hostname[MAX_SWAP_FILE_LENGTH];
    char dirstr[MAX_SWAP_FILE_LENGTH];
    char *pch;

#ifdef DEBUG
    kprintf("nfspgio: init() called\n");
#endif
    /* create the name of the swap volume */
    sprintf(dirstr,"%s:%s:%s/", NFS_SWAP_HOST, NFS_SWAP_FS, NFS_SWAP_DIR);

#ifdef DEBUG
    kprintf("nfspgio: swap dir=%s\n", dirstr);
#endif

    /* mount the swap volume */
    if (mount(NFS_SWAP_ROOT, NFS, dirstr) != OK) {
	panic("nfs init(): couldn't mount '%s' on NFS as '%s'",
	      NFS_SWAP_ROOT, dirstr);
    }

    /* make sure the swap file system is there */
    if ((control(NFS, NFS_STATFILE, NFS_SWAP_ROOT, &fattr) != OK) ||
	(fattr.fa_type != NFDIR)) {
	kprintf("No swap directory: '%s'\n", dirstr);
	kprintf("** No VM paging support! **\n");
	
	return(SYSERR);
    }

    /* make sure there is a subdirectory for this host */
    getname(hostname);
    if (pch = index(hostname, '.'))
	*pch = NULLCH;

    sprintf(swapinfo.dir,"%s%s", NFS_SWAP_ROOT, hostname);

#ifdef DEBUG
    kprintf("nfspgio: swapinfo.dir=%s\n", swapinfo.dir);
#endif

    if (control(NFS, NFS_STATFILE, swapinfo.dir, &fattr) != OK) {
	/* create it if not there */
	if (control(NFS, NFS_MKDIR, swapinfo.dir, &fattr) != OK) {
	    panic("Couldn't create swap directory '%s'", swapinfo.dir);
	}
	kprintf("Created swap directory '%s'\n", swapinfo.dir);
    }

    nfsclearswap();

    /* tell everyone that everything is initialized */
    mark(pgmark);
    
    /* wake up nfspgin and nfspgout threads */
    signaln(nfspginit_sem, 2);
}


/*-------------------------------------------------------------------------
 * nfspgkill - kill a process or addr space because of paging problems
 *-------------------------------------------------------------------------
 */
/*VARARGS2*/
LOCAL nfspgkill(id,msg,a1,a2,a3,a4,a5)
     int id;
     char *msg;
{
    kprintf("process %d killed while paging: ", id);
    kprintf(msg,a1,a2,a3,a4,a5);
    kprintf("\n");

    doterminate(id);

    kill(id);
}

/*-------------------------------------------------------------------------
 * fetchpage - get a page from the swap file
 *	ASSUMPTION:
 *        In storeage(), we check to make sure that the swap file has
 *        been created before we try to do the store.  We don't have
 *        to check here by the transitive property.  We can only get a
 *        fetchpage() if the page was stored and then marked invalid.
 *        However, we couldn't have written it if the swap file isn't
 *        done.   -- sdo
 *-------------------------------------------------------------------------
 */
LOCAL fetchpage(dev, id, wakepid, his_virt_addr, findex)
     int	dev;
     int	id;
     int	wakepid;
     unsigned	his_virt_addr;
     int	findex;
{
    unsigned long time1, time2;
    struct idinfo	*pinfo;
    int ret;
    union pte   pte;
    
#ifdef DEBUG
    kprintf("<F%d>", findex); 
#endif
    pinfo = IDTOPINFO(id);
    /* map his frame into my VM space */
    pte.value = GetPageMap(PGR_COPY_BEGIN);
    pte.st.pfn = toframenum(findex);
    SetPageMap(PGR_COPY_BEGIN, pte.value);
    
    his_virt_addr = truncpg(his_virt_addr);
    nfsSetFilePos(dev, id, pinfo, his_virt_addr);	/* set the file offset */
    getrealtim(&time1);
    if ((ret = read(dev, PGR_COPY_BEGIN, PGSIZ)) != PGSIZ) {
	mvtoalist(findex);	/* let frame manager reclaim it */
	nfspgkill(id, "read swap file failed (err:%d)", ret);
	return;
    }
    getrealtim(&time2);

    /* gather paging stats */
    accintime += time2 - time1;
    intimes[cintimes++] = time2 - time1;
    ++pinfo->reads;
    if (cintimes == ITIMSIZE) 
	cintimes = 0;

    /* all done, wake the waiting owner	*/
    signal(proctab[wakepid].pginsem);
}


/*-------------------------------------------------------------------------
 * storepage - send a page to the swap file
 *-------------------------------------------------------------------------
 */
LOCAL storepage(dev, findex)
     int dev;
     int *findex;
{
    int	id, ret;
    unsigned long time1, time2;
    unsigned his_virt_addr;
    struct ppte    ppte;
    struct ppte    *ppteptr = &ppte;
    struct fte     *ftptr;
    struct idinfo	*pinfo;
    union pte   pte;

    ftptr = &ft[*findex];
    id = ftptr->id;
    pinfo = IDTOPINFO(id);
    /* paging out in progress       */
    ftptr->bits.st.pgout = TRUE;  
    
    /* map this frame into my VM space */
    
    pte.value = GetPageMap(PGS_COPY_BEGIN);
    pte.st.pfn = toframenum(*findex);
    SetPageMap(PGS_COPY_BEGIN, pte.value);
    
    his_virt_addr = tovaddr(ftptr->pageno);
    nfsSetFilePos(dev, id, pinfo, his_virt_addr);	/* set the file file offset */
#ifdef DEBUG
    kprintf("<S%d>", *findex);
#endif
    getrealtim(&time1);
    /* page out to disk swap area -- possible ctxsw() */
    if ((ret = write(dev, PGS_COPY_BEGIN, PGSIZ)) != PGSIZ) {
	kprintf("write to swap file failed (err:%d)", ret);
	nfspgkill(id, "write to swap file failed (err:%d)", ret);
	return;
    }
    getrealtim(&time2);

    ftptr->bits.st.pgout = FALSE;

    /* gather paging stats */
    pg.pagingouts++;
    accouttime += time2 - time1;
    outtimes[couttimes++] = time2 - time1;
    if (couttimes == OTIMSIZE)
	couttimes = 0;
    
    /* adjust the pte entries	*/
    ftgetppte(*findex, ppteptr);
    setmod(ppteptr, FALSE);
    
    if (ftptr->bits.st.wanted) {
	/* pfault() have reclaimed it */
	ftptr->bits.st.wanted = FALSE;
	*findex = -1;	/* to prevent moving it to the  reclaim list */
    }
#ifdef DEBUG
    kprintf("<SR%d>", *findex);
#endif
}

/*-------------------------------------------------------------------------
 * load_aout - load a text or data page
 *-------------------------------------------------------------------------
 */
LOCAL load_aout(asid, wakepid, vaddr, findex)
     int asid, wakepid, findex;
     unsigned vaddr;
{
    int             fd, pmode, ret;
    unsigned        tsize, dsize, txtoff, loadstart, fpos;
    struct  ppte    ppte;
    struct  ppte    *ppteptr;
    struct aentry   *aptr;
    struct exec     *paout;
    union pte   pte;
    
    aptr = &addrtab[asidindex(asid)];
    paout = &aptr->aout;
    fd = aptr->devload;
    
    tsize     = N_TEXTSIZE(*paout);
    dsize     = N_DATASIZE(*paout);
    loadstart = N_TXTADDR(*paout);
    txtoff    = N_TXTOFF(*paout);

    /* seek to the correct location in the file */
    vaddr = truncpg(vaddr);
    fpos = txtoff + (vaddr - loadstart);
    seek(fd, fpos);

    /* map his frame into my VM space */
    pte.value = GetPageMap(PGR_COPY_BEGIN);
    pte.st.pfn = toframenum(findex);
    SetPageMap(PGR_COPY_BEGIN, pte.value);
    
    if ((ret = read(fd, PGR_COPY_BEGIN, PGSIZ)) != PGSIZ) {
	mvtoalist(findex);		/* let frame manager reclaim it */
	nfspgkill(wakepid, "read swap file failed (err:%d)", ret);
	return;
    }

    if (vaddr <= (loadstart + tsize)) {
	/* text segment */
	/* this page should be read-only UNLESS some of the     */
	/* DATA segment resides on it                    */
	if ((truncpg(N_DATADDR(*paout)) == vaddr))
	    pmode = URW_KRW;            /* user read/write      */
	else
	    pmode = UR_KR;              /* user read only       */
    } else if (vaddr <= (loadstart + tsize + dsize)) {
	/* data segment */
	pmode = URW_KRW;                /* user read/write      */
    }

    /* set proper page protections */
    ppteptr = &ppte;
    getppte(asid, vaddr, ppteptr);
    setprot(ppteptr, pmode);
    setmod(ppteptr, TRUE);
    setaout(ppteptr, FALSE);
    
    /* make sure 2-hand clock daemon does not reclaim it so soon */
    setreference(ppteptr, TRUE);
    /* all done, wake the waiting owner	*/
    signal(proctab[wakepid].pginsem);
}


/*-------------------------------------------------------------------------
 * nfsSetFilePos - set the device to an offset based on the virtual address
 *		   NB: do not try this at home
 *-------------------------------------------------------------------------
 */
LOCAL nfsSetFilePos(dev, id, pinfo, addr)
     int dev, id;
     struct idinfo *pinfo;
     unsigned addr;
{
    struct nfsblk *pnfs;
    unsigned pos;

    pnfs = &nfstab[devtab[dev].dvminor];

    /* set the file handle */
    blkcopy(pnfs->nfs_fhandle, pinfo->handle, NFS_FHSIZE);

    /* to keep the disk data as small as possible:		*/
    /* isasid(id): (address space id)				*/
    /*   Disk file is laid out the same as address space's.	*/ 
    /*   Pages are stored from the beginning of the disk file   */
    /*   in natural order.					*/
    /* 								*/
    /* ispid(id):						*/
    /*   Per process/thread stack space is laid out in reverse	*/
    /*   order, i.e., last page is first on disk.		*/
    /* 								*/
    if (ispid(id)) {
	pos = PGSIZ * (topgnum(vmaddrsp.stkstrt) - topgnum(addr));
    } else {
	pos = addr;
    }

    /* set the file position */
    pnfs->nfs_fpos = pos;
}


/*-------------------------------------------------------------------------
 * nfsrmid - remove the swapping files
 *-------------------------------------------------------------------------
 */
LOCAL nfsrmid(id)
     int id;
{
    char filbuf[MAX_SWAP_FILE_LENGTH];

    sprintf(filbuf,"%s/%d", swapinfo.dir, id);

    remove(filbuf);
}



/*-------------------------------------------------------------------------
 * nfsclearswap - clear the nfs swap directory
 *-------------------------------------------------------------------------
 */
LOCAL nfsclearswap()
{
#define MAXENTRIES 10

    struct nfs_readdir_entry ent[MAXENTRIES];
    struct nfs_readdir_req req;
    char filbuf[MAX_SWAP_FILE_LENGTH];
    int dev, j;

#ifdef DEBUG
    kprintf("nfsclearswap: clear swap dir=%s\n", swapinfo.dir);
#endif
    /* read the swap directory */
    if ((dev = open(NAMESPACE, swapinfo.dir, "ro")) == SYSERR) {
	panic("nfsclearswap: couldn't read swap directory '%s'", swapinfo.dir);
    }

    /* loop until all files erased */
    req.pent = ent;
    req.cookie = 0;
    req.eof = FALSE;
    while (!req.eof) {
	req.nentries = MAXENTRIES;

	if (control(dev, NFS_READDIR, &req) == SYSERR) {
	    panic("nfsclearswap: couldn't perform NFS_READDIR on '%s'",
		  swapinfo.dir);
	}

	for (j=0; j < req.nentries; ++j) {
	    if (*ent[j].filename == '.')
		continue;		/* leave "dot" files alone */
	    sprintf(filbuf,"%s/%s", swapinfo.dir, ent[j].filename);
#ifdef DEBUG
	    kprintf("nfsclearswap: Removing old file '%s'\n", filbuf);
#endif
	    remove(filbuf);
	}
    }					/* while (req.nentries == MAXENTRIES);*/

    close(dev);
}


/*-------------------------------------------------------------------------
 * doterminate - terminate an address space or process
 *-------------------------------------------------------------------------
 */
LOCAL doterminate(id)
     int id;
{
    PStype ps;
    struct idinfo	*pinfo;

    disable(ps);
    pinfo = IDTOPINFO(id);

    if (pinfo->factive) {
	pinfo->factive = FALSE;
	nfsrmid(id);	/* remove swap files */
    }
    restore(ps);
}

/*-------------------------------------------------------------------------
 * nfsinitid - start a new process or address space
 *-------------------------------------------------------------------------
 */
LOCAL int nfsinitid(id)
     int id;
{
    char swappath[MAX_SWAP_FILE_LENGTH];
    struct nfsblk *pnfs;
    struct idinfo   *pinfo;
    int dev;

    sprintf(swappath,"%s/%d", swapinfo.dir, id);
    if ((dev = open(NAMESPACE, swappath, "w")) == SYSERR) {
	nfspgkill(id, "Couldn't create process swap file");
	return(SYSERR);
    }

    pnfs = &nfstab[devtab[dev].dvminor];
    pinfo = IDTOPINFO(id);
    blkcopy(pinfo->handle, pnfs->nfs_fhandle, NFS_FHSIZE);

    close(dev);
    return(OK);
}

/*-------------------------------------------------------------------------
 * docreate - create an address space or process
 *-------------------------------------------------------------------------
 */
LOCAL docreate(id)
     int id;
{
    PStype ps;
    struct idinfo *pinfo;

    disable(ps);
    (void) nfsinitid(id);

    pinfo = IDTOPINFO(id);

    if (ispid(id))
	proctab[id].psinformed = TRUE;
    else if (isasid(id))
	addrtab[asidindex(id)].psinformed = TRUE;

    pinfo->factive = TRUE;
    pinfo->reads = 0;
    pinfo->writes = 0;
    restore(ps);
}


/*-------------------------------------------------------------------------
 * insertreq - insert a request into the list
 *-------------------------------------------------------------------------
 */
LOCAL insertreq(type, id, wakepid, findex, vaddr)
     int	type;		/* type of request to enqueue		*/
     int	id;   		/* id to operate on			*/
     int	wakepid;	/* process involved			*/
     int	findex;		/* frame to operate on			*/
     int	vaddr;		/* virtual address			*/
{
    PStype  	ps;
    int 	ret;
    struct pgreq *preq;
    
    disable(ps);
    preq = (struct pgreq *) getbuf(pgpool);

    preq->pm_type    = type;
    preq->pm_id      = id;
    preq->pm_wakepid = wakepid;
    preq->pm_fn      = findex;
    preq->pm_vaddr   = vaddr;

    switch (type) {
      case CREATE_REQ:
      case TERMIN_REQ:
	ret = enq(gpq_create, preq, 0);
	break;
      case FETCH_REQ:
      case FETCH_AOUT:
	ret = enq(gpq_page, preq, 0);
	break;
      default:
	panic("insertreq: illegal option");
    }
    if (ret <= 0) {
	panic("insertreq: couldn't enq");
    }
    restore(ps);
    signal(nfspgin_sem);	/* wakeup pgio server */
}
