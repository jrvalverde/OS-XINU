/* gcache.c - generalized cacheing routines */
/* Mon Oct 21, 1991 - Shawn Ostermann */

#include <conf.h>
#include <kernel.h>
#include <gcache.h>

LOCAL Bool ca_enabled;

#define PRINTERRORS

/* types used throughout */
typedef u_int	tcelen;		/* length of a cached entry		*/
typedef u_int	tceix;		/* index of a cached entry		*/
typedef u_int	thval;		/* type of the hashed value of a key	*/
typedef u_int	ttstamp;	/* type of a timestamp			*/


/* control block for a single cache */
enum cb_status { CB_INUSE=1, CB_FREE=2};
typedef enum cb_status cb_status;
struct cacheblk {
	cb_status	cb_status;	/* INUSE or FREE		*/
	char		cb_name[CA_NAMELEN]; /* name of the cache	*/
	int		cb_mutex;	/* mutual exclusion semaphore	*/
	u_int		cb_maxent;	/* maximum entries		*/
	u_int		cb_nument;	/* number of entries		*/
	u_int		cb_hashsize;	/* size of hash table		*/
	u_int		cb_maxlife;	/* max life of an entry (secs)	*/
	struct cacheentry *cb_cache;	/* free nodes for the cache	*/
	struct hashentry *cb_hash;	/* the hash table		*/
	tceix		cb_freelist;	/* list of free cacheentries	*/
	/* statistics variables, mostly for debugging			*/
	u_int		cb_lookups;	/* # lookups			*/
	u_int		cb_hits;	/* # hits			*/
	u_int		cb_tos;		/* # timed out entries		*/
	u_int		cb_fulls;	/* # removed, full table	*/
};



/* a single node in the hash table */
struct hashentry {
	tceix	he_ix;
};
#define NULL_PHE	((struct hashentry *) 0)
#define NULL_IX		0


/* a cached item in the hash list */
enum ce_status {CE_INUSE=11, CE_FREE=12};
typedef enum ce_status ce_status;
struct cacheentry {
	ce_status	ce_status;	/* INUSE or FREE		*/
	char		*ce_keyptr;	/* pointer to the key		*/
	tcelen		ce_keylen;	/* length of the key		*/
	char		*ce_resptr;	/* pointer to the result	*/
	tcelen		ce_reslen;	/* length of the result		*/
	thval		ce_hash;	/* value that was hashed in	*/
	ttstamp		ce_tsinsert;	/* timestamp - time inserted	*/
	ttstamp		ce_tsaccess;	/* timestamp - last access	*/
	tceix		ce_prev;	/* next entry on list		*/
	tceix		ce_next;	/* prev entry on list		*/
};
#define NULL_PCE	((struct cacheentry *) 0)


/* locally global information */
LOCAL int camutex;
LOCAL struct cacheblk catab[CA_NUMCACHES];


/* useful macros */
#define ISBADCID(cid) ((cid < 0) || (cid > CA_NUMCACHES) || \
		       (catab[cid].cb_status != CB_INUSE))
#define HASHTOIX(hash,pcb) ((hash) % (pcb->cb_hashsize))

/* local routines defns */
LOCAL char	*cagetmem();
LOCAL void	cadeleteold();
LOCAL void	caclear();
LOCAL void	cafreemem();
LOCAL tceix	cagetfree();
LOCAL tceix	cagetindex();
LOCAL thval	cahash();
LOCAL int	caisold();
LOCAL void	casetsize();
LOCAL void	caunlink();

/* external routine definitions */
extern char	*getmem();
extern int	freemem();
extern void	gettime();
extern void	wait();
extern void	signal();
extern void	bzero();
extern int	screate();
extern void	sdelete();
extern int	blkequ();
extern void	kprintf();

/*************************************************************************/
/**									**/
/**	GLOBAL ROUTINES							**/
/**									**/
/*************************************************************************/

/*
 * ====================================================================
 * cainit - initialize the caching tables
 *          called in initialize.c
 * ====================================================================
 */
int cainit()
{
    struct cacheblk *pcb;
    int cid;

    for (cid=0; cid < CA_NUMCACHES; ++cid) {
	pcb = &catab[cid];
	bzero(pcb,sizeof(struct cacheblk));
	pcb->cb_status = CB_FREE;
    }
    camutex = screate(1);
    ca_enabled = TRUE;
    return(OK);
}


/*
 * ====================================================================
 * cacreate - create a new cache
 * ====================================================================
 */
int cacreate(name,nentries,lifetime)
     char *name;
     int nentries;
     int lifetime;
{
    int cid;
    tceix ix;
    struct cacheblk *pcb;
    struct cacheentry *pce;

    /* check config limits */
    if (nentries >= CA_MAXENTRIES) {
	return(SYSERR);
    }

    wait(camutex);
    for (cid=0; cid < CA_NUMCACHES; ++cid) {
	pcb = &catab[cid];
	if (pcb->cb_status == CB_FREE)
	    break;
    }

    if (cid == CA_NUMCACHES) {
	signal(camutex);
	return(SYSERR);
    }

    bzero(pcb, sizeof(struct cacheblk));
    pcb->cb_status = CB_INUSE;
    pcb->cb_mutex = screate(1);
    strncpy(pcb->cb_name,name,CA_NAMELEN);
    pcb->cb_name[CA_NAMELEN-1] = NULLCH;
    pcb->cb_maxent = nentries;
    casetsize(pcb,nentries);
    pcb->cb_maxlife = lifetime;

    /* allocate the cache entries */
    /* (0 is reserved as a null pointer, so we allocate from 0 	*/
    /*  to maxent, rather than maxent-1)				*/
    pcb->cb_cache = (struct cacheentry *)
	cagetmem((1+pcb->cb_maxent) * sizeof(struct cacheentry));
    bzero(pcb->cb_cache,
	  (1+pcb->cb_maxent) * sizeof(struct cacheentry));
    /* put them all on the free list (only forward pointers) */
    for (ix=1; ix <= pcb->cb_maxent; ++ix) {
	pce = &pcb->cb_cache[ix];
	pce->ce_status = CE_FREE;
	pce->ce_next = ix+1;
    }
    pcb->cb_cache[pcb->cb_maxent].ce_next = NULL_IX;
    pcb->cb_freelist = 1;

    /* allocate the hash table */
    pcb->cb_hash = (struct hashentry *)
	cagetmem(pcb->cb_hashsize * sizeof(struct hashentry));
    for (ix=0; ix < pcb->cb_hashsize; ++ix) {
	pcb->cb_hash[ix].he_ix = NULL_IX;
    }

    signal(camutex);

    return(cid);
}


/*
 * ====================================================================
 * cadestroy - destroy an existing cache
 * ====================================================================
 */
int cadestroy(cid)
     int cid;
{
    struct cacheblk *pcb;

    if (ISBADCID(cid))
	return(SYSERR);

    pcb = &catab[cid];

    wait(pcb->cb_mutex);

    if (ISBADCID(cid))
	return(SYSERR);

    /* free up all the entries */
    (void) capurge(cid);

    /* free up the hash table */
    cafreemem(pcb->cb_hash, pcb->cb_hashsize * sizeof(struct hashentry));

    /* free up the cached blocks */
    cafreemem(pcb->cb_cache, (1+pcb->cb_maxent) * sizeof(struct cacheentry));

    /* delete the semaphore */
    sdelete(pcb->cb_mutex);

    /* zero out the table */
    bzero(pcb,sizeof(struct cacheblk));
    pcb->cb_status = CB_FREE;

    return(OK);
}



/*
 * ====================================================================
 * cainsert - insert a new entry into an existing cache
 * ====================================================================
 */
int cainsert(cid,pkey,keylen,pres,reslen)
     int cid;
     char *pkey;
     tcelen keylen;
     char *pres;
     tcelen reslen;
{
    struct cacheblk *pcb;
    struct cacheentry *pce;
    struct hashentry *phe;
    thval hash;
    tceix ixnew;

    /* check argument validity */
    if (ISBADCID(cid))
	return(SYSERR);
    if ((keylen > CA_MAXKEY) || (reslen > CA_MAXRES)) {
	return(SYSERR);
    }

    if (!ca_enabled)
	return(OK);

    pcb = &catab[cid];
    wait(pcb->cb_mutex);

    hash = cahash(pkey,keylen);
    phe = &pcb->cb_hash[HASHTOIX(hash,pcb)];

    if ((ixnew = cagetindex(pcb,pkey,keylen,hash)) != NULL_IX) {
	/* use the old one */
	caclear(pcb,ixnew);
	pce = &pcb->cb_cache[ixnew];
    } else {
	/* get a free cacheentry */
	ixnew = cagetfree(pcb);
	pce = &pcb->cb_cache[ixnew];

	/* ... and put it at the head of the list */
	pce->ce_prev = 0;
	pce->ce_next = phe->he_ix;
	pcb->cb_cache[phe->he_ix].ce_prev = ixnew;
	phe->he_ix = ixnew;
    }

    pce->ce_status = CE_INUSE;
    pce->ce_hash = hash;
    pce->ce_keyptr = cagetmem(keylen);
    pce->ce_keylen = keylen;
    blkcopy(pce->ce_keyptr,pkey,keylen);
    pce->ce_resptr = cagetmem(reslen);
    pce->ce_reslen = reslen;
    blkcopy(pce->ce_resptr,pres,reslen);
    gettime(&pce->ce_tsinsert);
    pce->ce_tsaccess = pce->ce_tsinsert;

    signal(pcb->cb_mutex);
    return(OK);
}


/*
 * ====================================================================
 * calookup - find an entry in the cache given the key, return info
 * ====================================================================
 */
int calookup(cid,pkey,keylen,pres,preslen)
     int cid;
     char *pkey;
     tcelen keylen;
     char *pres;
     tcelen *preslen;
{
    struct cacheblk *pcb;
    struct cacheentry *pce;
    thval hash;
    tceix ix;

    if (ISBADCID(cid))
	return(SYSERR);

    if (!ca_enabled)
	return(SYSERR);

    pcb = &catab[cid];
    wait(pcb->cb_mutex);
    hash = cahash(pkey,keylen);
    if ((ix = cagetindex(pcb,pkey,keylen,hash)) != NULL_IX) {
	pce = &pcb->cb_cache[ix];

	if (pce->ce_reslen <= *preslen) {
	    gettime(&pce->ce_tsaccess);
	    blkcopy(pres, pce->ce_resptr, pce->ce_reslen);
	    *preslen = pce->ce_reslen;
	    signal(pcb->cb_mutex);
	    return(OK);
	}
    }

    signal(pcb->cb_mutex);
    return(SYSERR);
}


/*
 * ====================================================================
 * caremove - remove an entry from the cache if it exists
 * ====================================================================
 */
int caremove(cid,pkey,keylen)
     int cid;
     char *pkey;
     tcelen keylen;
{
    struct cacheblk *pcb;
    unsigned hash;
    tceix ix;

    if (ISBADCID(cid))
	return(SYSERR);

    pcb = &catab[cid];
    wait(pcb->cb_mutex);
    hash = cahash(pkey,keylen);
    if ((ix = cagetindex(pcb,pkey,keylen,hash)) != NULL_IX) {
	caunlink(pcb,ix);
    }

    signal(pcb->cb_mutex);
    return(OK);
}


/*
 * ====================================================================
 * capurge - remove all entries in an existing cache
 * ====================================================================
 */
int capurge(cid)
     int cid;
{
    struct cacheblk *pcb;
    struct cacheentry *pce;
    struct hashentry *phe;
    tceix ix;

    if (ISBADCID(cid))
	return(SYSERR);

    pcb = &catab[cid];
    wait(pcb->cb_mutex);

    /* free all cached entries */
    for (ix=1; ix <= pcb->cb_maxent; ++ix) {
	pce = &pcb->cb_cache[ix];
	if (pce->ce_status == CE_INUSE)
	    caunlink(pcb,ix);
	pce->ce_status = CE_FREE;
    }

    /* clear the hash table */
    for (ix=0; ix < pcb->cb_hashsize; ++ix) {
	phe = &pcb->cb_hash[ix];
	bzero(phe,sizeof(struct hashentry));
	phe->he_ix = NULL_IX;
    }

    pcb->cb_nument = 0;

    signal(pcb->cb_mutex);
    return(OK);
}

/*
 * ====================================================================
 * cadump - dump contents of the cache structures
 * ====================================================================
 */
/*ARGSUSED*/
void cadump(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    int cid;
    Bool purge;
    Bool zero;
    int i;
    struct cacheblk *pcb;

    purge = zero = FALSE;

    for (i=1; i < nargs; ++i) {
	if  (strcmp(args[i],"-p") == 0) {
	    kprintf("purging all caches\n");
	    purge = TRUE;
	} else if  (strcmp(args[i],"-z") == 0) {
	    kprintf("zero'ing all cache counters\n");
	    zero = TRUE;
	} else if  (strcmp(args[i],"-d") == 0) {
	    kprintf("disabling all caches\n");
	    ca_enabled = FALSE;
	} else if  (strcmp(args[i],"-e") == 0) {
	    kprintf("re-enabling caches\n");
	    ca_enabled = TRUE;
	} else {
	    kprintf("Usage: %s [-p] [-z] [-e] [-d]\n", args[0]);
	    kprintf("  -p   purge all caches\n");
	    kprintf("  -z   zero all cache counters\n");
	    kprintf("  -d   disable cache access\n");
	    kprintf("  -e   re-enable cache access\n");
	    return;
	}
    }

    wait(camutex);
    kprintf("\nmutex: %d  maxcaches: %d   (caching %sabled)\n",
	    camutex, CA_NUMCACHES,
	    ca_enabled?"en":"DIS");
    kprintf("\
ix name            sem maxent nument htsize life tos  full finds  hits   hit%%\n");
    kprintf("\
== =============== === ====== ====== ====== ==== ==== ==== ====== ====== ====\n");
    for (cid=0; cid < CA_NUMCACHES; ++cid) {
	pcb = &catab[cid];
	if (pcb->cb_status == CB_FREE)
	    continue;

	if (purge)
	    capurge(cid);

	if (zero) {
	    pcb->cb_tos = 0;
	    pcb->cb_fulls = 0;
	    pcb->cb_lookups = 0;
	    pcb->cb_hits = 0;
	}

	kprintf("%2d %-15s %3d %6d %6d %6d %4d %4d %4d %6d %6d %3d%%",
		cid,
		pcb->cb_name,
		pcb->cb_mutex,
		pcb->cb_maxent,
		pcb->cb_nument,
		pcb->cb_hashsize,
		pcb->cb_maxlife,
		pcb->cb_tos,
		pcb->cb_fulls,
		pcb->cb_lookups,
		pcb->cb_hits,
		(pcb->cb_lookups)?
		((100 * pcb->cb_hits) / pcb->cb_lookups):0);
	kprintf("\n");
    }
    signal(camutex);
}



/*************************************************************************/
/**									**/
/**	LOCAL FUNCTIONS							**/
/**									**/
/*************************************************************************/


/*
 * ====================================================================
 * cahash - return the hash value for a key
 * ====================================================================
 */
LOCAL thval cahash(pkey,keylen)
     char *pkey;
     tcelen keylen;
{
    int i;
    thval hval;

    hval = 0;
    for (i=0; i < keylen; ++i)
	hval += *pkey++;
    return(hval);
}



/* try to reduce fragmentation */
#define CAMEMSIZE(nb) ((unsigned) (((nb) + 31) & ~31))
/*
 * ====================================================================
 * cagetmem - get memory for a cached entry
 * ====================================================================
 */
LOCAL char *cagetmem(nbytes)
     u_int nbytes;
{
    return(getmem(CAMEMSIZE(nbytes)));
}


/*
 * ====================================================================
 * cafreemem - free memory from a cached entry
 * ====================================================================
 */
LOCAL void cafreemem(ptr,nbytes)
     char *ptr;
     u_int nbytes;
{
    freemem(ptr,CAMEMSIZE(nbytes));
}




/*
 * ====================================================================
 * cadeleteold - delete the "oldest" cached entry
 *      N.B.  assumes MUTEX is already held
 * ====================================================================
 */
LOCAL void cadeleteold(pcb)
     struct cacheblk *pcb;
{
    struct cacheentry *pce;
    unsigned oldtime;
    tceix oldix;
    tceix ix;

    /* check everyone against the first entry */
    oldix = 1;
    pce = &pcb->cb_cache[oldix];
    oldtime = pce->ce_tsaccess;

    for (ix=2; ix <= pcb->cb_maxent; ++ix) {
	pce = &pcb->cb_cache[ix];
	if ((pce->ce_status == CE_INUSE) &&
	    (pce->ce_tsaccess < oldtime)) {
	    oldix = ix;
	    oldtime = pce->ce_tsaccess;
	}
    }

    /* nuke the oldest one */
    pce = &pcb->cb_cache[oldix];
    caunlink(pcb,oldix);
    return;
}


/*
 * ====================================================================
 * caclear - clear out the given entry, set status to FREE
 * ====================================================================
 */
LOCAL void caclear(pcb,ix)
     struct cacheblk *pcb;
     tceix ix;
{
    struct cacheentry *pce;

    pce = &pcb->cb_cache[ix];
    if (pce->ce_keyptr)
	cafreemem(pce->ce_keyptr,pce->ce_keylen);
    if (pce->ce_resptr)
	cafreemem(pce->ce_resptr,pce->ce_reslen);
    bzero(pce,sizeof(struct cacheentry));
    pce->ce_status = CE_FREE;
}



/*
 * ====================================================================
 * caisold - return TRUE if the given entry is "too old"
 * ====================================================================
 */
LOCAL int caisold(pcb,pce)
	struct cacheblk *pcb;
	struct cacheentry *pce;
{
    unsigned now;

    if (pcb->cb_maxlife == 0)
	return(FALSE);

    gettime(&now);

    return ((now - pce->ce_tsaccess) > pcb->cb_maxlife);
}


/*
 * ====================================================================
 * cagetindex - return the index of a matching entry, or SYSERR
 *      N.B.  assumes MUTEX is already held
 * ====================================================================
 */
LOCAL tceix cagetindex(pcb,pkey,keylen,hash)
     struct cacheblk *pcb;
     char *pkey;
     tcelen keylen;
     thval hash;
{
    struct cacheentry *pce;
    tceix ix;
    tceix nextix;

    ++pcb->cb_lookups;

    ix = pcb->cb_hash[HASHTOIX(hash,pcb)].he_ix;

    while (ix != NULL_IX) {
	pce = &pcb->cb_cache[ix];
	nextix = pce->ce_next;

	if ((pce->ce_hash == hash) &&
	    (pce->ce_keylen == keylen) &&
	    (blkequ(pkey,pce->ce_keyptr,keylen))) {
	    /* this is a match */
	    ++pcb->cb_hits;
	    if (caisold(pcb,pce)) {
		++pcb->cb_tos;
		caunlink(pcb,ix);
		return(NULL_IX);
	    } else {
		return(ix);
	    }
	}
	ix = nextix;
    }

    return(NULL_IX);
}


/*
 * ====================================================================
 * casetsize - set the hash table size
 * ====================================================================
 */
LOCAL void casetsize(pcb,nentries)
     struct cacheblk *pcb;
     int nentries;
{
    if (nentries <= 10) 
	pcb->cb_hashsize = 13;
    else if (nentries <= 25) 
	pcb->cb_hashsize = 29;
    else if (nentries <= 50) 
	pcb->cb_hashsize = 53;
    else if (nentries <= 100) 
	pcb->cb_hashsize = 101;
    else if (nentries <= 200) 
	pcb->cb_hashsize = 213;
    else 
	pcb->cb_hashsize = (nentries * 5) >> 2; /* nentries * 1.25 */	
    return;
}


/*
 * ====================================================================
 * cagetfree - return the index of an unused pce.  If there aren't any
 *            left, delete an old one and return it.
 *      N.B.  assumes MUTEX is already held
 * ====================================================================
 */
LOCAL tceix cagetfree(pcb)
	struct cacheblk *pcb;
{
    struct cacheentry *pce;
    tceix ix;

    /* if the free list is empty, delete the oldest entry */
    if (pcb->cb_freelist == NULL_IX) {
	cadeleteold(pcb);
	++pcb->cb_fulls;
    }

    /* remove the head of the list */
    ix = pcb->cb_freelist;
    pce = &pcb->cb_cache[ix];
    pcb->cb_freelist = pce->ce_next;
    ++pcb->cb_nument;

    return(ix);
}



/*
 * ====================================================================
 * caunlink - remove a cached entry from a list (and erase it)
 *	      return it to the free list
 * ====================================================================
 */
LOCAL void caunlink(pcb,ix)
     struct cacheblk *pcb;
     tceix ix;	
{
    struct cacheentry *pce;
    struct hashentry *phe;
    thval hash;

    pce = &pcb->cb_cache[ix];
    hash = pce->ce_hash;
    phe = &pcb->cb_hash[HASHTOIX(hash,pcb)];

    if (pce->ce_prev == NULL_IX)
	phe->he_ix = pce->ce_next;
    else 
	pcb->cb_cache[pce->ce_prev].ce_next = pce->ce_next;

    pcb->cb_cache[pce->ce_next].ce_prev = pce->ce_prev;

    caclear(pcb,ix);

    /* return it to the free list */
    pce->ce_next = pcb->cb_freelist;
    pcb->cb_freelist = ix;
    --pcb->cb_nument;
}
