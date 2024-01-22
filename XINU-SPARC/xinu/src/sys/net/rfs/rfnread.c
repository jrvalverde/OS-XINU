/* rfnread.c - rfnread */

#include <conf.h>
#include <kernel.h>
#include <network.h>


#define PRINTERRORS


#define MAXPKTS min(UPPLEN,8)

struct rfnstr {
	char	*pch;
	int	len;
	long	pos;
	struct	fpacket fp;
	int	replyd;
} rfnstr[MAXPKTS];


/*------------------------------------------------------------------------
 *  rfnread  --  perform more efficient reads
 *------------------------------------------------------------------------
 */
rfnread(name,ppos,buf,len)
     char *name;
     int *ppos;
     char *buf;
     int len;
{
	int npack;
	int reqpack;
	int reqlen;
	int ret;
	int nbytes;

	wait(Rf.rmutex);

#ifdef DEBUG
	kprintf("rfnread(%s,0x%x,%d,0x%x)\n",name,buf,len,*ppos);
#endif

	/* set up the headers */
	for (npack = 0; npack < MAXPKTS; ++npack) {
		rfnstr[npack].fp.fp_h.f_op = hs2net(FS_READ);
		strncpy(rfnstr[npack].fp.fp_h.f_name, name, RNAMLEN);
	}

	nbytes = 0;
	while (len > 0) {
		/* see how many packets it will take */
		npack = (len + RDATLEN - 1) / RDATLEN;
#ifdef DEBUG
		kprintf("\nrfnread: will take %d packets\n", npack);
#endif
		reqpack = min(npack,MAXPKTS);
		reqlen  = reqpack * RDATLEN;
		ret = rfnsend(reqpack,reqlen,buf,*ppos);
		if (ret == SYSERR) {
			continue;  /* just try again */
		}
		nbytes += ret;
		*ppos += ret;
		len -= ret;
		buf += ret;

		if (ret < reqlen) /* EOF */
		    break;
	}

	signal(Rf.rmutex);
	return(nbytes);
}


/*------------------------------------------------------------------------
 *  rfnsend  --  perform n sends
 *------------------------------------------------------------------------
 */
rfnsend(npack,len,pbuf,pos)
     int npack;
     int len;
     char *pbuf;
     int pos;
{
	STATWORD ps;    
	struct rfnstr *sptr;
	struct fpacket *fptr;
	int i;
	int plen;
	int ret;
	int nbytes;

	/* write the packet headers */
	for (i=0; i < npack; ++i) {
		sptr = &rfnstr[i];
		fptr = &sptr->fp;

		plen = min(len,RDATLEN);

		fptr->fp_h.f_pos   = (long) hl2net(pos);
		sptr->pos          = pos;
		sptr->pch          = pbuf;
		fptr->fp_h.f_count = hs2net(plen);
		sptr->len 	   = plen;
		sptr->replyd 	   = FALSE;
#ifdef DEBUG
		kprintf("rfnsend:   packet: %d   pos: 0x%x  pch: 0x%x  len: %d\n",
			i,pos,pbuf,plen);
#endif
		pos += plen;
		pbuf += plen;
		len -= plen;
	}

	/* make sure the device is opened */
	if (Rf.device == RCLOSED) {
		Rf.device = open(UDP, RSERVER, ANYLPORT);
		if (Rf.device == SYSERR ||
		    control(Rf.device, DG_SETMODE, DG_DMODE|DG_TMODE)
		    == SYSERR)
			return(SYSERR);
	}

	/* send the packets */
	disable(ps);
	control(Rf.device, DG_CLEAR);
	for (i=0; i < npack; ++i) {
#ifdef DEBUG
		kprintf("rfnsend:   sending packet: %d\n", i);
#endif
		if ( write(Rf.device, &rfnstr[i].fp, FPHLEN) == SYSERR) {
			restore(ps);
#ifdef PRINTERRORS
			kprintf("rfnsend: bad return from write\n");
#endif
			return(SYSERR);
		}
	}

	/* wait for the replies */
	nbytes = 0;
	for (i=0; i < npack; ++i) {
		sptr = &rfnstr[i];
		fptr = &rfnstr[i].fp;
		
		ret=read(Rf.device, &rfnstr[i].fp, FPHLEN + RDATLEN);
		if ((ret == TIMEOUT) || (ret == SYSERR)) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rfnsend: bad return from read(): %d\n",
				ret);
#endif
			restore(ps);
			return(SYSERR);
		}

		/* decode the header */
		fptr->fp_h.f_op = net2hs(fptr->fp_h.f_op);
		fptr->fp_h.f_pos = net2hl(fptr->fp_h.f_pos);
		fptr->fp_h.f_count = net2hs(fptr->fp_h.f_count);

#ifdef DEBUG
		kprintf("rfnsend: got reply op: %d  count: %d, pos: 0x%x\n",
			fptr->fp_h.f_op, fptr->fp_h.f_count, fptr->fp_h.f_pos);
#endif
		if (fptr->fp_h.f_op != FS_READ) {
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rfnsend: got ERROR back\n");
#endif
			restore(ps);
			return(SYSERR);
		}

		/* check for the correct pos (JUST FOR NOW) */
		fptr->fp_h.f_pos -= fptr->fp_h.f_count;
		if (fptr->fp_h.f_pos != sptr->pos) {
			/* just for now */
#if defined(DEBUG) || defined(PRINTERRORS)
			kprintf("rfnsend: got pos: 0x%x, expected 0x%x\n",
				fptr->fp_h.f_pos, sptr->pos);
#endif
			restore(ps);
			return(SYSERR);
		}

		/* store the results back */
		blkcopy(sptr->pch, fptr->fpdata, fptr->fp_h.f_count);

		nbytes += fptr->fp_h.f_count;
	}
	return(nbytes);
}
