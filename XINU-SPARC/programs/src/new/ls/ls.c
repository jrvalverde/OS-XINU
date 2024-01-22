/* ls.c - ls */

/* Written by Jim Griffioen - 6-8-90 */

#include <stdio.h>
#include <fcntl.h>
#include <a.out.h>


/*#define DEBUG*/


#define MAXFWID		19
#define MAXFLIN		4

#define STDIN_FD	0
#define STDOUT_FD	1
#define STDERR_FD	2

#define TRUE	1
#define FALSE	0
#define SYSERR	(-1)
#define	OK	(1)

#define MAXFNAME	50
#define SCREEN_WIDTH	79

/* for talking to the Vaxen */
#define vax2hs(x) (unsigned) ((unsigned) ((x)>>8) + (unsigned)((x)<<8))

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
main(nargs, args)
int	nargs;
char	*args[];
{
	char	*p, *buf;
	int	dev, len;
	char	str[256];
	struct	dirent	*d;
	char	aflag, cflag, tflag, Fflag;
	int	i,j, k;
	char	*bigbuf, *curname;
	char	*names[1000];
	int	namecount;
	int	maxlen, columns, itemspercolumn, leftover;
	char	dname[512];
	char	fullname[512];
	int	idx;
	char	outbuf[SCREEN_WIDTH+16];

	aflag = FALSE;
	cflag = FALSE;
	tflag = FALSE;
	Fflag = FALSE;
	strcpy(dname, ".");
	
	for (i=1; i<nargs; i++) {
		if (strcmp(args[i], "-a") == 0)
		    aflag = TRUE;
		else if (strcmp(args[i], "-1") == 0)
		    cflag = TRUE;
		else if (strcmp(args[i], "-t") == 0)
		    tflag = TRUE;
		else if (strcmp(args[i], "-F") == 0)
		    Fflag = TRUE;
		else if (strcmp(args[i], "-h") == 0) {
			help(args[0]);
		}
		else if (strcmp(args[i], "-help") == 0) {
			help(args[0]);
		}
		else if (i == (nargs-1)) {
			if (*(args[i]) != '-')
			    strcpy(dname, args[i]);
			else {
				fprintf(stderr, "ls: unknown option %s\n",
					args[i]);
				help(args[0]);
				exit(1);
			}
		}
		else {
			fprintf(stderr, "ls: unknown option %s\n", args[i]);
			help(args[0]);
			exit(1);
		}
	}


	if ( ((long)(buf=(char *)malloc(512))) == NULL) {
		fprintf(stderr, "ls: no memory\n");
		return(SYSERR);
	}
	if ( ((long)(bigbuf=(char *)malloc(512*1024))) == NULL) {
		fprintf(stderr, "ls: no memory\n");
		return(SYSERR);
	}
	if ((dev=open(dname, O_RDONLY, 0666)) < 0) {
		fprintf(stderr, "cannot open %s\n", dname);
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
				d->d_nlen = vax2hs(d->d_nlen);
				d->d_rlen = vax2hs(d->d_rlen);
			}
			
			if (d->d_inum == 0)
			    continue;
#ifdef DEBUG
			fprintf(stderr, "len:  %d\n", len);
			fprintf(stderr, "rlen: %d\n", d->d_rlen);
			fprintf(stderr, "nlen: %d\n", d->d_nlen);
			fprintf(stderr, "name: %s\n", d->d_nam);
#endif
			if (len < 512 ||
			    d->d_nlen != strlen(d->d_nam) ||
			    d->d_nlen > 255 ||
			    d->d_rlen < sizeof(struct dirent) ||
			    d->d_rlen > &buf[512] - p) {
				fprintf(stderr, "Not a directory\n");
				close(dev);
				free(buf);
				return(SYSERR);
		        }
			if (aflag || d->d_nam[0] != '.') {
				strcpy(curname, d->d_nam);
				strcpy(fullname, dname);
				strcat(fullname, curname);
				if (Fflag)
				    appendfiletype(fullname,curname);
				names[namecount] = curname;
				namecount++;
				curname += MAXFNAME;
			}
			if (d->d_rlen == 0)
			    break;
		}
	}
	free(buf);
	close(dev);

	if (namecount <= 0) {
		exit(0);
	}

	/* we now have all the names in an array */
	maxlen = sortlist(names, namecount);
	maxlen += 2;
	
	if (cflag) {
		for(i=0, j=0; i<namecount; i++) 
		    printf("%s\n", names[i]);
		return(OK);
	}

	/* calculate max number of columns */
	columns = SCREEN_WIDTH / maxlen;

	/* calculate max names per column */
	itemspercolumn = ( ((namecount%columns)==0) ?
			  namecount/columns : ((namecount/columns) + 1) );

	if (!tflag) {			/* recalculate # of columns	*/
		/* if !tflag, we list alphabetically down a column	*/
		/* so we fill columns from left to write, which may	*/
		/* eliminate some of the rightmost columns.		*/
		columns = ( ((namecount%itemspercolumn)==0) ?
			   namecount/itemspercolumn :
			   ((namecount/itemspercolumn) + 1) );
	}

	strcpy(outbuf, "");
	for(i=0, j=0; i<(columns*itemspercolumn); i++) {
		if (tflag)
		    idx = i;
		else
		    idx = j + ((i%columns) * itemspercolumn);
		if (idx < namecount) {
			strcat(outbuf, names[idx]);
			leftover = maxlen - strlen(names[idx]);
		}
		else {			/* skip entry */
			leftover = 0;
		}
		if (((i+1)%columns) != 0) {
			for(k=0; k<leftover; k++)
			    strcat(outbuf, " ");
		}
		else {
			strcat(outbuf, "\n");
			write(STDOUT_FD, outbuf, strlen(outbuf));
			strcpy(outbuf, "");
			j++;		/* inc row count */
		}
	}
	
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

/*---------------------------------------------------------------------------
 * appendfiletype - append the file type - / for dir, * for executable
 *---------------------------------------------------------------------------
 */
appendfiletype(fullname, fname)
char *fullname;
char *fname;
{
	int	dev;
	char	buf[600];
	struct dirent *d;
	struct exec *e;
	int	len;

	if ((dev=open(fullname, O_RDONLY, 0666)) < 0)
	    return(SYSERR);

	if ( (len=read(dev, buf, 512)) < 0) {
		close(dev);
		return(SYSERR);
	}
	close(dev);

	d = (struct dirent *)buf;
	e = (struct exec *)buf;

	if (len >= sizeof(struct dirent)) {
		/* this could be a Vax or a Sun, so be */
		/* prepared to swap integer fields     */
		buf[514] = '\0';	/* make sure string ends */
		if ( (d->d_nlen == strlen(d->d_nam)) ||
		    (vax2hs(d->d_nlen) == strlen(d->d_nam))) {
			strcat(fname, "/");
			return(OK);
		}
	}

	if (len >= sizeof(struct exec)) {
		if (e->a_magic==OMAGIC ||
		    e->a_magic==NMAGIC || e->a_magic==ZMAGIC) {
			strcat(fname, "*");
			return(OK);
		}
	}
	
	return(OK);
}


help(cmd)
char *cmd;
{
	printf("usage: %s [options] [directory]\n", cmd);
	printf("Options\n");
	printf("-------\n");
	printf("\t-a\tList all files\n");
	printf("\t-1\tOne column output\n");
	printf("\t-t\tTranspose columns and rows\n");
	printf("\t-F\tShow file type\n");
	printf("\t-h\tThis help message\n");
	printf("\t-help\tThis help message\n");
	exit(0);
}
