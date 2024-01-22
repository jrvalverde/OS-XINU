/* nfscache - fhandle caching routines */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>
#include <gcache.h>


/*#define DEBUG*/

#define NFS_CACHE_MAXENT  50
#define NFS_CACHE_MAXLIFE 300	/* 5 minutes */


/* the question */
static struct hcce_q {
     fhandle	handle;
     char	name[NFS_FILELEN];
};

/* the answer */
static struct hcce_a {
     fhandle	handle;
     struct fattr attr;
};


static int makequestion();


/*
 * ====================================================================
 * nfshclookup - look in the generic cache for an addr
 * ====================================================================
 */
int nfshclookup(pme, inhandle, name, outhandle, pattr)
     struct mountentry *pme;
     fhandle inhandle;
     char *name;
     fhandle outhandle;
     struct fattr *pattr;
{
    struct hcce_q q;
    struct hcce_a a;
    int cid = pme->mnt_hcache;
    int stat;
    int qlen;
    int alen;


    if (ISBADCACHE(cid))
	return(SYSERR);

#ifdef DEBUG
    kprintf("nfshclookup(%s) called\n", name);
#endif

    /* fill out the question */
    qlen = makequestion(&q,inhandle,name);

    alen = sizeof(a);

    stat = calookup(cid, &q, qlen, &a, &alen);

    if (stat != OK)
	return(SYSERR);			/* not found */

    /* copy out the answer */
    blkcopy(outhandle,a.handle,sizeof(fhandle));
    blkcopy(pattr,&a.attr,sizeof(struct fattr));
#ifdef DEBUG
    kprintf("nfshclookup(%s): found it\n", name);
#endif
    return(OK);
}


/*
 * ====================================================================
 * nfshcinsert - insert a new entry in the cache
 * ====================================================================
 */
int nfshcinsert(pme, inhandle, name, outhandle, pattr)
     struct mountentry *pme;
     fhandle inhandle;
     char *name;
     fhandle outhandle;
     struct fattr *pattr;
{

    struct hcce_q q;
    struct hcce_a a;
    int cid = pme->mnt_hcache;
    int qlen;

    if (ISBADCACHE(cid))
	return(SYSERR);

#ifdef DEBUG
    kprintf("nfshcinsert(%s) called\n", name);
#endif

    /* fill out the question */
    qlen = makequestion(&q,inhandle,name);

    /* ... and the answer */
    bzero(&a,sizeof(a));
    blkcopy(a.handle,outhandle,sizeof(fhandle));
    blkcopy(&a.attr,pattr,sizeof(struct fattr));

    return(cainsert(cid, &q, qlen, &a, sizeof(a)));
}



/*
 * ====================================================================
 * nfshccreate - create a file handle cache for the given nfs mount 
 * ====================================================================
 */
nfshccreate(pme)
     struct mountentry *pme;
{
    char caname[128];

    sprintf(caname,"%s@%s", pme->mnt_fs, pme->mnt_host);
    pme->mnt_hcache = cacreate(caname, NFS_CACHE_MAXENT, NFS_CACHE_MAXLIFE);
#ifdef DEBUG
    kprintf("hchccreate: made cache %s\n", caname);
#endif
}


/*
 * ====================================================================
 * nfshcdestroy - destroy the file handle cache for the given nfs mount 
 * ====================================================================
 */
nfshcdestroy(pme)
     struct mountentry *pme;
{
    if (!ISBADCACHE(pme->mnt_hcache))
	cadestroy(pme->mnt_hcache);
    pme->mnt_hcache = BADCID;
}



/*
 * ====================================================================
 * nfshcpurge - purge the file handle cache
 * ====================================================================
 */
nfshcpurge(pme)
     struct mountentry *pme;
{
    if (!ISBADCACHE(pme->mnt_hcache))
	capurge(pme->mnt_hcache);
}



/*
 * ====================================================================
 * makequestion - make up a question packet, return the length
 * ====================================================================
 */
static int makequestion(pq,handle,name)
     struct hcce_q *pq;
     fhandle handle;
     char *name;
{
    char *pchto;
	
    bzero(pq, sizeof(struct hcce_q));
    blkcopy(pq->handle, handle, sizeof(fhandle));

    /* copy the name in */
    pchto = pq->name;
    while (*name != NULLCH) {
	*pchto++ = *name++;
    }

    /* compute the length (short string) */
    return((int) ((unsigned) pchto - (unsigned) pq));
}

