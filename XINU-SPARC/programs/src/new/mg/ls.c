/* Written by Jim Griffioen - 6-8-90 */
/* hacked into a function by Scott Ballew 9-7-90 */

#ifdef XINU8
#ifndef NO_DIRED

#include <stdio.h>
#include <fcntl.h>

#define OK	(1)
#define SYSERR	(-1)

#define MAXFNAME	50

/* for talking to the Vaxen */
#define h2vax(x) (unsigned) ((unsigned) ((x)>>8) + (unsigned)((x)<<8))
#define vax2h(x) (unsigned) ((unsigned) ((x)>>8) + (unsigned)((x)<<8))

struct	dirent	{	/* UNIX directory entry		*/
	long	d_inum;	/* file's inode number		*/
	short	d_rlen;	/* length of this record	*/
	short	d_nlen;	/* length of this file's name	*/
	char	d_nam[1]; /* start of file name		*/
};

/*------------------------------------------------------------------------
 *  ls  -  (command ls) list contents of remote file system directory
 *	    similar to unix ls
 *------------------------------------------------------------------------
 */
int local_ls(bp, dname)
     register void *bp;
     char *dname;
{
    char	*p, *buf;
    int	dev, len;
    struct	dirent	*d;
    int	i,j;
    char	*bigbuf, *curname;
    char	*names[1000];
    int	namecount;
    char	fullname[512];
    char line[256];

    if ( ((long)(buf=(char *)malloc(512))) == NULL) {
	ewprintf("ls: no memory");
	return(SYSERR);
    }
    if ( ((long)(bigbuf=(char *)malloc(512*1024))) == NULL) {
	ewprintf("ls: no memory");
	return(SYSERR);
    }
    if ((dev=open(dname, O_RDONLY, 0666)) < 0) {
	ewprintf("cannot open %s", dname);
	free(buf);
	return(SYSERR);
    }

    if (dname[strlen(dname)-1] != '/') /* need later for fullname */
	strcat(dname, "/");

    namecount = 0;
    curname = bigbuf;
    while ( (len = read(dev, buf, 512)) > 0) {
	for (p=buf ; p< &buf[512] ; p += d->d_rlen) {
	    d = (struct dirent *)p;
	    /* this could be a Vax or a Sun, so be */
	    /* prepared to swap integer fields     */
	    if (d->d_nlen != strlen(d->d_nam)) {
		d->d_nlen = vax2h(d->d_nlen);
		d->d_rlen = vax2h(d->d_rlen);
	    }

	    if (d->d_inum == 0)
		continue;
	    if (len < 512 ||
		d->d_nlen != strlen(d->d_nam) ||
		d->d_nlen > 255 ||
		d->d_rlen < sizeof(struct dirent) ||
		d->d_rlen > &buf[512] - p) {
		ewprintf("Not a directory");
		close(dev);
		free(buf);
		return(SYSERR);
	    }
	    strcpy(curname, d->d_nam);
	    strcpy(fullname, dname);
	    strcat(fullname, curname);
	    names[namecount] = curname;
	    namecount++;
	    curname += MAXFNAME;
	    if (d->d_rlen == 0)
		break;
	}
    }
    free(buf);
    close(dev);

    if (namecount <= 0) {
	free(bigbuf);
	return(0);
    }

    /* we now have all the names in an array */
    sortlist(names, namecount);

    for(i=0, j=0; i<namecount; i++) {
	sprintf(line, "  %s", names[i]);
	addline(bp, line);
    }
    free(bigbuf);
    return(OK);
}

/*---------------------------------------------------------------------------
 * sortlist - sort the file name
 *---------------------------------------------------------------------------
 */
sortlist(list, count)
     char *list[];
     int count;
{
    int i,j;
    char sb[256];
    int maxlen = 0;

    for (i = 1; i < count; i++) {
	for (j = 0; j < (count-i); j++) {
	    if (strlen(list[j]) > maxlen)
		maxlen = strlen(list[j]);
	    if (strcmp(list[j], list[j+1]) > 0) {
		strcpy(sb, list[j]);
		strcpy(list[j], list[j+1]);
		strcpy(list[j+1], sb);
	    }
	}
    }
    return(maxlen);
}
#endif
#endif
