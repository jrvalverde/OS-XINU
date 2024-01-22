
/* $Header$ */

/* 
 * dir.c - opendir, readdir, closedir, telldir, and seekdir for xinu v8
 * 
 * Programmer:	Scott M Ballew
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Sep 13 11:05:22 1990
 *
 */

/*
 * $Log$
 */

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/file.h>

#define BUFSIZE	512

#define swapbytes(x)	(unsigned) (((unsigned)(x)<<8) + (unsigned) ((x)>>8))

DIR *opendir(dirname)
     char *dirname;
{
    register DIR *retval;
    
    if (!dirname) {
	return(NULL); 
    }

    if ((retval = (DIR *) malloc(sizeof(DIR))) == NULL) {
	return(NULL);
    }

    bzero(retval, sizeof(DIR));
    if ((retval->dd_fd = open(dirname, O_RDONLY)) < 0) {
	free(retval);
	return(NULL);
    }
    if ((retval->dd_buf = (char *) malloc(BUFSIZE)) == NULL) {
	close(retval->dd_fd);
	free(retval);
	return(NULL);
    }
    retval->dd_bsize = BUFSIZE;
    return(retval);
}

static struct dirent dirbuffer;

struct dirent *readdir(dp)
     register DIR *dp;
{
    register struct dirent *p;

    if (!dp->dd_size) {	/* read in another buffer */
	if ((dp->dd_size = read(dp->dd_fd, dp->dd_buf, dp->dd_bsize)) <= 0) {
	    dp->dd_size = 0;
	    return(NULL);
	}
	dp->dd_loc = 0;
    }

    p = (struct dirent *) &dp->dd_buf[dp->dd_loc - sizeof(off_t)];
    /* this could be a VAX or a Sun, so perhaps we need a byte swap */
    if (p->d_namlen != strlen(p->d_name)) {
	p->d_namlen = swapbytes(p->d_namlen);
	p->d_reclen = swapbytes(p->d_reclen);
    }

    /* try to make sure it is a directory */
    if (p->d_fileno == 0 |
	p->d_namlen != strlen(p->d_name) |
	p->d_namlen > MAXNAMLEN |
	p->d_reclen > &dp->dd_buf[dp->dd_bsize] - (char *) p) {
	return(NULL);
    }

    /* ok, copy it to the returnable one */
    bcopy(p, &dirbuffer, DIRSIZ(p));
    dp->dd_off += p->d_reclen;
    dp->dd_loc += p->d_reclen;
    dp->dd_size -= p->d_reclen;
    
    dirbuffer.d_off = dp->dd_off;
    return(&dirbuffer);
}

closedir(dp)
     register DIR *dp;
{
    close(dp->dd_fd);
    free(dp->dd_buf);
    free(dp);
    return;
}

#ifndef telldir		/* may be a defined macro */
long telldir(dp)
     register DIR *dp;
{
    return(dp->dd_off);
}
#endif

void seekdir(dp, loc)
     register DIR *dp;
     register long loc;
{
    register long seekval;

    seekval = (loc / dp->dd_bsize) * dp->dd_bsize;
    lseek(dp->dd_fd, seekval, L_SET);
    if ((dp->dd_size = read(dp->dd_fd, dp->dd_buf, dp->dd_bsize)) <= 0) {
	dp->dd_size = 0;
	return;
    }
    dp->dd_loc = loc - seekval;
    dp->dd_off = loc;
    dp->dd_size -= dp->dd_loc;
    return;
}
    
